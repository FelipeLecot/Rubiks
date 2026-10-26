#include "cube.h"
#include "moves.h"
#include "solvability.h"
#include <array>
#include <iostream>
#include <string>

// ── 4x4 parity: legal on the 4x4, illegal after reduction to 3x3 ──────────────
//
// Both algorithms below use lowercase r, l, u for INNER slices and Uw for the
// wide U turn.  They are built from legal 4x4 moves, so the resulting positions
// are legal 4x4 states.  The demo then projects each one to a 3x3 (corners +
// one wing per dedge + solved centers) and runs the 3x3 solvability checker.
// The projection fails, which is exactly the reduction paradox.

using C4 = Cube<4>;
using P4 = Permutation<96>;

static P4 cat(std::initializer_list<P4> xs) {
    auto r = P4::identity();
    for (auto& x : xs) r = x.compose(r);
    return r;
}
static P4 half(P4 p) { return p.compose(p); }

static int off_color(const C4& c) {
    int n = 0;
    for (int f = 0; f < 6; ++f)
        for (int r = 0; r < 4; ++r)
            for (int col = 0; col < 4; ++col)
                if (c.s[C4::idx((Face)f, r, col)] != f) ++n;
    return n;
}

static bool centers_solved(const C4& c) {
    for (int f = 0; f < 6; ++f)
        for (int r = 1; r <= 2; ++r)
            for (int col = 1; col <= 2; ++col)
                if (c.s[C4::idx((Face)f, r, col)] != f) return false;
    return true;
}

// 3x3 (r,c) -> 4x4 coordinate.  Edge positions take the left/top wing of the
// dedge; for a paired dedge either wing gives the same face colour.
static Cube<3> project_to_3x3(const C4& c) {
    static const int map[3][3][2] = {
        {{0,0},{0,1},{0,3}},
        {{1,0},{-1,-1},{1,3}},
        {{3,0},{3,2},{3,3}}
    };
    Cube<3> o;
    for (int f = 0; f < 6; ++f) {
        for (int r = 0; r < 3; ++r) {
            for (int col = 0; col < 3; ++col) {
                int v;
                if (r == 1 && col == 1) v = f; // solved centers
                else                    v = c.s[C4::idx((Face)f, map[r][col][0], map[r][col][1])];
                o.s[f * 9 + r * 3 + col] = v;
            }
        }
    }
    return o;
}

static void report(const char* name, const char* alg, const P4& p) {
    auto c = C4::solved().apply(p);
    auto p3 = project_to_3x3(c);

    std::cout << "=== " << name << " ===\n";
    std::cout << "algorithm: " << alg << "\n";
    std::cout << "off-color stickers: " << off_color(c)
              << "  (0 = solved; 4 = one dedge flipped / two dedges swapped)\n";
    std::cout << "centers solved: " << centers_solved(c) << "\n";
    std::cout << "projected 3x3 is_solvable: " << is_solvable(p3) << "\n";
    std::cout << "  (the 4x4 state is legal: it was built from 4x4 moves)\n\n";
}

int main() {
    const auto U2  = make_move<4>(Face::U, 0, Dir::Half);
    const auto B2  = make_move<4>(Face::B, 0, Dir::Half);
    const auto F2  = make_move<4>(Face::F, 0, Dir::Half);

    const auto ri  = make_move<4>(Face::R, 1, Dir::CW);   // inner r
    const auto rip = make_move<4>(Face::R, 1, Dir::CCW);  // inner r'
    const auto ri2 = half(ri);
    const auto li  = make_move<4>(Face::L, 1, Dir::CW);   // inner l
    const auto lip = make_move<4>(Face::L, 1, Dir::CCW);  // inner l'
    const auto ui2 = half(make_move<4>(Face::U, 1, Dir::CW)); // inner u2

    const auto Uw  = cat({make_move<4>(Face::U, 0, Dir::CW),
                          make_move<4>(Face::U, 1, Dir::CW)});
    const auto Uw2 = half(Uw);

    // OLL parity: a single dedge flipped.
    const auto oll = cat({ri2, B2, U2, li, U2, rip, U2, ri, U2,
                          F2, ri, F2, lip, B2, ri2});

    // PLL parity: two dedges swapped.
    const auto pll = cat({ri2, U2, ri2, Uw2, ri2, ui2});

    report("OLL parity (one dedge flipped)",
           "r2 B2 U2 l U2 r' U2 r U2 F2 r F2 l' B2 r2", oll);
    report("PLL parity (two dedges swapped)",
           "r2 U2 r2 Uw2 r2 u2", pll);

    std::cout << "The four off-color stickers in the OLL case are the four\n";
    std::cout << "stickers of one dedge; in the PLL case they are two swapped\n";
    std::cout << "dedges.  Both projections fail the 3x3 invariants: the OLL\n";
    std::cout << "projection has an odd edge-flip sum, and the PLL projection\n";
    std::cout << "has mismatched corner/edge permutation parities.\n";

    return 0;
}
