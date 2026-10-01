// cube_engine.h — shared 3x3 search engine for the tools in src/tools.
//
// Sticker model helpers (algorithm parsing, rotations), a cubie model with
// Kociemba numbering, the metric-generic search move set (HTM / STM / QTM),
// pattern databases and the near-solved table. Header-only; include it from
// exactly one translation unit per executable.
#pragma once


#include "cube.h"
#include "moves.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using Perm54 = Permutation<54>;
using Cube3 = Cube<3>;

// ═════════════════════════════════════════════════════════════════════════════
// Sticker model: algorithm parsing (full WCA notation incl. wide/slice/rotation)
// ═════════════════════════════════════════════════════════════════════════════

struct AlgInfo {
    Perm54 perm = Perm54::identity();
    int htm = 0, stm = 0, qtm = 0;
};

static Perm54 layer_turn(Face f, int layer, int quarters) {
    auto m = make_move<3>(f, layer, Dir::CW);
    auto r = Perm54::identity();
    for (int i = 0; i < quarters; ++i) r = m.compose(r);
    return r;
}

static AlgInfo parse_alg(const std::string& alg) {
    AlgInfo info;
    size_t i = 0;
    auto face_of = [](char c) -> int {
        switch (std::toupper(c)) {
            case 'U': return U; case 'D': return D; case 'F': return F;
            case 'B': return B; case 'L': return L; case 'R': return R;
        }
        return -1;
    };
    while (i < alg.size()) {
        char c = alg[i];
        if (std::isspace((unsigned char)c) || c == '(' || c == ')') { ++i; continue; }
        ++i;
        bool wide = false;
        if (i < alg.size() && alg[i] == 'w') { wide = true; ++i; }
        int q = 1;
        if (i < alg.size() && alg[i] == '2') { q = 2; ++i; }
        if (i < alg.size() && alg[i] == '\'') { q = (q == 2) ? 2 : 3; ++i; }

        // Each token expands to a list of (face, layer) CW quarter turns applied q times.
        std::vector<std::pair<int,int>> layers; // (face, layer) turned CW
        std::vector<std::pair<int,int>> anti;   // (face, layer) turned CCW
        int kind; // 0 = outer/wide face turn, 1 = slice, 2 = rotation
        if (face_of(c) >= 0 && std::isupper((unsigned char)c) && !wide) {
            layers = {{face_of(c), 0}}; kind = 0;
        } else if (face_of(c) >= 0) {             // r, Rw ...
            layers = {{face_of(c), 0}, {face_of(c), 1}}; kind = 0;
        } else if (c == 'M') { layers = {{L, 1}}; kind = 1; }
        else if (c == 'E') { layers = {{D, 1}}; kind = 1; }
        else if (c == 'S') { layers = {{F, 1}}; kind = 1; }
        else if (c == 'x') { layers = {{R, 0}, {R, 1}}; anti = {{L, 0}}; kind = 2; }
        else if (c == 'y') { layers = {{U, 0}, {U, 1}}; anti = {{D, 0}}; kind = 2; }
        else if (c == 'z') { layers = {{F, 0}, {F, 1}}; anti = {{B, 0}}; kind = 2; }
        else throw std::runtime_error(std::string("bad move '") + c + "' in: " + alg);

        auto step = Perm54::identity();
        for (auto [f, l] : layers) step = layer_turn(Face(f), l, 1).compose(step);
        for (auto [f, l] : anti)   step = layer_turn(Face(f), l, 3).compose(step);
        for (int k = 0; k < q; ++k) info.perm = step.compose(info.perm);

        int qcost = (q == 2) ? 2 : 1;
        if (kind == 0) { info.htm += 1; info.stm += 1; info.qtm += qcost; }
        if (kind == 1) { info.htm += 2; info.stm += 1; info.qtm += 2 * qcost; }
    }
    return info;
}

