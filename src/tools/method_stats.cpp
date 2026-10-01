// method_stats — step-optimal move counts of speedsolving methods.
//
// For N uniformly random cube states, every method is simulated step by step,
// and each step is solved in the fewest possible moves (IDA* with pattern
// databases built for that step's goal). Where a solver has a free choice
// (which F2L slot next), the shortest option is taken. Every move counts,
// AUF included, and the final state of every solve is re-checked on the
// sticker model.
//
// This measures the methods' structure, not human performance: real solves
// are longer (steps are rarely optimal), but also plan across steps
// (XCross, last-layer influence), which step-optimal solving ignores.
//
// Usage: method_stats [N] [--stm] [--seed S] [--near D] [--cache DIR] [--csv FILE]

#include "cube_engine.h"

#include <cmath>
#include <functional>
#include <mutex>
#include <numeric>
#include <random>

// ═════════════════════════════════════════════════════════════════════════════
// Step goals
// ═════════════════════════════════════════════════════════════════════════════
// Piece ids (Kociemba): corners URF UFL ULB UBR DFR DLF DBL DRB = 0..7,
// edges UR UF UL UB DR DF DL DB FR FL BL BR = 0..11.

struct StepGoal {
    std::string name;
    std::vector<int> C, E;   // pieces that must be home; newest pieces last
    bool eo = false;         // all edges oriented
    bool co = false;         // all corners oriented
    bool xfree = false;      // home up to an M-slice offset (Roux, corners-first)
    bool solved = false;     // the whole cube
};

// Targets for xfree goals: pieces solved while the M-slice centers are off by
// k quarter turns, seen from the centers' frame.
static CubieCube XFRAME[4];
static void init_xframe() {
    for (int k = 0; k < 4; ++k) {
        std::string alg;
        for (int i = 0; i < k; ++i) alg += "M ";
        XFRAME[k] = to_cubie(normalize(Cube3::solved().apply(parse_alg(alg).perm)));
    }
}

static bool reached(const CubieCube& t, const StepGoal& g) {
    if (g.solved) return t == CubieCube::solved();
    if (g.eo) for (int i = 0; i < 12; ++i) if (t.eo[i]) return false;
    if (g.co) for (int i = 0; i < 8; ++i) if (t.co[i]) return false;
    for (int k = 0; k < (g.xfree ? 4 : 1); ++k) {
        const CubieCube& T = XFRAME[k];
        bool ok = true;
        for (int c : g.C) {
            for (int i = 0; i < 8 && ok; ++i)
                if (t.cp[i] == c) ok = T.cp[i] == c && t.co[i] == T.co[i];
            if (!ok) break;
        }
        for (int e : g.E) {
            if (!ok) break;
            for (int i = 0; i < 12 && ok; ++i)
                if (t.ep[i] == e) ok = T.ep[i] == e && t.eo[i] == T.eo[i];
        }
        if (ok) return true;
    }
    return false;
}

// ═════════════════════════════════════════════════════════════════════════════
// Goal pattern databases: tracked corners × tracked edges, multi-source
// ═════════════════════════════════════════════════════════════════════════════

struct GoalPattern {
    Pattern cp, ep;  // corner part, edge part (either may track nothing)
    bool xfree;
    std::vector<uint8_t> dist;
    std::string key;

    GoalPattern(std::vector<int> C, bool coAll, std::vector<int> E, bool eoAll, bool xf)
        : cp("c", true, C, coAll), ep("e", false, E, eoAll), xfree(xf) {
        key = "g";
        for (int c : C) key += "c" + std::to_string(c);
        for (int e : E) key += "e" + std::to_string(e);
        key += std::string(coAll ? "_co" : "") + (eoAll ? "_eo" : "") + (xf ? "_x" : "");
    }
    uint64_t size() const { return cp.size() * ep.size(); }
    uint64_t index(const CubieCube& c) const { return cp.index(c.cp, c.co) * ep.size() + ep.index(c.ep, c.eo); }
    int h(const CubieCube& c) const { return dist[index(c)]; }

