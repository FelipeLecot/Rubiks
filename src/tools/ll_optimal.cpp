// ll_optimal — how short can OLL / PLL algorithms get?
//
// For every OLL and PLL case this tool
//   1. takes a standard speedcubing algorithm (the finger-trick friendly one),
//      runs it through the sticker model (cube.h / moves.h) and checks that it
//      really is a last-layer algorithm,
//   2. builds the case it solves and searches for the provably shortest
//      solution with IDA* over a cubie model, pruned by pattern databases,
//   3. re-verifies the optimal solution on the sticker model.
//
// Move metrics
//   HTM  (half turn metric):  any outer-face turn = 1, slices = 2, rotations = 0
//   STM  (slice turn metric): any layer turn = 1 (M, r, R2 ...), rotations = 0
//   QTM  (quarter turn metric): quarter turn = 1, half turn = 2
// Pre- and post-AUF (U turns before / after) are free, as is customary.
//
// Usage: ll_optimal [pll|oll|all] [--stm|--qtm] [--cache DIR] [--only CASE]
//        ll_optimal zbll [--stm|--qtm] [--near D] [--from I] [--to J] [--data FILE]
//                        [--plus N] [--plus-max-opt K]
//        ll_optimal finish "<scramble>" "<skeleton>" [--back K]
//   zbll:   every last-layer case with oriented edges (493); all optimal
//           solutions (with pre- and post-AUF) are written to FILE, and with
//           --plus N also all solutions up to N moves longer (FILE.plus1 ...)
//           for cases whose optimum is at most K (default 13).
//   finish: FMC helper (HTM, nothing free): drops the last 0..K skeleton moves
//           and finds every optimal finish, ranking them by the total length
//           after cancellation at the join.
//   --stm / --qtm: slice turn metric (M, E, S count 1) / quarter turn metric.
//   --near D: exact table of all positions within D moves that replaces the
//             last D levels of each search (default 6; 7 in HTM needs ~4 GB).
//   --syms K: also consult the edge databases from K rotated viewpoints
//             (admissible but slower on this hardware; default 0).
// The pattern databases (~1.2 GB per metric) are built on first run and cached.
#include "cube_engine.h"

// ═════════════════════════════════════════════════════════════════════════════
// IDA*
// ═════════════════════════════════════════════════════════════════════════════

enum class Goal { Solved, OLL };

static bool is_goal(const CubieCube& c, Goal g) {
    if (g == Goal::Solved) return c == CubieCube::solved();
    for (int i = 0; i < 8; ++i) if (c.co[i]) return false;
    for (int i = 0; i < 12; ++i) if (c.eo[i]) return false;
    for (int i = 4; i < 8; ++i) if (c.cp[i] != i) return false;
    for (int i = 4; i < 12; ++i) if (c.ep[i] != i) return false;
    return true;
}

struct Search {
    std::vector<Pattern*> pdbs;
    Goal goal;
    Metric metric = HTM;
    std::vector<int> path;
    std::vector<std::vector<int>> solutions;
    size_t maxSolutions = 2000;

    int cost(int m) const { return MV[m].cost[metric]; }
    std::vector<int> symIdx;  // extra viewpoints for the edge databases
    const NearTable* near = nullptr;  // built for the same metric
    bool exact = true;                // searching at the optimal depth
    int h(const CubieCube& c) const {
        int v = 0;
        for (auto* p : pdbs) v = std::max(v, p->h(c));
        for (int k : symIdx) {
            CubieCube t = SYMS_INV[k] * c * SYMS[k];
            for (auto* p : pdbs) if (!p->corners) v = std::max(v, p->h(t));
        }
        return v;
    }
    // U is never the first move (pre-AUF is free and searched separately)
    // and, for PLL, never the last move (post-AUF likewise).
    void dfs(const CubieCube& c, int left, int lastAxis, bool noUlast) {
        if (solutions.size() >= maxSolutions) return;
        if (left == 0) {
            if (is_goal(c, goal) && !(noUlast && !path.empty() && has_U_part(path.back())))
                solutions.push_back(path);
            return;
        }
        // Within reach of the near-solved table the exact distance is known.
        // Searches run at the first depth that has solutions, so on every
        // solution path the remaining distance equals the moves left
        // (a shorter completion would have given a shorter solution).
        // (Beyond the optimal depth, only "at most `left` away" can be required.)
        if (near && goal == Goal::Solved && left <= near->depth) {
            int d = near->get(c);
            if (exact ? d != left : d > left) return;
        } else if (h(c) > left) return;
        for (int m = 0; m < (int)MV.size(); ++m) {
            if (MV[m].axis == lastAxis) continue;
            int cm = cost(m);
            if (cm > left) continue;
            path.push_back(m);
            dfs(c * MV[m].c, left - cm, MV[m].axis, noUlast);
            path.pop_back();
        }
    }
};

// ═════════════════════════════════════════════════════════════════════════════
// Cases
// ═════════════════════════════════════════════════════════════════════════════

struct Case { std::string name, alg; };