// The 24 whole-cube rotations, as sticker permutations.
static const std::vector<Perm54>& rotations() {
    static std::vector<Perm54> rots = [] {
        std::vector<Perm54> out{Perm54::identity()};
        Perm54 gens[2] = {parse_alg("x").perm, parse_alg("y").perm};
        for (size_t i = 0; i < out.size(); ++i)
            for (auto& g : gens) {
                auto n = g.compose(out[i]);
                if (std::find(out.begin(), out.end(), n) == out.end()) out.push_back(n);
            }
        return out;
    }();
    return rots;
}

// Rotate the whole cube so that every center is back on its home face.
static Cube3 normalize(const Cube3& c) {
    for (auto& r : rotations()) {
        Cube3 n = c.apply(r);
        bool ok = true;
        for (int f = 0; f < 6; ++f) ok &= n.s[Cube3::idx(f, 1, 1)] == f;
        if (ok) return n;
    }
    throw std::runtime_error("no rotation normalizes centers");
}

// ═════════════════════════════════════════════════════════════════════════════
// Cubie model
// ═════════════════════════════════════════════════════════════════════════════
// Positions / pieces follow Kociemba's numbering:
//   corners URF UFL ULB UBR DFR DLF DBL DRB
//   edges   UR UF UL UB DR DF DL DB FR FL BL BR
// A state s means: position i holds piece s.cp[i] twisted by s.co[i].
// s*m (apply m after s): cp[i] = s.cp[m.cp[i]], co[i] = s.co[m.cp[i]] + m.co[i].

struct CubieCube {
    uint8_t cp[8], co[8], ep[12], eo[12];

    static CubieCube solved() {
        CubieCube c;
        for (int i = 0; i < 8; ++i) { c.cp[i] = i; c.co[i] = 0; }
        for (int i = 0; i < 12; ++i) { c.ep[i] = i; c.eo[i] = 0; }
        return c;
    }
    CubieCube operator*(const CubieCube& m) const {
        CubieCube r;
        for (int i = 0; i < 8; ++i) {
            r.cp[i] = cp[m.cp[i]];
            r.co[i] = (co[m.cp[i]] + m.co[i]) % 3;
        }
        for (int i = 0; i < 12; ++i) {
            r.ep[i] = ep[m.ep[i]];
            r.eo[i] = (eo[m.ep[i]] + m.eo[i]) & 1;
        }
        return r;
    }
    bool operator==(const CubieCube& o) const { return std::memcmp(this, &o, sizeof o) == 0; }
    bool operator<(const CubieCube& o) const { return std::memcmp(this, &o, sizeof o) < 0; }
};

// Facelets of each cubie position; first facelet is the U/D (or F/B) one,
// corners listed clockwise.
static int CF[8][3], EF[12][2];
static void init_facelets() {
    auto I = [](int f, int r, int c) { return Cube3::idx(f, r, c); };
    int cf[8][3] = {
        {I(U,2,2), I(R,0,0), I(F,0,2)},  // URF
        {I(U,2,0), I(F,0,0), I(L,0,2)},  // UFL
        {I(U,0,0), I(L,0,0), I(B,0,2)},  // ULB
        {I(U,0,2), I(B,0,0), I(R,0,2)},  // UBR
        {I(D,0,2), I(F,2,2), I(R,2,0)},  // DFR
        {I(D,0,0), I(L,2,2), I(F,2,0)},  // DLF
        {I(D,2,0), I(B,2,2), I(L,2,0)},  // DBL
        {I(D,2,2), I(R,2,2), I(B,2,0)},  // DRB
    };
    int ef[12][2] = {
        {I(U,1,2), I(R,0,1)}, {I(U,2,1), I(F,0,1)}, {I(U,1,0), I(L,0,1)}, {I(U,0,1), I(B,0,1)},
        {I(D,1,2), I(R,2,1)}, {I(D,0,1), I(F,2,1)}, {I(D,1,0), I(L,2,1)}, {I(D,2,1), I(B,2,1)},
        {I(F,1,2), I(R,1,0)}, {I(F,1,0), I(L,1,2)}, {I(B,1,2), I(L,1,0)}, {I(B,1,0), I(R,1,2)},
    };
    std::memcpy(CF, cf, sizeof cf);
    std::memcpy(EF, ef, sizeof ef);
}

