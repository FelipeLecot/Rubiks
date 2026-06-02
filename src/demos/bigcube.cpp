#include "cube.h"
#include "moves.h"
#include "solvability.h"
#include <iostream>

// Demonstrates OLL parity and PLL parity unique to even-N cubes (4x4).
//
// OLL parity: a single edge piece appears flipped. On a 3x3 this is impossible
// (the edge-flip invariant), but on a 4x4 the inner-slice moves can produce it.
//
// PLL parity: two adjacent edge pieces are swapped. Again impossible on 3x3
// but reachable on 4x4 through inner-slice moves.

int main() {
    // ── OLL parity on 4x4 ────────────────────────────────────────────────────
    // The sequence r2 U2 r2 Uw2 r2 u2 produces OLL parity on a 4x4.
    // (r = right two layers; Uw = U + u layer together)
    // We build it from layer moves.
    auto r0  = make_move<4>(Face::R, 0);          // R outer layer
    auto r1  = make_move<4>(Face::R, 1);          // r inner layer
    auto r2  = r0.compose(r1);                    // r (wide R)
    auto r2h = r2.compose(r2);                    // r2 (half turn)

    auto u0  = make_move<4>(Face::U, 0);
    auto u1  = make_move<4>(Face::U, 1);
    auto Uw  = u0.compose(u1);                    // Uw (wide U)
    auto Uw2 = Uw.compose(Uw);
    auto u1h = u1.compose(u1);                    // u2 (inner U half)

    // r2 U2 r2 Uw2 r2 u2
    auto U0h = u0.compose(u0);
    auto oll_parity = r2h.compose(U0h).compose(r2h).compose(Uw2).compose(r2h).compose(u1h);

    auto c4 = Cube<4>::solved().apply(oll_parity);

    std::cout << "=== 4x4 OLL parity sequence applied ===\n";
    std::cout << "Cube is solved: " << c4.is_solved() << "\n";

    // Count how many edge stickers are mismatched on top layer
    int mismatched = 0;
    for (int col = 1; col <= 2; ++col) {
        if (c4.s[Cube<4>::idx(Face::U, 3, col)] != Face::U) ++mismatched;
        if (c4.s[Cube<4>::idx(Face::F, 0, col)] != Face::F) ++mismatched;
    }
    std::cout << "Top-front edge mismatches: " << mismatched
              << "  (0 = solved, 4 = OLL parity)\n\n";

    // ── PLL parity on 4x4 ────────────────────────────────────────────────────
    // r2 B2 U2 l U2 r' U2 r U2 F2 r F2 l' B2 r2
    auto l0  = make_move<4>(Face::L, 0);
    auto l1  = make_move<4>(Face::L, 1);
    auto lw  = l0.compose(l1);
    auto l0i = l0.inverse();
    auto l1i = l1.inverse();
    auto lwi = lw.inverse();

    auto r0i = r0.inverse();
    auto B0  = make_move<4>(Face::B, 0);
    auto B0h = B0.compose(B0);
    auto F0  = make_move<4>(Face::F, 0);
    auto F0h = F0.compose(F0);

    // r2 B2 U2 l U2 r' U2 r U2 F2 r F2 l' B2 r2
    auto pll_parity =
        r2h.compose(B0h).compose(U0h)
           .compose(lw).compose(U0h)
           .compose(r0i).compose(U0h)
           .compose(r0).compose(U0h)
           .compose(F0h).compose(r0)
           .compose(F0h).compose(lwi)
           .compose(B0h).compose(r2h);

    auto c4p = Cube<4>::solved().apply(pll_parity);
    std::cout << "=== 4x4 PLL parity sequence applied ===\n";
    std::cout << "Cube is solved: " << c4p.is_solved() << "\n";

    // Top face: top row of F should be all F-colored in solved state
    std::cout << "Top of F face (expect FFFF if solved):\n  ";
    for (int col = 0; col < 4; ++col)
        std::cout << "UDFBLR"[c4p.s[Cube<4>::idx(Face::F, 0, col)]];
    std::cout << "\n";

    return 0;
}