// Common speedsolving algorithms (the finger-trick optimised versions).
static const std::vector<Case> PLL = {
    {"Aa", "x R' U R' D2 R U' R' D2 R2 x'"},
    {"Ab", "x R2 D2 R U R' D2 R U' R x'"},
    {"E",  "x' R U' R' D R U R' D' R U R' D R U' R' D' x"},
    {"F",  "R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R"},
    {"Ga", "R2 U R' U R' U' R U' R2 U' D R' U R D'"},
    {"Gb", "R' U' R U D' R2 U R' U R U' R U' R2 D"},
    {"Gc", "R2 U' R U' R U R' U R2 U D' R U' R' D"},
    {"Gd", "R U R' U' D R2 U' R U' R' U R' U R2 D'"},
    {"H",  "M2 U M2 U2 M2 U M2"},
    {"Ja", "x R2 F R F' R U2 r' U r U2 x'"},
    {"Jb", "R U R' F' R U R' U' R' F R2 U' R'"},
    {"Na", "R U R' U R U R' F' R U R' U' R' F R2 U' R' U2 R U' R'"},
    {"Nb", "R' U R U' R' F' U' F R U R' F R' F' R U' R"},
    {"Ra", "R U' R' U' R U R D R' U' R D' R' U2 R'"},
    {"Rb", "R2 F R U R U' R' F' R U2 R' U2 R"},
    {"T",  "R U R' U' R' F R2 U' R' U' R U R' F'"},
    {"Ua", "M2 U M U2 M' U M2"},
    {"Ub", "M2 U' M U2 M' U' M2"},
    {"V",  "R' U R' U' y R' F' R2 U' R' U R' F R F"},
    {"Y",  "F R U' R' U' R U R' F' R U R' U' R' F R F'"},
    {"Z",  "M' U M2 U M2 U M' U2 M2"},
};

static const std::vector<Case> OLL = {
    {"1",  "R U2 R2 F R F' U2 R' F R F'"},
    {"2",  "F R U R' U' F' f R U R' U' f'"},
    {"3",  "f R U R' U' f' U' F R U R' U' F'"},
    {"4",  "f R U R' U' f' U F R U R' U' F'"},
    {"5",  "r' U2 R U R' U r"},
    {"6",  "r U2 R' U' R U' r'"},
    {"7",  "r U R' U R U2 r'"},
    {"8",  "l' U' L U' L' U2 l"},
    {"9",  "R U R' U' R' F R2 U R' U' F'"},
    {"10", "R U R' U R' F R F' R U2 R'"},
    {"11", "r U R' U R' F R F' R U2 r'"},
    {"12", "M' R' U' R U' R' U2 R U' R r'"},
    {"13", "F U R U' R2 F' R U R U' R'"},
    {"14", "R' F R U R' F' R F U' F'"},
    {"15", "r' U' r R' U' R U r' U r"},
    {"16", "r U r' R U R' U' r U' r'"},
    {"17", "F R' F' R2 r' U R U' R' U' M'"},
    {"18", "r U R' U R U2 r2 U' R U' R' U2 r"},
    {"19", "r' R U R U R' U' M' R' F R F'"},
    {"20", "r U R' U' M2 U R U' R' U' M'"},
    {"21", "R U2 R' U' R U R' U' R U' R'"},
    {"22", "R U2 R2 U' R2 U' R2 U2 R"},
    {"23", "R2 D' R U2 R' D R U2 R"},
    {"24", "r U R' U' r' F R F'"},
    {"25", "F' r U R' U' r' F R"},
    {"26", "R U2 R' U' R U' R'"},
    {"27", "R U R' U R U2 R'"},
    {"28", "r U R' U' r' R U R U' R'"},
    {"29", "R U R' U' R U' R' F' U' F R U R'"},
    {"30", "F R' F R2 U' R' U' R U R' F2"},
    {"31", "R' U' F U R U' R' F' R"},
    {"32", "L U F' U' L' U L F L'"},
    {"33", "R U R' U' R' F R F'"},
    {"34", "R U R2 U' R' F R U R U' F'"},
    {"35", "R U2 R2 F R F' R U2 R'"},
    {"36", "L' U' L U' L' U L U L F' L' F"},
    {"37", "F R' F' R U R U' R'"},
    {"38", "R U R' U R U' R' U' R' F R F'"},
    {"39", "L F' L' U' L U F U' L'"},
    {"40", "R' F R U R' U' F' U R"},
    {"41", "R U R' U R U2 R' F R U R' U' F'"},
    {"42", "R' U' R U' R' U2 R F R U R' U' F'"},
    {"43", "F' U' L' U L F"},
    {"44", "F U R U' R' F'"},
    {"45", "F R U R' U' F'"},
    {"46", "R' U' R' F R F' U R"},
    {"47", "R' U' R' F R F' R' F R F' U R"},
    {"48", "F R U R' U' R U R' U' F'"},
    {"49", "r U' r2 U r2 U r2 U' r"},
    {"50", "r' U r2 U' r2 U' r2 U r'"},
    {"51", "F U R U' R' U R U' R' F'"},
    {"52", "R U R' U R U' B U' B' R'"},
    {"53", "l' U2 L U L' U' L U L' U l"},
    {"54", "r U2 R' U' R U R' U' R U' r'"},
    {"55", "R' F R U R U' R2 F' R2 U' R' U R U R'"},
    {"56", "r' U' r U' R' U R U' R' U R r' U r"},
    {"57", "R U R' U' M' U R U' r'"},
};

static CubieCube U_POW[4];