    void build(const std::string& dir, Metric metric) {
        static const char* suffix[3] = {"", "_stm", "_qtm"};
        std::string file = dir + "/" + key + suffix[metric] + ".pdb";
        {
            std::ifstream in(file, std::ios::binary);
            if (in) {
                dist.resize(size());
                in.read((char*)dist.data(), size());
                if (in.gcount() == (std::streamsize)size()) return;
            }
        }
        auto t0 = std::chrono::steady_clock::now();
        dist.assign(size(), 0xFF);
        for (int k = 0; k < (xfree ? 4 : 1); ++k) dist[index(XFRAME[k])] = 0;
        std::vector<int> units;
        for (int m = 0; m < (int)MV.size(); ++m) if (MV[m].cost[metric] == 1) units.push_back(m);
        for (int d = 0;; ++d) {
            uint64_t added = 0;
            #pragma omp parallel for schedule(dynamic, 65536) reduction(+:added)
            for (int64_t i = 0; i < (int64_t)size(); ++i) {
                if (dist[i] != d) continue;
                CubieCube s;
                cp.decode(i / ep.size(), s.cp, s.co);
                ep.decode(i % ep.size(), s.ep, s.eo);
                for (int m : units) {
                    uint64_t j = index(s * MV[m].c);
                    if (dist[j] == 0xFF) { dist[j] = d + 1; ++added; }
                }
            }
            if (added == 0) break;
        }
        auto secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        std::fprintf(stderr, "  built %-40s %s %10llu entries, max %d, %.1fs\n", key.c_str(),
                     METRIC_NAMES[metric], (unsigned long long)size(),
                     *std::max_element(dist.begin(), dist.end()), secs);
        std::ofstream out(file, std::ios::binary);
        out.write((const char*)dist.data(), size());
    }
};

// Databases for a goal: its corners, its edges in chunks, and a mixed one
// covering the newest pieces (e.g. the F2L pair just being inserted).
struct PatternStore {
    std::string dir;
    Metric metric;
    std::map<std::string, std::unique_ptr<GoalPattern>> all;

    const GoalPattern* get(std::vector<int> C, bool co, std::vector<int> E, bool eo, bool xf) {
        auto p = std::make_unique<GoalPattern>(C, co, E, eo, xf);
        auto it = all.find(p->key);
        if (it != all.end()) return it->second.get();
        p->build(dir, metric);
        auto* raw = p.get();
        all[p->key] = std::move(p);
        return raw;
    }
    std::vector<const GoalPattern*> for_goal(const StepGoal& g) {
        std::vector<const GoalPattern*> out;
        if (!g.C.empty() || g.co) out.push_back(get(g.C, g.co, {}, false, g.xfree));
        size_t chunk = g.eo ? 4 : 6;
        for (size_t i = 0; i < g.E.size(); i += chunk) {
            std::vector<int> part(g.E.begin() + i, g.E.begin() + std::min(g.E.size(), i + chunk));
            out.push_back(get({}, false, part, g.eo, g.xfree));
        }
        if (g.E.empty() && g.eo) out.push_back(get({}, false, {}, true, false));
        if (!g.C.empty() && !g.E.empty()) {
            auto tail = [](const std::vector<int>& v, size_t n) {
                return std::vector<int>(v.end() - std::min(n, v.size()), v.end());
            };
            out.push_back(get(tail(g.C, 1), false, tail(g.E, 4), false, g.xfree));
            if (g.C.size() > 1) out.push_back(get(tail(g.C, 2), false, tail(g.E, 3), false, g.xfree));
        }
        return out;
    }
};

// ═════════════════════════════════════════════════════════════════════════════
// Step search: IDA*, first optimal solution
// ═════════════════════════════════════════════════════════════════════════════

struct Solvers {
    Metric metric;
    PatternStore store;
    Pattern* full[3];            // corners + two 7-edge sets, for the solved goal
    const NearTable* near = nullptr;
};

struct StepSearch {
    const StepGoal& g;
    std::vector<const GoalPattern*> pats;
    const Solvers& S;
    std::vector<int> path;

