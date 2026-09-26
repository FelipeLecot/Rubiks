# How short can OLL and PLL algorithms get?

Speedcubing algorithms for the last layer (57 OLL + 21 PLL cases) are chosen
for **finger tricks**: mostly `R`, `U`, `F` turns, wide/slice moves and no
regrips. They are not the shortest possible. This page gives, for each case,
the common algorithm next to the **proven minimum number of moves**, computed
by `src/tools/ll_optimal.cpp`.

## Headline numbers

| | Common algs, avg HTM | **Optimal, avg HTM** | Common algs, avg QTM | **Optimal, avg QTM** | Longest optimal case (HTM) |
|---|--:|--:|--:|--:|---|
| PLL (21) | 13.95 | **11.48** | 16.48 | **13.67** | E, V: 14 |
| OLL (57) | 10.11 | **9.37** | 10.93 | **10.04** | 20: 12 |

- **PLL has the most slack.** Na drops from 21 to 13, F from 18 to 13, T from
  14 to 10, Y from 17 to 13. Only Aa/Ab, Ja, Rb and V are already optimal.
- **OLL is already close to optimal.** 33 of the 57 common algs are already
  optimal (sunes, 43–46, 33, 37, 48, 51, …). The biggest gains are OLL 55
  (15 → 11), 56 (14 → 11), 29/41/42 (13 → 10).
- The optimal solutions are usually awkward to execute (`B`, `D`, `L`, lots of
  half turns), which is why nobody uses them. For example, the optimal T perm is
  `F2 D R2 U' R2 F2 D' L2 U L2`. In practice, speed comes from ergonomics
  (TPS), not from move count.

## Method

* **Metrics.** HTM (half-turn metric): any face turn counts 1, including
  `R2`. Wide moves count 1 and slice moves (`M`, `E`, `S`) count 2. STM
  counts any layer turn as 1. QTM counts quarter turns, so `R2` = 2.
  Rotations are free. Pre- and post-AUF (a `U` turn before or after) are
  free, as is usual when quoting last-layer move counts.
* **Case definition.** Each case is taken as the state that its common
  algorithm solves. The algorithm is run on the repo's sticker model
  (`cube.h`/`moves.h`), which checks that it keeps F2L intact (and, for PLL,
  keeps the orientation). The 21 PLL and 57 OLL cases come out pairwise
  distinct under AUF, so the lists are complete.
* **Search.** IDA* over a cubie model using only outer-face turns. The
  lower bound is the maximum of several pattern databases:
  * PLL: all 8 corners (88M entries) and two overlapping 7-edge sets (511M each).
  * OLL: the last-layer pieces are indistinguishable (only their orientation
    matters), with the F2L corners, D edges and E-slice edges tracked.

  The goal set for OLL is "F2L solved and last layer oriented" (any LL
  permutation). The first search depth that finds a solution is the optimum.
* **Verification.** Every reported optimal solution is replayed on the
  sticker model and checked to solve the case (up to AUF). The
  `# optimal` counts in the tool's output are the number of distinct optimal
  move sequences, including the ones done from the other three sides.
* The optimal columns use face turns only. Allowing slice moves in the search
  would lower some STM values (e.g. H perm is 7 STM with `M2`). The optimal
  HTM numbers stay correct for HTM.

## PLL

