#include "cube.h"
#include "moves.h"
#include "solvability.h"
#include <iostream>

// Demonstrates: moves as permutations, parity, and why a single edge-flip
// or corner-twist produces an unsolvable state.

int main() {
    using C3 = Cube<3>;

    auto mU  = make_move<3>(Face::U, 0);
    auto mF  = make_move<3>(Face::F, 0);
    auto mR  = make_move<3>(Face::R, 0);
    auto mRi = make_move<3>(Face::R, 0, Dir::CCW);
    auto mUi = make_move<3>(Face::U, 0, Dir::CCW);

    std::cout << "=== Move parities ===\n";
    std::cout << "U  parity: " << mU.parity() << "\n";
    std::cout << "F  parity: " << mF.parity() << "\n";
    std::cout << "R  parity: " << mR.parity() << "\n";
    std::cout << "UF parity: " << mU.compose(mF).parity() << "  (product of two odd = even)\n\n";

    std::cout << "=== Move orders ===\n";
    std::cout << "U  order: " << mU.order() << "\n";
    std::cout << "U2 order: " << mU.compose(mU).order() << "\n";
    auto sexy = compose_moves<3>({mR, mU, mRi, mUi});
    std::cout << "Sexy move (RUR'U') order: " << sexy.order() << "\n\n";

    // A single-edge flip is unsolvable: manually flip UF edge stickers
    auto bad = C3::solved();
    std::swap(bad.s[C3::idx(Face::U,2,1)], bad.s[C3::idx(Face::F,0,1)]);
    std::cout << "=== Solvability ===\n";
    std::cout << "Solved cube solvable:      " << is_solvable(C3::solved()) << "\n";
    std::cout << "Single edge-flip solvable: " << is_solvable(bad) << "\n";

    // PLL monoswap: swap exactly two corners (UFL ↔ UFR)
    auto bad2 = C3::solved();
    std::swap(bad2.s[C3::idx(Face::U,2,0)], bad2.s[C3::idx(Face::U,2,2)]);
    std::swap(bad2.s[C3::idx(Face::F,0,0)], bad2.s[C3::idx(Face::F,0,2)]);
    std::swap(bad2.s[C3::idx(Face::L,0,2)], bad2.s[C3::idx(Face::R,0,0)]);
    std::cout << "Corner 2-swap solvable:    " << is_solvable(bad2) << "\n";

    return 0;
}
