#include "cube.h"
#include "moves.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <functional>
#include <numeric>

// Demonstrates BFS on the Cayley graph of a 2x2 Rubik's cube.
// The 2x2 has 3,674,160 states — small enough to explore fully.
// We BFS from solved, find the diameter (God's Number = 14 in HTM for 2x2),
// and identify the "superflip" analogue (farthest state).

// Hash for Cube<2> state
struct Cube2Hash {
    size_t operator()(const Cube<2>& c) const {
        size_t h = 0;
        for (int x : c.s) h = h * 31 + x;
        return h;
    }
};

struct Cube2Eq {
    bool operator()(const Cube<2>& a, const Cube<2>& b) const { return a == b; }
};

using DistMap = std::unordered_map<Cube<2>, int, Cube2Hash, Cube2Eq>;

int main() {
    // The 2x2 has no centers, so without fixing a corner every state appears 24 times
    // (once per whole-cube orientation). We fix the DBL corner by restricting generators
    // to {R, U, F} — none of which move the DBL corner — giving exactly 3,674,160 states.
    std::vector<std::pair<std::string, Permutation<Cube<2>::Stickers>>> gens;
    const Face faces[] = {Face::R, Face::U, Face::F};
    const char* fnames[] = {"R","U","F"};
    for (int i = 0; i < 3; ++i) {
        auto cw   = make_move<2>(faces[i], 0, Dir::CW);
        auto ccw  = make_move<2>(faces[i], 0, Dir::CCW);
        auto half = make_move<2>(faces[i], 0, Dir::Half);
        gens.push_back({std::string(fnames[i]),      cw  });
        gens.push_back({std::string(fnames[i])+"'",  ccw });
        gens.push_back({std::string(fnames[i])+"2",  half});
    }

    auto start = Cube<2>::solved();
    DistMap dist;
    dist[start] = 0;

    std::queue<Cube<2>> q;
    q.push(start);

    int max_dist = 0;
    Cube<2> farthest = start;
    size_t total = 0;

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        int d = dist[cur];

        for (auto& [name, gen] : gens) {
            auto next = cur.apply(gen);
            if (dist.find(next) == dist.end()) {
                dist[next] = d + 1;
                if (d + 1 > max_dist) {
                    max_dist = d + 1;
                    farthest = next;
                }
                q.push(next);
                ++total;
            }
        }
    }

    std::cout << "=== Cayley graph BFS on 2x2 ===\n";
    std::cout << "Total states reachable: " << dist.size() << "  (expect 3,674,160)\n";
    std::cout << "God's Number (diameter): " << max_dist
              << "  (with RUF generators; 14 with all 6 faces)\n\n";

    // Distribution of states by distance
    std::vector<int> cnt(max_dist + 1, 0);
    for (auto& [state, d] : dist) cnt[d]++;
    std::cout << "States at each distance:\n";
    for (int d = 0; d <= max_dist; ++d)
        std::cout << "  d=" << d << ": " << cnt[d] << "\n";

    std::cout << "\nFarthest state (one of possibly many at d=" << max_dist << "):\n";
    std::cout << farthest;

    return 0;
}