| Case | Common algorithm | HTM | STM | QTM | **Optimal HTM** | **Optimal QTM** | Moves saved (HTM) | Optimal HTM solution (pre-AUF in parentheses) |
|---|---|--:|--:|--:|--:|--:|--:|---|
| Aa | `x R' U R' D2 R U' R' D2 R2 x'` | 9 | 9 | 12 | **9** | **12** | 0 | `(U2) F2 L2 F' R' F L2 F' R F'` |
| Ab | `x R2 D2 R U R' D2 R U' R x'` | 9 | 9 | 12 | **9** | **12** | 0 | `(U2) F R' F L2 F' R F L2 F2` |
| E | `x' R U' R' D R U R' D' R U R' D R U' R' D' x` | 16 | 16 | 16 | **14** | **14** | 2 | `F2 U' F2 U2 R2 U R2 U F2 R2 U2 R2 U F2` |
| F | `R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R` | 18 | 18 | 19 | **13** | **16** | 5 | `(U') L R' F2 L D' R F2 L' U L' R2 B2 R2` |
| Ga | `R2 U R' U R' U' R U' R2 U' D R' U R D'` | 15 | 15 | 17 | **12** | **14** | 3 | `(U) F2 D R' U R' U' R D' F2 L' U L` |
| Gb | `R' U' R U D' R2 U R' U R U' R U' R2 D` | 15 | 15 | 17 | **12** | **14** | 3 | `(U2) F2 L2 D R2 D' R2 U R2 U' L2 R2 F2` |
| Gc | `R2 U' R U' R U R' U R2 U D' R U' R' D` | 15 | 15 | 17 | **12** | **14** | 3 | `(U) F2 D' L U' L U L' D F2 R U' R'` |
| Gd | `R U R' U' D R2 U' R U' R' U R' U R2 D'` | 15 | 15 | 17 | **12** | **14** | 3 | `R U R' F2 D' L U' L' U L' D F2` |
| H | `M2 U M2 U2 M2 U M2` | 11 | 7 | 20 | **9** | **12** | 2 | `F2 L2 R2 B2 D F2 L2 R2 B2` |
| Ja | `x R2 F R F' R U2 r' U r U2 x'` | 10 | 10 | 13 | **10** | **12** | 0 | `(U') R' U L' U2 R U' R' U2 L R` |
| Jb | `R U R' F' R U R' U' R' F R2 U' R'` | 13 | 13 | 14 | **10** | **12** | 3 | `L R U2 R' U' R U2 L' U R'` |
| Na | `R U R' U R U R' F' R U R' U' R' F R2 U' R' U2 R U' R'` | 21 | 21 | 23 | **13** | **16** | 8 | `F2 U2 R2 U F2 U2 F2 R2 U R2 U2 R2 F2` |
| Nb | `R' U R U' R' F' U' F R U R' F R' F' R U' R` | 17 | 17 | 17 | **13** | **16** | 4 | `R U' R2 F2 U' R F2 R' U F2 R2 U R'` |
| Ra | `R U' R' U' R U R D R' U' R D' R' U2 R'` | 15 | 15 | 16 | **13** | **14** | 2 | `(U) R U' R F2 U R U R U' R' U' F2 R2` |
| Rb | `R2 F R U R U' R' F' R U2 R' U2 R` | 13 | 13 | 16 | **13** | **14** | 0 | `R2 F R U R U' R' F' R U2 R' U2 R` |
| T | `R U R' U' R' F R2 U' R' U' R U R' F'` | 14 | 14 | 15 | **10** | **14** | 4 | `(U) F2 D R2 U' R2 F2 D' L2 U L2` |
| Ua | `M2 U M U2 M' U M2` | 11 | 7 | 16 | **9** | **12** | 2 | `F2 U' L R' F2 L' R U' F2` |
| Ub | `M2 U' M U2 M' U' M2` | 11 | 7 | 16 | **9** | **12** | 2 | `F2 U L R' F2 L' R U F2` |
| V | `R' U R' U' y R' F' R2 U' R' U R' F R F` | 14 | 14 | 15 | **14** | **15** | 0 | `F' U F' U' R' F' R2 U' R' U R' F R F` |
| Y | `F R U' R' U' R U R' F' R U R' U' R' F R F'` | 17 | 17 | 17 | **13** | **15** | 4 | `F U F' R2 F U' F' U' R2 U R2 U R2` |
| Z | `M' U M2 U M2 U M' U2 M2` | 14 | 9 | 21 | **12** | **13** | 2 | `F2 R2 U' F2 U R2 F2 R2 U R2 U' R2` |
| **avg** | | 13.95 | | 16.48 | **11.48** | **13.67** | 2.48 | |

## OLL

OLL numbers follow the usual numbering. A slightly different numbering in
another source doesn't change the results, because each row is defined by its
algorithm.