// Canonical key of a case up to pre-/post-AUF.
static std::string case_key(const CubieCube& s, bool pll) {
    std::string best;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < (pll ? 4 : 1); ++b) {
            CubieCube t = U_POW[b] * s * U_POW[a];
            std::string k;
            for (int i = 0; i < 4; ++i) k += char('0' + t.co[i]);
            for (int i = 0; i < 4; ++i) k += char('0' + t.eo[i]);
            if (pll) {
                for (int i = 0; i < 4; ++i) k += char('0' + t.cp[i]);
                for (int i = 0; i < 4; ++i) k += char('a' + t.ep[i]);
            }
            if (best.empty() || k < best) best = k;
        }
    return best;
}

// Search moves → notation. Centers are tracked on a sticker cube so that a
// slice pair becomes M/E/S and later face turns are named by where their
// center physically is (for HTM/QTM the centers never move).
static std::vector<std::string> to_tokens(const std::vector<int>& ids, Metric metric) {
    static const char* suf[4] = {"", "", "2", "'"};
    std::vector<std::string> out;
    Cube3 cube = Cube3::solved();
    auto where = [&](int center) {
        for (int p = 0; p < 6; ++p) if (cube.s[Cube3::idx(p, 1, 1)] == center) return p;
        return -1;
    };
    auto emit = [&](const std::string& t) { out.push_back(t); cube = cube.apply(parse_alg(t).perm); };
    for (int m : ids) {
        const auto& v = MV[m];
        int f1 = 2 * v.axis, f2 = f1 + 1;
        if (metric == STM && v.cost[STM] == 1 && v.a && v.b) {
            // f1^a f2^b with a+b ≡ 0: the middle layer turns like f1^b.
            int p = where(f1), q = v.b;
            char name; int pw;
            switch (p) {
                case L: name = 'M'; pw = q; break;
                case R: name = 'M'; pw = (4 - q) % 4; break;
                case D: name = 'E'; pw = q; break;
                case U: name = 'E'; pw = (4 - q) % 4; break;
                case F: name = 'S'; pw = q; break;
                default: name = 'S'; pw = (4 - q) % 4; break;
            }
            emit(std::string(1, name) + suf[pw]);
        } else {
            if (v.a) emit(std::string(1, FACE_NAMES[where(f1)]) + suf[v.a]);
            if (v.b) emit(std::string(1, FACE_NAMES[where(f2)]) + suf[v.b]);
        }
    }
    return out;
}

static std::string join(const std::vector<std::string>& t) {
    std::string s;
    for (auto& x : t) { if (!s.empty()) s += ' '; s += x; }
    return s;
}

static std::string moves_to_string(const std::vector<int>& p, Metric metric = HTM) {
    return join(to_tokens(p, metric));
}

static std::vector<std::string> invert_tokens(const std::vector<std::string>& t) {
    std::vector<std::string> r;
    for (auto it = t.rbegin(); it != t.rend(); ++it) {
        std::string x = *it;
        if (x.size() == 1) x += '\'';
        else if (x[1] == '\'') x = x.substr(0, 1);
        r.push_back(x);
    }
    return r;
}

// Rough ergonomics score used to pick one of several optimal solutions:
// prefer R/U turns, then L/F/M, over B, D, E and S.
static int ergonomics(const std::vector<std::string>& tokens) {
    int s = 0;
    for (auto& t : tokens) {
        switch (t[0]) {
            case 'U': case 'R': break;
            case 'L': case 'F': case 'M': s += 1; break;
            case 'D': case 'E': case 'S': s += 3; break;
            default: s += 4;
        }
    }
    return s;
}

struct Result {
    std::string name, alg;
    AlgInfo info;
    int opt = -1;
    std::string optAlg;
    size_t optCount = 0;
    bool capped = false;
    double secs = 0;
};

// Face relabeling for a y rotation: YMAP[f] is the face that f becomes.
static int YMAP[6];
static void init_ymap() {
    Perm54 y = parse_alg("y").perm, yi = y.inverse();
    for (int f = 0; f < 6; ++f) {
        Perm54 conj = y.compose(layer_turn(Face(f), 0, 1)).compose(yi);
        for (int g = 0; g < 6; ++g)
            if (conj == layer_turn(Face(g), 0, 1)) YMAP[f] = g;
    }
}

// Pre-AUF that makes `moves` solve `start` (up to post-AUF for PLL), or -1.
static int find_pre_auf(const CubieCube& start, const std::vector<int>& moves, bool pll) {
    for (int a = 0; a < 4; ++a) {
        CubieCube c = start * U_POW[a];
        for (int m : moves) c = c * MV[m].c;
        if (!pll && is_goal(c, Goal::OLL)) return a;
        if (pll) for (int b = 0; b < 4; ++b) if (c * U_POW[b] == CubieCube::solved()) return a;
    }
    return -1;
}

// Pre-AUF U^a, then moves, then post-AUF U^b.
struct Hit { int a; std::vector<int> moves; std::vector<std::string> tokens; int b = 0; };
struct StateResult {
    int opt = -1;
    std::vector<Hit> hits;  // all optimal solutions found, most comfortable first
    bool capped = false;
    std::vector<std::vector<Hit>> plus;  // plus[i]: all solutions of length opt+1+i
};

static const char* AUF_NAMES[4] = {"", "U", "U2", "U'"};

