#pragma once
#include "cube.h"
#include <array>
#include <algorithm>
#include <numeric>

// ── 3x3 solvability (full invariant check) ────────────────────────────────────
//
// A 3x3 state is reachable from solved iff three independent invariants hold:
//   1. Permutation parity: corner perm parity == edge perm parity (both even XOR both odd)
//   2. Corner twist sum ≡ 0 (mod 3)
//   3. Edge flip sum ≡ 0 (mod 2)
//
// Pieces are identified by their color sets. Orientations are computed
// relative to the U/D axis: twist=0 when the U/D-color sticker is on a U/D face.

namespace detail3 {

// For N=3, sticker index shorthand
constexpr int idx(int f, int r, int c) { return f*9 + r*3 + c; }

// 8 corner positions. Each row: [UD_sticker, FB_sticker, LR_sticker].
// CW orientation cycle for each corner: UD→FB→LR (twist 0→1→2).
constexpr std::array<std::array<int,3>,8> CORNERS = {{
    {idx(U,2,0), idx(F,0,0), idx(L,0,2)},  // 0: UFL
    {idx(U,2,2), idx(F,0,2), idx(R,0,0)},  // 1: UFR
    {idx(U,0,0), idx(B,0,2), idx(L,0,0)},  // 2: UBL
    {idx(U,0,2), idx(B,0,0), idx(R,0,2)},  // 3: UBR
    {idx(D,0,0), idx(F,2,0), idx(L,2,2)},  // 4: DFL
    {idx(D,0,2), idx(F,2,2), idx(R,2,0)},  // 5: DFR
    {idx(D,2,0), idx(B,2,2), idx(L,2,0)},  // 6: DBL
    {idx(D,2,2), idx(B,2,0), idx(R,2,2)},  // 7: DBR
}};

// 12 edge positions. Each row: [first_sticker, second_sticker].
// Orientation 0: the U/D or F/B sticker of the piece is on a U/D or F/B face.
constexpr std::array<std::array<int,2>,12> EDGES = {{
    {idx(U,2,1), idx(F,0,1)},  //  0: UF
    {idx(U,1,2), idx(R,0,1)},  //  1: UR
    {idx(U,0,1), idx(B,0,1)},  //  2: UB
    {idx(U,1,0), idx(L,0,1)},  //  3: UL
    {idx(F,1,2), idx(R,1,0)},  //  4: FR
    {idx(F,1,0), idx(L,1,2)},  //  5: FL
    {idx(B,1,0), idx(R,1,2)},  //  6: BR
    {idx(B,1,2), idx(L,1,0)},  //  7: BL
    {idx(D,0,1), idx(F,2,1)},  //  8: DF
    {idx(D,1,2), idx(R,2,1)},  //  9: DR
    {idx(D,2,1), idx(B,2,1)},  // 10: DB
    {idx(D,1,0), idx(L,2,1)},  // 11: DL
}};

// Identify which of the 8 solved corner pieces occupies a position,
// based on the set of 3 sticker colors. Returns solved-position index 0..7.
// Colors map directly to face indices (U=0,D=1,F=2,B=3,L=4,R=5).
int find_corner(int c0, int c1, int c2) {
    // Each corner is uniquely identified by which 3 face-colors it has.
    // Solved corners and their color sets:
    static const std::array<std::array<int,3>,8> solved_colors = {{
        {U,F,L}, {U,F,R}, {U,B,L}, {U,B,R},
        {D,F,L}, {D,F,R}, {D,B,L}, {D,B,R},
    }};
    std::array<int,3> got = {c0,c1,c2};
    std::sort(got.begin(), got.end());
    for (int i = 0; i < 8; ++i) {
        auto ref = solved_colors[i];
        std::sort(ref.begin(), ref.end());
        if (ref == got) return i;
    }
    return -1; // invalid state: unknown color combination
}

// Identify which of the 12 solved edge pieces occupies a position.
int find_edge(int c0, int c1) {
    static const std::array<std::array<int,2>,12> solved_colors = {{
        {U,F},{U,R},{U,B},{U,L},{F,R},{F,L},{B,R},{B,L},{D,F},{D,R},{D,B},{D,L}
    }};
    std::array<int,2> got = {c0,c1};
    std::sort(got.begin(), got.end());
    for (int i = 0; i < 12; ++i) {
        auto ref = solved_colors[i];
        std::sort(ref.begin(), ref.end());
        if (ref == got) return i;
    }
    return -1;
}

// Corner orientation at position pos: 0,1,2 CW twists from standard.
// Standard: the piece's U/D-colored sticker is on the U or D face (slot 0).
int corner_orientation(const Cube<3>& cube, int pos) {
    int c0 = cube.s[CORNERS[pos][0]]; // sticker on U/D face of this position
    int c1 = cube.s[CORNERS[pos][1]]; // sticker on F/B face
    int c2 = cube.s[CORNERS[pos][2]]; // sticker on L/R face
    bool c0_is_ud = (c0==U || c0==D);
    bool c1_is_ud = (c1==U || c1==D);
    if (c0_is_ud) return 0;
    if (c1_is_ud) return 1; // one CW twist: U/D color is on FB face
    return 2;               // two CW twists: U/D color is on LR face
}

// Edge orientation at position pos: 0=good, 1=flipped.
// An edge is good when the sticker on its U/D or F/B face-slot has a color
// that "matches" that face category:
//   - slot on U or D face → sticker should be U or D color
//   - slot on F or B face → sticker should be F or B color
// This is equivalent to the standard "can be placed without U/D quarter-turns" criterion.
int edge_orientation(const Cube<3>& cube, int pos) {
    int sticker_idx = EDGES[pos][0];
    int face  = sticker_idx / 9;   // which face the sticker lives on
    int color = cube.s[sticker_idx];
    if (face == U || face == D) return (color == U || color == D) ? 0 : 1;
    else                        return (color == F || color == B) ? 0 : 1;
}

int perm_parity(const std::array<int,8>& p) {
    std::array<bool,8> vis{};
    int sign = 1;
    for (int i = 0; i < 8; ++i) {
        if (vis[i]) continue;
        int len = 0, j = i;
        while (!vis[j]) { vis[j]=true; j=p[j]; ++len; }
        if (len%2==0) sign=-sign;
    }
    return sign;
}
int perm_parity(const std::array<int,12>& p) {
    std::array<bool,12> vis{};
    int sign = 1;
    for (int i = 0; i < 12; ++i) {
        if (vis[i]) continue;
        int len = 0, j = i;
        while (!vis[j]) { vis[j]=true; j=p[j]; ++len; }
        if (len%2==0) sign=-sign;
    }
    return sign;
}

} // namespace detail3

// Returns true iff the 3x3 state is reachable from solved.
inline bool is_solvable(const Cube<3>& cube) {
    using namespace detail3;

    std::array<int,8>  cp; // corner permutation
    std::array<int,12> ep; // edge permutation
    int twist_sum = 0;
    int flip_sum  = 0;

    for (int pos = 0; pos < 8; ++pos) {
        int c0 = cube.s[CORNERS[pos][0]];
        int c1 = cube.s[CORNERS[pos][1]];
        int c2 = cube.s[CORNERS[pos][2]];
        int piece = find_corner(c0, c1, c2);
        if (piece < 0) return false; // impossible color set
        cp[pos] = piece;
        twist_sum += corner_orientation(cube, pos);
    }

    for (int pos = 0; pos < 12; ++pos) {
        int c0 = cube.s[EDGES[pos][0]];
        int c1 = cube.s[EDGES[pos][1]];
        int piece = find_edge(c0, c1);
        if (piece < 0) return false;
        ep[pos] = piece;
        flip_sum += edge_orientation(cube, pos);
    }

    if (twist_sum % 3 != 0) return false;
    if (flip_sum  % 2 != 0) return false;
    if (perm_parity(cp) != perm_parity(ep)) return false;
    return true;
}
