# Shortest possible ZBLL algorithms (slice turn metric)

Every ZBLL case solved **provably optimally in STM**: every layer turn counts 1
(`R`, `R2`, `M`, `M2`, `r`, …), rotations are free, and so are AUF before and after.
This is the metric in which speedcubers usually quote "the shortest alg". For
the half-turn metric (face turns only, the FMC metric), see [ZBLL_FMC.md](ZBLL_FMC.md).

All 12800 optimal solutions are in
[`zbll_stm_optimal_solutions.txt`](zbll_stm_optimal_solutions.txt), with pre- and
post-AUF in parentheses. Every one of them was replayed on the repo's sticker
model and checked to solve its case.

## Summary

| Set | Cases | Mean | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | Median # optimal |
|---|--:|--:|--:|--:|--:|--:|--:|--:|--:|--:|--:|
| T | 72 | 11.74 |  | 2 | 4 | 5 | 14 | 23 | 23 | 1 | 8 |
| U | 72 | 11.88 |  |  | 7 | 4 | 8 | 26 | 26 | 1 | 24 |
| L | 72 | 11.46 |  | 2 | 8 | 4 | 18 | 21 | 19 |  | 12 |
| H | 40 | 12.05 |  |  |  | 2 | 11 | 10 | 17 |  | 8 |
| Pi | 72 | 12.14 |  |  | 2 | 2 | 7 | 34 | 27 |  | 8 |
| S | 72 | 11.53 | 3 |  | 4 | 4 | 17 | 27 | 16 | 1 | 16 |
| AS | 72 | 11.53 | 3 |  | 4 | 4 | 17 | 27 | 16 | 1 | 16 |
| PLL | 21 | 10.57 | 4 |  | 2 | 3 | 4 | 3 | 4 | 1 | 24 |
| **All** | 493 | 11.69 | 10 | 4 | 31 | 28 | 96 | 171 | 148 | 5 | 12 |

- **Mean 11.69 STM, max 14.** In HTM the mean is 12.10 and the max 15.
- **Slices help less often than you'd expect.** For 322 of 493 cases the STM
  optimum equals the HTM optimum. 145 cases save 1 move, 24 save 2, and only
  two save more:

  | Case | HTM | STM | Shortest STM solution |
  |---|--:|--:|---|
  | PLL-3 (Z perm) | 12 | **7** | `(U) S2 D M2 D' M S2 M'` |
  | U-65 | 12 | **9** | `(U2) R' F' M U' M' F M U L` |
  | T-65 | 15 | **13** | `(U') R2 F2 R U' M' F' M U M' F L' U2 R2` |
  | T-28 | 14 | **12** | `(U2) F2 U' R' F2 M S2 R F R' B2 R U2` |
  | U-63 | 13 | **11** | `(U) L F' U' F' M' U2 M2 U' F U R'` |

- **The hardest cases** (14 STM) are T-20, U-20, S-66, AS-67 and PLL-17 (the V perm).
- Among the PLLs: Z perm 7 (`S2 D M2 D' M S2 M'`, versus 9 for the usual
  `M' U M2 U M2 U M' U2 M2`), H and Ua/Ub 7, and the G perms 11 (versus 15 for
  the usual ones).

## How it's computed

The search runs in the frame of the centers. There a slice move is the same as
turning the two faces beside it the other way (`M` ≡ `L' R` up to a
rotation), so STM is a face-turn search in which such a pair costs 1. Pattern
databases (all corners, two 7-edge sets) and a table of all 53,926,092
positions within 6 STM of solved are built for this metric. Solutions are
translated back to physical notation by tracking where the centers go. Mirror
and inverse cases are derived from each other (solutions map one-to-one; the
counts are checked to match) and verified like the searched ones.

Case names (`T-1` … `T-72`) are this tool's own numbering, the same as in
[ZBLL_FMC.md](ZBLL_FMC.md). The setup column identifies the case: apply it
to a solved cube.

## All cases

