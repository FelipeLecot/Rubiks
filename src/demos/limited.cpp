#include "cube.h"
#include "moves.h"
#include "group.h"
#include "solvability.h"
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <queue>
#include <unordered_map>

// ── Can you solve a cube with only some of the moves? ─────────────────────────
//
// The reachable positions from solved under a set S of moves form the subgroup
// <S>.  If the scrambled position is outside <S>, no sequence of moves in S can
// ever solve it.  This demo computes |<R,U>| on a 3x3 exactly (Schreier–Sims)
// and exhibits a linear-algebra invariant that proves a state built with F is
// outside the subgroup.
//
// The second half is the linear-algebra view.  On the 54 sticker positions,
// every allowed move is a permutation matrix.  If O is an orbit of positions
// under <S>, the sum of the sticker values on O is a conserved linear
// functional.  The space of such functionals has dimension equal to the
// number of orbits: 25 for <R,U>, 8 for the full cube group.

using C3 = Cube<3>;
using C2 = Cube<2>;

struct C2Hash {
    std::size_t operator()(const C2& c) const {
        std::size_t h = 0;
        for (int x : c.s) h = h * 31 + x;
        return h;
    }
};
struct C2Eq {
    bool operator()(const C2& a, const C2& b) const { return a == b; }
};

static std::string pos_name(int p) {
    const char* faces = "UDFBLR";
    int f = p / 9, rem = p % 9;
    return std::string(1, faces[f]) + "(" + std::to_string(rem / 3) + "," +
           std::to_string(rem % 3) + ")";
}

