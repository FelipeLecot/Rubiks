#pragma once
#include "cube.h"
#include <array>
#include <cstdint>

// ── A perfect 0…3,674,159 index for 2x2 states ────────────────────────────────
//
// A 2x2 position is determined by the permutation of the seven corners other
// than DBL and the orientations of six of them (the seventh twist is fixed by
// the corner-twist sum).  That gives exactly 7! * 3^6 = 3,674,160 states.
//
// The index is only used as a visited-set index for states generated from the
// solved cube, so no inverse map is needed.  The BFS in godsnumber.cpp checks
// that it reaches all 3,674,160 indices, which also verifies uniqueness.

namespace c2key {

constexpr int NumStates = 3674160;

constexpr int POS[8][3] = {
    {Cube<2>::idx(Face::U,1,0), Cube<2>::idx(Face::F,0,0), Cube<2>::idx(Face::L,0,1)},
    {Cube<2>::idx(Face::U,1,1), Cube<2>::idx(Face::F,0,1), Cube<2>::idx(Face::R,0,0)},
    {Cube<2>::idx(Face::U,0,0), Cube<2>::idx(Face::B,0,1), Cube<2>::idx(Face::L,0,0)},
    {Cube<2>::idx(Face::U,0,1), Cube<2>::idx(Face::B,0,0), Cube<2>::idx(Face::R,0,1)},
    {Cube<2>::idx(Face::D,0,0), Cube<2>::idx(Face::F,1,0), Cube<2>::idx(Face::L,1,1)},
    {Cube<2>::idx(Face::D,0,1), Cube<2>::idx(Face::F,1,1), Cube<2>::idx(Face::R,1,0)},
    {Cube<2>::idx(Face::D,1,0), Cube<2>::idx(Face::B,1,1), Cube<2>::idx(Face::L,1,0)},
    {Cube<2>::idx(Face::D,1,1), Cube<2>::idx(Face::B,1,0), Cube<2>::idx(Face::R,1,1)},
};

constexpr int SOLVED_COLORS[8][3] = {
    {0,2,4},{0,2,5},{0,3,4},{0,3,5},{1,2,4},{1,2,5},{1,3,4},{1,3,5}
};

constexpr std::array<int,64> make_lut() {
    std::array<int,64> lut{};
    for (auto& x : lut) x = -1;
    for (int i = 0; i < 8; ++i) {
        int m = 0;
        for (int k = 0; k < 3; ++k) m |= 1 << SOLVED_COLORS[i][k];
        lut[m] = i;
    }
    return lut;
}

inline const std::array<int,64> LUT = make_lut();

inline int of(const Cube<2>& c) {
    int perm[7], ori[7], n = 0;
    for (int p = 0; p < 8; ++p) {
        if (p == 6) continue; // DBL is fixed by {R,U,F}
        int c0 = c.s[POS[p][0]], c1 = c.s[POS[p][1]], c2 = c.s[POS[p][2]];
        perm[n] = LUT[(1 << c0) | (1 << c1) | (1 << c2)];
        bool a = (c0 == 0 || c0 == 1);
        bool b = (c1 == 0 || c1 == 1);
        ori[n] = a ? 0 : (b ? 1 : 2);
        ++n;
    }

    // Lehmer rank of the seven piece ids.
    static const int fact[7] = {1, 1, 2, 6, 24, 120, 720};
    int rank = 0;
    for (int i = 0; i < 7; ++i) {
        int cnt = 0;
        for (int j = i + 1; j < 7; ++j) if (perm[j] < perm[i]) ++cnt;
        rank += cnt * fact[6 - i];
    }

    // Six independent orientations; the seventh is fixed by the twist sum.
    int code = 0;
    for (int i = 0; i < 6; ++i) code = code * 3 + ori[i];
    return rank * 729 + code;
}

} // namespace c2key
