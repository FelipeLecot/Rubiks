#include "cube.h"
#include "moves.h"
#include "group.h"
#include "solvability.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>

// ── What happens to the centers? ──────────────────────────────────────────────
//
// 3x3, face turns only : every center sticker is a fixed point of the
//                        permutation.  The center cubie can still rotate in
//                        place, which matters only for a marked "supercube".
// 3x3, slice turns too : the six centers form one orbit of size 6.
// 4x4                   : the 24 centers form one orbit; there is no fixed
//                        center to anchor the reduction.
// 5x5                   : the six exact centers are fixed by the reduction
//                        moves (layers 0 and 1) but a central-slice turn
//                        permutes them as an orbit of 6.  The 24 orthogonal
//                        and 24 diagonal centers are separate orbits.

template<int N>
std::vector<Permutation<6*N*N>> moves_up_to_layer(int maxLayer) {
    std::vector<Permutation<6*N*N>> g;
    const Face faces[] = {Face::U, Face::D, Face::F, Face::B, Face::L, Face::R};
    for (int layer = 0; layer <= maxLayer; ++layer)
        for (Face f : faces) {
            g.push_back(make_move<N>(f, layer, Dir::CW));
            g.push_back(make_move<N>(f, layer, Dir::CCW));
            g.push_back(make_move<N>(f, layer, Dir::Half));
        }
    return g;
}

template<int N>
std::vector<int> centers() {
    std::vector<int> out;
    for (int f = 0; f < 6; ++f)
        for (int r = 1; r <= N - 2; ++r)
            for (int c = 1; c <= N - 2; ++c)
                out.push_back(Cube<N>::idx((Face)f, r, c));
    return out;
}

template<int N>
std::string pos_name(int p) {
    const char* faces = "UDFBLR";
    int f = p / (N * N), rem = p % (N * N);
    return std::string(1, faces[f]) + "(" + std::to_string(rem / N) + "," +
           std::to_string(rem % N) + ")";
}

template<int N>
int parity_on(const Permutation<6*N*N>& m, const std::vector<int>& pos) {
    std::array<int, 6*N*N> index;
    index.fill(-1);
    for (int i = 0; i < (int)pos.size(); ++i) index[pos[i]] = i;

    std::vector<int> p(pos.size());
    for (int i = 0; i < (int)pos.size(); ++i) {
        int dst = m.p[pos[i]];
        if (index[dst] < 0) return 0; // the move leaves the set
        p[i] = index[dst];
    }

    std::vector<bool> vis(pos.size(), false);
    int sign = 1;
    for (int i = 0; i < (int)pos.size(); ++i) {
        if (vis[i]) continue;
        int len = 0, j = i;
        while (!vis[j]) { vis[j] = true; j = p[j]; ++len; }
        if (len % 2 == 0) sign = -sign;
    }
    return sign;
}

int corner_parity(const Cube<3>& c) {
    std::array<int, 8> cp{};
    for (int pos = 0; pos < 8; ++pos) {
        cp[pos] = detail3::find_corner(
            c.s[detail3::CORNERS[pos][0]],
            c.s[detail3::CORNERS[pos][1]],
            c.s[detail3::CORNERS[pos][2]]);
    }
    return detail3::perm_parity(cp);
}

void center_twist_demo(const char* name, std::vector<std::pair<Face,Dir>> seq) {
    int twist = 0;
    auto c = Cube<3>::solved();
    for (auto [f, d] : seq) {
        c = c.apply(make_move<3>(f, 0, d));
        twist += (d == Dir::CW) ? 1 : (d == Dir::CCW) ? -1 : 2;
    }
    twist = ((twist % 4) + 4) % 4;
    int parity = corner_parity(c);
    std::cout << "  " << name << ": twist sum = " << twist
              << " (mod 2 = " << (twist % 2) << "), corner parity = "
              << ((parity > 0) ? "even" : "odd") << "\n";
}