    int h(const CubieCube& c) const {
        int v = 0;
        for (auto* p : pats) v = std::max(v, p->h(c));
        if (g.solved) for (auto* p : S.full) v = std::max(v, p->h(c));
        return v;
    }
    bool dfs(const CubieCube& c, int left, int lastAxis) {
        if (left == 0) return reached(c, g);
        if (g.solved && S.near && left <= S.near->depth) {
            if (S.near->get(c) != left) return false;  // exact below the table depth
        } else if (h(c) > left) return false;
        for (int m = 0; m < (int)MV.size(); ++m) {
            if (MV[m].axis == lastAxis) continue;
            int cm = MV[m].cost[S.metric];
            if (cm > left) continue;
            path.push_back(m);
            if (dfs(c * MV[m].c, left - cm, MV[m].axis)) return true;
            path.pop_back();
        }
        return false;
    }
    std::vector<int> solve(const CubieCube& start) {
        for (int depth = 0;; ++depth) {
            path.clear();
            if (dfs(start, depth, -1)) return path;
        }
    }
};

static int cost_of(const std::vector<int>& p, Metric metric) {
    int s = 0;
    for (int m : p) s += MV[m].cost[metric];
    return s;
}

// ═════════════════════════════════════════════════════════════════════════════
// Methods
// ═════════════════════════════════════════════════════════════════════════════

struct Slot { int corner, edge; const char* name; };
static const Slot SLOTS[4] = {{4, 8, "FR"}, {5, 9, "FL"}, {6, 10, "BL"}, {7, 11, "BR"}};

static StepGoal goal(std::string name, std::vector<int> C, std::vector<int> E,
                     bool eo = false, bool co = false, bool xfree = false) {
    StepGoal g{name, C, E, eo, co, xfree, false};
    return g;
}
static StepGoal solved_goal(std::string name) { StepGoal g; g.name = name; g.solved = true; return g; }

// One method = a list of steps; a step may be a greedy choice among goals.
struct Step {
    std::string name;
    std::function<std::vector<StepGoal>(const std::vector<int>& done)> options;
    int slotsTaken = 0;  // > 0: this step picks an F2L slot (records which one)
};

struct Method { std::string name; std::vector<Step> steps; };

static const std::vector<int> CROSS = {4, 5, 6, 7};

// Cross + earlier slots (sorted) + the new slot last, optionally + EO.
static StepGoal f2l_goal(const std::string& name, std::vector<int> prev, int slot, bool eo) {
    std::sort(prev.begin(), prev.end());
    prev.push_back(slot);
    std::vector<int> C, E = CROSS;
    for (int s : prev) { C.push_back(SLOTS[s].corner); E.push_back(SLOTS[s].edge); }
    return goal(name, C, E, eo);
}

static Step pair_step(const std::string& name, bool eo = false) {
    return {name, [name, eo](const std::vector<int>& done) {
        std::vector<StepGoal> opts;
        for (int s = 0; s < 4; ++s) {
            if (std::find(done.begin(), done.end(), s) != done.end()) continue;
            opts.push_back(f2l_goal(name, done, s, eo));
        }
        return opts;
    }, 1};
}
static Step fixed_step(StepGoal g) {
    return {g.name, [g](const std::vector<int>&) { return std::vector<StepGoal>{g}; }, 0};
}