int main() {
    // ── The restricted move set ───────────────────────────────────────────────
    std::vector<Permutation<54>> ru;
    for (Face f : {Face::R, Face::U}) {
        ru.push_back(make_move<3>(f, 0, Dir::CW));
        ru.push_back(make_move<3>(f, 0, Dir::CCW));
    }

    const std::uint64_t order_ru = group::order<54>(ru);
    const std::uint64_t index_ru = 588597166080ULL; // |G| / |<R,U>|

    std::cout << "=== 3x3 with generators {R, U} ===\n";
    std::cout << "|<R,U>|          = " << order_ru << "\n";
    std::cout << "[G:<R,U>]        = " << index_ru << "\n";
    std::cout << "probability a random legal state is in <R,U> = "
              << (1.0L / (long double)index_ru) << "\n\n";

    // ── The invariant that makes <R,U> proper ─────────────────────────────────
    auto orbits_ru = group::orbits<54>(ru);
    int fixed = 0;
    for (auto& o : orbits_ru) if (o.size() == 1) ++fixed;

    std::cout << "=== Orbit decomposition under <R,U> ===\n";
    std::cout << "number of sticker orbits: " << orbits_ru.size() << "\n";
    std::cout << "orbit sizes: ";
    for (auto& o : orbits_ru) std::cout << o.size() << " ";
    std::cout << "\n";
    std::cout << "fixed sticker positions: " << fixed << "\n";
    std::cout << "  (centers, the DFL and DBL corners, and the DF/DB/DL/FL/BL edges)\n\n";

    // The full move set has only 8 sticker orbits: 24 corners, 24 edges, and
    // the six fixed centers.
    std::vector<Permutation<54>> full;
    for (Face f : {Face::U, Face::D, Face::F, Face::B, Face::L, Face::R}) {
        full.push_back(make_move<3>(f, 0, Dir::CW));
        full.push_back(make_move<3>(f, 0, Dir::CCW));
    }
    auto orbits_full = group::orbits<54>(full);
    std::cout << "number of sticker orbits under all six faces: "
              << orbits_full.size() << "  (24, 24, and six 1s)\n\n";

    // ── A linear-algebra witness ──────────────────────────────────────────────
    auto F = make_move<3>(Face::F, 0, Dir::CW);
    auto f_state = C3::solved().apply(F);

    int witness = -1;
    for (int p = 0; p < 54; ++p) {
        bool is_fixed = false;
        for (auto& o : orbits_ru)
            if (o.size() == 1 && o[0] == p) is_fixed = true;
        if (is_fixed && f_state.s[p] != C3::solved().s[p]) { witness = p; break; }
    }

    std::cout << "=== Linear-algebra witness ===\n";
    if (witness >= 0) {
        std::cout << "F changes the color at fixed position " << pos_name(witness)
                  << " from " << "UDFBLR"[C3::solved().s[witness]]
                  << " to " << "UDFBLR"[f_state.s[witness]] << ".\n";
    }
    std::cout << "For each <R,U>-orbit O, the sum of sticker values on O is invariant.\n";
    std::cout << "That is " << orbits_ru.size()
              << " independent linear functionals (the full group has only "
              << orbits_full.size() << ").\n";
    std::cout << "F-solved is a legal 3x3 position (is_solvable = "
              << is_solvable(f_state) << ") but is not in <R,U>.\n\n";

    // A parity-illegal state is outside even the full group.
    auto flipped = C3::solved();
    std::swap(flipped.s[C3::idx(Face::U, 2, 1)], flipped.s[C3::idx(Face::F, 0, 1)]);
    std::cout << "Single edge flip is_solvable = " << is_solvable(flipped)
              << "  (illegal even with all six faces)\n\n";

    // ── The same question on the 2x2 ──────────────────────────────────────────
    std::vector<Permutation<24>> ru2, ruf2;
    for (Face f : {Face::R, Face::U}) {
        ru2.push_back(make_move<2>(f, 0, Dir::CW));
        ru2.push_back(make_move<2>(f, 0, Dir::CCW));
    }
    for (Face f : {Face::R, Face::U, Face::F}) {
        ruf2.push_back(make_move<2>(f, 0, Dir::CW));
        ruf2.push_back(make_move<2>(f, 0, Dir::CCW));
    }
    std::cout << "=== 2x2 comparison ===\n";
    std::cout << "|<R,U>|   = " << group::order<24>(ru2) << "\n";
    std::cout << "|<R,U,F>| = " << group::order<24>(ruf2)
              << "  (the full 2x2 group)\n";
    std::cout << "Adding F closes the gap; on the 3x3, {R,U} remains a proper,\n";
    std::cout << "tiny subgroup of the 4.3e19-position cube group.\n";

    // ── A positive solve inside the 2x2 {R,U} subgroup ────────────────────────
    auto Rc = make_move<2>(Face::R, 0, Dir::CW);
    auto Ri = make_move<2>(Face::R, 0, Dir::CCW);
    auto Uc = make_move<2>(Face::U, 0, Dir::CW);
    auto Ui = make_move<2>(Face::U, 0, Dir::CCW);
    auto U2 = Uc.compose(Uc);

    std::vector<std::pair<std::string, Permutation<24>>> letters = {
        {"R", Rc}, {"R'", Ri}, {"U", Uc}, {"U'", Ui}
    };

    C2 target = C2::solved().apply(Rc).apply(Uc).apply(Ri).apply(U2).apply(Rc);
    std::unordered_map<C2, std::string, C2Hash, C2Eq> word;
    std::queue<C2> q;
    word[target] = "";
    q.push(target);

    while (!q.empty() && !word.count(C2::solved())) {
        C2 cur = q.front(); q.pop();
        std::string curWord = word[cur];
        for (auto& [name, m] : letters) {
            C2 next = cur.apply(m);
            if (!word.count(next)) {
                word.emplace(next, curWord + name);
                q.push(next);
            }
        }
    }

    std::cout << "\n=== A positive limited solve on the 2x2 ===\n";
    std::cout << "scramble (built from R,U only): R U R' U2 R\n";
    auto it = word.find(C2::solved());
    if (it != word.end())
        std::cout << "shortest {R,U} solution: " << it->second << "\n";
    std::cout << "search visited " << word.size()
              << " states, all inside <R,U>.\n";

    return 0;
}