int main() {
    // ── 3x3 ───────────────────────────────────────────────────────────────────
    auto c3 = centers<3>();
    auto face3 = moves_up_to_layer<3>(0);
    auto slice3 = moves_up_to_layer<3>(1);

    bool fixed3 = true;
    for (auto& g : face3)
        for (int p : c3) if (g.p[p] != p) fixed3 = false;

    std::cout << "=== 3x3 ===\n";
    std::cout << "face turns only: all six centers fixed? " << fixed3 << "\n";
    std::cout << "sticker orbit of U center under face turns: "
              << group::orbit_of(Cube<3>::idx(Face::U,1,1), face3).size() << "\n";
    std::cout << "sticker orbit of U center if middle-slice turns are allowed: "
              << group::orbit_of(Cube<3>::idx(Face::U,1,1), slice3).size() << "\n";
    std::cout << "  (the standard cube group is the face-turn group; a slice-turn\n";
    std::cout << "   group is a different, larger group)\n\n";

    std::cout << "marked-center (supercube) orientation:\n";
    std::cout << "  each face turn rotates exactly one center; if the center is\n";
    std::cout << "  marked, the orientation vector lives in Z4^6 and\n";
    std::cout << "  sum(twists) mod 2 equals the corner-permutation parity.\n";
    center_twist_demo("R", {{Face::R,Dir::CW}});
    center_twist_demo("R2", {{Face::R,Dir::Half}});
    center_twist_demo("R U R' U'", {{Face::R,Dir::CW},{Face::U,Dir::CW},
                                     {Face::R,Dir::CCW},{Face::U,Dir::CCW}});
    center_twist_demo("R U", {{Face::R,Dir::CW},{Face::U,Dir::CW}});
    std::cout << "  allowed center orientations: 4^6 / 2 = 2048 per ordinary position\n";
    std::cout << "  ordinary solvability is unaffected; a supercube multiplies the\n";
    std::cout << "  position count by 2048.\n\n";

    // ── 4x4 ───────────────────────────────────────────────────────────────────
    auto c4 = centers<4>();
    auto all4 = moves_up_to_layer<4>(1); // outer and inner slices are both legal

    int fixed4 = 0;
    for (int p : c4) {
        bool fixed = true;
        for (auto& g : all4) if (g.p[p] != p) { fixed = false; break; }
        if (fixed) ++fixed4;
    }

    std::cout << "=== 4x4 ===\n";
    std::cout << "center positions: " << c4.size() << "\n";
    std::cout << "center positions fixed by every move: " << fixed4 << "\n";
    std::cout << "orbit of one center: "
              << group::orbit_of(Cube<4>::idx(Face::U,1,1), all4).size()
              << "  (all 24 centers)\n";

    auto R0 = make_move<4>(Face::R, 0, Dir::CW);
    auto R1 = make_move<4>(Face::R, 1, Dir::CW);
    std::cout << "R outer: center-permutation parity = " << parity_on<4>(R0, c4)
              << ", full 96-sticker parity = " << R0.parity() << "\n";
    std::cout << "R inner: center-permutation parity = " << parity_on<4>(R1, c4)
              << ", full 96-sticker parity = " << R1.parity() << "\n";
    std::cout << "  (center parity is a new, independent contribution to the\n";
    std::cout << "   parity bookkeeping; there is no fixed center frame)\n";

    int moved_center = -1;
    for (int p : c4) if (R1.p[p] != p) { moved_center = p; break; }
    if (moved_center >= 0) {
        std::cout << "inner R moves a center: " << pos_name<4>(moved_center)
                  << " -> " << pos_name<4>(R1.p[moved_center]) << "\n\n";
    }

    // ── 5x5 ───────────────────────────────────────────────────────────────────
    auto c5 = centers<5>();
    auto reduction5 = moves_up_to_layer<5>(1); // outer + inner reduction slices
    auto full5 = moves_up_to_layer<5>(2);      // also the central slice

    std::cout << "=== 5x5 ===\n";
    std::cout << "center positions: " << c5.size() << "\n";
    std::cout << "under skeleton-preserving moves (layers 0,1):\n";
    std::cout << "  exact center orbit: "
              << group::orbit_of(Cube<5>::idx(Face::U,2,2), reduction5).size() << "\n";
    std::cout << "  orthogonal center orbit: "
              << group::orbit_of(Cube<5>::idx(Face::U,2,1), reduction5).size() << "\n";
    std::cout << "  diagonal center orbit: "
              << group::orbit_of(Cube<5>::idx(Face::U,1,1), reduction5).size() << "\n";
    std::cout << "under all moves (including the central slice):\n";
    std::cout << "  exact center orbit: "
              << group::orbit_of(Cube<5>::idx(Face::U,2,2), full5).size() << "\n";
    std::cout << "  orthogonal center orbit: "
              << group::orbit_of(Cube<5>::idx(Face::U,2,1), full5).size() << "\n";
    std::cout << "  diagonal center orbit: "
              << group::orbit_of(Cube<5>::idx(Face::U,1,1), full5).size() << "\n";

    for (int layer = 0; layer <= 2; ++layer) {
        auto m = make_move<5>(Face::R, layer, Dir::CW);
        std::cout << "  R layer " << layer << ": center parity = "
                  << parity_on<5>(m, c5) << ", full 150-sticker parity = "
                  << m.parity() << "\n";
    }

    std::cout << "\nThe 3x3 skeleton of a 5x5 is corners + midges + the six\n";
    std::cout << "exact centers.  Outer moves act on it as 3x3 moves; layers\n";
    std::cout << "1..(N-3)/2 fix it and move only the extra centers/wings.\n";
    std::cout << "The old 'inner 3x3 patch' is not that skeleton; embedded.cpp\n";
    std::cout << "now demonstrates the correct projection.\n";

    return 0;
}