static std::vector<Method> methods() {
    const std::vector<int> F2L_C = {4, 5, 6, 7}, F2L_E = {4, 5, 6, 7, 8, 9, 10, 11};
    std::vector<Method> ms;
    auto cross = fixed_step(goal("cross", {}, CROSS));
    ms.push_back({"CFOP", {cross, pair_step("pair 1"), pair_step("pair 2"), pair_step("pair 3"),
                           pair_step("pair 4"),
                           fixed_step(goal("OLL", F2L_C, F2L_E, true, true)),
                           fixed_step(solved_goal("PLL"))}});
    ms.push_back({"CFOP + 1LLL", {cross, pair_step("pair 1"), pair_step("pair 2"), pair_step("pair 3"),
                                  pair_step("pair 4"), fixed_step(solved_goal("1LLL"))}});
    ms.push_back({"ZB (ZBLS + ZBLL)", {cross, pair_step("pair 1"), pair_step("pair 2"), pair_step("pair 3"),
                                       pair_step("ZBLS", true), fixed_step(solved_goal("ZBLL"))}});
    // ZZ: EOLine, left block, right block (EO kept throughout), ZBLL.
    ms.push_back({"ZZ (ZBLL)", {
        fixed_step(goal("EOLine", {}, {5, 7}, true)),
        fixed_step(goal("left block", {5, 6}, {5, 7, 6, 9, 10}, true)),
        fixed_step(goal("right block", {5, 6, 4, 7}, {5, 7, 6, 9, 10, 4, 8, 11}, true)),
        fixed_step(solved_goal("ZBLL"))}});
    // Petrus: 2x2x2 at DBL, 2x2x3, EO, finish F2L, ZBLL.
    ms.push_back({"Petrus (ZBLL)", {
        fixed_step(goal("2x2x2", {6}, {6, 7, 10})),
        fixed_step(goal("2x2x3", {6, 5}, {6, 7, 10, 5, 9})),
        fixed_step(goal("EO", {6, 5}, {6, 7, 10, 5, 9}, true)),
        fixed_step(goal("F2L", {6, 5, 4, 7}, {6, 7, 10, 5, 9, 4, 8, 11}, true)),
        fixed_step(solved_goal("ZBLL"))}});
    // Roux: blocks and corners only need to agree with each other (M slice free).
    ms.push_back({"Roux", {
        fixed_step(goal("first block", {5, 6}, {6, 9, 10}, false, false, true)),
        fixed_step(goal("second block", {5, 6, 4, 7}, {6, 9, 10, 4, 8, 11}, false, false, true)),
        fixed_step(goal("CMLL", {5, 6, 4, 7, 0, 1, 2, 3}, {6, 9, 10, 4, 8, 11}, false, false, true)),
        fixed_step(solved_goal("LSE"))}});
    // Corners first, then edges layer by layer, last six edges like Roux.
    ms.push_back({"Corners first", {
        fixed_step(goal("corners", {0, 1, 2, 3, 4, 5, 6, 7}, {}, false, false, true)),
        fixed_step(goal("L edges", {0, 1, 2, 3, 4, 5, 6, 7}, {2, 6, 9, 10}, false, false, true)),
        fixed_step(goal("R edges", {0, 1, 2, 3, 4, 5, 6, 7}, {2, 6, 9, 10, 0, 4, 8, 11}, false, false, true)),
        fixed_step(solved_goal("L6E"))}});
    return ms;
}

// Every goal a method can produce (so databases are built before solving).
static std::vector<StepGoal> all_goals(const Method& m) {
    std::vector<StepGoal> out;
    std::function<void(size_t, std::vector<int>)> walk = [&](size_t i, std::vector<int> done) {
        if (i == m.steps.size()) return;
        auto opts = m.steps[i].options(done);
        for (auto& g : opts) out.push_back(g);
        if (m.steps[i].slotsTaken) {
            for (int s = 0; s < 4; ++s)
                if (std::find(done.begin(), done.end(), s) == done.end()) {
                    auto d = done; d.push_back(s);
                    walk(i + 1, d);
                }
        } else walk(i + 1, done);
    };
    walk(0, {});
    return out;
}

// ═════════════════════════════════════════════════════════════════════════════
// Driver
// ═════════════════════════════════════════════════════════════════════════════

static CubieCube random_state(std::mt19937_64& rng) {
    CubieCube c = CubieCube::solved();
    std::shuffle(c.cp, c.cp + 8, rng);
    std::shuffle(c.ep, c.ep + 12, rng);
    auto parity = [](const uint8_t* p, int n) {
        int inv = 0;
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) inv += p[i] > p[j];
        return inv & 1;
    };
    if (parity(c.cp, 8) != parity(c.ep, 12)) std::swap(c.ep[0], c.ep[1]);
    int cs = 0, es = 0;
    for (int i = 0; i < 7; ++i) { c.co[i] = rng() % 3; cs += c.co[i]; }
    c.co[7] = (3 - cs % 3) % 3;
    for (int i = 0; i < 11; ++i) { c.eo[i] = rng() % 2; es += c.eo[i]; }
    c.eo[11] = es % 2;
    return c;
}