static std::string hit_to_string(const Hit& h, bool withPost = false) {
    std::string s = join(h.tokens);
    if (h.a) s = std::string("(") + AUF_NAMES[h.a] + ") " + s;
    if (withPost && h.b) s += std::string(" (") + AUF_NAMES[h.b] + ")";
    return s;
}

// Optimal solutions of a last-layer state (goal: solved up to AUF for
// Goal::Solved, oriented last layer for Goal::OLL).
// Every solution can also be performed from the other three sides
// (y-conjugates). Collect them all, find their AUFs and check each on the
// sticker model; most comfortable first.
static std::vector<Hit> expand_hits(const CubieCube& start, bool pll, Metric metric,
                                    const std::vector<std::vector<int>>& found) {
    Cube3 caseCube = from_cubie(start);
    std::set<std::vector<int>> all;
    for (auto sol : found)
        for (int c = 0; c < 4; ++c) {
            all.insert(sol);
            for (int& m : sol) {
                const auto& v = MV[m];
                int g1 = YMAP[2 * v.axis];  // where the axis' first face goes
                m = (g1 % 2 == 0) ? MV_ID[g1 / 2][v.a][v.b] : MV_ID[g1 / 2][v.b][v.a];
            }
        }
    std::vector<Hit> hits;
    for (auto& sol : all) {
        int a = find_pre_auf(start, sol, pll);
        if (a < 0) throw std::runtime_error("conjugated solution does not solve the case");
        Hit h{a, sol, to_tokens(sol, metric)};
        // Slices may leave the whole cube rotated, which is fine.
        Cube3 c = normalize(caseCube.apply(parse_alg(hit_to_string(h)).perm));
        bool ok = false;
        for (int b = 0; b < 4 && !ok; ++b) {
            Cube3 d = c.apply(parse_alg(AUF_NAMES[b]).perm);
            ok = pll ? d.is_solved() : is_goal(to_cubie(d), Goal::OLL);
            if (ok) h.b = b;
        }
        if (!ok) throw std::runtime_error("solution failed sticker verification: " + hit_to_string(h));
        hits.push_back(h);
    }
    std::stable_sort(hits.begin(), hits.end(), [](const Hit& x, const Hit& y) {
        return ergonomics(x.tokens) < ergonomics(y.tokens);
    });
    return hits;
}

static StateResult solve_state(const CubieCube& start, bool pll, const Search& proto,
                               int plus = 0, int plusMaxOpt = 99) {
    StateResult r;
    // For a last-layer state s, U^b s U^a is a y-rotated view of s U^(a+b),
    // so only a+b matters: PLL needs the 4 pre-AUFs, OLL (which ignores the
    // last-layer permutation, i.e. both AUFs) just one.
    std::vector<int> variants = pll ? std::vector<int>{0, 1, 2, 3} : std::vector<int>{0};
    auto search_depth = [&](int depth, bool exact) {
        std::vector<std::vector<int>> found;
        // One task per (variant, first move); the first move is never U.
        std::vector<std::pair<int,int>> tasks;
        for (int a : variants)
            for (int m = 0; m < (int)MV.size(); ++m)
                if (!has_U_part(m)) tasks.push_back({a, m});
        #pragma omp parallel for schedule(dynamic, 1)
        for (int t = 0; t < (int)tasks.size(); ++t) {
            auto [a, m] = tasks[t];
            Search s = proto;
            s.exact = exact;
            if (!exact) s.maxSolutions *= 50;
            int cm = s.cost(m);
            if (cm > depth) continue;
            s.path = {m};
            s.dfs(start * U_POW[a] * MV[m].c, depth - cm, MV[m].axis, pll);
            #pragma omp critical
            found.insert(found.end(), s.solutions.begin(), s.solutions.end());
        }
        return found;
    };

    auto expand = [&](const std::vector<std::vector<int>>& found) {
        return expand_hits(start, pll, proto.metric, found);
    };

    std::vector<std::vector<int>> found;
    for (int depth = 1; found.empty() && depth <= 30; ++depth) {
        found = search_depth(depth, true);
        if (!found.empty()) r.opt = depth;
    }
    r.capped = found.size() >= proto.maxSolutions;
    r.hits = expand(found);
    if (r.opt <= plusMaxOpt)
        for (int i = 1; i <= plus; ++i) {
            // Only the cost-exact solutions of this length (proper ones: the
            // move rules already exclude trivially padded sequences).
            auto more = search_depth(r.opt + i, false);
            r.plus.push_back(expand(more));
        }
    return r;
}

static Result solve_case(const Case& cs, bool pll, const Search& proto) {
    Result r{cs.name, cs.alg, parse_alg(cs.alg)};
    auto t0 = std::chrono::steady_clock::now();
    Cube3 caseCube = normalize(Cube3::solved().apply(r.info.perm.inverse()));
    StateResult sr = solve_state(to_cubie(caseCube), pll, proto);
    r.opt = sr.opt;
    r.optCount = sr.hits.size();
    r.capped = sr.capped;
    r.optAlg = hit_to_string(sr.hits.front());
    r.secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    return r;
}