| Case | Common algorithm | HTM | STM | QTM | **Optimal HTM** | **Optimal QTM** | Moves saved (HTM) | Optimal HTM solution (pre-AUF in parentheses) |
|---|---|--:|--:|--:|--:|--:|--:|---|
| 1 | `R U2 R2 F R F' U2 R' F R F'` | 11 | 11 | 14 | **11** | **12** | 0 | `R U2 R2 F R F' U2 R' F R F'` |
| 2 | `F R U R' U' F' f R U R' U' f'` | 12 | 12 | 12 | **11** | **12** | 1 | `(U2) F R' F' R U2 F R' F' R2 U2 R'` |
| 3 | `f R U R' U' f' U' F R U R' U' F'` | 13 | 13 | 13 | **11** | **12** | 2 | `(U2) R' F2 R2 U2 R' F R U2 R2 F2 R` |
| 4 | `f R U R' U' f' U F R U R' U' F'` | 13 | 13 | 13 | **11** | **12** | 2 | `R' F2 R2 U2 R' F' R U2 R2 F2 R` |
| 5 | `r' U2 R U R' U r` | 7 | 7 | 8 | **7** | **8** | 0 | `(U2) R' F2 L F L' F R` |
| 6 | `r U2 R' U' R U' r'` | 7 | 7 | 8 | **7** | **8** | 0 | `L F2 R' F' R F' L'` |
| 7 | `r U R' U R U2 r'` | 7 | 7 | 8 | **7** | **8** | 0 | `L F R' F R F2 L'` |
| 8 | `l' U' L U' L' U2 l` | 7 | 7 | 8 | **7** | **8** | 0 | `R' F' L F' L' F2 R` |
| 9 | `R U R' U' R' F R2 U R' U' F'` | 11 | 11 | 12 | **10** | **10** | 1 | `R' U' R F R' F' U F R F'` |
| 10 | `R U R' U R' F R F' R U2 R'` | 11 | 11 | 12 | **10** | **10** | 1 | `(U) F U F' R' F R U' R' F' R` |
| 11 | `r U R' U R' F R F' R U2 r'` | 11 | 11 | 12 | **11** | **12** | 0 | `(U) R' F' U' F2 R' F' R2 U' R' U2 R` |
| 12 | `M' R' U' R U' R' U2 R U' R r'` | 12 | 11 | 13 | **11** | **12** | 1 | `(U2) F R U R2 F R F2 U F U2 F'` |
| 13 | `F U R U' R2 F' R U R U' R'` | 11 | 11 | 12 | **10** | **10** | 1 | `F U R U2 R' U' R U R' F'` |
| 14 | `R' F R U R' F' R F U' F'` | 10 | 10 | 10 | **10** | **10** | 0 | `F' U' L' U2 L U' F R U' R'` |
| 15 | `r' U' r R' U' R U r' U r` | 10 | 10 | 10 | **10** | **10** | 0 | `(U2) R' F' R L' U' L U R' F R` |
| 16 | `r U r' R U R' U' r U' r'` | 10 | 10 | 10 | **10** | **10** | 0 | `F' L' F R F' U' L U F R'` |
| 17 | `F R' F' R2 r' U R U' R' U' M'` | 12 | 11 | 13 | **11** | **12** | 1 | `(U') R' F' U2 F2 U R U' R' F' U2 R` |
| 18 | `r U R' U R U2 r2 U' R U' R' U2 r` | 13 | 13 | 16 | **11** | **12** | 2 | `L F R U2 R' U2 R U2 R' F' L'` |
| 19 | `r' R U R U R' U' M' R' F R F'` | 13 | 12 | 13 | **11** | **12** | 2 | `R' U2 F R U R' U' F2 U2 F R` |
| 20 | `r U R' U' M2 U R U' R' U' M'` | 13 | 11 | 15 | **12** | **13** | 1 | `L R F U2 R2 U2 R2 U2 R2 F' L' R'` |
| 21 | `R U2 R' U' R U R' U' R U' R'` | 11 | 11 | 12 | **11** | **11** | 0 | `(U) R U R' U R U' R' U R U2 R'` |
| 22 | `R U2 R2 U' R2 U' R2 U2 R` | 9 | 9 | 14 | **9** | **11** | 0 | `R U2 R2 U' R2 U' R2 U2 R` |
| 23 | `R2 D' R U2 R' D R U2 R` | 9 | 9 | 12 | **9** | **11** | 0 | `(U2) R2 D R' U2 R D' R' U2 R'` |
| 24 | `r U R' U' r' F R F'` | 8 | 8 | 8 | **8** | **8** | 0 | `F R F' L F R' F' L'` |
| 25 | `F' r U R' U' r' F R` | 8 | 8 | 8 | **8** | **8** | 0 | `(U') F R' F' L F R F' L'` |
| 26 | `R U2 R' U' R U' R'` | 7 | 7 | 8 | **7** | **7** | 0 | `R U2 R' U' R U' R'` |
| 27 | `R U R' U R U2 R'` | 7 | 7 | 8 | **7** | **7** | 0 | `R U R' U R U2 R'` |
| 28 | `r U R' U' r' R U R U' R'` | 10 | 10 | 10 | **9** | **10** | 1 | `R2 F2 L F L' F2 R F' R` |
| 29 | `R U R' U' R U' R' F' U' F R U R'` | 13 | 13 | 13 | **10** | **10** | 3 | `(U) F R' U' R2 U' R2 U2 R U' F'` |
| 30 | `F R' F R2 U' R' U' R U R' F2` | 11 | 11 | 13 | **10** | **10** | 1 | `(U') R' F U F2 U F2 U2 F' U R` |
| 31 | `R' U' F U R U' R' F' R` | 9 | 9 | 9 | **9** | **9** | 0 | `R' U' F U R U' R' F' R` |
| 32 | `L U F' U' L' U L F L'` | 9 | 9 | 9 | **9** | **9** | 0 | `(U') F U R' U' F' U F R F'` |
| 33 | `R U R' U' R' F R F'` | 8 | 8 | 8 | **8** | **8** | 0 | `R U R' U' R' F R F'` |
| 34 | `R U R2 U' R' F R U R U' F'` | 11 | 11 | 12 | **10** | **10** | 1 | `(U) F U F' U' R' F' L F L' R` |
| 35 | `R U2 R2 F R F' R U2 R'` | 9 | 9 | 12 | **9** | **12** | 0 | `R U2 R2 F R F' R U2 R'` |
| 36 | `L' U' L U' L' U L U L F' L' F` | 12 | 12 | 12 | **10** | **11** | 2 | `(U') R U R2 F' U' F U R2 U2 R'` |
| 37 | `F R' F' R U R U' R'` | 8 | 8 | 8 | **8** | **8** | 0 | `F R' F' R U R U' R'` |
| 38 | `R U R' U R U' R' U' R' F R F'` | 12 | 12 | 12 | **10** | **11** | 2 | `(U') F R U R2 U' F' U F R F'` |
| 39 | `L F' L' U' L U F U' L'` | 9 | 9 | 9 | **9** | **9** | 0 | `(U') F R' F' U' F U R U' F'` |
| 40 | `R' F R U R' U' F' U R` | 9 | 9 | 9 | **9** | **9** | 0 | `R' F R U R' U' F' U R` |
| 41 | `R U R' U R U2 R' F R U R' U' F'` | 13 | 13 | 14 | **10** | **11** | 3 | `F U R2 D R' U' R D' R2 F'` |
| 42 | `R' U' R U' R' U2 R F R U R' U' F'` | 13 | 13 | 14 | **10** | **11** | 3 | `(U) R' U' F2 D' F U F' D F2 R` |
| 43 | `F' U' L' U L F` | 6 | 6 | 6 | **6** | **6** | 0 | `(U') R' U' F' U F R` |
| 44 | `F U R U' R' F'` | 6 | 6 | 6 | **6** | **6** | 0 | `F U R U' R' F'` |
| 45 | `F R U R' U' F'` | 6 | 6 | 6 | **6** | **6** | 0 | `F R U R' U' F'` |
| 46 | `R' U' R' F R F' U R` | 8 | 8 | 8 | **8** | **8** | 0 | `R' U' R' F R F' U R` |
| 47 | `R' U' R' F R F' R' F R F' U R` | 12 | 12 | 12 | **10** | **10** | 2 | `(U') R' F' U' F U F' U' F U R` |
| 48 | `F R U R' U' R U R' U' F'` | 10 | 10 | 10 | **10** | **10** | 0 | `F R U R' U' R U R' U' F'` |
| 49 | `r U' r2 U r2 U r2 U' r` | 9 | 9 | 12 | **9** | **12** | 0 | `(U') F R' F2 R U2 R U2 R' F` |
| 50 | `r' U r2 U' r2 U' r2 U r'` | 9 | 9 | 12 | **9** | **12** | 0 | `(U') F' R U2 R' U2 R' F2 R F'` |
| 51 | `F U R U' R' U R U' R' F'` | 10 | 10 | 10 | **10** | **10** | 0 | `F U R U' R' U R U' R' F'` |
| 52 | `R U R' U R U' B U' B' R'` | 10 | 10 | 10 | **10** | **10** | 0 | `R' U' R U' R' U F' U F R` |
| 53 | `l' U2 L U L' U' L U L' U l` | 11 | 11 | 12 | **10** | **12** | 1 | `(U) F' L F L' U2 F2 R' F' R F'` |
| 54 | `r U2 R' U' R U R' U' R U' r'` | 11 | 11 | 12 | **10** | **12** | 1 | `(U') F R' F' R U2 F2 L F L' F` |
| 55 | `R' F R U R U' R2 F' R2 U' R' U R U R'` | 15 | 15 | 17 | **11** | **12** | 4 | `(U) R U2 R2 U' R U' R' U2 F R F'` |
| 56 | `r' U' r U' R' U R U' R' U R r' U r` | 14 | 14 | 14 | **11** | **12** | 3 | `R' F' L' F R2 F' U' L U F R'` |
| 57 | `R U R' U' M' U R U' r'` | 10 | 9 | 10 | **10** | **10** | 0 | `L' R U R' U' L R' F R F'` |
| **avg** | | 10.11 | | 10.93 | **9.37** | **10.04** | 0.74 | |

## Reproducing

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
cd build
./ll_optimal pll          # ~6 min first time (builds ~1.2 GB of pattern databases, cached)
./ll_optimal oll          # seconds
./ll_optimal all --qtm    # quarter-turn metric, ~12 min
```
