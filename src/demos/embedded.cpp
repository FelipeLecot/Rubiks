#include "cube.h"
#include "moves.h"
#include <iostream>
#include <array>

// Demonstrates: a 5x5 contains an embedded 3x3.
// The inner-layer stickers of a 5x5 (layer 1, the "inner ring") form
// a sub-puzzle structurally equivalent to a 3x3.
// Similarly, 7x7 embeds 5x5 and 3x3, and so on.

// Extract the 3x3 "inner face" sticker values from a 5x5 cube.
// For face f: rows 1..3, cols 1..3  (0-indexed, skipping outer ring).
std::array<int,9> inner_face(const Cube<5>& c, int f) {
    std::array<int,9> out;
    int k = 0;
    for (int r = 1; r <= 3; ++r)
        for (int col = 1; col <= 3; ++col)
            out[k++] = c.s[Cube<5>::idx(f, r, col)];
    return out;
}

void print_face(const char* label, std::array<int,9> face) {
    const char* names = "UDFBLR";
    std::cout << label << ": ";
    for (int i = 0; i < 9; ++i) std::cout << names[face[i]];
    std::cout << "\n";
}

int main() {
    // On a 5x5, layer 0 = outermost (moves the face + outer ring),
    // layer 1 = inner ring (equivalent to U move on embedded 3x3),
    // layer 2 = center slice (M-slice analogue).
    auto c5 = Cube<5>::solved();

    std::cout << "=== 5x5 inner 3x3 (solved) ===\n";
    for (int f = 0; f < 6; ++f) {
        char label[4] = {'f','a','c','e'};
        label[0] = "UDFBLR"[f]; label[1] = '\0';
        print_face(label, inner_face(c5, f));
    }

    // Apply layer-1 U move (inner ring only — doesn't touch outer ring or face)
    auto u1 = make_move<5>(Face::U, 1); // layer 1
    c5 = c5.apply(u1);
    std::cout << "\n=== After inner-U (layer 1) ===\n";
    for (int f = 0; f < 6; ++f) {
        char label[2] = {"UDFBLR"[f], '\0'};
        print_face(label, inner_face(c5, f));
    }

    std::cout << "\n=== Order of inner-U on 5x5 ===\n";
    std::cout << u1.order() << "  (same as U on 3x3: 4)\n";

    // Show that inner moves on NxN all have order 4 (quarter turns)
    std::cout << "\n=== Inner-U orders across cube sizes ===\n";
    for (const char* tag : {"3x3 layer0", "5x5 layer1", "7x7 layer1", "7x7 layer2"}) {
        // We print textually — instantiating all sizes would be verbose
    }
    auto u_3 = make_move<3>(Face::U, 0);
    auto u_7_1 = make_move<7>(Face::U, 1);
    auto u_7_2 = make_move<7>(Face::U, 2);
    std::cout << "3x3 U  order: " << u_3.order()   << "\n";
    std::cout << "5x5 U1 order: " << u1.order()    << "\n";
    std::cout << "7x7 U1 order: " << u_7_1.order() << "\n";
    std::cout << "7x7 U2 order: " << u_7_2.order() << "\n";

    return 0;
}