// Sticker cube (centers at home) → cubie cube.
static CubieCube to_cubie(const Cube3& c) {
    CubieCube r;
    for (int i = 0; i < 8; ++i) {
        int o = 0;
        while (o < 3 && c.s[CF[i][o]] != U && c.s[CF[i][o]] != D) ++o;
        if (o == 3) throw std::runtime_error("bad corner");
        int c1 = c.s[CF[i][(o + 1) % 3]], c2 = c.s[CF[i][(o + 2) % 3]];
        int piece = -1;
        for (int j = 0; j < 8; ++j)
            if (c1 == CF[j][1] / 9 && c2 == CF[j][2] / 9) piece = j;
        if (piece < 0) throw std::runtime_error("bad corner colors");
        r.cp[i] = piece; r.co[i] = o;
    }
    for (int i = 0; i < 12; ++i) {
        int a = c.s[EF[i][0]], b = c.s[EF[i][1]], piece = -1, o = 0;
        for (int j = 0; j < 12; ++j) {
            if (a == EF[j][0] / 9 && b == EF[j][1] / 9) { piece = j; o = 0; }
            if (b == EF[j][0] / 9 && a == EF[j][1] / 9) { piece = j; o = 1; }
        }
        if (piece < 0) throw std::runtime_error("bad edge colors");
        r.ep[i] = piece; r.eo[i] = o;
    }
    return r;
}

// 18 face moves: index = face*3 + k, k = 0 (CW), 1 (half), 2 (CCW).
static CubieCube MOVES[18];
static const char* FACE_NAMES = "UDFBLR";
static std::string move_name(int m) {
    static const char* suf[3] = {"", "2", "'"};
    return std::string(1, FACE_NAMES[m / 3]) + suf[m % 3];
}
static void init_moves() {
    for (int f = 0; f < 6; ++f)
        for (int k = 0; k < 3; ++k)
            MOVES[f * 3 + k] = to_cubie(Cube3::solved().apply(layer_turn(Face(f), 0, k + 1)));
}

static CubieCube inverse(const CubieCube& c) {
    CubieCube r;
    for (int i = 0; i < 8; ++i) { r.cp[c.cp[i]] = i; r.co[c.cp[i]] = (3 - c.co[i]) % 3; }
    for (int i = 0; i < 12; ++i) { r.ep[c.ep[i]] = i; r.eo[c.ep[i]] = c.eo[i]; }
    return r;
}

// Cubie cube → sticker cube (centers at home); inverse of to_cubie.
static Cube3 from_cubie(const CubieCube& c) {
    Cube3 r = Cube3::solved();
    for (int i = 0; i < 8; ++i)
        for (int k = 0; k < 3; ++k) r.s[CF[i][(c.co[i] + k) % 3]] = CF[c.cp[i]][k] / 9;
    for (int i = 0; i < 12; ++i)
        for (int k = 0; k < 2; ++k) r.s[EF[i][(c.eo[i] + k) % 2]] = EF[c.ep[i]][k] / 9;
    return r;
}