struct SolveRecord { std::vector<int> steps; int total = 0; };

// Search moves as plain face turns (a slice pair like L' R is the same move
// as M in the centers' frame), for the independent sticker check.
static std::string face_turns(const std::vector<int>& p) {
    static const char* suf[4] = {"", "", "2", "'"};
    std::string s;
    for (int m : p) {
        const auto& v = MV[m];
        if (v.a) s += std::string(" ") + FACE_NAMES[2 * v.axis] + suf[v.a];
        if (v.b) s += std::string(" ") + FACE_NAMES[2 * v.axis + 1] + suf[v.b];
    }
    return s;
}

int main(int argc, char** argv) {
    int N = 100, nearDepth = -1;
    uint64_t seed = 1;
    Metric metric = HTM;
    std::string cacheDir = ".", csv;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--stm") metric = STM;
        else if (a == "--seed" && i + 1 < argc) seed = std::stoull(argv[++i]);
        else if (a == "--near" && i + 1 < argc) nearDepth = std::atoi(argv[++i]);
        else if (a == "--cache" && i + 1 < argc) cacheDir = argv[++i];
        else if (a == "--csv" && i + 1 < argc) csv = argv[++i];
        else N = std::atoi(a.c_str());
    }
    if (nearDepth < 0) nearDepth = metric == HTM ? 7 : 6;
    init_facelets();
    init_moves();
    init_syms();
    init_axis_moves();
    init_xframe();

    Solvers S;
    S.metric = metric;
    S.store.dir = cacheDir;
    S.store.metric = metric;
    Pattern fc("pll_corners", true, {0,1,2,3,4,5,6,7}, false);
    Pattern fe1("pll_edges_a", false, {0,1,2,3,4,5,6}, false);
    Pattern fe2("pll_edges_b", false, {5,6,7,8,9,10,11}, false);
    for (auto* p : {&fc, &fe1, &fe2}) p->build(cacheDir, metric);
    S.full[0] = &fc; S.full[1] = &fe1; S.full[2] = &fe2;
    NearTable near;
    if (nearDepth > 0) { near.build(nearDepth, metric); S.near = &near; }

    auto ms = methods();
    std::map<std::string, std::vector<const GoalPattern*>> patsFor;  // goal name+pieces → databases
    auto goal_key = [](const StepGoal& g) {
        std::string k = g.name + "|";
        for (int c : g.C) k += std::to_string(c) + ",";
        k += "|";
        for (int e : g.E) k += std::to_string(e) + ",";
        return k + (g.eo ? "eo" : "") + (g.co ? "co" : "") + (g.xfree ? "x" : "") + (g.solved ? "s" : "");
    };
    for (auto& m : ms)
        for (auto& g : all_goals(m))
            if (!g.solved && !patsFor.count(goal_key(g))) patsFor[goal_key(g)] = S.store.for_goal(g);
    // Greedy slot orders can produce any order of the same pieces; make sure
    // every order used at run time has its databases (built lazily otherwise).
    std::fprintf(stderr, "databases ready (%zu)\n", S.store.all.size());

    std::mt19937_64 rng(seed);
    std::vector<CubieCube> states(N);
    for (auto& s : states) s = random_state(rng);

    // results[method][scramble]
    std::vector<std::vector<SolveRecord>> results(ms.size(), std::vector<SolveRecord>(N));
    std::mutex lazyMutex;
    auto t0 = std::chrono::steady_clock::now();
    int doneCount = 0;
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < N; ++i) {
        for (size_t mi = 0; mi < ms.size(); ++mi) {
            const Method& m = ms[mi];
            CubieCube cur = states[i];
            std::vector<int> slots, allMoves;
            SolveRecord rec;
            for (auto& st : m.steps) {
                auto opts = st.options(slots);
                int best = 1 << 30, bestIdx = -1;
                std::vector<int> bestPath;
                for (size_t oi = 0; oi < opts.size(); ++oi) {
                    std::vector<const GoalPattern*> pats;
                    if (!opts[oi].solved) {
                        std::lock_guard<std::mutex> lock(lazyMutex);
                        auto key = goal_key(opts[oi]);
                        if (!patsFor.count(key)) patsFor[key] = S.store.for_goal(opts[oi]);
                        pats = patsFor[key];
                    }
                    StepSearch ss{opts[oi], pats, S, {}};
                    auto p = ss.solve(cur);
                    int c = cost_of(p, metric);
                    if (c < best) { best = c; bestIdx = (int)oi; bestPath = p; }
                }
                if (st.slotsTaken) {
                    // which slot did the chosen option add?
                    for (int s = 0; s < 4; ++s)
                        if (std::find(slots.begin(), slots.end(), s) == slots.end() &&
                            opts[bestIdx].C.back() == SLOTS[s].corner) { slots.push_back(s); break; }
                }
                for (int mv : bestPath) cur = cur * MV[mv].c;
                allMoves.insert(allMoves.end(), bestPath.begin(), bestPath.end());
                rec.steps.push_back(best);
                rec.total += best;
            }
            // Independent check: the scramble followed by all steps is solved.
            Cube3 check = from_cubie(states[i]).apply(parse_alg(face_turns(allMoves)).perm);
            if (!check.is_solved())
                throw std::runtime_error("method " + m.name + " did not solve scramble " + std::to_string(i));
            results[mi][i] = rec;
        }
        #pragma omp critical
        {
            ++doneCount;
            double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
            if (doneCount % 10 == 0 || doneCount == N)
                std::fprintf(stderr, "  %d/%d scrambles (%.0fs)\n", doneCount, N, secs);
        }
    }

    // ── report ──
    std::printf("Step-optimal move counts, %s, %d random states (seed %llu)\n\n", METRIC_NAMES[metric], N,
                (unsigned long long)seed);
    std::printf("| Method | Mean | SD | Median | Min | Max | Mean per step |\n|---|--:|--:|--:|--:|--:|---|\n");
    for (size_t mi = 0; mi < ms.size(); ++mi) {
        std::vector<int> tot;
        for (auto& r : results[mi]) tot.push_back(r.total);
        std::sort(tot.begin(), tot.end());
        double mean = std::accumulate(tot.begin(), tot.end(), 0.0) / N, var = 0;
        for (int t : tot) var += (t - mean) * (t - mean);
        double sd = N > 1 ? std::sqrt(var / (N - 1)) : 0;
        std::string steps;
        for (size_t s = 0; s < ms[mi].steps.size(); ++s) {
            double sm = 0;
            for (auto& r : results[mi]) sm += r.steps[s];
            char buf[64];
            std::snprintf(buf, sizeof buf, "%s%s %.2f", s ? " · " : "", ms[mi].steps[s].name.c_str(), sm / N);
            steps += buf;
        }
        std::printf("| %s | **%.2f** | %.2f | %.1f | %d | %d | %s |\n", ms[mi].name.c_str(), mean, sd,
                    N % 2 ? tot[N / 2] : (tot[N / 2 - 1] + tot[N / 2]) / 2.0, tot.front(), tot.back(),
                    steps.c_str());
    }
    if (!csv.empty()) {
        std::ofstream out(csv);
        out << "method,scramble,total";
        size_t maxSteps = 0;
        for (auto& m : ms) maxSteps = std::max(maxSteps, m.steps.size());
        for (size_t s = 0; s < maxSteps; ++s) out << ",step" << s + 1;
        out << "\n";
        for (size_t mi = 0; mi < ms.size(); ++mi)
            for (int i = 0; i < N; ++i) {
                out << '"' << ms[mi].name << "\"," << i << "," << results[mi][i].total;
                for (int v : results[mi][i].steps) out << "," << v;
                out << "\n";
            }
    }
    return 0;
}