// Validate the algorithm table: each alg must be a last-layer algorithm of the
// right kind, and the cases must be pairwise distinct and non-trivial.
static void validate(const std::vector<Case>& cases, bool pll) {
    std::map<std::string, std::string> seen;
    std::string solvedKey = case_key(CubieCube::solved(), pll);
    for (auto& cs : cases) {
        auto info = parse_alg(cs.alg);
        Cube3 caseCube = normalize(Cube3::solved().apply(info.perm.inverse()));
        CubieCube s = to_cubie(caseCube);
        bool f2l = true;
        for (int i = 4; i < 8; ++i) f2l &= s.cp[i] == i && s.co[i] == 0;
        for (int i = 4; i < 12; ++i) f2l &= s.ep[i] == i && s.eo[i] == 0;
        if (!f2l) throw std::runtime_error(cs.name + ": algorithm breaks F2L: " + cs.alg);
        if (pll) {
            for (int i = 0; i < 4; ++i)
                if (s.co[i] || s.eo[i]) throw std::runtime_error(cs.name + ": not a PLL (changes orientation)");
        }
        std::string key = case_key(s, pll);
        if (key == solvedKey) throw std::runtime_error(cs.name + ": trivial case");
        if (seen.count(key)) throw std::runtime_error(cs.name + " duplicates " + seen[key]);
        seen[key] = cs.name;
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// ZBLL: every last-layer case with oriented edges, for fewest-moves use
// ═════════════════════════════════════════════════════════════════════════════

struct ZbllCase { std::string set; int index; CubieCube state; };

// Corner-orientation key of a state up to AUF (identifies the ZBLL subset).
static std::string co_key(const CubieCube& s) {
    std::string best;
    for (int a = 0; a < 4; ++a) {
        CubieCube t = s * U_POW[a];
        std::string k;
        for (int i = 0; i < 4; ++i) k += char('0' + t.co[i]);
        if (best.empty() || k < best) best = k;
    }
    return best;
}

static std::vector<ZbllCase> enumerate_zbll() {
    // Subsets are named after the OCLL case that has the same corner pattern.
    std::map<std::string, std::string> setName;
    const std::pair<const char*, const char*> ocll[] = {
        {"21", "H"}, {"22", "Pi"}, {"23", "U"}, {"24", "T"}, {"25", "L"}, {"26", "AS"}, {"27", "S"}};
    for (auto [num, name] : ocll)
        for (auto& cs : OLL) if (cs.name == num)
            setName[co_key(to_cubie(normalize(Cube3::solved().apply(parse_alg(cs.alg).perm.inverse()))))] = name;
    setName["0000"] = "PLL";

    std::map<std::string, CubieCube> classes;  // case key → representative
    std::array<int, 4> cp = {0, 1, 2, 3};
    do {
        std::array<int, 4> ep = {0, 1, 2, 3};
        do {
            for (int co = 0; co < 27; ++co) {
                CubieCube c = CubieCube::solved();
                int sum = 0, x = co;
                for (int i = 0; i < 3; ++i) { c.co[i] = x % 3; x /= 3; sum += c.co[i]; }
                c.co[3] = (3 - sum % 3) % 3;
                for (int i = 0; i < 4; ++i) { c.cp[i] = cp[i]; c.ep[i] = ep[i]; }
                // corner and edge permutation parities must match
                auto parity = [](const std::array<int,4>& p) {
                    int inv = 0;
                    for (int i = 0; i < 4; ++i) for (int j = i + 1; j < 4; ++j) inv += p[i] > p[j];
                    return inv & 1;
                };
                if (parity(cp) != parity(ep)) continue;
                if (c == CubieCube::solved()) continue;
                std::string key = case_key(c, true);
                if (key == case_key(CubieCube::solved(), true)) continue;
                classes.emplace(key, c);
            }
        } while (std::next_permutation(ep.begin(), ep.end()));
    } while (std::next_permutation(cp.begin(), cp.end()));

    std::map<std::string, int> counter;
    std::vector<ZbllCase> out;
    for (auto& [key, c] : classes) {
        std::string set = setName.at(co_key(c));
        out.push_back({set, ++counter[set], c});
    }
    std::stable_sort(out.begin(), out.end(), [](const ZbllCase& x, const ZbllCase& y) {
        static const std::vector<std::string> order = {"T", "U", "L", "H", "Pi", "S", "AS", "PLL"};
        auto ix = [&](const std::string& s) { return std::find(order.begin(), order.end(), s) - order.begin(); };
        return ix(x.set) < ix(y.set);
    });
    return out;
}

// Mirror (left-right reflection) and inverse of a search-move sequence. If W
// solves a case, mirror(W) solves the mirrored case and inverse(W) the
// inverse case, and optimal solutions map one-to-one — so only about a
// quarter of the cases need a search.
static int neg(int p) { return (4 - p) % 4; }
static std::vector<int> mirror_moves(const std::vector<int>& p) {
    std::vector<int> r;
    for (int m : p) {
        const auto& v = MV[m];  // L^a R^b ↔ L^-b R^-a; other axes just reverse
        r.push_back(v.axis == 2 ? MV_ID[2][neg(v.b)][neg(v.a)] : MV_ID[v.axis][neg(v.a)][neg(v.b)]);
    }
    return r;
}
static std::vector<int> inverse_moves(const std::vector<int>& p) {
    std::vector<int> r;
    for (auto it = p.rbegin(); it != p.rend(); ++it) {
        const auto& v = MV[*it];
        r.push_back(MV_ID[v.axis][neg(v.a)][neg(v.b)]);
    }
    return r;
}
// Transform 1 = mirror, 2 = inverse, 3 = both. Returns (pre-AUF, moves, post-AUF).
static Hit transform_hit(const Hit& h, int t) {
    Hit r{h.a, h.moves, {}, h.b};
    if (t & 1) { r.moves = mirror_moves(r.moves); r.a = neg(r.a); r.b = neg(r.b); }
    if (t & 2) { r.moves = inverse_moves(r.moves); std::swap(r.a, r.b); r.a = neg(r.a); r.b = neg(r.b); }
    return r;
}
// The state that (U^a) W (U^b) solves.
static CubieCube solved_by(const Hit& h) {
    CubieCube c = U_POW[h.a];
    for (int m : h.moves) c = c * MV[m].c;
    return inverse(c * U_POW[h.b]);
}

// Output:
//   stdout            markdown summary + one row per case
//   <dataFile>        every optimal solution of every case (one per line)
static size_t pl_size(const StateResult& r) { return r.plus.size(); }

static void run_zbll(const Search& proto, const std::string& dataFile, int from, int to,
                     int plus, int plusMaxOpt) {
    auto cases = enumerate_zbll();
    std::map<std::string, int> sizes;
    for (auto& c : cases) sizes[c.set]++;
    std::fprintf(stderr, "ZBLL cases: %zu (", cases.size());
    for (auto& [k, v] : sizes) std::fprintf(stderr, "%s %d ", k.c_str(), v);
    std::fprintf(stderr, ")\n");

    std::ofstream data(dataFile, std::ios::app);
    std::printf("| Case | Optimal %s | # optimal | First faces | Last faces | Setup (inverse of a solution) | Shortest, most comfortable solution |\n",
                METRIC_NAMES[proto.metric]);
    std::printf("|---|--:|--:|---|---|---|---|\n");
    std::map<std::string, int> byKey;
    for (int i = 0; i < (int)cases.size(); ++i) byKey[case_key(cases[i].state, true)] = i;
    std::vector<std::unique_ptr<StateResult>> results(cases.size());
    std::vector<std::string> origin(cases.size());
    auto name_of = [&](int i) { return cases[i].set + "-" + std::to_string(cases[i].index); };
    for (int i = from; i < (int)cases.size() && i < to; ++i) {
        auto& zc = cases[i];
        auto t0 = std::chrono::steady_clock::now();
        if (!results[i]) {
            results[i] = std::make_unique<StateResult>(solve_state(zc.state, true, proto, plus, plusMaxOpt));
            origin[i] = "searched";
            // Derive the mirror / inverse / mirror-inverse images.
            static const char* tname[4] = {"", "mirror", "inverse", "mirror inverse"};
            for (int t = 1; t <= 3; ++t) {
                const StateResult& src = *results[i];
                auto key = case_key(solved_by(transform_hit(src.hits.front(), t)), true);
                auto it = byKey.find(key);
                if (it == byKey.end()) throw std::runtime_error("image case not found");
                int j = it->second;
                if (results[j] || j < from || j >= to) continue;
                auto map_all = [&](const std::vector<Hit>& hs) {
                    std::vector<std::vector<int>> out;
                    for (auto& h : hs) out.push_back(transform_hit(h, t).moves);
                    return expand_hits(cases[j].state, true, proto.metric, out);
                };
                auto d = std::make_unique<StateResult>();
                d->opt = src.opt;
                d->capped = src.capped;
                d->hits = map_all(src.hits);
                for (auto& pl : src.plus) d->plus.push_back(map_all(pl));
                bool same = d->hits.size() == src.hits.size();
                for (size_t k = 0; k < pl_size(src) && same; ++k) same = d->plus[k].size() == src.plus[k].size();
                if (!same) throw std::runtime_error("symmetry changed the number of solutions");
                results[j] = std::move(d);
                origin[j] = std::string(tname[t]) + " of " + name_of(i);
            }
        }
        const StateResult& r = *results[i];
        double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        std::string name = zc.set + "-" + std::to_string(zc.index);
        std::set<char> first, last;
        for (auto& h : r.hits) { first.insert(h.tokens.front()[0]); last.insert(h.tokens.back()[0]); }
        std::string fs(first.begin(), first.end()), ls(last.begin(), last.end());
        // Setup: undo the chosen solution, then its pre-AUF.
        const Hit& best = r.hits.front();
        std::string setup = join(invert_tokens(best.tokens));
        if (best.a) setup += std::string(" ") + AUF_NAMES[(4 - best.a) % 4];
        std::printf("| %s | **%d** | %zu%s | %s | %s | `%s` | `%s` |\n", name.c_str(), r.opt, r.hits.size(),
                    r.capped ? "+" : "", fs.c_str(), ls.c_str(), setup.c_str(), hit_to_string(best).c_str());
        std::fflush(stdout);
        data << "# " << name << "  optimal " << r.opt << " " << METRIC_NAMES[proto.metric]
             << "  setup: " << setup << "\n";
        for (auto& h : r.hits) data << hit_to_string(h, true) << "\n";
        data.flush();
        for (size_t i = 0; i < r.plus.size(); ++i) {
            std::ofstream more(dataFile + ".plus" + std::to_string(i + 1), std::ios::app);
            more << "# " << name << "  " << r.opt + 1 + i << " " << METRIC_NAMES[proto.metric]
                 << " (optimal " << r.opt << ")  setup: " << setup << "\n";
            for (auto& h : r.plus[i]) more << hit_to_string(h, true) << "\n";
        }
        std::fprintf(stderr, "  %s: %d, %zu solutions%s (%.1fs, %s)\n", name.c_str(), r.opt, r.hits.size(),
                     r.plus.empty() ? "" : (", +1: " + std::to_string(r.plus[0].size())).c_str(), secs,
                     origin[i].c_str());
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FMC finish: best ending for a skeleton, counting cancellations
// ═════════════════════════════════════════════════════════════════════════════
// Nothing is free in FMC (AUF included), so this solves to the exact solved
// state. For k = 0..back it drops the last k skeleton moves and finds every
// optimal solution from there; the total is the length of skeleton-minus-k
// followed by the solution after merging turns of the same axis at the join.

// Face-turn tokens → search moves (merging same-axis neighbours).
static std::vector<int> tokens_to_moves(const std::vector<std::string>& toks) {
    std::vector<int> out;
    for (auto& t : toks) {
        int f = std::string(FACE_NAMES).find(t[0]);
        if (f < 0 || f > 5) throw std::runtime_error("finish: only face turns (U D F B L R) are supported: " + t);
        int p = t.size() == 1 ? 1 : t[1] == '2' ? 2 : 3;
        int ax = f / 2, a = (f % 2 == 0) ? p : 0, b = (f % 2 == 1) ? p : 0;
        if (!out.empty() && MV[out.back()].axis == ax) {
            const auto& v = MV[out.back()];
            a = (a + v.a) % 4; b = (b + v.b) % 4;
            out.pop_back();
            if (!a && !b) continue;
        }
        out.push_back(MV_ID[ax][a][b]);
    }
    return out;
}

static std::vector<std::string> split(const std::string& s) {
    std::istringstream in(s);
    std::vector<std::string> out;
    for (std::string t; in >> t;) out.push_back(t);
    return out;
}

static void run_finish(const Search& proto, const std::string& scramble, const std::string& skeleton, int back) {
    auto sk = split(skeleton);
    std::printf("Scramble: %s\nSkeleton: %s (%zu moves)\n\n", scramble.c_str(), skeleton.c_str(), sk.size());
    int bestTotal = 1 << 30;
    std::string bestLine;
    for (int k = 0; k <= back && k <= (int)sk.size(); ++k) {
        std::vector<std::string> kept(sk.begin(), sk.end() - k);
        Cube3 cube = Cube3::solved().apply(parse_alg(scramble + " " + join(kept)).perm);
        CubieCube start = to_cubie(normalize(cube));
        auto t0 = std::chrono::steady_clock::now();
        std::vector<std::vector<int>> found;
        int opt = -1;
        for (int depth = 0; found.empty() && depth <= 30; ++depth) {
            if (depth == 0) { if (start == CubieCube::solved()) found.push_back({}); opt = 0; continue; }
            #pragma omp parallel for schedule(dynamic, 1)
            for (int m = 0; m < (int)MV.size(); ++m) {
                Search s = proto;
                int cm = s.cost(m);
                if (cm > depth) continue;
                s.path = {m};
                s.dfs(start * MV[m].c, depth - cm, MV[m].axis, false);
                #pragma omp critical
                found.insert(found.end(), s.solutions.begin(), s.solutions.end());
            }
            opt = depth;
        }
        // Total after merging at the join; keep the best few.
        struct Fin { int total; std::string text; };
        std::vector<Fin> fins;
        for (auto& sol : found) {
            auto toks = to_tokens(sol, HTM);
            Cube3 check = cube.apply(parse_alg(join(toks)).perm);
            if (!normalize(check).is_solved()) throw std::runtime_error("finish failed sticker verification");
            std::vector<std::string> all = kept;
            all.insert(all.end(), toks.begin(), toks.end());
            int total = 0;
            for (int m : tokens_to_moves(all)) total += MV[m].cost[HTM];
            fins.push_back({total, join(toks)});
        }
        std::sort(fins.begin(), fins.end(), [](const Fin& x, const Fin& y) { return x.total < y.total; });
        double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        std::printf("drop last %d: finish needs %d, %zu optimal finishes, best total %d  (%.1fs)\n",
                    k, opt, fins.size(), fins.empty() ? -1 : fins[0].total, secs);
        for (size_t i = 0; i < fins.size() && i < 5; ++i)
            std::printf("    total %2d   %s | %s\n", fins[i].total, join(kept).c_str(), fins[i].text.c_str());
        if (!fins.empty() && fins[0].total < bestTotal) {
            bestTotal = fins[0].total;
            bestLine = join(kept) + " | " + fins[0].text;
        }
        std::fflush(stdout);
    }
    std::printf("\nBest: %d moves   %s\n", bestTotal, bestLine.c_str());
}

int main(int argc, char** argv) {
    std::string which = "all", cacheDir = ".", only;
    Metric metric = HTM;
    int syms = 0, nearDepth = 6, zbllFrom = 0, zbllTo = 1 << 30;
    std::string zbllData = "zbll_solutions.txt";
    std::vector<std::string> positional;
    int back = 2, plus = 0, plusMaxOpt = 13;
    NearTable near;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--qtm") metric = QTM;
        else if (a == "--stm") metric = STM;
        else if (a == "--cache" && i + 1 < argc) cacheDir = argv[++i];
        else if (a == "--syms" && i + 1 < argc) syms = std::atoi(argv[++i]);
        else if (a == "--only" && i + 1 < argc) only = argv[++i];
        else if (a == "--near" && i + 1 < argc) nearDepth = std::atoi(argv[++i]);
        else if (a == "--from" && i + 1 < argc) zbllFrom = std::atoi(argv[++i]);
        else if (a == "--to" && i + 1 < argc) zbllTo = std::atoi(argv[++i]);
        else if (a == "--data" && i + 1 < argc) zbllData = argv[++i];
        else if (a == "--back" && i + 1 < argc) back = std::atoi(argv[++i]);
        else if (a == "--plus" && i + 1 < argc) plus = std::atoi(argv[++i]);
        else if (a == "--plus-max-opt" && i + 1 < argc) plusMaxOpt = std::atoi(argv[++i]);
        else positional.push_back(a);
    }
    if (!positional.empty()) which = positional[0];
    init_facelets();
    init_moves();
    init_ymap();
    init_syms();
    init_axis_moves();
    for (int i = 0; i < 4; ++i) U_POW[i] = i ? U_POW[i - 1] * MOVES[U * 3] : CubieCube::solved();
    {   // round trip sanity check of the two cube models
        CubieCube c = CubieCube::solved();
        for (int m : {5, 9, 13, 1, 17, 7, 3, 11, 15}) c = c * MOVES[m];
        if (!(to_cubie(from_cubie(c)) == c)) { std::cerr << "cubie/sticker round trip failed\n"; return 1; }
    }

    try {
        validate(PLL, true);
        validate(OLL, false);
    } catch (std::exception& e) {
        std::cerr << "algorithm table error: " << e.what() << "\n";
        return 1;
    }
    std::fprintf(stderr, "algorithm tables OK: %zu distinct PLL cases, %zu distinct OLL cases\n",
                 PLL.size(), OLL.size());

    // PLL: full corner database + two halves of the edges.
    Pattern pllC("pll_corners", true, {0,1,2,3,4,5,6,7}, false);
    Pattern pllE1("pll_edges_a", false, {0,1,2,3,4,5,6}, false);
    Pattern pllE2("pll_edges_b", false, {5,6,7,8,9,10,11}, false);
    // OLL: last-layer pieces are interchangeable, only their orientation matters.
    Pattern ollC("oll_corners", true, {4,5,6,7}, true);
    Pattern ollE1("oll_edges_d", false, {4,5,6,7}, true);
    Pattern ollE2("oll_edges_e", false, {8,9,10,11}, true);

    const char* mname = METRIC_NAMES[metric];
    auto make_search = [&](std::vector<Pattern*> pdbs, Goal goal) {
        for (auto* p : pdbs) p->build(cacheDir, metric);
        Search search;
        search.pdbs = pdbs;
        search.goal = goal;
        search.metric = metric;
        for (int k = 1; k <= syms && k < (int)SYMS.size(); ++k) search.symIdx.push_back(k);
        // The table holds distances to the solved cube, so it only applies
        // when that is the goal (not for OLL, whose goal is a set of states).
        if (nearDepth > 0 && goal == Goal::Solved) {
            if (near.depth != nearDepth) near.build(nearDepth, metric);
            search.near = &near;
        }
        return search;
    };
    auto run = [&](const std::vector<Case>& cases, bool pll, std::vector<Pattern*> pdbs) {
        Search search = make_search(pdbs, pll ? Goal::Solved : Goal::OLL);
        std::printf("\n%s  (optimal in %s, AUF free)\n", pll ? "PLL" : "OLL", mname);
        std::printf("| Case | Common algorithm | HTM | STM | QTM | Optimal %s | # optimal | An optimal solution |\n", mname);
        std::printf("|---|---|---|---|---|---|---|---|\n");
        for (auto& cs : cases) {
            if (!only.empty() && cs.name != only) continue;
            Result r = solve_case(cs, pll, search);
            std::printf("| %s | `%s` | %d | %d | %d | **%d** | %zu%s | `%s` |\n", r.name.c_str(),
                        r.alg.c_str(), r.info.htm, r.info.stm, r.info.qtm, r.opt, r.optCount,
                        r.capped ? "+" : "", r.optAlg.c_str());
            std::fflush(stdout);
            std::fprintf(stderr, "  %s %s: %d (%.1fs)\n", pll ? "PLL" : "OLL", r.name.c_str(), r.opt, r.secs);
        }
        for (auto* p : pdbs) { p->dist.clear(); p->dist.shrink_to_fit(); }
    };
    if (which == "pll" || which == "all") run(PLL, true, {&pllC, &pllE1, &pllE2});
    if (which == "zbll") {
        Search search = make_search({&pllC, &pllE1, &pllE2}, Goal::Solved);
        run_zbll(search, zbllData, zbllFrom, zbllTo, plus, plusMaxOpt);
    }
    if (which == "oll" || which == "all") run(OLL, false, {&ollC, &ollE1, &ollE2});
    if (which == "finish") {
        if (positional.size() != 3) { std::cerr << "usage: ll_optimal finish \"<scramble>\" \"<skeleton>\" [--back K]\n"; return 1; }
        Search search = make_search({&pllC, &pllE1, &pllE2}, Goal::Solved);
        run_finish(search, positional[1], positional[2], back);
    }
    return 0;
}
