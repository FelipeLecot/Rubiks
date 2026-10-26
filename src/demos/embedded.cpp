#include "cube.h"
#include "moves.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>

// ── The real embedded 3x3 inside an odd cube ──────────────────────────────────
//
// On a 5x5, the 8 corners, the 12 midges (the middle edge piece of each edge),
// and the six exact centers form a 3x3 skeleton.  An outer face turn acts on
// this skeleton exactly as the corresponding 3x3 move.  The other 48 centers
// and 24 wings are extra pieces.
//
// Inner slices at depths 1..(N-3)/2 fix the skeleton; they move only the extra
// pieces.  Restricting an outer move to the skeleton gives a homomorphism from
// the outer-move subgroup onto the 3x3 face-turn group; the inner slices lie in
// the kernel of the skeleton action.  The central slice projects to a slice
// move, not a 3x3 face turn, and reduction handles it separately.

template<int N>
std::array<int,9> skeleton(const Cube<N>& c, int face) {
    constexpr int mid = (N - 1) / 2;
    int ids[3] = {0, mid, N - 1};
    std::array<int,9> out{};
    int k = 0;
    for (int r = 0; r < 3; ++r)
        for (int col = 0; col < 3; ++col)
            out[k++] = c.s[Cube<N>::idx(face, ids[r], ids[col])];
    return out;
}

template<int N>
bool skeleton_solved(const Cube<N>& c) {
    for (int f = 0; f < 6; ++f) {
        auto a = skeleton<N>(c, f);
        for (int x : a) if (x != f) return false;
    }
    return true;
}

template<int N>
bool skeleton_matches(const Cube<N>& big, const Cube<3>& small) {
    for (int f = 0; f < 6; ++f) {
        auto a = skeleton<N>(big, f);
        for (int k = 0; k < 9; ++k)
            if (a[k] != small.s[f * 9 + k]) return false;
    }
    return true;
}

void print_3x3_face(const char* label, const std::array<int,9>& a) {
    const char* names = "UDFBLR";
    std::cout << "  " << label << ": ";
    for (int i = 0; i < 9; ++i) {
        std::cout << names[a[i]];
        if (i % 3 == 2 && i != 8) std::cout << "/";
    }
    std::cout << "\n";
}

int main() {
    // A nontrivial sequence of outer face turns.
    std::vector<std::pair<Face,Dir>> seq = {
        {Face::R,Dir::CW}, {Face::U,Dir::CW}, {Face::R,Dir::CCW},
        {Face::U,Dir::CCW}, {Face::F,Dir::Half}, {Face::D,Dir::CW},
        {Face::B,Dir::CCW}
    };

    // ── 5x5 ───────────────────────────────────────────────────────────────────
    auto c5 = Cube<5>::solved();
    auto c3 = Cube<3>::solved();
    for (auto [f, d] : seq) {
        c5 = c5.apply(make_move<5>(f, 0, d));
        c3 = c3.apply(make_move<3>(f, 0, d));
    }

    std::cout << "=== 5x5 skeleton after an outer-move sequence ===\n";
    std::cout << "5x5 skeleton == 3x3 state: " << skeleton_matches<5>(c5, c3) << "\n";
    print_3x3_face("5x5 F skeleton", skeleton<5>(c5, Face::F));
    print_3x3_face("3x3 F face    ", {
        c3.s[Cube<3>::idx(Face::F,0,0)], c3.s[Cube<3>::idx(Face::F,0,1)], c3.s[Cube<3>::idx(Face::F,0,2)],
        c3.s[Cube<3>::idx(Face::F,1,0)], c3.s[Cube<3>::idx(Face::F,1,1)], c3.s[Cube<3>::idx(Face::F,1,2)],
        c3.s[Cube<3>::idx(Face::F,2,0)], c3.s[Cube<3>::idx(Face::F,2,1)], c3.s[Cube<3>::idx(Face::F,2,2)]
    });

    // ── Inner layers ──────────────────────────────────────────────────────────
    std::cout << "\n=== What the inner layers see ===\n";
    bool layer1_fixes = true;
    for (int f = 0; f < 6; ++f)
        if (!skeleton_solved<5>(Cube<5>::solved().apply(
                make_move<5>((Face)f, 1, Dir::CW))))
            layer1_fixes = false;
    std::cout << "5x5 layer 1 fixes the skeleton: " << layer1_fixes << "\n";

    auto innerR = make_move<5>(Face::R, 1, Dir::CW);
    auto moved = Cube<5>::solved().apply(innerR);
    std::cout << "after an inner R: skeleton solved = " << skeleton_solved<5>(moved)
              << ", whole cube solved = " << moved.is_solved() << "\n";
    std::cout << "  (the center/wing pieces moved; the 3x3 skeleton did not)\n";

    bool layer2_fixes = true;
    for (int f = 0; f < 6; ++f)
        if (!skeleton_solved<5>(Cube<5>::solved().apply(
                make_move<5>((Face)f, 2, Dir::CW))))
            layer2_fixes = false;
    std::cout << "5x5 central slice (layer 2) fixes the skeleton: "
              << layer2_fixes << "  (it is a slice move, not a 3x3 face turn)\n";

    // ── 7x7 ───────────────────────────────────────────────────────────────────
    auto c7 = Cube<7>::solved();
    auto c3b = Cube<3>::solved();
    for (auto [f, d] : seq) {
        c7 = c7.apply(make_move<7>(f, 0, d));
        c3b = c3b.apply(make_move<3>(f, 0, d));
    }
    std::cout << "\n=== 7x7 ===\n";
    std::cout << "7x7 skeleton == 3x3 state: " << skeleton_matches<7>(c7, c3b) << "\n";
    for (int layer = 1; layer <= 2; ++layer) {
        bool fixes = true;
        for (int f = 0; f < 6; ++f)
            if (!skeleton_solved<7>(Cube<7>::solved().apply(
                    make_move<7>((Face)f, layer, Dir::CW))))
                fixes = false;
        std::cout << "7x7 layer " << layer << " fixes the skeleton: " << fixes << "\n";
    }

    std::cout << "\n=== Group-theoretic reading ===\n";
    std::cout << "Restricting an outer move to the skeleton gives a homomorphism\n";
    std::cout << "from the outer-move subgroup onto the 3x3 face-turn group.\n";
    std::cout << "Every 3x3 face turn has an outer-move preimage, so it is onto.\n";
    std::cout << "Inner slices at depths 1..(N-3)/2 fix the skeleton and lie in\n";
    std::cout << "the kernel of the skeleton action.\n";
    std::cout << "The central slice projects to a slice move on the skeleton, not\n";
    std::cout << "to a 3x3 face turn; reduction handles it separately.\n";
    std::cout << "Reduction solves the kernel first (centers and wings), leaving\n";
    std::cout << "the outer-move image, which is an ordinary 3x3.\n";

    return 0;
}