// Whole-cube rotations as cubie arrangements. Conjugating a state by one maps
// face turns to face turns, so the distance to solved is unchanged — which
// lets one pattern database be consulted "from several viewpoints".
static std::vector<CubieCube> SYMS, SYMS_INV;
static void init_syms() {
    for (auto& rot : rotations()) {
        CubieCube S = to_cubie(Cube3::solved().apply(rot)), Si = inverse(S);
        for (int m = 0; m < 18; ++m) {
            CubieCube c = Si * MOVES[m] * S;
            bool ok = false;
            for (int n = 0; n < 18; ++n) ok |= c == MOVES[n];
            if (!ok) throw std::runtime_error("rotation does not conjugate moves to moves");
        }
        SYMS.push_back(S);
        SYMS_INV.push_back(Si);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// Metrics and the search move set
// ═════════════════════════════════════════════════════════════════════════════
// Searches run in the frame of the centers. There a slice move is the same as
// turning the two faces beside it the other way (M ≡ L' R, up to a whole-cube
// rotation) and a wide move the same as the opposite face turn (r ≡ L), so all
// metrics can be expressed with face turns. A search move is any non-trivial
// turn of one axis — U^a D^b, F^a B^b or L^a R^b — and consecutive search
// moves use different axes, which removes the trivial redundancies.

enum Metric { HTM = 0, STM = 1, QTM = 2 };
static const char* METRIC_NAMES[3] = {"HTM", "STM", "QTM"};

struct AxisMove { CubieCube c; int axis, a, b; int cost[3]; };
static std::vector<AxisMove> MV;
static int MV_ID[3][4][4];

static void init_axis_moves() {
    auto qt = [](int p) { return p == 0 ? 0 : p == 2 ? 2 : 1; };
    for (int ax = 0; ax < 3; ++ax)
        for (int a = 0; a < 4; ++a)
            for (int b = 0; b < 4; ++b) {
                MV_ID[ax][a][b] = -1;
                if (!a && !b) continue;
                AxisMove m;
                m.axis = ax; m.a = a; m.b = b;
                m.c = CubieCube::solved();
                if (a) m.c = m.c * MOVES[(2 * ax) * 3 + a - 1];
                if (b) m.c = m.c * MOVES[(2 * ax + 1) * 3 + b - 1];
                m.cost[HTM] = (a > 0) + (b > 0);
                m.cost[QTM] = qt(a) + qt(b);
                m.cost[STM] = (a && b && (a + b) % 4 == 0) ? 1 : m.cost[HTM];  // slice
                MV_ID[ax][a][b] = (int)MV.size();
                MV.push_back(m);
            }
}

// A move whose U/D-axis part includes a U turn (absorbed by AUF).
static bool has_U_part(int m) { return MV[m].axis == 0 && MV[m].a != 0; }

// ═════════════════════════════════════════════════════════════════════════════
// Pattern databases
// ═════════════════════════════════════════════════════════════════════════════
// A pattern tracks the positions (and twists) of a subset of pieces of one kind.
//   oriAll = false: twist of the tracked pieces only
//   oriAll = true : twist of every position (untracked pieces are
//                   indistinguishable but their orientation still counts)
// The database stores the exact face-turn distance of the pattern to its
// solved pattern, which lower-bounds the distance of the full cube.

struct Pattern {
    bool corners;             // corners or edges
    std::vector<int> tracked; // piece ids
    bool oriAll;
    int n, mod, k;
    uint64_t permCount, oriCount;
    std::vector<uint8_t> dist;
    std::string name;

    Pattern(std::string nm, bool c, std::vector<int> t, bool oa)
        : corners(c), tracked(std::move(t)), oriAll(oa), name(std::move(nm)) {
        n = corners ? 8 : 12;
        mod = corners ? 3 : 2;
        k = (int)tracked.size();
        permCount = 1;
        for (int j = 0; j < k; ++j) permCount *= (n - j);
        int od = oriAll ? n - 1 : (k == n ? k - 1 : k);
        oriCount = 1;
        for (int j = 0; j < od; ++j) oriCount *= mod;
    }
    uint64_t size() const { return permCount * oriCount; }

    uint64_t index(const uint8_t* p, const uint8_t* o) const {
        int where[12];
        for (int i = 0; i < n; ++i) where[p[i]] = i;
        uint64_t pi = 0;
        unsigned used = 0;
        for (int j = 0; j < k; ++j) {
            int pos = where[tracked[j]];
            int digit = __builtin_popcount(~used & ((1u << pos) - 1));
            pi = pi * (n - j) + digit;
            used |= 1u << pos;
        }
        uint64_t oi = 0;
        if (oriAll) {
            for (int i = 0; i < n - 1; ++i) oi = oi * mod + o[i];
        } else {
            int od = (k == n) ? k - 1 : k;
            for (int j = 0; j < od; ++j) oi = oi * mod + o[where[tracked[j]]];
        }
        return pi * oriCount + oi;
    }
    uint64_t index(const CubieCube& c) const {
        return corners ? index(c.cp, c.co) : index(c.ep, c.eo);
    }

    // Build some full piece arrangement with this index.
    void decode(uint64_t idx, uint8_t* p, uint8_t* o) const {
        uint64_t oi = idx % oriCount, pi = idx / oriCount;
        int digits[12];
        for (int j = k - 1; j >= 0; --j) { digits[j] = pi % (n - j); pi /= (n - j); }
        bool usedPos[12] = {}, usedPiece[12] = {};
        int pos[12];
        for (int j = 0; j < k; ++j) {
            // the digits[j]-th free position
            int cnt = digits[j], q = 0;
            for (;; ++q) if (!usedPos[q]) { if (cnt == 0) break; --cnt; }
            pos[j] = q; usedPos[q] = true;
            p[q] = tracked[j]; usedPiece[tracked[j]] = true;
        }
        int nextPiece = 0;
        for (int i = 0; i < n; ++i) if (!usedPos[i]) {
            while (usedPiece[nextPiece]) ++nextPiece;
            p[i] = nextPiece; usedPiece[nextPiece] = true;
        }
        for (int i = 0; i < n; ++i) o[i] = 0;
        if (oriAll) {
            int sum = 0;
            for (int i = n - 2; i >= 0; --i) { o[i] = oi % mod; oi /= mod; sum += o[i]; }
            o[n - 1] = (mod - sum % mod) % mod;
        } else {
            int od = (k == n) ? k - 1 : k, sum = 0;
            for (int j = od - 1; j >= 0; --j) { o[pos[j]] = oi % mod; oi /= mod; sum += o[pos[j]]; }
            if (k == n) o[pos[k - 1]] = (mod - sum % mod) % mod;
        }
    }

    void build(const std::string& cacheDir, Metric metric) {
        static const char* suffix[3] = {"", "_stm", "_qtm"};
        std::string file = cacheDir + "/" + name + suffix[metric] + ".pdb";
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
        CubieCube s = CubieCube::solved();
        dist[index(s)] = 0;
        // Keep going until a layer adds nothing: `added` may over-count when
        // two threads reach the same entry, so it cannot decide completion.
        for (int d = 0;; ++d) {
            uint64_t added = 0;
            #pragma omp parallel for schedule(dynamic, 65536) reduction(+:added)
            for (int64_t i = 0; i < (int64_t)size(); ++i) {
                if (dist[i] != d) continue;
                uint8_t p[12], o[12];
                decode(i, p, o);
                for (const auto& am : MV) {
                    if (am.cost[metric] != 1) continue;
                    const auto& mv = am.c;
                    const uint8_t* mp = corners ? mv.cp : mv.ep;
                    const uint8_t* mo = corners ? mv.co : mv.eo;
                    uint8_t np[12], no[12];
                    for (int q = 0; q < n; ++q) {
                        np[q] = p[mp[q]];
                        no[q] = (o[mp[q]] + mo[q]) % mod;
                    }
                    uint64_t j = index(np, no);
                    if (dist[j] == 0xFF) { dist[j] = d + 1; ++added; }
                }
            }
            if (added == 0) break;
        }
        uint64_t unreached = std::count(dist.begin(), dist.end(), (uint8_t)0xFF);
        auto secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        std::fprintf(stderr, "  built %-12s %s %11llu entries, max depth %d, %.1fs%s\n", name.c_str(),
                     METRIC_NAMES[metric],
                     (unsigned long long)size(), *std::max_element(dist.begin(), dist.end()),
                     secs, unreached ? " (UNREACHED ENTRIES!)" : "");
        std::ofstream out(file, std::ios::binary);
        out.write((const char*)dist.data(), size());
    }
    int h(const CubieCube& c) const { return dist[index(c)]; }
};

// ═════════════════════════════════════════════════════════════════════════════
// Near-solved table: exact distance of every position within D face turns
// ═════════════════════════════════════════════════════════════════════════════
// Replaces the last D levels of the search tree by one hash lookup each.

struct NearTable {
    // 16-byte slots: a = corners (40 bits) | distance << 60, b = edges (60 bits).
    struct Slot { uint64_t a = ~0ULL, b = 0; };
    std::vector<Slot> slots;
    uint64_t mask = 0;
    int depth = 0;

    static void pack(const CubieCube& c, uint64_t& a, uint64_t& b) {
        a = 0; b = 0;
        for (int i = 0; i < 8; ++i) a = (a << 5) | (c.cp[i] << 2) | c.co[i];
        for (int i = 0; i < 12; ++i) b = (b << 5) | (c.ep[i] << 1) | c.eo[i];
    }
    uint64_t slot_of(uint64_t a, uint64_t b) const {
        uint64_t h = (a * 0x9E3779B97F4A7C15ULL) ^ (b * 0xC2B2AE3D27D4EB4FULL);
        return (h ^ (h >> 29)) & mask;
    }
    static constexpr uint64_t KEY = (1ULL << 40) - 1;
    // Distance if within the table, else 0xFF.
    int get(const CubieCube& c) const {
        uint64_t a, b; pack(c, a, b);
        for (uint64_t i = slot_of(a, b);; i = (i + 1) & mask) {
            const Slot& s = slots[i];
            if (s.a == ~0ULL) return 0xFF;
            if ((s.a & KEY) == a && s.b == b) return int(s.a >> 60);
        }
    }
    bool insert(const CubieCube& c, int d) {
        uint64_t a, b; pack(c, a, b);
        for (uint64_t i = slot_of(a, b);; i = (i + 1) & mask) {
            Slot& s = slots[i];
            if (s.a == ~0ULL) { s.a = a | (uint64_t(d) << 60); s.b = b; return true; }
            if ((s.a & KEY) == a && s.b == b) return false;
        }
    }
    void build(int D, Metric metric) {
        auto t0 = std::chrono::steady_clock::now();
        depth = D;
        uint64_t cap = 1;
        static const double branching[3] = {13.35, 19.0, 9.4};  // rough growth per level
        double expect = 1; for (int d = 1; d <= D; ++d) expect *= branching[metric];
        while (cap < 1.6 * expect) cap <<= 1;
        slots.assign(cap, Slot{});
        mask = cap - 1;
        std::vector<CubieCube> frontier{CubieCube::solved()}, next;
        insert(frontier[0], 0);
        uint64_t total = 1;
        for (int d = 0; d < D; ++d) {
            next.clear();
            for (auto& c : frontier)
                for (const auto& am : MV) {
                    if (am.cost[metric] != 1) continue;
                    CubieCube n = c * am.c;
                    if (insert(n, d + 1)) next.push_back(n);
                }
            total += next.size();
            if (total > 0.9 * cap) throw std::runtime_error("near-solved table too full; lower --near");
            frontier.swap(next);
        }
        auto secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        std::fprintf(stderr, "  built near-solved table: %llu positions within %d %s, %.1fs\n",
                     (unsigned long long)total, D, METRIC_NAMES[metric], secs);
    }
};