| Case | Optimal STM | # optimal | First faces | Last faces | Setup (inverse of a solution) | Shortest, most comfortable solution |
|---|--:|--:|---|---|---|---|
| T-1 | **12** | 72 | BFLR | BFLR | `L U2 R' U F2 R' F2 R U2 L' U R` | `R' U' L U2 R' F2 R F2 U' R U2 L'` |
| T-2 | **12** | 8 | BFLR | BFLR | `F U2 F2 L F L' F U' R U' R' F' U2` | `(U2) F R U R' U F' L F' L' F2 U2 F'` |
| T-3 | **13** | 32 | BFLR | BFLR | `L U' R2 U F R2 F' L' B U2 B' U' R2 U2` | `(U2) R2 U B U2 B' L F R2 F' U' R2 U L'` |
| T-4 | **13** | 28 | BFLMRS | BFLR | `R' U' R U' R' U2 R2 U R' U R U2 R' U2` | `(U2) R U2 R' U' R U' R2 U2 R U R' U R` |
| T-5 | **13** | 36 | BFLR | BDFLR | `R' U' R U' R U2 R2 U' R2 U' R2 U R U2` | `(U2) R' U' R2 U R2 U R2 U2 R' U R' U R` |
| T-6 | **13** | 32 | BFLR | BFLR | `R' D F2 R2 U R2 D' F2 R U L2 U' L2 U` | `(U') L2 U L2 U' R' F2 D R2 U' R2 F2 D' R` |
| T-7 | **13** | 36 | BFLR | BDFLR | `R U R' U R' U2 R2 U R2 U R2 U' R' U2` | `(U2) R U R2 U' R2 U' R2 U2 R U' R U' R'` |
| T-8 | **12** | 4 | BFLR | BFLR | `L B' R2 F2 R' D R D' F2 R2 B L'` | `L B' R2 F2 D R' D' R F2 R2 B L'` |
| T-9 | **12** | 8 | BFLR | BFLR | `F2 R2 F L F' R2 F2 R' F' L' F R` | `R' F' L F R F2 R2 F L' F' R2 F2` |
| T-10 | **12** | 8 | BFLR | BFLR | `F' U2 F2 R' F' R F' U L' U L F` | `F' L' U' L U' F R' F R F2 U2 F` |
| T-11 | **12** | 4 | BFLR | BFLR | `L' F R2 B2 R D' R' D B2 R2 F' L` | `L' F R2 B2 D' R D R' B2 R2 F' L` |
| T-12 | **13** | 28 | BFLMRS | BFLR | `R U R' U R U2 R2 U' R U' R' U2 R U'` | `(U) R' U2 R U R' U R2 U2 R' U' R U' R'` |
| T-13 | **10** | 8 | BFLR | BFLR | `L' U R' U' R L U2 R' U' R` | `R' U R U2 L' R' U R U' L` |
| T-14 | **11** | 8 | BFLMRS | BFLMRS | `M U R U' L U R2 U R U2 R' U` | `(U') R U2 R' U' R2 U' L' U R' U' M'` |
| T-15 | **10** | 16 | BFLR | BFLR | `R' F R' D2 L B' L' D2 R2 F' U2` | `(U2) F R2 D2 L B L' D2 R F' R` |
| T-16 | **11** | 8 | BFLMRS | BFLMRS | `M' U' R' U L' U' R2 U' R' U2 R U` | `(U') R' U2 R U R2 U L U' R U M` |
| T-17 | **13** | 48 | BFLR | BFLR | `L F R U2 R' U R U2 R2 F R F2 L' U2` | `(U2) L F2 R' F' R2 U2 R' U' R U2 R' F' L'` |
| T-18 | **13** | 8 | BFLMRS | BFLR | `L D' L' F2 M' F' R F L' U F' U F U2` | `(U2) F' U' F U' L F' R' F M F2 L D L'` |
| T-19 | **13** | 8 | BFLR | MSU | `U' F' U F' R U' L' U M' U2 R B R' U2` | `(U2) R B' R' U2 M U' L U R' F U' F U` |
| T-20 | **14** | 416 | BFLMRS | BDFLMRS | `L' U' L U2 S2 U F2 R' F2 U' S2 R' U R U'` | `(U) R' U' R S2 U F2 R F2 U' S2 U2 L' U L` |
| T-21 | **13** | 8 | BFLMRS | BFLR | `R' D R F2 M' F L' F' R U' F U' F' U2` | `(U2) F U F' U R' F L F' M F2 R' D' R` |
| T-22 | **13** | 48 | BFLR | BFLR | `L F2 R' F' R2 U2 R' U' R U2 R' F' L' U2` | `(U2) L F R U2 R' U R U2 R2 F R F2 L'` |
| T-23 | **10** | 8 | BFLR | BFLR | `L U' R U R' L' U2 R U R' U2` | `(U2) R U' R' U2 L R U' R' U L'` |
| T-24 | **13** | 8 | BFLR | MSU | `U F U' F L' U R U' M' U2 L' B' L U2` | `(U2) L' B L U2 M U R' U' L F' U F' U'` |
| T-25 | **11** | 8 | BFLR | BFLRU | `R2 B L' U2 M' U' R D' R' U' R' U` | `(U') R U R D R' U M U2 L B' R2` |
| T-26 | **12** | 12 | BFLR | BDFLR | `D F2 L' F R F' M' U' R U2 R' F' U'` | `(U) F R U2 R' U M F R' F' L F2 D'` |
| T-27 | **11** | 4 | BFLR | BFLR | `R' F E2 F D2 L' D2 F' E2 R F' U'` | `(U) F R' E2 F D2 L D2 F' E2 F' R` |
| T-28 | **12** | 4 | BFLR | U | `U2 R' B2 R F' R' S2 M' F2 R U F2 U2` | `(U2) F2 U' R' F2 M S2 R F R' B2 R U2` |
| T-29 | **8** | 8 | BFLR | BFLR | `R' F' L' F R F' L F` | `F' L' F R' F' L F R` |
| T-30 | **9** | 8 | BFLR | MS | `M' F' U2 F M U2 R U R' U2` | `(U2) R U' R' U2 M' F' U2 F M` |
| T-31 | **11** | 20 | MS | BDEFLRU | `U' F2 U F2 E' R U L' U' M' S U` | `(U') S' M U L U' R' E F2 U' F2 U` |
| T-32 | **11** | 12 | BFLR | BFLR | `L2 U' R' U M F R' F R F' L' U2` | `(U2) L F R' F' R F' M' U' R U L2` |
| T-33 | **11** | 8 | BFLR | BFLMRS | `R2 S2 D R' U R D' S2 R' U' R' U` | `(U') R U R S2 D R' U' R D' S2 R2` |
| T-34 | **13** | 40 | BFLMRS | BFLMRS | `F2 R' U R2 U' F2 U F2 R' F2 R' U' R` | `R' U R F2 R F2 U' F2 U R2 U' R F2` |
| T-35 | **12** | 44 | BFLMRS | BFLMRS | `M2 U2 M U' F2 U2 F2 U' F2 R U L'` | `L U' R' F2 U F2 U2 F2 U M' U2 M2` |
| T-36 | **10** | 12 | BFLR | BFLR | `R U R' F' U' L' U2 L U F U2` | `(U2) F' U' L' U2 L U F R U' R'` |
| T-37 | **8** | 8 | BFLR | BFLR | `L F R F' L' F R' F'` | `F R F' L F R' F' L'` |
| T-38 | **11** | 8 | BFLR | BFLRU | `R2 F' L U2 M U R' D R U R U` | `(U') R' U' R' D' R U' M' U2 L' F R2` |
| T-39 | **11** | 20 | MS | BDEFLRU | `U F2 U' F2 E L' U' R U M' S' U'` | `(U) S M U' R' U L E' F2 U F2 U'` |
| T-40 | **11** | 4 | BFLR | BFLR | `L F' E2 F' D2 R D2 F E2 L' F U` | `(U') F' L E2 F' D2 R' D2 F E2 F L'` |
| T-41 | **12** | 44 | BFLMRS | BFLMRS | `M2 U2 M U F2 U2 F2 U F2 L' U' R` | `R' U L F2 U' F2 U2 F2 U' M' U2 M2` |
| T-42 | **12** | 4 | BFLR | U | `U2 F L2 F' R F M2 S' R2 F' U' R2 U'` | `(U) R2 U F R2 S M2 F' R' F L2 F' U2` |
| T-43 | **11** | 8 | BFLR | BFLMRS | `R2 S2 D' R U' R' D S2 R U R U` | `(U') R' U' R' S2 D' R U R' D S2 R2` |
| T-44 | **12** | 12 | BFLR | BDFLR | `R' F' R L U2 M U' R' U' R2 D R2 U2` | `(U2) R2 D' R2 U R U M' U2 L' R' F R` |
| T-45 | **13** | 40 | BFLMRS | BFLMRS | `R2 F U' F2 U R2 U' R2 F R2 F U F' U` | `(U') F U' F' R2 F' R2 U R2 U' F2 U F' R2` |
| T-46 | **9** | 8 | BFLR | MS | `M' F2 U F2 U' F2 L' U' R` | `R' U L F2 U F2 U' F2 M` |
| T-47 | **10** | 12 | BFLR | BFLR | `F R U' R' U' R U2 R' U' F' U2` | `(U2) F U R U2 R' U R U R' F'` |
| T-48 | **11** | 12 | BFLR | BFLR | `R' F' M' F2 R F' U' F' M U F U'` | `(U) F' U' M' F U F R' F2 M F R` |
| T-49 | **9** | 4 | BFLR | BFLR | `F2 R2 F L2 F' R2 F L2 F U2` | `(U2) F' L2 F' R2 F L2 F' R2 F2` |
| T-50 | **12** | 4 | BFLR | U | `U2 F R U2 M' U R2 U L' U' R2 B2` | `B2 R2 U L U' R2 U' M U2 R' F' U2` |
| T-51 | **13** | 100 | BFLMRS | BDFLMRSU | `R2 U' M U2 R U' R' U2 L U R' F R2 U` | `(U') R2 F' R U' L' U2 R U R' U2 M' U R2` |
| T-52 | **12** | 8 | BFLR | BFLR | `F2 D R F' L2 U' R2 B' R' U M2 B'` | `B M2 U' R B R2 U L2 F R' D' F2` |
| T-53 | **12** | 4 | BFLR | U | `U2 B' R' U2 M U' R2 U' L U R2 F2 U2` | `(U2) F2 R2 U' L' U R2 U M' U2 R B U2` |
| T-54 | **12** | 8 | BFLR | BFLR | `F' R' U F U F' U2 F U F' R F U` | `(U') F' R' F U' F' U2 F U' F' U' R F` |
| T-55 | **13** | 32 | BFLMRS | BDFLMRS | `D2 F M' F2 R L2 U R2 U' L U R2 F2 U2` | `(U2) F2 R2 U' L' U R2 U' L2 R' F2 M F' D2` |
| T-56 | **12** | 8 | BFLR | BFLR | `R F U' R' U' R U2 R' U' R F' R'` | `R F R' U R U2 R' U R U F' R'` |
| T-57 | **9** | 4 | BFLR | BFLR | `F2 L2 F' R2 F L2 F' R2 F' U2` | `(U2) F R2 F L2 F' R2 F L2 F2` |
| T-58 | **13** | 32 | BFLMRS | BDFLMRS | `M2 U' R2 F2 R' U2 R F2 R' U2 R' U M2` | `M2 U' R U2 R F2 R' U2 R F2 R2 U M2` |
| T-59 | **13** | 100 | BFLMRS | BDFLMRSU | `R2 U' R' U2 M' U' R U2 L' U' R B' R2 U` | `(U') R2 B R' U L U2 R' U M U2 R U R2` |
| T-60 | **13** | 152 | BFLMRS | BFLMRSU | `F R U' R' U R U R' U R U' R' F'` | `F R U R' U' R U' R' U' R U R' F'` |
| T-61 | **13** | 48 | BDFLMRS | BDFLMRSU | `U R' U' L F' U2 F U R U' M U2 R' U` | `(U') R U2 M' U R' U' F' U2 F L' U R U'` |
| T-62 | **12** | 8 | BFLR | BFLR | `F2 U F' U' R U2 F U2 F' R' U F' U'` | `(U) F U' R F U2 F' U2 R' U F U' F2` |
| T-63 | **11** | 4 | BFLR | BFLR | `L' U L2 D R' F2 R D' L2 U' L U'` | `(U) L' U L2 D R' F2 R D' L2 U' L` |
| T-64 | **12** | 8 | BFLR | BFLR | `F2 D2 B D2 F' R2 E' R' U R' D' L' U2` | `(U2) L D R U' R E R2 F D2 B' D2 F2` |
| T-65 | **13** | 64 | BFLR | BFLR | `R2 U2 L F' M U' M' F M U R' F2 R2 U` | `(U') R2 F2 R U' M' F' M U M' F L' U2 R2` |
| T-66 | **13** | 48 | BDFLMRS | BDFLMRSU | `U' L U R' F U2 F' U' L' U' L U2 M` | `M' U2 L' U L U F U2 F' R U' L' U` |
| T-67 | **12** | 8 | BFLR | BFLR | `L D R U' R E R2 F D2 B' D2 F2 U'` | `(U) F2 D2 B D2 F' R2 E' R' U R' D' L'` |
| T-68 | **11** | 4 | BFLR | BFLR | `R U' R2 D' L F2 L' D R2 U R' U'` | `(U) R U' R2 D' L F2 L' D R2 U R'` |
| T-69 | **12** | 8 | BFLR | BFLR | `R2 L' F R F' M' U' R U2 R' U2 R2` | `R2 U2 R U2 R' U M F R' F' L R2` |
| T-70 | **13** | 96 | BFLMRS | BFLMRSU | `L' U R U' L2 U' R' U L' U' R U2 R' U` | `(U') R U2 R' U L U' R U L2 U R' U' L` |
| T-71 | **12** | 8 | BFLR | BFLR | `R' U F' R' U2 R U2 F U' R' U R2 U'` | `(U) R2 U' R U F' U2 R' U2 R F U' R` |
| T-72 | **12** | 8 | BFLR | BFLR | `R2 U' R U F' U2 R' U2 R F U' R U'` | `(U) R' U F' R' U2 R U2 F U' R' U R2` |
| U-1 | **12** | 72 | BFLR | BFLR | `R' U' L U2 R' F2 R F2 U' R U2 L' U2` | `(U2) L U2 R' U F2 R' F2 R U2 L' U R` |
| U-2 | **13** | 32 | BFLR | BDFLR | `L' U2 R F2 M U F2 U' R2 D L F2 L U2` | `(U2) L' F2 L' D' R2 U F2 U' M' F2 R' U2 L` |
| U-3 | **12** | 8 | BFLR | BFLR | `F R U R' U F' L F' L' F2 U2 F' U2` | `(U2) F U2 F2 L F L' F U' R U' R' F'` |
| U-4 | **13** | 28 | BFLR | BFLMRS | `R U2 R' U' R U' R2 U2 R U R' U R U2` | `(U2) R' U' R U' R' U2 R2 U R' U R U2 R'` |
| U-5 | **13** | 36 | BFLR | BFLRU | `R U R2 U' R2 U' R2 U2 R U' R U' R' U'` | `(U) R U R' U R' U2 R2 U R2 U R2 U' R'` |
| U-6 | **12** | 8 | BFLR | BFLR | `F' L' U' L U' F R' F R F2 U2 F` | `F' U2 F2 R' F' R F' U L' U L F` |
| U-7 | **13** | 36 | BFLR | BFLRU | `R' U' R2 U R2 U R2 U2 R' U R' U R U` | `(U') R' U' R U' R U2 R2 U' R2 U' R2 U R` |
| U-8 | **12** | 4 | BFLR | BFLR | `L' F R2 B2 D' R D R' B2 R2 F' L U2` | `(U2) L' F R2 B2 R D' R' D B2 R2 F' L` |
| U-9 | **12** | 8 | BFLR | BFLR | `R' F' L F R F2 R2 F L' F' R2 F2 U2` | `(U2) F2 R2 F L F' R2 F2 R' F' L' F R` |
| U-10 | **13** | 32 | BFLR | BDFLR | `R U2 L' F2 M U' F2 U L2 D' R' F2 R'` | `R F2 R D L2 U' F2 U M' F2 L U2 R'` |
| U-11 | **12** | 4 | BFLR | BFLR | `L B' R2 F2 D R' D' R F2 R2 B L' U2` | `(U2) L B' R2 F2 R' D R D' F2 R2 B L'` |
| U-12 | **13** | 28 | BFLR | BFLMRS | `R' U2 R U R' U R2 U2 R' U' R U' R' U'` | `(U) R U R' U R U2 R2 U' R U' R' U2 R` |
| U-13 | **13** | 56 | BDFLR | BDFLR | `R U' R' U F' L F' L' F2 R' F R F' U'` | `(U) F R' F' R F2 L F L' F U' R U R'` |
| U-14 | **13** | 32 | BFLMRS | BFLMRS | `R' U L' U' L U' R U R' F2 R' F2 R2 U2` | `(U2) R2 F2 R F2 R U' R' U L' U L U' R` |
| U-15 | **13** | 64 | BFLMRS | BFLMRS | `L' F2 R F' M' U L' U2 R U' L U' R' U2` | `(U2) R U L' U R' U2 L U' M F R' F2 L` |
| U-16 | **13** | 32 | BFLMRS | BFLMRS | `M D2 M2 U' R2 U L2 U L U R2 U2 R U` | `(U') R' U2 R2 U' L' U' L2 U' R2 U M2 D2 M'` |
| U-17 | **13** | 40 | BFLR | BFLRU | `U2 L U R' F U2 F' M U F R U2 R' U2` | `(U2) R U2 R' F' U' M' F U2 F' R U' L' U2` |
| U-18 | **12** | 16 | MS | BFLR | `R' U2 L' F2 R' F' R F2 L2 F U2 M` | `M' U2 F' L2 F2 R' F R F2 L U2 R` |
| U-19 | **12** | 16 | BFLR | MS | `M' U' R2 U2 L' U L U2 R2 L' U2 R U` | `(U') R' U2 L R2 U2 L' U' L U2 R2 U M` |
| U-20 | **14** | 80 | BFLMRS | BDFLMRS | `R' U R U' R' U L' U' L U' F2 R' F2 R2 U2` | `(U2) R2 F2 R F2 U L' U L U' R U R' U' R` |
| U-21 | **12** | 16 | MS | BFLR | `L U2 R F2 L F L' F2 R2 F' U2 M` | `M' U2 F R2 F2 L F' L' F2 R' U2 L'` |
| U-22 | **13** | 40 | BFLR | BFLRU | `U2 R' U' L F' U2 F M U' F' L' U2 L U2` | `(U2) L' U2 L F U M' F' U2 F L' U R U2` |
| U-23 | **13** | 56 | BDFLR | BDFLR | `L' U L U' F R' F R F2 L F' L' F U` | `(U') F' L F L' F2 R' F' R F' U L' U' L` |
| U-24 | **12** | 16 | BFLR | MS | `M U R2 U2 L U' L' U2 R2 L U2 R' U` | `(U') R U2 L' R2 U2 L U L' U2 R2 U' M'` |
| U-25 | **9** | 4 | BFLR | BFLR | `R' U2 R' D' L F2 L' D R2 U` | `(U') R2 D' L F2 L' D R U2 R` |
| U-26 | **11** | 28 | BFLR | BFLMRS | `R' U2 R U R2 D' R U R' D R2` | `R2 D' R U' R' D R2 U' R' U2 R` |
| U-27 | **13** | 52 | BFLMRS | BDFLMRS | `M U F2 U' M2 U' R U L' U2 R' U' R U2` | `(U2) R' U R U2 L U' R' U M2 U F2 U' M'` |
| U-28 | **12** | 28 | BFLMRS | BFLMRS | `M F' R U2 L' U2 L U2 R' U2 F M'` | `M F' U2 R U2 L' U2 L U2 R' F M'` |
| U-29 | **9** | 4 | BFLR | BFLR | `R' U2 R' D' R U2 R' D R2 U` | `(U') R2 D' R U2 R' D R U2 R` |
| U-30 | **12** | 4 | BFLR | BFLR | `R2 S2 R U2 R' D' L F2 L' D S2 R2 U` | `(U') R2 S2 D' L F2 L' D R U2 R' S2 R2` |
| U-31 | **12** | 8 | BFLR | BFLR | `R' B2 R' F' R' L2 F2 L' D2 M D' R2 U` | `(U') R2 D M' D2 L F2 L2 R F R B2 R` |
| U-32 | **13** | 36 | BFLMRS | BFLMRS | `R U2 R' F U2 F' U' R F U' F' U2 R'` | `R U2 F U F' R' U F U2 F' R U2 R'` |
| U-33 | **13** | 8 | BFLR | BFLR | `R' F' M2 U' F2 U M2 F' R U' L' U' L U` | `(U') L' U L U R' F M2 U' F2 U M2 F R` |
| U-34 | **13** | 40 | BFLMRS | BDFLMRS | `S R U' L U' R2 U L' U R2 U2 R S' U` | `(U') S R' U2 R2 U' L U' R2 U L' U R' S'` |
| U-35 | **12** | 4 | BFLR | BFLR | `L' F' U' F2 U M' F R U' R' F' R U'` | `(U) R' F R U R' F' M U' F2 U F L` |
| U-36 | **11** | 4 | BFLR | BFLR | `R2 D' L' F2 L' D2 R' D L2 D2 R' U2` | `(U2) R D2 L2 D' R D2 L F2 L D R2` |
| U-37 | **9** | 4 | BFLR | BFLR | `R U2 R D R' U2 R D' R2 U` | `(U') R2 D R' U2 R D' R' U2 R'` |
| U-38 | **9** | 4 | BFLR | BFLR | `L U2 L D R' F2 R D' L2 U'` | `(U) L2 D R' F2 R D' L' U2 L'` |
| U-39 | **12** | 8 | BFLR | BFLR | `R B2 R F' M F2 L' D2 R' L2 D' R2 U` | `(U') R2 D L2 R D2 L F2 M' F R' B2 R'` |
| U-40 | **13** | 52 | BFLMRS | BDFLMRS | `M U' F2 U M2 U L' U' R U2 L U L' U2` | `(U2) L U' L' U2 R' U L U' M2 U' F2 U M'` |
| U-41 | **12** | 4 | BFLR | BFLR | `R F U F2 U' M' F' L' U L F L' U` | `(U') L F' L' U' L F M U F2 U' F' R'` |
| U-42 | **12** | 28 | BFLMRS | BFLMRS | `M F L' U2 R U2 R' U2 L U2 F' M'` | `M F U2 L' U2 R U2 R' U2 L F' M'` |
| U-43 | **13** | 8 | BFLR | BFLR | `L F M2 U F2 U' M2 F L' U R U R' U'` | `(U) R U' R' U' L F' M2 U F2 U' M2 F' L'` |
| U-44 | **11** | 28 | BFLR | BFLMRS | `R U2 R2 D' R U' R' D R2 U' R' U2` | `(U2) R U R2 D' R U R' D R2 U2 R'` |
| U-45 | **13** | 40 | BFLMRS | BDFLMRS | `S R' U L' U R2 U' L U' R2 U2 R' S' U` | `(U') S R U2 R2 U L' U R2 U' L U' R S'` |
| U-46 | **12** | 4 | BFLR | BFLR | `L2 S2 L' U2 L D R' F2 R D' S2 L2 U'` | `(U) L2 S2 D R' F2 R D' L' U2 L S2 L2` |
| U-47 | **11** | 4 | BFLR | BFLR | `L2 D R F2 R D2 L D' R2 D2 L U2` | `(U2) L' D2 R2 D L' D2 R' F2 R' D' L2` |
| U-48 | **13** | 36 | BFLMRS | BFLMRS | `F' U2 F R' U2 R U F' R' U R U2 F U` | `(U') F' U2 R' U' R F U' R' U2 R F' U2 F` |
| U-49 | **9** | 4 | BFLR | BFLR | `F R2 F L2 F' R2 F L2 F2 U` | `(U') F2 L2 F' R2 F L2 F' R2 F'` |
| U-50 | **12** | 8 | BFLR | BFLR | `F' R' F U' F' U2 F U' F' U' R F U'` | `(U) F' R' U F U F' U2 F U F' R F` |
| U-51 | **13** | 100 | BFLMRS | BDFLMRSU | `R' F2 R U' R2 F2 R2 U' R' U2 R' F2 R2` | `R2 F2 R U2 R U R2 F2 R2 U R' F2 R` |
| U-52 | **12** | 8 | BFLR | BFLR | `F M2 D' R F R2 D L2 B R' U' B2 U'` | `(U) B2 U R B' L2 D' R2 F' R' D M2 F'` |
| U-53 | **12** | 8 | BFLR | BFLR | `R F R' U R U2 R' U R U F' R' U2` | `(U2) R F U' R' U' R U2 R' U' R F' R'` |
| U-54 | **12** | 4 | BFLR | D | `D2 L2 F R F' L2 F' M' F2 L' U' F2 U` | `(U') F2 U L F2 M F L2 F R' F' L2 D2` |
| U-55 | **13** | 32 | BFLMRS | BFLMRSU | `M2 U L' U2 L' F2 L U2 L' F2 L2 U' M2 U` | `(U') M2 U L2 F2 L U2 L' F2 L U2 L U' M2` |
| U-56 | **12** | 4 | BFLR | D | `D2 R2 F' L' F R2 F M' F2 R U F2 U'` | `(U) F2 U' R' F2 M F' R2 F' L F R2 D2` |
| U-57 | **9** | 4 | BFLR | BFLR | `F' L2 F' R2 F L2 F' R2 F2 U'` | `(U) F2 R2 F L2 F' R2 F L2 F` |
| U-58 | **13** | 32 | BFLMRS | BFLMRSU | `M2 U' R U2 R F2 R' U2 R F2 R2 U M2 U'` | `(U) M2 U' R2 F2 R' U2 R F2 R' U2 R' U M2` |
| U-59 | **13** | 100 | BFLMRS | BDFLMRSU | `F R2 F' U F2 R2 F2 U F U2 F R2 F2 U` | `(U') F2 R2 F' U2 F' U' F2 R2 F2 U' F R2 F'` |
| U-60 | **13** | 152 | BFLMRS | BDFLMRS | `F R U R' U' R U' R' U' R U R' F' U2` | `(U2) F R U' R' U R U R' U R U' R' F'` |
| U-61 | **11** | 24 | BFLR | BFLR | `R' F M2 F2 U' F U F M2 F' R U2` | `(U2) R' F M2 F' U' F' U F2 M2 F' R` |
| U-62 | **10** | 4 | BFLR | BFLR | `R' U F U' F' U' R F U2 F' U` | `(U') F U2 F' R' U F U F' U' R` |
| U-63 | **11** | 24 | BFLMRS | BFLMRS | `R U' F' U M2 U2 M F U F L' U'` | `(U) L F' U' F' M' U2 M2 U' F U R'` |
| U-64 | **12** | 32 | BFLMRS | BDEMRS | `M U' M' F U R U' R' F' M U M' U2` | `(U2) M U' M' F R U R' U' F' M U M'` |
| U-65 | **9** | 8 | BFLR | BFLR | `L' U' M' F' M U M' F R U2` | `(U2) R' F' M U' M' F M U L` |
| U-66 | **11** | 24 | BFLR | BFLR | `R2 L F' U2 M U M' F M U R U` | `(U') R' U' M' F' M U' M' U2 F L' R2` |
| U-67 | **12** | 32 | BFLMRS | BDEMRS | `M U' M' F R U R' U' F' M U M' U'` | `(U) M U' M' F U R U' R' F' M U M'` |
| U-68 | **11** | 24 | BFLMRS | BFLMRS | `L' U F U' M2 U2 M F' U' F' R U'` | `(U) R' F U F M' U2 M2 U F' U' L` |
| U-69 | **10** | 4 | BFLR | BFLR | `F U2 F' R' U F U F' U' R U'` | `(U) R' U F U' F' U' R F U2 F'` |
| U-70 | **12** | 320 | BFLR | BFLR | `F' M2 U2 R' F' U2 M2 U2 F' R U2 F U` | `(U') F' U2 R' F U2 M2 U2 F R U2 M2 F` |
| U-71 | **10** | 4 | BFLR | BFLR | `R' U2 R F U' R' U' R U F' U'` | `(U) F U' R' U R U F' R' U2 R` |
| U-72 | **10** | 4 | BFLR | BFLR | `F U' R' U R U F' R' U2 R U` | `(U') R' U2 R F U' R' U' R U F'` |
| L-1 | **11** | 8 | BFLR | BFLR | `F' R D2 R' F U2 F' R D2 R' F U2` | `(U2) F' R D2 R' F U2 F' R D2 R' F` |
| L-2 | **12** | 4 | BFLR | U | `U M U R' U' L U' R U R' U F' U` | `(U') F U' R U' R' U L' U R U' M' U'` |
| L-3 | **13** | 36 | BFLMRS | BFLMRS | `F U R2 U2 R2 L' U R2 U' L U2 R2 F' U2` | `(U2) F R2 U2 L' U R2 U' L R2 U2 R2 U' F'` |
| L-4 | **13** | 64 | BFLMRS | BFLMRS | `M U' L U' L' U2 L U' R' U R U2 R'` | `R U2 R' U' R U L' U2 L U L' U M'` |
| L-5 | **12** | 4 | BFLR | D | `D' F L' F L F' R F' L' F M' F` | `F' M F' L F R' F L' F' L F' D` |
| L-6 | **13** | 36 | BFLMRS | BFLMRS | `F R2 U2 L' U R2 U' R2 L U2 R2 U' F' U` | `(U') F U R2 U2 L' R2 U R2 U' L U2 R2 F'` |
| L-7 | **13** | 36 | BFLMRS | BFLMRS | `F' L2 U2 R U' L2 U R' L2 U2 L2 U F U` | `(U') F' U' L2 U2 L2 R U' L2 U R' U2 L2 F` |
| L-8 | **13** | 36 | BFLMRS | BFLMRS | `F' U' L2 U2 R L2 U' L2 U R' U2 L2 F` | `F' L2 U2 R U' L2 U L2 R' U2 L2 U F` |
| L-9 | **13** | 96 | BFLR | BFLR | `R' F M' U2 L' D2 L U2 M F' R' B2 R2 U2` | `(U2) R2 B2 R F M' U2 L' D2 L U2 M F' R` |
| L-10 | **12** | 4 | BFLR | D | `D F' R F' R' F L' F R F' M' F' U2` | `(U2) F M F R' F' L F' R F R' F D'` |
| L-11 | **12** | 4 | BFLR | U | `U' M U' L U R' U L' U' L U' F U` | `(U') F' U L' U L U' R U' L' U M' U` |
| L-12 | **13** | 16 | BFLR | BFLR | `L' F' L F' L' F2 M' U2 R U R' U R U2` | `(U2) R' U' R U' R' U2 M F2 L F L' F L` |
| L-13 | **9** | 4 | BFLR | BFLR | `L2 D R' F2 R D' L' U2 L' U2` | `(U2) L U2 L D R' F2 R D' L2` |
| L-14 | **12** | 4 | BFLR | BFLR | `R U' R' F' R U M' F U2 F' U' L' U2` | `(U2) L U F U2 F' M U' R' F R U R'` |
| L-15 | **13** | 36 | BFLMRS | BFLMRS | `F' U2 R' U' R F U' R' U2 R F' U2 F` | `F' U2 F R' U2 R U F' R' U R U2 F` |
| L-16 | **12** | 28 | BFLMRS | BFLMRS | `M F U2 L' U2 R U2 R' U2 L F' M' U'` | `(U) M F L' U2 R U2 R' U2 L U2 F' M'` |
| L-17 | **9** | 4 | BFLR | BFLR | `R2 D R' U2 R D' R' U2 R'` | `R U2 R D R' U2 R D' R2` |
| L-18 | **12** | 4 | BFLR | BFLR | `L2 S2 D R' F2 R D' L' U2 L S2 L2 U2` | `(U2) L2 S2 L' U2 L D R' F2 R D' S2 L2` |
| L-19 | **12** | 8 | BFLR | BFLR | `R2 F' M F2 L' D2 R' L2 D' R' U2 R'` | `R U2 R D L2 R D2 L F2 M' F R2` |
| L-20 | **13** | 52 | BFLMRS | BFLMRS | `M' U' F' U' F U R' L' F R F' U R U'` | `(U) R' U' F R' F' L R U' F' U F U M` |
| L-21 | **11** | 4 | BFLR | BFLR | `L' D2 R2 D L' D2 R' F2 R' D' L2 U'` | `(U) L2 D R F2 R D2 L D' R2 D2 L` |
| L-22 | **13** | 40 | BFLMRS | BFLMRS | `S R U2 R2 U L' U R2 U' L U' R S'` | `S R' U L' U R2 U' L U' R2 U2 R' S'` |
| L-23 | **11** | 28 | BFLMRS | BFLR | `R U R2 D' R U R' D R2 U2 R' U'` | `(U) R U2 R2 D' R U' R' D R2 U' R'` |
| L-24 | **13** | 8 | BFLR | BFLR | `R U' R' U' L F' M2 U F2 U' M2 F' L'` | `L F M2 U F2 U' M2 F L' U R U R'` |
| L-25 | **9** | 4 | BFLR | BFLR | `R2 D' L F2 L' D R U2 R U2` | `(U2) R' U2 R' D' L F2 L' D R2` |
| L-26 | **11** | 28 | BFLMRS | BFLR | `R2 D' R U' R' D R2 U' R' U2 R U'` | `(U) R' U2 R U R2 D' R U R' D R2` |
| L-27 | **13** | 52 | BFLMRS | BFLMRS | `R2 U2 L U' R U R' U M F R' F R2 U'` | `(U) R2 F' R F' M' U' R U' R' U L' U2 R2` |
| L-28 | **12** | 28 | BFLMRS | BFLMRS | `M F' U2 R U2 L' U2 L U2 R' F M' U` | `(U') M F' R U2 L' U2 L U2 R' U2 F M'` |
| L-29 | **13** | 40 | BFLMRS | BFLMRS | `S R' U2 R2 U' L U' R2 U L' U R' S' U2` | `(U2) S R U' L U' R2 U L' U R2 U2 R S'` |
| L-30 | **12** | 8 | BFLR | BFLR | `R2 F' R' L2 F2 L' D2 M D' R U2 R U2` | `(U2) R' U2 R' D M' D2 L F2 L2 R F R2` |
| L-31 | **12** | 4 | BFLR | BFLR | `R2 S2 D' L F2 L' D R U2 R' S2 R2 U2` | `(U2) R2 S2 R U2 R' D' L F2 L' D S2 R2` |
| L-32 | **13** | 36 | BFLMRS | BFLMRS | `R U2 F U F' R' U F U2 F' R U2 R' U` | `(U') R U2 R' F U2 F' U' R F U' F' U2 R'` |
| L-33 | **11** | 4 | BFLR | BFLR | `R D2 L2 D' R D2 L F2 L D R2 U` | `(U') R2 D' L' F2 L' D2 R' D L2 D2 R'` |
| L-34 | **9** | 4 | BFLR | BFLR | `R2 D' R U2 R' D R U2 R U2` | `(U2) R' U2 R' D' R U2 R' D R2` |
| L-35 | **12** | 4 | BFLR | BFLR | `L' U L F L' U' M' F' U2 F U R U2` | `(U2) R' U' F' U2 F M U L F' L' U' L` |
| L-36 | **13** | 8 | BFLR | BFLR | `L' U L U R' F M2 U' F2 U M2 F R` | `R' F' M2 U' F2 U M2 F' R U' L' U' L` |
| L-37 | **8** | 8 | BFLR | BFLR | `L F R' F' L' F R F' U'` | `(U) F R' F' L F R F' L'` |
| L-38 | **12** | 12 | BFLR | BFLRU | `U L F2 L' F M' U L' U' R U2 B' U` | `(U') B U2 R' U L U' M F' L F2 L' U'` |
| L-39 | **10** | 12 | BFLR | BFLR | `R' U F U2 F' U' R F U' F' U` | `(U') F U F' R' U F U2 F' U' R` |
| L-40 | **11** | 4 | BFLR | BFLR | `F R' E2 F D2 L D2 F' E2 F' R U` | `(U') R' F E2 F D2 L' D2 F' E2 R F'` |
| L-41 | **12** | 4 | BFLR | U | `U2 F' L' U2 M' E2 L U L' D2 L F2 U'` | `(U) F2 L' D2 L U' L' E2 M U2 L F U2` |
| L-42 | **12** | 44 | BFLMRS | BFLMRS | `M' F' U2 M U M' F M U R U R' U2` | `(U2) R U' R' U' M' F' M U' M' U2 F M` |
| L-43 | **9** | 8 | MS | BFLR | `R U' R' U2 M' F' U2 F M` | `M' F' U2 F M U2 R U R'` |
| L-44 | **11** | 8 | BFLR | BFLR | `R U2 M' U L' U' F R B2 R' F' U` | `(U') F R B2 R' F' U L U' M U2 R'` |
| L-45 | **13** | 40 | BFLMRS | BFLMRS | `R' U R F2 R F2 U' F2 U R2 U' R F2 U'` | `(U) F2 R' U R2 U' F2 U F2 R' F2 R' U' R` |
| L-46 | **11** | 8 | BFLMRS | BFLR | `R2 L' F2 M' F L' U2 F' U2 F M'` | `M F' U2 F U2 L F' M F2 L R2` |
| L-47 | **11** | 20 | BFLMRS | EMS | `M' F' L' B' L F M U' R' U2 R U` | `(U') R' U2 R U M' F' L' B L F M` |
| L-48 | **11** | 12 | BFLR | BFLR | `R U S' R' U' R' F R2 S R' F' U'` | `(U) F R S' R2 F' R U R S U' R'` |
| L-49 | **8** | 8 | BFLR | BFLR | `R' F' L F R F' L' F U` | `(U') F' L F R' F' L' F R` |
| L-50 | **11** | 20 | BFLMRS | EMS | `M' F R B R' F' M U L U2 L' U'` | `(U) L U2 L' U' M' F R B' R' F' M` |
| L-51 | **11** | 8 | BFLR | BFLR | `L' U2 M' U' R U F' L' B2 L F U'` | `(U) F' L' B2 L F U' R' U M U2 L` |
| L-52 | **11** | 4 | BFLR | BFLR | `R' F E2 R' D2 B' D2 R E2 R F'` | `F R' E2 R' D2 B D2 R E2 F' R` |
| L-53 | **11** | 8 | BFLMRS | BFLR | `R L2 F2 M' F' R U2 F U2 F' M'` | `M F U2 F' U2 R' F M F2 L2 R'` |
| L-54 | **9** | 8 | MS | BFLR | `L' F R U2 F U2 F' U2 M' U2` | `(U2) M U2 F U2 F' U2 R' F' L` |
| L-55 | **12** | 44 | BFLMRS | BFLMRS | `M F U' F2 U R' U' F2 U R F' M' U'` | `(U) M F R' U' F2 U R U' F2 U F' M'` |
| L-56 | **10** | 12 | BFLR | BFLR | `F U R U2 R' U R U R' F'` | `F R U' R' U' R U2 R' U' F'` |
| L-57 | **13** | 40 | BFLMRS | BFLMRS | `F U' F' R2 F' R2 U R2 U' F2 U F' R2 U2` | `(U2) R2 F U' F2 U R2 U' R2 F R2 F U F'` |
| L-58 | **12** | 4 | BFLR | U | `U2 F R U2 M' E2 R' U' R D2 R' F2 U` | `(U') F2 R D2 R' U R E2 M U2 R' F' U2` |
| L-59 | **12** | 12 | BFLR | BFLRU | `U' R' F2 R F' M' U' R U L' U2 B U'` | `(U) B' U2 L U' R' U M F R' F2 R U` |
| L-60 | **11** | 12 | BFLR | BFLR | `F' U' M' F U F' M F2 R' F' R U2` | `(U2) R' F R F2 M' F U' F' M U F` |
| L-61 | **11** | 48 | BFLR | BFLR | `F R U R' F R' F' R2 U' R' F'` | `F R U R2 F R F' R U' R' F'` |
| L-62 | **9** | 8 | BFLR | BFLR | `R' F2 L2 D' L' D L' F2 R U2` | `(U2) R' F2 L D' L D L2 F2 R` |
| L-63 | **11** | 24 | BFLMRS | BFLMRS | `R' U' L2 F2 R' D' R F2 L' U M` | `M' U' L F2 R' D R F2 L2 U R` |
| L-64 | **12** | 16 | BFLR | BFLR | `R' D' L' U2 L' D R' D' L2 F2 D R2` | `R2 D' F2 L2 D R D' L U2 L D R` |
| L-65 | **12** | 16 | BFLR | BFLR | `R' D2 L D R L U L U' M' F R U'` | `(U) R' F' M U L' U' L' R' D' L' D2 R` |
| L-66 | **13** | 72 | BFLMRS | BFLMRS | `R' F' R F' M' U' L' U2 R U L U2 L' U2` | `(U2) L U2 L' U' R' U2 L U M F R' F R` |
| L-67 | **13** | 72 | BFLMRS | BFLMRS | `L F L' F M' U R U2 L' U' R' U2 R U` | `(U') R' U2 R U L U2 R' U' M F' L F' L'` |
| L-68 | **11** | 24 | BFLMRS | BFLMRS | `L U R2 F2 L D L' F2 R U' M U'` | `(U) M' U R' F2 L D' L' F2 R2 U' L'` |
| L-69 | **10** | 8 | BFLR | BFLR | `R' F2 R2 U' L' U R2 L F2 R U` | `(U') R' F2 L' R2 U' L U R2 F2 R` |
| L-70 | **12** | 16 | BFLR | BFLR | `L D2 R' D' R' L' U' R' U M' F' L' U` | `(U') L F M U' R U L R D R D2 L'` |
| L-71 | **9** | 8 | BFLR | BFLR | `L F2 R' D R' D' R2 F2 L' U2` | `(U2) L F2 R2 D R D' R F2 L'` |
| L-72 | **10** | 8 | BFLR | BFLR | `R' F2 R2 L' U' L U R2 F2 R` | `R' F2 R2 U' L' U L R2 F2 R` |
| H-1 | **13** | 448 | BFLMRS | BFLMRS | `M2 U' M2 U' F2 U2 F M2 F' M2 F' U2 F2` | `F2 U2 F M2 F M2 F' U2 F2 U M2 U M2` |
| H-2 | **11** | 4 | BFLR | BFLR | `R' U' R U' R' U R U' R' U2 R U` | `(U') R' U2 R U R' U' R U R' U R` |
| H-3 | **11** | 4 | BFLR | BFLR | `R U R' U R U' R' U R U2 R' U'` | `(U) R U2 R' U' R U R' U' R U' R'` |
| H-4 | **11** | 288 | BFLMRS | BFLMRS | `F M2 F U2 F' U2 F M F2 M F'` | `F M' F2 M' F' U2 F U2 F' M2 F'` |
| H-5 | **11** | 4 | BFLR | BFLR | `R U2 R' U' R U R' U' R U' R' U'` | `(U) R U R' U R U' R' U R U2 R'` |
| H-6 | **11** | 4 | BFLR | BFLR | `R' U2 R U R' U' R U R' U R U` | `(U') R' U' R U' R' U R U' R' U2 R` |
| H-7 | **11** | 16 | BFLR | BFLR | `R' E F E' R U R2 D' L' D R2 U'` | `(U) R2 D' L D R2 U' R' E F' E' R` |
| H-8 | **11** | 288 | BFLMRS | BFLMRS | `F M' F2 M' F' U2 F U2 F' M2 F' U'` | `(U) F M2 F U2 F' U2 F M F2 M F'` |
| H-9 | **12** | 8 | BFLR | BFLR | `F U R' U F2 U' F2 U' R F2 U2 F U2` | `(U2) F' U2 F2 R' U F2 U F2 U' R U' F'` |
| H-10 | **13** | 24 | BFLMRS | BFLMRS | `R' U2 R U R' F U F' R U F U2 F' U'` | `(U) F U2 F' U' R' F U' F' R U' R' U2 R` |
| H-11 | **13** | 144 | BFLMRS | BDFLMRSU | `M F R' F' M' U2 R U R' U R2 U2 R'` | `R U2 R2 U' R U' R' U2 M F R F' M'` |
| H-12 | **13** | 24 | BFLMRS | BFLMRS | `F U2 F' U' F R' U' R F' U' R' U2 R U2` | `(U2) R' U2 R U F R' U R F' U F U2 F'` |
| H-13 | **12** | 4 | BFLR | BFLR | `R2 F2 L F L' F R2 U2 B' R B R' U'` | `(U) R B' R' B U2 R2 F' L F' L' F2 R2` |
| H-14 | **10** | 8 | BFLR | BFLR | `L' U2 F U2 F' U2 F' M' U2 R U'` | `(U) R' U2 M F U2 F U2 F' U2 L` |
| H-15 | **12** | 4 | BFLR | BFLR | `F' R' U' F2 U2 F2 U' F2 U' F2 R F U'` | `(U) F' R' F2 U F2 U F2 U2 F2 U R F` |
| H-16 | **13** | 8 | BFLR | BFLR | `F2 D R D' F2 L' D' U2 R' U R E' B` | `B' E R' U' R U2 D L F2 D R' D' F2` |
| H-17 | **10** | 8 | BFLR | BFLR | `R U2 F' U2 F U2 F M' U2 L' U` | `(U') L U2 M F' U2 F' U2 F U2 R'` |
| H-18 | **12** | 4 | BFLR | BFLR | `F2 R2 B' R' B R' F2 U2 L F' L' F U2` | `(U2) F' L F L' U2 F2 R B' R B R2 F2` |
| H-19 | **12** | 8 | BFLR | BFLR | `R' U' F U' R2 U R2 U F' R2 U2 R' U'` | `(U) R U2 R2 F U' R2 U' R2 U F' U R` |
| H-20 | **12** | 4 | BFLR | BFLR | `R F U R2 U2 R2 U R2 U R2 F' R' U2` | `(U2) R F R2 U' R2 U' R2 U2 R2 U' F' R'` |
| H-21 | **13** | 8 | BFLR | BFLR | `R2 U' R2 F U R2 U' R2 F' U' R2 U R2 U2` | `(U2) R2 U' R2 U F R2 U R2 U' F' R2 U R2` |
| H-22 | **13** | 8 | BFLR | BFLR | `F2 U F2 R' U' F2 U F2 R U F2 U' F2 U'` | `(U) F2 U F2 U' R' F2 U' F2 U R F2 U' F2` |
| H-23 | **13** | 8 | BFLR | BFLR | `R U2 R2 U' R2 U' R D' L F2 L' D R2 U` | `(U') R2 D' L F2 L' D R' U R2 U R2 U2 R'` |
| H-24 | **12** | 4 | MS | MS | `M' F R' F L D2 F2 R F' L' F M U` | `(U') M' F' L F R' F2 D2 L' F' R F' M` |
| H-25 | **13** | 24 | BFLR | BFLR | `R' U' R F2 R D' F2 D F2 R2 U R F2` | `F2 R' U' R2 F2 D' F2 D R' F2 R' U R` |
| H-26 | **11** | 4 | BFLR | BFLR | `L' U R U' L U' R' U' R U' R' U'` | `(U) R U R' U R U L' U R' U' L` |
| H-27 | **12** | 16 | BFLMRS | BFLMRS | `R U' R' U2 R L U' R2 U L' U' R` | `R' U L U' R2 U L' R' U2 R U R'` |
| H-28 | **13** | 24 | BFLR | BFLR | `F' R2 F L F' R2 L' U2 R' F' R U2 F` | `F' U2 R' F R U2 L R2 F L' F' R2 F` |
| H-29 | **12** | 16 | BFLMRS | BFLMRS | `R' U R U2 R' L' U R2 U' L U R' U2` | `(U2) R U' L' U R2 U' L R U2 R' U' R` |
| H-30 | **13** | 24 | BFLR | BFLR | `F U F' R2 F' D R2 D' R2 F2 U' F' R2 U` | `(U') R2 F U F2 R2 D R2 D' F R2 F U' F'` |
| H-31 | **12** | 4 | MS | MS | `M' F' L F' R' D2 F2 L' F R F' M U'` | `(U) M' F R' F' L F2 D2 R F L' F M` |
| H-32 | **11** | 4 | BFLR | BFLR | `L U' R' U L' U R U R' U R U'` | `(U) R' U' R U' R' U' L U' R U L'` |
| H-33 | **13** | 72 | BDFLMRS | BDFLMRSU | `R U R' U F' U F U' F2 L F L' F U'` | `(U) F' L F' L' F2 U F' U' F U' R U' R'` |
| H-34 | **13** | 96 | BDFLMRS | BDFLMRSU | `U2 R F' U2 S' L2 U2 B U M U2 R' L' U2` | `(U2) L R U2 M' U' B' U2 L2 S U2 F R' U2` |
| H-35 | **13** | 4 | BFLR | BFLR | `F' M F' R U' R2 F2 R2 U R' F M' F U'` | `(U) F' M F' R U' R2 F2 R2 U R' F M' F` |
| H-36 | **11** | 64 | BFLR | BFLR | `R' F2 R2 U2 R' F2 R U2 R2 F2 R U'` | `(U) R' F2 R2 U2 R' F2 R U2 R2 F2 R` |
| H-37 | **13** | 72 | BDFLMRS | BDFLMRSU | `L' U' L U' F U' F' U F2 R' F' R F'` | `F R' F R F2 U' F U F' U L' U L` |
| H-38 | **13** | 4 | BFLR | BFLR | `R S R F' U F2 R2 F2 U' F R' S' R'` | `R S R F' U F2 R2 F2 U' F R' S' R'` |
| H-39 | **13** | 96 | BDFLMRS | BDFLMRSU | `U2 L' F U2 S R2 U2 B' U R L U2 M U` | `(U') M' U2 L' R' U' B U2 R2 S' U2 F' L U2` |
| H-40 | **11** | 48 | BFLR | BFLR | `R S2 L' F2 U2 R' F2 R2 D2 R' F2` | `F2 R D2 R2 F2 R U2 F2 L S2 R'` |
| Pi-1 | **13** | 56 | BFLMRS | BFLMRS | `M' F' U2 F2 U M' F2 M U F2 U2 F' M U2` | `(U2) M' F U2 F2 U' M' F2 M U' F2 U2 F M` |
| Pi-2 | **12** | 20 | MS | BDFLR | `R U2 R2 U' R2 U R2 U2 R' S' U2 S` | `S' U2 S R U2 R2 U' R2 U R2 U2 R'` |
| Pi-3 | **12** | 20 | MS | BDFLR | `R' U2 R2 U R2 U' R2 U2 R S' U2 S` | `S' U2 S R' U2 R2 U R2 U' R2 U2 R` |
| Pi-4 | **11** | 4 | BFLR | BFLR | `L' F' U' F U M' F' U' F U R U` | `(U') R' U' F' U F M U' F' U F L` |
| Pi-5 | **9** | 4 | BFLR | BFLR | `R' U2 R2 U R2 U R2 U2 R' U'` | `(U) R U2 R2 U' R2 U' R2 U2 R` |
| Pi-6 | **12** | 20 | BFLR | MS | `S' U2 S R' U2 R2 U R2 U' R2 U2 R` | `R' U2 R2 U R2 U' R2 U2 R S' U2 S` |
| Pi-7 | **11** | 16 | BFLMRS | BFLMRS | `F U R U' R' F2 L' U' L U F U'` | `(U) F' U' L' U L F2 R U R' U' F'` |
| Pi-8 | **11** | 16 | BFLMRS | BFLMRS | `F' U' L' U L F2 R U R' U' F' U'` | `(U) F U R U' R' F2 L' U' L U F` |
| Pi-9 | **11** | 32 | BFLR | BFLR | `R' U2 R2 U S R2 S' U R2 U2 R' U'` | `(U) R U2 R2 U' S R2 S' U' R2 U2 R` |
| Pi-10 | **12** | 20 | BFLR | MS | `M U R2 U2 L U R2 U2 R F2 R2 F2 U` | `(U') F2 R2 F2 R' U2 R2 U' L' U2 R2 U' M'` |
| Pi-11 | **9** | 4 | BFLR | BFLR | `R U2 R2 U' R2 U' R2 U2 R U` | `(U') R' U2 R2 U R2 U R2 U2 R'` |
| Pi-12 | **11** | 4 | BFLR | BFLR | `R F U F' U' M' F U F' U' L'` | `L U F U' F' M U F U' F' R'` |
| Pi-13 | **13** | 16 | BFLMRS | BFLR | `R' U' R' F2 R2 U R' F2 R U' R2 F2 R2 U` | `(U') R2 F2 R2 U R' F2 R U' R2 F2 R U R` |
| Pi-14 | **12** | 4 | BFLR | MS | `M' U' R F2 U' R' U2 R U F2 U L'` | `L U' F2 U' R' U2 R U F2 R' U M` |
| Pi-15 | **13** | 24 | BFLMRS | BFLMRS | `L U R2 F2 U' L D L' U F2 R U' M U'` | `(U) M' U R' F2 U' L D' L' U F2 R2 U' L'` |
| Pi-16 | **12** | 4 | MS | BFLR | `L' F U2 F R F2 R' F' U2 R F' M' U'` | `(U) M F R' U2 F R F2 R' F' U2 F' L` |
| Pi-17 | **13** | 8 | BFLR | BFLR | `R2 D R2 D' L F2 D' F2 D L' F2 U F2` | `F2 U' F2 L D' F2 D F2 L' D R2 D' R2` |
| Pi-18 | **12** | 16 | BFLR | BFLR | `L' U' L U' F2 R' F2 R U2 R U2 R'` | `R U2 R' U2 R' F2 R F2 U L' U L` |
| Pi-19 | **12** | 4 | BFLR | BFLR | `R' F2 L2 D' L2 U' L D L' U F2 R U2` | `(U2) R' F2 U' L D' L' U L2 D L2 F2 R` |
| Pi-20 | **13** | 8 | BFLR | BFLR | `R B2 R' U R2 F2 L' D' L' D2 L2 F2 R2 U2` | `(U2) R2 F2 L2 D2 L D L F2 R2 U' R B2 R'` |
| Pi-21 | **12** | 4 | BFLR | BFLR | `L F2 U R' D R U' R2 D' R2 F2 L' U'` | `(U) L F2 R2 D R2 U R' D' R U' F2 L'` |
| Pi-22 | **12** | 32 | BFLR | BFLR | `F R' F2 L F' L' F' R F' R U2 R'` | `R U2 R' F R' F L F L' F2 R F'` |
| Pi-23 | **13** | 16 | BFLR | BFLMRS | `M U R' U' L U2 R U' R' U R U2 R' U2` | `(U2) R U2 R' U' R U R' U2 L' U R U' M'` |
| Pi-24 | **12** | 16 | BFLR | BFLR | `R' F2 R U2 R U2 R' F2 U' R U' R' U'` | `(U) R U R' U F2 R U2 R' U2 R' F2 R` |
| Pi-25 | **12** | 8 | BFLR | BFLR | `R U2 R2 F U' R2 U' R2 U F' U R` | `R' U' F U' R2 U R2 U F' R2 U2 R'` |
| Pi-26 | **12** | 8 | BFLR | BFLR | `F' U2 F2 R' U F2 U F2 U' R U' F' U` | `(U') F U R' U F2 U' F2 U' R F2 U2 F` |
| Pi-27 | **13** | 8 | BFLR | BFLR | `B' E R' U' R D U2 L F2 D R' D' F2 U` | `(U') F2 D R D' F2 L' U2 D' R' U R E' B` |
| Pi-28 | **13** | 24 | BFLMRS | BFLMRS | `M' U' R U' R' U R U2 L' U R' U2 R U'` | `(U) R' U2 R U' L U2 R' U' R U R' U M` |
| Pi-29 | **12** | 4 | BFLR | BFLR | `F' L F L' U2 F2 R B' R B R2 F2 U'` | `(U) F2 R2 B' R' B R' F2 U2 L F' L' F` |
| Pi-30 | **10** | 8 | BFLR | BFLR | `R F2 M' U' F2 U' F2 U F2 L' U` | `(U') L F2 U' F2 U F2 U M F2 R'` |
| Pi-31 | **12** | 4 | BFLR | BFLR | `R F R2 U' R2 U' R2 U2 R2 U' F' R'` | `R F U R2 U2 R2 U R2 U R2 F' R'` |
| Pi-32 | **13** | 144 | BFLMRS | BDFLMRSU | `R U2 R2 U' R U' R' U2 M F R F' M' U'` | `(U) M F R' F' M' U2 R U R' U R2 U2 R'` |
| Pi-33 | **12** | 4 | BFLR | BFLR | `F' R' F2 U F2 U F2 U2 F2 U R F U` | `(U') F' R' U' F2 U2 F2 U' F2 U' F2 R F` |
| Pi-34 | **12** | 4 | BFLR | BFLR | `R B' R' B U2 R2 F' L F' L' F2 R2 U2` | `(U2) R2 F2 L F L' F R2 U2 B' R B R'` |
| Pi-35 | **13** | 24 | BFLMRS | BFLMRS | `M U R' U R U' R' U2 L U' R U2 R' U'` | `(U) R U2 R' U L' U2 R U R' U' R U' M'` |
| Pi-36 | **10** | 8 | BFLR | BFLR | `L' F2 M' U F2 U F2 U' F2 R U'` | `(U) R' F2 U F2 U' F2 U' M F2 L` |
| Pi-37 | **13** | 24 | BFLR | BFLR | `F2 R' U' R2 F2 D' F2 D R' F2 R' U R U` | `(U') R' U' R F2 R D' F2 D F2 R2 U R F2` |
| Pi-38 | **13** | 8 | BFLR | BFLR | `F2 U F2 U' R' F2 U' F2 U R F2 U' F2 U` | `(U') F2 U F2 R' U' F2 U F2 R U F2 U' F2` |
| Pi-39 | **11** | 4 | BFLR | BFLR | `R' U' R U' R' U' L U' R U L' U'` | `(U) L U' R' U L' U R U R' U R` |
| Pi-40 | **13** | 8 | BFLR | BFLR | `R2 D' L F2 L' D R' U R2 U R2 U2 R'` | `R U2 R2 U' R2 U' R D' L F2 L' D R2` |
| Pi-41 | **12** | 4 | MS | MS | `M' F' L F R' F2 D2 L' F' R F' M` | `M' F R' F L D2 F2 R F' L' F M` |
| Pi-42 | **12** | 4 | MS | MS | `M' F R' F' L F2 D2 R F L' F M` | `M' F' L F' R' D2 F2 L' F R F' M` |
| Pi-43 | **11** | 4 | BFLR | BFLR | `R U R' U R U L' U R' U' L U'` | `(U) L' U R U' L U' R' U' R U' R'` |
| Pi-44 | **13** | 8 | BFLR | BFLR | `R2 U' R2 U F R2 U R2 U' F' R2 U R2` | `R2 U' R2 F U R2 U' R2 F' U' R2 U R2` |
| Pi-45 | **13** | 24 | BFLR | BFLR | `R2 F U F2 R2 D R2 D' F R2 F U' F'` | `F U F' R2 F' D R2 D' R2 F2 U' F' R2` |
| Pi-46 | **12** | 16 | BFLMRS | BFLMRS | `R U' L' U R2 U' R L U2 R' U' R U'` | `(U) R' U R U2 L' R' U R2 U' L U R'` |
| Pi-47 | **12** | 16 | BFLMRS | BFLMRS | `R' U L U' R2 U R' L' U2 R U R' U'` | `(U) R U' R' U2 L R U' R2 U L' U' R` |
| Pi-48 | **13** | 24 | BFLR | BFLR | `F' U2 R' F R U2 R2 L F L' F' R2 F` | `F' R2 F L F' L' R2 U2 R' F' R U2 F` |
| Pi-49 | **12** | 32 | BFLR | BFLR | `R U2 R' F R' F L F L' F2 R F' U` | `(U') F R' F2 L F' L' F' R F' R U2 R'` |
| Pi-50 | **12** | 16 | BFLR | BFLR | `R U2 R' U2 R' F2 R F2 U L' U L U` | `(U') L' U' L U' F2 R' F2 R U2 R U2 R'` |
| Pi-51 | **13** | 16 | BFLR | BFLMRS | `M' U' R U L' U2 R' U R U' R' U2 R` | `R' U2 R U R' U' R U2 L U' R' U M` |
| Pi-52 | **13** | 8 | BFLR | BFLR | `R2 F2 L2 D2 L D L F2 R2 U' R B2 R' U'` | `(U) R B2 R' U R2 F2 L' D' L' D2 L2 F2 R2` |
| Pi-53 | **12** | 4 | BFLR | BFLR | `L F2 R2 D R2 U R' D' R U' F2 L' U2` | `(U2) L F2 U R' D R U' R2 D' R2 F2 L'` |
| Pi-54 | **12** | 4 | BFLR | BFLR | `R' F2 U' L D' L' U L2 D L2 F2 R U` | `(U') R' F2 L2 D' L2 U' L D L' U F2 R` |
| Pi-55 | **12** | 4 | MS | BFLR | `R F' U2 F' L' F2 L F U2 L' F M' U` | `(U') M F' L U2 F' L' F2 L F U2 F R'` |
| Pi-56 | **12** | 16 | BFLR | BFLR | `R U R' U F2 R U2 R' U2 R' F2 R` | `R' F2 R U2 R U2 R' F2 U' R U' R'` |
| Pi-57 | **13** | 8 | BFLR | BFLR | `L2 D' L2 D R' F2 D F2 D' R F2 U' F2` | `F2 U F2 R' D F2 D' F2 R D' L2 D L2` |
| Pi-58 | **12** | 4 | BFLR | MS | `M' U L' F2 U L U2 L' U' F2 U' R` | `R' U F2 U L U2 L' U' F2 L U' M` |
| Pi-59 | **13** | 16 | BFLMRS | BFLR | `F U F R2 F2 U' F R2 F' U F2 R2 F2` | `F2 R2 F2 U' F R2 F' U F2 R2 F' U' F'` |
| Pi-60 | **13** | 24 | BFLMRS | BFLMRS | `R' U' L2 F2 U R' D' R U' F2 L' U M U` | `(U') M' U' L F2 U R' D R U' F2 L2 U R` |
| Pi-61 | **13** | 24 | BFLR | BFLR | `F R2 U' R U2 R U R' U R' U R2 F' U2` | `(U2) F R2 U' R U' R U' R' U2 R' U R2 F'` |
| Pi-62 | **12** | 8 | BFLR | BFLR | `R F' U' R2 U' F U F' R2 U F R' U2` | `(U2) R F' U' R2 F U' F' U R2 U F R'` |
| Pi-63 | **13** | 56 | BFLMRS | BFLMRS | `M U' L U' L' U' R' U' L U' R U' R' U` | `(U') R U R' U L' U R U L U L' U M'` |
| Pi-64 | **12** | 8 | BFLMRS | BFLMRS | `M' F U F' M U F U' R U' R' F' U'` | `(U) F R U R' U F' U' M' F U' F' M` |
| Pi-65 | **13** | 48 | BFLR | BFLR | `F R' F' L F2 R F L' F U2 F U2 F' U2` | `(U2) F U2 F' U2 F' L F' R' F2 L' F R F'` |
| Pi-66 | **12** | 8 | BFLMRS | BFLMRS | `F R U R' U F' U' M' F U' F' M U2` | `(U2) M' F U F' M U F U' R U' R' F'` |
| Pi-67 | **13** | 24 | BFLR | BFLR | `F R2 U' R U' R U' R' U2 R' U R2 F' U` | `(U') F R2 U' R U2 R U R' U R' U R2 F'` |
| Pi-68 | **13** | 56 | BFLMRS | BFLMRS | `M' U L' U L U R U L' U R' U R U` | `(U') R' U' R U' L U' R' U' L' U' L U' M` |
| Pi-69 | **12** | 8 | BFLR | BFLR | `F' R U F2 U R' U' R F2 U' R' F` | `F' R U F2 R' U R U' F2 U' R' F` |
| Pi-70 | **13** | 160 | BFLMRS | BDFLMRSU | `L' U' L F R' U2 R2 U R2 U R U' F' U'` | `(U) F U R' U' R2 U' R2 U2 R F' L' U L` |
| Pi-71 | **12** | 8 | BFLR | BFLR | `F' R U F2 R' U R U' F2 U' R' F U` | `(U') F' R U F2 U R' U' R F2 U' R' F` |
| Pi-72 | **12** | 8 | BFLR | BFLR | `R F' U' R2 F U' F' U R2 U F R' U` | `(U') R F' U' R2 U' F U F' R2 U F R'` |
| S-1 | **12** | 8 | BFLR | BFLR | `R' U L D' U' F2 D R2 U2 L' U R' U` | `(U') R U' L U2 R2 D' F2 U D L' U' R` |
| S-2 | **7** | 4 | BFLR | BFLR | `R' U' R U' R' U2 R` | `R' U2 R U R' U R` |
| S-3 | **11** | 24 | BFLMRS | BFLMRS | `R U2 R2 U2 R2 U R2 U R2 U' R' U'` | `(U) R U R2 U' R2 U' R2 U2 R2 U2 R'` |
| S-4 | **11** | 16 | BFLMRS | BFLMRS | `R' U2 R2 U2 R U' S2 R U' R' S2 U` | `(U') S2 R U R' S2 U R' U2 R2 U2 R` |
| S-5 | **9** | 4 | MS | BFLR | `B' L F' L' F2 R U2 R' S U2` | `(U2) S' R U2 R' F2 L F L' B` |
| S-6 | **9** | 4 | BFLR | MS | `M' D' F2 D R2 U' R' U L' U` | `(U') L U' R U R2 D' F2 D M` |
| S-7 | **11** | 4 | BFLR | BFLR | `R2 S2 R' U2 R' U' R U' R S2 R2` | `R2 S2 R' U R' U R U2 R S2 R2` |
| S-8 | **7** | 4 | BFLR | BFLR | `R U2 R' U' R U' R' U2` | `(U2) R U R' U R U2 R'` |
| S-9 | **11** | 40 | BFLMRS | BFLMRS | `R U R' U R' U' R2 U' R2 U2 R U2` | `(U2) R' U2 R2 U R2 U R U' R U' R'` |
| S-10 | **11** | 4 | BFLR | BFLR | `R2 S2 R U' R U' R' U2 R' S2 R2 U2` | `(U2) R2 S2 R U2 R U R' U R' S2 R2` |
| S-11 | **11** | 24 | BFLMRS | BFLMRS | `R' U' R2 U R2 U R2 U2 R2 U2 R U2` | `(U2) R' U2 R2 U2 R2 U' R2 U' R2 U R` |
| S-12 | **12** | 28 | BFLMRS | BFLMRS | `F R U R' U' F' R' U' F' U F R` | `R' F' U' F U R F U R U' R' F'` |
| S-13 | **10** | 8 | BFLR | MS | `M' F2 U F2 U' F2 U' R U L'` | `L U' R' U F2 U F2 U' F2 M` |
| S-14 | **13** | 32 | BFLR | BFLMRS | `R' U2 R' D' R U R' D R2 U' R' U2 R U2` | `(U2) R' U2 R U R2 D' R U' R' D R U2 R` |
| S-15 | **11** | 4 | BFLR | BFLR | `R' U' R U' R2 D' L F2 L' D R2 U` | `(U') R2 D' L F2 L' D R2 U R' U R` |
| S-16 | **9** | 4 | MS | BFLR | `R F2 R' F2 L' F R F' M' U'` | `(U) M F R' F' L F2 R F2 R'` |
| S-17 | **13** | 84 | BFLMRS | BFLMRS | `M U' L U R' U' L' U2 R U' L U R' U2` | `(U2) R U' L' U R' U2 L U R U' L' U M'` |
| S-18 | **12** | 12 | BFLMRS | BFLR | `F' L D F2 D' L2 U' L F M' U' M U'` | `(U) M' U M F' L' U L2 D F2 D' L' F` |
| S-19 | **11** | 4 | BFLR | BFLR | `R' F U F2 U F2 U2 F2 U F R U'` | `(U) R' F' U' F2 U2 F2 U' F2 U' F' R` |
| S-20 | **13** | 28 | BFLMRS | BDFLRU | `R2 F R F2 R2 F M' U' R2 U L' U R` | `R' U' L U' R2 U M F' R2 F2 R' F' R2` |
| S-21 | **12** | 12 | BFLMRS | BDR | `R L U2 R2 D' F2 D R2 U L' U R' U2` | `(U2) R U' L U' R2 D' F2 D R2 U2 L' R'` |
| S-22 | **13** | 40 | BFLMRS | BFLMRS | `L' U F U' M U' F' U2 F' U' M2 U2 R U` | `(U') R' U2 M2 U F U2 F U M' U F' U' L` |
| S-23 | **12** | 24 | BFLMRS | BFLR | `F2 U F2 L' U R U R' U L U' F2 U2` | `(U2) F2 U L' U' R U' R' U' L F2 U' F2` |
| S-24 | **11** | 4 | BFLR | BFLR | `R' U' R U' R2 D' R U2 R' D R2 U` | `(U') R2 D' R U2 R' D R2 U R' U R` |
| S-25 | **10** | 8 | MS | BFLR | `L' F R F' U2 F' U2 F U2 M' U'` | `(U) M U2 F' U2 F U2 F R' F' L` |
| S-26 | **12** | 24 | BFLR | BFLMRS | `F2 U' R U L' U L U R' F2 U F2` | `F2 U' F2 R U' L' U' L U' R' U F2` |
| S-27 | **13** | 28 | BFLR | BEFLR | `R F L' F R2 F' M' U R2 U2 R U R2 U` | `(U') R2 U' R' U2 R2 U' M F R2 F' L F' R'` |
| S-28 | **9** | 4 | BFLR | MS | `M' U' R U L' U2 R' U2 R U2` | `(U2) R' U2 R U2 L U' R' U M` |
| S-29 | **13** | 84 | BFLMRS | BDFLMRSU | `R' U' R F R' U2 L U' R U L' U2 F' U` | `(U') F U2 L U' R' U L' U2 R F' R' U R` |
| S-30 | **12** | 12 | BFLR | BFLMRS | `M' U' M F R U' R2 D' F2 D R F'` | `F R' D' F2 D R2 U R' F' M' U M` |
| S-31 | **11** | 4 | BFLR | BFLR | `F R U R2 U2 R2 U R2 U R F'` | `F R' U' R2 U' R2 U2 R2 U' R' F'` |
| S-32 | **11** | 4 | BFLR | BFLR | `L2 D R' F2 R D' L2 U' L U' L'` | `L U L' U L2 D R' F2 R D' L2` |
| S-33 | **11** | 4 | BFLR | BFLR | `R2 D R' U2 R D' R2 U' R U' R' U2` | `(U2) R U R' U R2 D R' U2 R D' R2` |
| S-34 | **13** | 40 | BFLMRS | BFLMRS | `L' U R U' L U' R D R' U2 R D' R2` | `R2 D R' U2 R D' R' U L' U R' U' L` |
| S-35 | **13** | 32 | BFLMRS | BDFLRU | `U' L U R' U' F R U2 R' U' F' U2 M U2` | `(U2) M' U2 F U R U2 R' F' U R U' L' U` |
| S-36 | **12** | 12 | BFLR | BFLMRS | `M U' L2 U L2 U R' U L' F2 L2 F2 U'` | `(U) F2 L2 F2 L U' R U' L2 U' L2 U M'` |
| S-37 | **10** | 4 | BFLR | BFLR | `R' F U2 F' R F R' U2 R F' U'` | `(U) F R' U2 R F' R' F U2 F' R` |
| S-38 | **11** | 4 | BFLR | BFLR | `R U2 R F2 D L' B2 L D' F2 R2 U` | `(U') R2 F2 D L' B2 L D' F2 R' U2 R'` |
| S-39 | **12** | 12 | BFLR | BFLR | `R' L' U2 R U R' U2 L2 U' R U L'` | `L U' R' U L2 U2 R U' R' U2 L R` |
| S-40 | **12** | 8 | BFLR | BFLR | `R2 B R' L2 U2 L F' L' U2 M2 F' R U2` | `(U2) R' F M2 U2 L F L' U2 L2 R B' R2` |
| S-41 | **12** | 20 | BFLR | BFLMRS | `M U R' U2 L U' F2 R' F2 R2 U2 R'` | `R U2 R2 F2 R F2 U L' U2 R U' M'` |
| S-42 | **12** | 20 | BFLMRS | BFLR | `R U2 R2 F2 U' R2 U' R2 U F2 U R U` | `(U') R' U' F2 U' R2 U R2 U F2 R2 U2 R'` |
| S-43 | **12** | 4 | BFLR | BFLR | `F' D F2 U R2 U R2 F' U2 F2 D' F2 U'` | `(U) F2 D F2 U2 F R2 U' R2 U' F2 D' F` |
| S-44 | **11** | 4 | BFLR | BFLR | `L2 F2 D' R B2 R' D F2 L U2 L` | `L' U2 L' F2 D' R B2 R' D F2 L2` |
| S-45 | **12** | 28 | BFLR | BFLR | `R' F U' F' U' R F U' R' U' R F' U'` | `(U) F R' U R U F' R' U F U F' R` |
| S-46 | **12** | 4 | BFLR | BFLR | `R2 D' R2 U2 R' F2 U F2 U R2 D R' U` | `(U') R D' R2 U' F2 U' F2 R U2 R2 D R2` |
| S-47 | **12** | 12 | BFLR | BFLR | `L' U R U' L2 U2 R' U R U2 R' L'` | `L R U2 R' U' R U2 L2 U R' U' L` |
| S-48 | **12** | 8 | BFLR | BFLR | `R2 F' R2 U R2 D' F' E' L' U L2 F2 U` | `(U') F2 L2 U' L E F D R2 U' R2 F R2` |
| S-49 | **7** | 4 | BFLR | BFLR | `L' U R U' L U R' U` | `(U') R U' L' U R' U' L` |
| S-50 | **12** | 44 | BFLMRS | BFLRU | `F E R U' R' D U2 R2 U' R U R` | `R' U' R' U R2 U2 D' R U R' E' F'` |
| S-51 | **12** | 44 | BFLR | BDFLMRS | `L U2 R' U2 F' U2 F M U R U R' U` | `(U') R U' R' U' M' F' U2 F U2 R U2 L'` |
| S-52 | **12** | 8 | BFLR | BFLR | `L2 D F' E R' U' R2 F2 U' F2 R' F2 U'` | `(U) F2 R F2 U F2 R2 U R E' F D' L2` |
| S-53 | **12** | 16 | BFLMRS | BFLRU | `R2 L F R F' L F L' U2 M' U M2 U2` | `(U2) M2 U' M U2 L F' L' F R' F' L' R2` |
| S-54 | **12** | 16 | BFLR | BDLMS | `L' R2 U R E' R2 D' B U F U' F2 U2` | `(U2) F2 U F' U' B' D R2 E R' U' R2 L` |
| S-55 | **12** | 20 | BFLMRS | BDFLR | `R' U2 R' F' R U R U' R' F U2 R U2` | `(U2) R' U2 F' R U R' U' R' F R U2 R` |
| S-56 | **12** | 44 | BFLR | BDFLMRS | `M' F' U F U F' U2 F U R U' L' U` | `(U') L U R' U' F' U2 F U' F' U' F M` |
| S-57 | **11** | 4 | BFLR | BFLR | `L2 S2 L U R U' L U R S2 R2 U` | `(U') R2 S2 R' U' L' U R' U' L' S2 L2` |
| S-58 | **12** | 20 | BFLR | BFLMRS | `F U2 F' R' U' R F R' U R U2 F' U2` | `(U2) F U2 R' U' R F' R' U R F U2 F'` |
| S-59 | **12** | 44 | BFLMRS | BFLRU | `R2 U L2 F' M' F2 L' U' M2 F' U' R U'` | `(U) R' U F M2 U L F2 M F L2 U' R2` |
| S-60 | **10** | 8 | BFLR | BFLR | `R2 D' F D' L2 F' D2 R2 U B'` | `B U' R2 D2 F L2 D F' D R2` |
| S-61 | **13** | 24 | BFLR | BFLR | `R2 D R F2 L D' L' F2 D2 F2 D F2 R U2` | `(U2) R' F2 D' F2 D2 F2 L D L' F2 R' D' R2` |
| S-62 | **11** | 8 | BFLR | BFLR | `R' U' R U' L U' R' U L' U2 R U'` | `(U) R' U2 L U' R U L' U R' U R` |
| S-63 | **13** | 16 | BFLR | BFLR | `F2 M' U F U2 F' R U2 R' U' M U F U` | `(U') F' U' M' U R U2 R' F U2 F' U' M F2` |
| S-64 | **13** | 48 | BFLR | BFLR | `F R U' R2 U' R U' R' U2 R2 U R' F' U` | `(U') F R U' R2 U2 R U R' U R2 U R' F'` |
| S-65 | **13** | 16 | BFLR | BFLR | `L' U2 F R' F' R U2 L F U2 F2 U2 F U2` | `(U2) F' U2 F2 U2 F' L' U2 R' F R F' U2 L` |
| S-66 | **14** | 144 | BFLR | BFLRU | `F R U R' U' R U R' F R' F' R U' F' U` | `(U') F U R' F R F' R U' R' U R U' R' F'` |
| S-67 | **13** | 24 | BFLR | BFLR | `R U' R2 D' U' R U' R' D U2 R2 U R' U` | `(U') R U' R2 U2 D' R U R' U D R2 U R'` |
| S-68 | **13** | 16 | BFLR | BFLR | `F U M' U' L' U2 L F' U2 F U M F2 U'` | `(U) F2 M' U' F' U2 F L' U2 L U M U' F'` |
| S-69 | **13** | 32 | BFLMRS | BDFLMRS | `R U2 R2 F2 R F2 R U2 R' U L' U L U` | `(U') L' U' L U' R U2 R' F2 R' F2 R2 U2 R'` |
| S-70 | **12** | 24 | BFLMRS | BFLMRS | `M' U' R' U L' D' U' R U R' D R2 U'` | `(U) R2 D' R U' R' U D L U' R U M` |
| S-71 | **11** | 8 | BFLR | BFLR | `R U2 L' U R' U' L U' R U' R' U'` | `(U) R U R' U L' U R U' L U2 R'` |
| S-72 | **13** | 32 | BFLMRS | BFLMRSU | `R U R' U L' U2 L F2 R' F2 R2 U2 R' U'` | `(U) R U2 R2 F2 R F2 L' U2 L U' R U' R'` |
| AS-1 | **12** | 8 | BFLR | BFLR | `R U' L U2 R2 D' F2 D U L' U' R` | `R' U L U' D' F2 D R2 U2 L' U R'` |
| AS-2 | **11** | 24 | BFLMRS | BFLMRS | `R U R2 U' R2 U' R2 U2 R2 U2 R' U2` | `(U2) R U2 R2 U2 R2 U R2 U R2 U' R'` |
| AS-3 | **7** | 4 | BFLR | BFLR | `R' U2 R U R' U R U2` | `(U2) R' U' R U' R' U2 R` |
| AS-4 | **11** | 16 | BFLMRS | BFLMRS | `R U2 R2 U2 R' U S2 R' U R S2` | `S2 R' U' R S2 U' R U2 R2 U2 R'` |
| AS-5 | **11** | 4 | BFLR | BFLR | `R2 S2 R' U R' U R U2 R S2 R2 U2` | `(U2) R2 S2 R' U2 R' U' R U' R S2 R2` |
| AS-6 | **11** | 4 | BFLR | BFLR | `R2 S2 R U2 R U R' U R' S2 R2` | `R2 S2 R U' R U' R' U2 R' S2 R2` |
| AS-7 | **9** | 4 | BFLR | MS | `M' D F2 D' L2 U L U' R U` | `(U') R' U L' U' L2 D F2 D' M` |
| AS-8 | **11** | 24 | BFLMRS | BFLMRS | `R' U2 R2 U2 R2 U' R2 U' R2 U R U` | `(U') R' U' R2 U R2 U R2 U2 R2 U2 R` |
| AS-9 | **11** | 40 | BFLMRS | BFLMRS | `R' U2 R2 U R2 U R U' R U' R' U2` | `(U2) R U R' U R' U' R2 U' R2 U2 R` |
| AS-10 | **9** | 4 | MS | BFLR | `B R' F R F2 L' U2 L S'` | `S L' U2 L F2 R' F' R B'` |
| AS-11 | **7** | 4 | BFLR | BFLR | `R U R' U R U2 R'` | `R U2 R' U' R U' R'` |
| AS-12 | **12** | 28 | BFLMRS | BFLMRS | `R' F' U' F U R F U R U' R' F' U2` | `(U2) F R U R' U' F' R' U' F' U F R` |
| AS-13 | **10** | 8 | MS | BFLR | `R F' L' F U2 F U2 F' U2 M' U` | `(U') M U2 F U2 F' U2 F' L F R'` |
| AS-14 | **13** | 32 | BFLMRS | BDFLRU | `R' U2 R U R2 D' R U' R' D R U2 R U'` | `(U) R' U2 R' D' R U R' D R2 U' R' U2 R` |
| AS-15 | **11** | 4 | BFLR | BFLR | `R2 D' L F2 L' D R2 U R' U R` | `R' U' R U' R2 D' L F2 L' D R2` |
| AS-16 | **9** | 4 | BFLR | MS | `M U R' U' L U2 R U2 R'` | `R U2 R' U2 L' U R U' M'` |
| AS-17 | **13** | 40 | BFLMRS | BFLMRS | `L U' R' U L' U R' D' R U2 R' D R2 U2` | `(U2) R2 D' R U2 R' D R U' L U' R U L'` |
| AS-18 | **11** | 4 | BFLR | BFLR | `R' F' U' F2 U2 F2 U' F2 U' F' R U` | `(U') R' F U F2 U F2 U2 F2 U F R` |
| AS-19 | **12** | 12 | BFLR | BFLMRS | `M' D F2 D' L2 U' R U' R' U2 R L U'` | `(U) L' R' U2 R U R' U L2 D F2 D' M` |
| AS-20 | **13** | 28 | BFLR | BEFLR | `R S R U R2 F' U' F R S' R U' R2 U'` | `(U) R2 U R' S R' F' U F R2 U' R' S' R'` |
| AS-21 | **11** | 4 | BFLR | BFLR | `R2 D' R U2 R' D R2 U R' U R` | `R' U' R U' R2 D' R U2 R' D R2` |
| AS-22 | **13** | 84 | BFLMRS | BDFLMRSU | `L U' R' U L' U2 R F R' U R U' F' U'` | `(U) F U R' U' R F' R' U2 L U' R U L'` |
| AS-23 | **12** | 24 | BFLR | BFLMRS | `F2 U L' U' R U' R' U' L F2 U' F2` | `F2 U F2 L' U R U R' U L U' F2` |
| AS-24 | **12** | 12 | BFLR | BFLMRS | `M U R2 U' R2 U' L U' R F2 R2 F2 U` | `(U') F2 R2 F2 R' U L' U R2 U R2 U' M'` |
| AS-25 | **10** | 8 | BFLR | MS | `M' F2 U' F2 U F2 U L' U' R` | `R' U L U' F2 U' F2 U F2 M` |
| AS-26 | **12** | 24 | BFLMRS | BFLR | `F2 U' F2 R U' L' U' L U' R' U F2 U2` | `(U2) F2 U' R U L' U L U R' F2 U F2` |
| AS-27 | **13** | 28 | BFLMRS | BDFLRU | `F2 U' F M F R U' R' F2 U F M' F` | `F' M F' U' F2 R U R' F' M' F' U F2` |
| AS-28 | **9** | 4 | MS | BFLR | `L' F2 L F2 R F' L' F M' U` | `(U') M F' L F R' F2 L' F2 L` |
| AS-29 | **13** | 40 | BFLMRS | BFLMRS | `R U' F' U M U F U2 F U M2 U2 L' U'` | `(U) L U2 M2 U' F' U2 F' U' M' U' F U R'` |
| AS-30 | **11** | 4 | BFLR | BFLR | `F R' U' R2 U' R2 U2 R2 U' R' F' U2` | `(U2) F R U R2 U2 R2 U R2 U R F'` |
| AS-31 | **12** | 12 | BFLMRS | BFLR | `F R' D' F2 D R2 U R' F' M' U M U` | `(U') M' U' M F R U' R2 D' F2 D R F'` |
| AS-32 | **11** | 4 | BFLR | BFLR | `L U L' U L2 D R' F2 R D' L2 U'` | `(U) L2 D R' F2 R D' L2 U' L U' L'` |
| AS-33 | **12** | 12 | BFLMRS | BDR | `R' L' U2 L2 D F2 D' L2 U' R U' L U2` | `(U2) L' U R' U L2 D F2 D' L2 U2 L R` |
| AS-34 | **13** | 84 | BFLMRS | BFLMRS | `M' U L' U' R U L U2 R' U L' U' R` | `R' U L U' R U2 L' U' R' U L U' M` |
| AS-35 | **13** | 32 | BFLR | BFLMRS | `R U2 R D R' U' R D' R2 U R U2 R'` | `R U2 R' U' R2 D R' U R D' R' U2 R'` |
| AS-36 | **11** | 4 | BFLR | BFLR | `R U R' U R2 D R' U2 R D' R2 U` | `(U') R2 D R' U2 R D' R2 U' R U' R'` |
| AS-37 | **7** | 4 | BFLR | BFLR | `L U' R' U L' U' R U` | `(U') R' U L U' R U L'` |
| AS-38 | **12** | 44 | BFLMRS | BFLRU | `R' F' R U2 M U' R' U' R2 L D R2 U` | `(U') R2 D' L' R2 U R U M' U2 R' F R` |
| AS-39 | **12** | 44 | BFLR | BDFLMRS | `M' F U' F' U' F U2 F' U' L' U R U'` | `(U) R' U' L U F U2 F' U F U F' M` |
| AS-40 | **12** | 8 | BFLR | BFLR | `F2 R F2 U F2 R2 U R E' F D' L2 U'` | `(U) L2 D F' E R' U' R2 F2 U' F2 R' F2` |
| AS-41 | **12** | 20 | BFLR | BFLMRS | `R' U2 F' R U R' U' R' F R U2 R U'` | `(U) R' U2 R' F' R U R U' R' F U2 R` |
| AS-42 | **12** | 20 | BFLMRS | BDFLR | `F U2 R' U' R F' R' U R F U2 F' U'` | `(U) F U2 F' R' U' R F R' U R U2 F'` |
| AS-43 | **12** | 16 | BFLR | DFMRS | `F2 B U' F' E F2 D L' U' R' U R2 U'` | `(U) R2 U' R U L D' F2 E' F U B' F2` |
| AS-44 | **12** | 44 | BFLR | BDFLMRS | `R' U2 L U2 F U2 F' M U' L' U' L U'` | `(U) L' U L U M' F U2 F' U2 L' U2 R` |
| AS-45 | **11** | 4 | BFLR | BFLR | `L2 S2 L' U' R' U L' U' R' S2 R2 U` | `(U') R2 S2 R U L U' R U L S2 L2` |
| AS-46 | **12** | 16 | BFLMRS | BFLRU | `R' L2 F' L' F R' F' R U2 M' U' M2 U2` | `(U2) M2 U M U2 R' F R F' L F L2 R` |
| AS-47 | **12** | 44 | BFLMRS | BFLRU | `R F L' F' U' F2 U F' U' F' U M'` | `M U' F U F U' F2 U F L F' R'` |
| AS-48 | **10** | 8 | BFLR | BFLR | `B U' R2 D2 F L2 D F' D R2 U` | `(U') R2 D' F D' L2 F' D2 R2 U B'` |
| AS-49 | **10** | 4 | BFLR | BFLR | `F R' U2 R F' R' F U2 F' R U2` | `(U2) R' F U2 F' R F R' U2 R F'` |
| AS-50 | **12** | 12 | BFLR | BFLR | `L U' R' U L2 U2 R U' R' U2 R L U2` | `(U2) L' R' U2 R U R' U2 L2 U' R U L'` |
| AS-51 | **11** | 4 | BFLR | BFLR | `R2 F2 D L' B2 L D' F2 R' U2 R'` | `R U2 R F2 D L' B2 L D' F2 R2` |
| AS-52 | **12** | 8 | BFLR | BFLR | `L2 B' R2 L U2 R' F R U2 M2 F L' U2` | `(U2) L F' M2 U2 R' F' R U2 L' R2 B L2` |
| AS-53 | **12** | 4 | BFLR | BFLR | `F2 D F2 U2 F R2 U' R2 U' F2 D' F` | `F' D F2 U R2 U R2 F' U2 F2 D' F2` |
| AS-54 | **12** | 4 | BFLR | BFLR | `R D' R2 U' F2 U' F2 R U2 R2 D R2 U2` | `(U2) R2 D' R2 U2 R' F2 U F2 U R2 D R'` |
| AS-55 | **12** | 20 | BFLMRS | BFLR | `F' U2 F2 R2 U F2 U F2 U' R2 U' F'` | `F U R2 U F2 U' F2 U' R2 F2 U2 F` |
| AS-56 | **12** | 12 | BFLR | BFLR | `R L U2 R' U' R U2 L2 U R' U' L U2` | `(U2) L' U R U' L2 U2 R' U R U2 L' R'` |
| AS-57 | **12** | 28 | BFLR | BFLR | `F R' U R U F' R' U F U F' R U2` | `(U2) R' F U' F' U' R F U' R' U' R F'` |
| AS-58 | **12** | 20 | BFLR | BFLMRS | `R' U' F2 U' R2 U R2 U F2 R2 U2 R'` | `R U2 R2 F2 U' R2 U' R2 U F2 U R` |
| AS-59 | **11** | 4 | BFLR | BFLR | `L' U2 L' F2 D' R B2 R' D F2 L2 U'` | `(U) L2 F2 D' R B2 R' D F2 L U2 L` |
| AS-60 | **12** | 8 | BFLR | BFLR | `F2 L2 U' L E F D R2 U' R2 F R2 U` | `(U') R2 F' R2 U R2 D' F' E' L' U L2 F2` |
| AS-61 | **13** | 24 | BFLR | BFLR | `R' F2 D' F2 D2 F2 L D L' F2 R' D' R2 U2` | `(U2) R2 D R F2 L D' L' F2 D2 F2 D F2 R` |
| AS-62 | **11** | 8 | BFLR | BFLR | `R' U2 L U' R U L' U R' U R` | `R' U' R U' L U' R' U L' U2 R` |
| AS-63 | **13** | 16 | BFLR | BFLR | `F' U' M' U R U2 R' F U2 F' U' M F2` | `F2 M' U F U2 F' R U2 R' U' M U F` |
| AS-64 | **13** | 48 | BFLR | BFLR | `F R U' R2 U2 R U R' U R2 U R' F' U2` | `(U2) F R U' R2 U' R U' R' U2 R2 U R' F'` |
| AS-65 | **12** | 24 | BFLMRS | BFLMRS | `M U R U' L D U R' U' R D' R2 U` | `(U') R2 D R' U R U' D' L' U R' U' M'` |
| AS-66 | **13** | 24 | BFLR | BFLR | `R U' R2 D' U2 R U R' D U R2 U R' U2` | `(U2) R U' R2 U' D' R U' R' U2 D R2 U R'` |
| AS-67 | **14** | 144 | BFLR | BFLRU | `F U R' F R F' R U' R' U R U' R' F'` | `F R U R' U' R U R' F R' F' R U' F'` |
| AS-68 | **13** | 16 | BFLR | BFLR | `L U' R' U' M F U2 F U2 F' R U2 R' U2` | `(U2) R U2 R' F U2 F' U2 F' M' U R U L'` |
| AS-69 | **13** | 32 | BFLMRS | BDFLMRS | `R U2 R2 F2 R F2 L' U2 L U' R U' R'` | `R U R' U L' U2 L F2 R' F2 R2 U2 R'` |
| AS-70 | **13** | 16 | BFLR | BFLR | `R U2 F' L F L' U2 R' F' U2 F2 U2 F'` | `F U2 F2 U2 F R U2 L F' L' F U2 R'` |
| AS-71 | **11** | 8 | BFLR | BFLR | `R U R' U L' U R U' L U2 R'` | `R U2 L' U R' U' L U' R U' R'` |
| AS-72 | **13** | 32 | BFLMRS | BFLMRSU | `L' U' L U' R U2 R' F2 R' F2 R2 U2 R' U2` | `(U2) R U2 R2 F2 R F2 R U2 R' U L' U L` |
| PLL-1 | **7** | 12 | BFLMRS | BFLMRS | `F2 U' M' U2 M U' F2 U` | `(U') F2 U M' U2 M U F2` |
| PLL-2 | **7** | 12 | BFLMRS | BFLMRS | `F2 U M' U2 M U F2 U` | `(U') F2 U' M' U2 M U' F2` |
| PLL-3 | **7** | 16 | MS | MS | `M S2 M' D M2 D' S2 U'` | `(U) S2 D M2 D' M S2 M'` |
| PLL-4 | **7** | 32 | BFLMRS | BFLMRS | `M2 U' M2 U2 M2 U' M2` | `M2 U M2 U2 M2 U M2` |
| PLL-5 | **10** | 48 | BFLR | BFLR | `R' L' U2 R U R' U2 L U' R` | `R' U L' U2 R U' R' U2 L R` |
| PLL-6 | **12** | 32 | BFLMRS | BDFLMRS | `R F2 M' U L' B' R U2 R2 U' R B U'` | `(U) B' R' U R2 U2 R' B L U' M F2 R'` |
| PLL-7 | **10** | 24 | BFLR | BFLR | `L2 U' L2 D F2 R2 U R2 D' F2 U2` | `(U2) F2 D R2 U' R2 F2 D' L2 U L2` |
| PLL-8 | **12** | 32 | BFLMRS | BDFLMRS | `R' B2 M U' L F R' U2 R2 U R' F' U'` | `(U) F R U' R2 U2 R F' L' U M' B2 R` |
| PLL-9 | **9** | 8 | BFLR | BFLR | `F2 L2 F' R' F L2 F' R F' U'` | `(U) F R' F L2 F' R F L2 F2` |
| PLL-10 | **11** | 12 | BFLR | BFLR | `F2 M2 U' R2 U R2 D' R2 D L2 B2 U` | `(U') B2 L2 D' R2 D R2 U' R2 U M2 F2` |
| PLL-11 | **11** | 12 | BFLR | BFLR | `F2 L2 U' R2 U R2 D' R2 D M2 B2` | `B2 M2 D' R2 D R2 U' R2 U L2 F2` |
| PLL-12 | **12** | 32 | BFLMRS | BFLMRS | `R D' L B2 R' U R' L2 F2 L2 U' M U2` | `(U2) M' U L2 F2 L2 R U' R B2 L' D R'` |
| PLL-13 | **11** | 12 | BFLR | BFLR | `B2 M2 U R2 U' R2 D R2 D' L2 F2 U` | `(U') F2 L2 D R2 D' R2 U R2 U' M2 B2` |
| PLL-14 | **9** | 8 | BFLR | BFLR | `F R' F L2 F' R F L2 F2` | `F2 L2 F' R' F L2 F' R F'` |
| PLL-15 | **10** | 48 | BFLR | BFLR | `R L U2 R' U' R U2 L' U R' U2` | `(U2) R U' L U2 R' U R U2 L' R'` |
| PLL-16 | **11** | 12 | BFLR | BFLR | `B2 L2 U R2 U' R2 D R2 D' M2 F2 U2` | `(U2) F2 M2 D R2 D' R2 U R2 U' L2 B2` |
| PLL-17 | **14** | 192 | BFLR | BFLR | `F' R' F' R U' R U R2 F R U F U' F U2` | `(U2) F' U F' U' R' F' R2 U' R' U R' F R F` |
| PLL-18 | **13** | 336 | BFLR | BFLR | `R2 U' R2 U' R2 U F U F' R2 F U' F' U'` | `(U) F U F' R2 F U' F' U' R2 U R2 U R2` |
| PLL-19 | **13** | 52 | BFLR | BFLRU | `R U' R2 F2 U' R F2 R' U F2 R2 U R'` | `R U' R2 F2 U' R F2 R' U F2 R2 U R'` |
| PLL-20 | **13** | 8 | BFLR | BFLR | `L' F R' B2 R F' M' U L' D2 L U' R U'` | `(U) R' U L' D2 L U' M F R' B2 R F' L` |
| PLL-21 | **13** | 52 | BFLR | BFLRU | `F2 R2 U2 R2 U' R2 F2 U2 F2 U' R2 U2 F2` | `F2 U2 R2 U F2 U2 F2 R2 U R2 U2 R2 F2` |

## Reproducing

```sh
cd build && ./ll_optimal zbll --stm --near 6 --data zbll_stm_solutions.txt > zbll_stm.md   # ~1–2 h on 4 cores
```
