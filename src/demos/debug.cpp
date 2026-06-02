#include "cube.h"
#include "moves.h"
#include <iostream>

// Known orders for 3x3 (standard HTM, all stickers including centers):
//   R U  → 105    F U  → 105    L U  → 105
//   R F  → 105    R B  → 105    R D  → 105
//   RUR'U' → 6    U4=I, R4=I
int main() {
    auto R = make_move<3>(Face::R, 0);
    auto L = make_move<3>(Face::L, 0);
    auto U = make_move<3>(Face::U, 0);
    auto D = make_move<3>(Face::D, 0);
    auto F = make_move<3>(Face::F, 0);
    auto B = make_move<3>(Face::B, 0);
    auto id = Permutation<54>::identity();

    // Single face orders (all must be 4)
    std::cout << "U^4=id: " << (U.compose(U).compose(U).compose(U) == id) << "\n";
    std::cout << "R^4=id: " << (R.compose(R).compose(R).compose(R) == id) << "\n";
    std::cout << "F^4=id: " << (F.compose(F).compose(F).compose(F) == id) << "\n";
    std::cout << "B^4=id: " << (B.compose(B).compose(B).compose(B) == id) << "\n";
    std::cout << "L^4=id: " << (L.compose(L).compose(L).compose(L) == id) << "\n";
    std::cout << "D^4=id: " << (D.compose(D).compose(D).compose(D) == id) << "\n";

    // Two-face orders (all must be 105 or similar well-known values)
    auto print_order = [](const char* name, auto m) {
        std::cout << "order(" << name << ") = " << m.order() << "\n";
    };
    print_order("RU", U.compose(R));
    print_order("RF", F.compose(R));
    print_order("RD", D.compose(R));
    print_order("RB", B.compose(R));
    print_order("FU", U.compose(F));
    print_order("LU", U.compose(L));
    print_order("FD", D.compose(F));
    print_order("LD", D.compose(L));

    // Commutator RUR'U' = 6
    auto Ri = R.inverse(); auto Ui = U.inverse();
    print_order("RUR'U'", Ui.compose(Ri).compose(U).compose(R));

    // Sanity: scramble + inverse = solved
    auto scr = F.compose(R).compose(U).compose(B).compose(L);
    auto cube = Cube<3>::solved().apply(scr).apply(scr.inverse());
    std::cout << "scramble*inv = solved: " << cube.is_solved() << "\n";
    return 0;
}
