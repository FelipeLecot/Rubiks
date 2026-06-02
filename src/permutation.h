#pragma once
#include <array>
#include <numeric>
#include <cassert>

// A permutation of Size elements. p[i] = where position i maps TO.
template<int Size>
struct Permutation {
    std::array<int, Size> p;

    static Permutation identity() {
        Permutation r;
        for (int i = 0; i < Size; ++i) r.p[i] = i;
        return r;
    }

    // (this ∘ other)(x) = this(other(x))  — apply other first, then this
    Permutation compose(const Permutation& other) const {
        Permutation r;
        for (int i = 0; i < Size; ++i) r.p[i] = p[other.p[i]];
        return r;
    }

    Permutation inverse() const {
        Permutation r;
        for (int i = 0; i < Size; ++i) r.p[p[i]] = i;
        return r;
    }

    // +1 = even, -1 = odd
    int parity() const {
        std::array<bool, Size> visited{};
        int sign = 1;
        for (int i = 0; i < Size; ++i) {
            if (visited[i]) continue;
            int len = 0;
            int j = i;
            while (!visited[j]) { visited[j] = true; j = p[j]; ++len; }
            if (len % 2 == 0) sign = -sign;
        }
        return sign;
    }

    // Smallest k such that this^k = identity.
    // Computed as the lcm of the cycle lengths — exact regardless of magnitude
    // (Rubik element orders reach 1260 on a 3x3, far beyond Size).
    long long order() const {
        std::array<bool, Size> visited{};
        long long lcm = 1;
        for (int i = 0; i < Size; ++i) {
            if (visited[i]) continue;
            int len = 0, j = i;
            while (!visited[j]) { visited[j] = true; j = p[j]; ++len; }
            lcm = std::lcm(lcm, (long long)len);
        }
        return lcm;
    }

    bool operator==(const Permutation& o) const { return p == o.p; }
    bool operator!=(const Permutation& o) const { return p != o.p; }
};
