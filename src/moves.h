#pragma once
#include "cube.h"
#include "permutation.h"
#include <array>

// ── coordinate helpers ────────────────────────────────────────────────────────
// Each face is an NxN grid. We need to rotate face tiles and cycle the
// adjacent edge strips. All moves are derived algorithmically; no tables.

namespace detail {

// Rotate face f by 90° CW in-place (builds the permutation cycles).
// Adds to `p` the cycles for the face tiles only.
template<int N>
void add_face_rotation(std::array<int, 6*N*N>& p, Face f) {
    // CW rotation: (r,c) → (c, N-1-r)
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            int src = Cube<N>::idx(f, r, c);
            int dst = Cube<N>::idx(f, c, N-1-r);
            p[src] = dst;
        }
}

// For a slice parallel to face `f` at depth `layer` (0 = outermost):
// returns the four strips of N sticker indices that cycle together under a
// CW turn of that face, in order (so strip[0][i] → strip[1][i] → ...).
//
// Strips cycle as strips[0][i] → strips[1][i] → strips[2][i] → strips[3][i] → strips[0][i].
//
// Coordinate system (for any N, layer 0 = outermost):
//   lo = layer index (near edge), hi = N-1-layer (far edge)
//   Each face viewed from outside: row 0 = top, col 0 = left.
//   B face: col 0 = toward R globally, col N-1 = toward L globally.
//   L face: col 0 = toward B globally, col N-1 = toward F globally.
//   R face: col 0 = toward F globally, col N-1 = toward B globally.
//
//   U CW: F(lo,i)→L(lo,i)→B(lo,i)→R(lo,i)
//   D CW: F(hi,i)→R(hi,i)→B(hi,i)→L(hi,i)
//   F CW: U(hi,i)→R(i,lo)→D(lo,hi-i)→L(hi-i,hi)
//   B CW: U(lo,i)→L(hi-i,lo)→D(hi,hi-i)→R(i,hi)
//   L CW: U(i,lo)→F(i,lo)→D(i,lo)→B(hi-i,hi)
//   R CW: U(i,hi)→B(hi-i,lo)→D(i,hi)→F(i,hi)
template<int N>
std::array<std::array<int,N>, 4> edge_strips(Face f, int layer) {
    std::array<std::array<int,N>, 4> strips;
    auto idx = [](int face, int r, int c){ return Cube<N>::idx(face, r, c); };
    int lo = layer, hi = N-1-layer;

    if (f == Face::U) {
        // CW from above: pieces FL→BL→BR→FR, stickers F→L→B→R
        for (int i = 0; i < N; ++i) strips[0][i] = idx(F, lo, i);
        for (int i = 0; i < N; ++i) strips[1][i] = idx(L, lo, i);
        for (int i = 0; i < N; ++i) strips[2][i] = idx(B, lo, i);
        for (int i = 0; i < N; ++i) strips[3][i] = idx(R, lo, i);
    } else if (f == Face::D) {
        // CW from below: pieces FL→FR→BR→BL (opposite of U), stickers F→R→B→L
        for (int i = 0; i < N; ++i) strips[0][i] = idx(F, hi, i);
        for (int i = 0; i < N; ++i) strips[1][i] = idx(R, hi, i);
        for (int i = 0; i < N; ++i) strips[2][i] = idx(B, hi, i);
        for (int i = 0; i < N; ++i) strips[3][i] = idx(L, hi, i);
    } else if (f == Face::F) {
        for (int i = 0; i < N; ++i) strips[0][i] = idx(U, hi, i);
        for (int i = 0; i < N; ++i) strips[1][i] = idx(R, i, lo);
        for (int i = 0; i < N; ++i) strips[2][i] = idx(D, lo, hi-i);
        for (int i = 0; i < N; ++i) strips[3][i] = idx(L, hi-i, hi);
    } else if (f == Face::B) {
        for (int i = 0; i < N; ++i) strips[0][i] = idx(U, lo, i);
        for (int i = 0; i < N; ++i) strips[1][i] = idx(L, hi-i, lo);
        for (int i = 0; i < N; ++i) strips[2][i] = idx(D, hi, hi-i);
        for (int i = 0; i < N; ++i) strips[3][i] = idx(R, i, hi);
    } else if (f == Face::L) {
        for (int i = 0; i < N; ++i) strips[0][i] = idx(U, i, lo);
        for (int i = 0; i < N; ++i) strips[1][i] = idx(F, i, lo);
        for (int i = 0; i < N; ++i) strips[2][i] = idx(D, i, lo);
        for (int i = 0; i < N; ++i) strips[3][i] = idx(B, hi-i, hi);
    } else { // R
        for (int i = 0; i < N; ++i) strips[0][i] = idx(U, i, hi);
        for (int i = 0; i < N; ++i) strips[1][i] = idx(B, hi-i, lo);
        for (int i = 0; i < N; ++i) strips[2][i] = idx(D, i, hi);
        for (int i = 0; i < N; ++i) strips[3][i] = idx(F, i, hi);
    }
    return strips;
}

} // namespace detail

// ── public API ────────────────────────────────────────────────────────────────

enum class Dir { CW, CCW, Half };

// Build the permutation for turning face `f`, layer `layer` (0 = outermost),
// in direction `dir`. Works for any N.
template<int N>
Permutation<6*N*N> make_move(Face f, int layer, Dir dir = Dir::CW) {
    constexpr int S = 6*N*N;
    auto p = Permutation<S>::identity();

    // 1. Face rotation (only for the outermost layer)
    if (layer == 0) detail::add_face_rotation<N>(p.p, f);

    // 2. Edge strip cycle: strip[0]→strip[1]→strip[2]→strip[3]→strip[0]
    auto strips = detail::edge_strips<N>(f, layer);
    // CW: dst[k+1] gets src from strip[k]  (i.e. strip[0][i] goes to strip[1][i])
    for (int i = 0; i < N; ++i) {
        p.p[strips[0][i]] = strips[1][i];
        p.p[strips[1][i]] = strips[2][i];
        p.p[strips[2][i]] = strips[3][i];
        p.p[strips[3][i]] = strips[0][i];
    }

    if (dir == Dir::CCW) return p.inverse();
    if (dir == Dir::Half) return p.compose(p);
    return p;
}

// Convenience: compose a sequence of moves
template<int N>
Permutation<6*N*N> compose_moves(std::initializer_list<Permutation<6*N*N>> moves) {
    auto result = Permutation<6*N*N>::identity();
    for (auto& m : moves) result = m.compose(result);
    return result;
}
