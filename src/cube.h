#pragma once
#include "permutation.h"
#include <array>
#include <ostream>

// Face indices
enum Face { U=0, D=1, F=2, B=3, L=4, R=5 };

// An NxN cube. Sticker index: face*N*N + row*N + col
// Solved state: sticker at position i has color i / (N*N)  (i.e. face number)
template<int N>
struct Cube {
    static constexpr int Stickers = 6 * N * N;
    std::array<int, Stickers> s;

    static Cube solved() {
        Cube c;
        for (int i = 0; i < Stickers; ++i) c.s[i] = i / (N * N);
        return c;
    }

    static int idx(int face, int row, int col) {
        return face * N * N + row * N + col;
    }

    Cube apply(const Permutation<Stickers>& perm) const {
        Cube c;
        for (int i = 0; i < Stickers; ++i) c.s[perm.p[i]] = s[i];
        return c;
    }

    bool is_solved() const { return *this == solved(); }

    bool operator==(const Cube& o) const { return s == o.s; }
};

template<int N>
std::ostream& operator<<(std::ostream& os, const Cube<N>& c) {
    const char* names = "UDFBLR";
    for (int f = 0; f < 6; ++f) {
        os << names[f] << ": ";
        for (int i = 0; i < N*N; ++i) os << c.s[f*N*N + i];
        os << '\n';
    }
    return os;
}
