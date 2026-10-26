#include "cube.h"
#include "moves.h"
#include "group.h"
#include "key2.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <vector>

// ── The coset machinery behind God's Number = 20 ─────────────────────────────
//
// The 3x3 part computes the order of the Kociemba half-turn subgroup
//   H = <U, D, F2, B2, R2, L2>
// and its index in the cube group.  The 2x2 part is a complete miniature of the
// coset solver used by Rokicki–Kociemba–Davidson–Dethridge: for every coset
// H*r it measures the distances from r to all elements of H.  Visiting all of
// H by phase-1 maneuvers of length N proves every element of H*r is solvable in
// at most N moves, because (h^{-1} r) * m = id whenever r*m = h.

using C2 = Cube<2>;

int main() {
    // ── The 3x3 subgroup and its cosets ───────────────────────────────────────
    std::vector<Permutation<54>> hgens;
    hgens.push_back(make_move<3>(Face::U, 0, Dir::CW));
    hgens.push_back(make_move<3>(Face::D, 0, Dir::CW));
    hgens.push_back(make_move<3>(Face::F, 0, Dir::Half));
    hgens.push_back(make_move<3>(Face::B, 0, Dir::Half));
    hgens.push_back(make_move<3>(Face::R, 0, Dir::Half));
    hgens.push_back(make_move<3>(Face::L, 0, Dir::Half));

    const std::uint64_t h_order = group::order<54>(hgens);
    const std::uint64_t h_formula = 40320ULL * 24ULL * 40320ULL / 2ULL;
    const std::uint64_t index = 2217093120ULL; // |G| / |H|
    const std::uint64_t reduced = 55882296ULL;

    std::cout << "=== 3x3: H = <U, D, F2, B2, R2, L2> ===\n";
    std::cout << "|H| (Schreier-Sims) = " << h_order << "\n";
    std::cout << "|H| (8! * 4! * 8! / 2) = " << h_formula << "\n";
    std::cout << "|G| = 43,252,003,274,489,856,000\n";
    std::cout << "[G:H] = " << index << " cosets\n";
    std::cout << "index / 48 = " << index / 48
              << "  (the action on cosets is not free)\n";
    std::cout << "cosets after symmetry + set covering = " << reduced << "\n";
    std::cout << "estimated work = " << reduced << " * 20 s = "
              << (reduced * 20.0 / (365.25 * 24 * 3600)) << " CPU-years\n\n";

    std::cout << "Coset-solver lemma:\n";
    std::cout << "  if r*m = h with h in H, then (h^-1 r)*m = id.\n";
    std::cout << "  As h runs over H, h^-1 r runs over the whole coset H*r.\n";
    std::cout << "  Visiting every h by maneuvers of length <= N therefore solves\n";
    std::cout << "  every element of the coset in at most N moves.\n\n";

    // ── Miniature coset proof on the 2x2 ──────────────────────────────────────
    std::vector<Permutation<24>> full;
    const Face faces[] = {Face::R, Face::U, Face::F};
    for (int i = 0; i < 3; ++i) {
        full.push_back(make_move<2>(faces[i], 0, Dir::CW));
        full.push_back(make_move<2>(faces[i], 0, Dir::CCW));
        full.push_back(make_move<2>(faces[i], 0, Dir::Half));
    }

    std::vector<Permutation<24>> h2gens;
    for (int i = 0; i < 3; ++i)
        h2gens.push_back(make_move<2>(faces[i], 0, Dir::Half));

    // Enumerate H2 = <R2,U2,F2> as group elements.
    std::vector<Permutation<24>> H2{Permutation<24>::identity()};
    std::set<std::array<int,24>> seen_h2;
    seen_h2.insert(Permutation<24>::identity().p);
    for (std::size_t i = 0; i < H2.size(); ++i) {
        for (auto& g : h2gens) {
            auto x = H2[i].compose(g);
            if (seen_h2.insert(x.p).second) H2.push_back(x);
        }
    }

    // Full BFS from solved on the 2x2.
    std::vector<unsigned char> dist(c2key::NumStates, 255);
    std::vector<C2> q;
    std::vector<int> qk;
    q.reserve(c2key::NumStates);
    qk.reserve(c2key::NumStates);

    int start = c2key::of(C2::solved());
    dist[start] = 0;
    q.push_back(C2::solved());
    qk.push_back(start);
    int diameter = 0;

    for (std::size_t head = 0; head < q.size(); ++head) {
        int d = dist[qk[head]];
        for (auto& g : full) {
            C2 x = q[head].apply(g);
            int k = c2key::of(x);
            if (dist[k] == 255) {
                dist[k] = (unsigned char)(d + 1);
                if (d + 1 > diameter) diameter = d + 1;
                q.push_back(x);
                qk.push_back(k);
            }
        }
    }

    // Diameter of H2 itself, using only the H2 generators.
    std::map<int,int> hdist;
    std::vector<C2> hq{C2::solved()};
    hdist[start] = 0;
    int hdiam = 0;
    for (std::size_t head = 0; head < hq.size(); ++head) {
        int d = hdist[c2key::of(hq[head])];
        for (auto& g : h2gens) {
            C2 x = hq[head].apply(g);
            int k = c2key::of(x);
            if (!hdist.count(k)) {
                hdist[k] = d + 1;
                if (d + 1 > hdiam) hdiam = d + 1;
                hq.push_back(x);
            }
        }
    }

    // Visit every coset once, and record both the nearest and farthest
    // distance from its representative to H.
    std::vector<unsigned char> coset_seen(c2key::NumStates, 0);
    int cosets = 0, max_min = 0, max_max = 0;

    for (std::size_t i = 0; i < q.size(); ++i) {
        int k = qk[i];
        if (coset_seen[k]) continue;
        ++cosets;

        int mn = 255, mx = 0;
        for (auto& h : H2) {
            int v = dist[c2key::of(q[i].apply(h.inverse()))];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        for (auto& h : H2)
            coset_seen[c2key::of(q[i].apply(h))] = 1;

        if (mn > max_min) max_min = mn;
        if (mx > max_max) max_max = mx;
    }

    std::cout << "=== 2x2 miniature coset proof ===\n";
    std::cout << "states reached: " << q.size() << " (expect 3,674,160)\n";
    std::cout << "diameter by BFS: " << diameter << "\n";
    std::cout << "|H2| = " << H2.size() << ", diameter(H2) = " << hdiam << "\n";
    std::cout << "cosets H2*r: " << cosets << "  (expect 153,090)\n";
    std::cout << "max over cosets of min distance to H2: " << max_min << "\n";
    std::cout << "max over cosets of max distance to H2: " << max_max << "\n";
    std::cout << "naive two-phase bound = " << max_min << " + " << hdiam
              << " = " << max_min + hdiam << "\n";
    std::cout << "coset bound = " << max_max
              << "  (the true diameter, because max_h d(r,h) includes h = id)\n";

    return 0;
}
