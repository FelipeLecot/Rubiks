# Optimal ZBLL, for Fewest Moves

Every ZBLL case (last layer with all edges already oriented), solved
**provably optimally** in HTM, with **all** optimal solutions listed.
AUF before and after is free. Computed by `ll_optimal zbll`.

For FMC, one short algorithm per case isn't enough; you also need the
alternatives. The last moves of your skeleton can **cancel** with the first
moves of the finish, so it helps to know every optimal solution and which
faces it starts and ends with. That's what this table and the solutions file
give.

Files:
- This page: one row per case with the optimal length, the number of optimal
  solutions, the faces they can start/end with, a setup, and one solution.
- [`zbll_optimal_solutions.txt`](zbll_optimal_solutions.txt): all
  11424 optimal
  solutions, grouped by case. Pre-AUF is shown in parentheses. For example,
  `awk '/^# T-12 /{f=1;next} /^#/{f=0} f && /^R/' zbll_optimal_solutions.txt` lists the
  solutions of T-12 that start with an R turn.

## Summary

| Set | Cases | Mean optimal HTM | 7 moves | 8 moves | 9 moves | 10 moves | 11 moves | 12 moves | 13 moves | 14 moves | 15 moves | Median # optimal solutions |
|---|--:|--:|--:|--:|--:|--:|--:|--:|--:|--:|--:|--:|
| T | 72 | 12.17 |  | 2 | 2 | 7 | 2 | 26 | 25 | 7 | 1 | 12 |
| U | 72 | 12.31 |  |  | 6 | 4 | 6 | 9 | 40 | 7 |  | 16 |
| L | 72 | 11.97 |  | 2 | 6 | 6 | 8 | 15 | 26 | 9 |  | 12 |
| H | 40 | 12.50 |  |  |  |  | 10 | 8 | 15 | 6 | 1 | 8 |
| Pi | 72 | 12.46 |  |  | 2 |  | 7 | 24 | 32 | 7 |  | 8 |
| S | 72 | 11.82 | 3 |  |  | 6 | 15 | 22 | 23 | 3 |  | 8 |
| AS | 72 | 11.82 | 3 |  |  | 6 | 15 | 22 | 23 | 3 |  | 8 |
| PLL | 21 | 11.48 |  |  | 5 | 3 |  | 5 | 6 | 2 |  | 24 |
| **All** | 493 | 12.10 | 6 | 4 | 21 | 32 | 63 | 131 | 190 | 44 | 2 | 12 |

- **Mean 12.10 HTM, max 15.** Only two cases (T-65 and H-1) need 15 moves.
- S and AS have identical distributions, as they must: each AS case is the
  mirror of an S case. The 21 PLL cases, found by enumeration here, give
  exactly the same optimal lengths as the separate PLL run.
- Coverage check: enumerating all last-layer states with oriented edges and
  grouping by pre/post AUF gives exactly **493** cases (T, U, L, Pi, S, AS
  72 each, H 40, PLL 21), the known ZBLL count.

## Observations

- **The hardest cases have comfortable optimal solutions.** Both 15-movers
  have optimal solutions in only three faces. T-65's optimal solutions
  include `R2 F R U R' U' R' F' R' U' R2 U2 R U2 R`, which is <R,U,F> only
  and contains the familiar `R U R' U'` trigger. Of the 46 cases needing 14+
  moves, only 4 force B or D. The mid-length cases (9–13 moves) are where
  optimality costs ergonomics.
- **35% of cases (175) need B or D.** For those, every optimal solution,
  done from any side, uses a B or D turn. 144 cases require D in every
  optimal solution.
- **Face usage:** 28 cases have a 2-gen <R,U> optimal solution, 168 have one
  in <R,U,F>, and 318 have one avoiding B and D entirely.
- **140 cases have an essentially unique optimal solution** (the same move
  sequence, done from any of the four sides). For FMC these are the ones where
  a cancellation has to come from AUF alone, or from a solution one move
  longer.
- **44% of cases (216): every optimal solution starts and ends on the same
  face**, e.g. `F … F'`, `R … R'`, the setup–commutator–undo shape. That's
  good for FMC: a skeleton ending in that face can cancel on both sides of an
  inverse-scramble (NISS) trick.
- Only 39 cases have an optimal solution without any half turn.

## How the numbers are proven

- IDA* over face turns. The lower bound is an exact 8-corner pattern database
  plus two 7-edge databases, and a table of all 109,043,123 positions within
  7 moves of solved (exactly the published count) finishes each search. Every
  search stops at the first depth that has a solution and then enumerates
  every solution at that depth.
- Solutions are enumerated up to the redundancy rules (no two turns of the
  same face in a row; opposite faces in a fixed order, so `L R` is listed but
  `R L` is not). They include the versions done from all four sides, and no
  solution starts or ends with `U` (that is AUF).
- Every listed solution, all of them rather than a sample, is replayed on the
  repo's independent sticker model and checked to solve its case.

Case names (`T-1` … `T-72`) are this tool's own numbering, not any
published ZBLL list. The *setup* column identifies each case: apply it to a
solved cube to get the case.

## All cases

| Case | Optimal HTM | # optimal | First faces | Last faces | Setup (inverse of a solution) | Shortest, most comfortable solution |
|---|--:|--:|---|---|---|---|
| T-1 | **12** | 72 | BFLR | BFLR | `L U2 R' U F2 R' F2 R U2 L' U R` | `R' U' L U2 R' F2 R F2 U' R U2 L'` |
| T-2 | **12** | 8 | BFLR | BFLR | `F U2 F2 L F L' F U' R U' R' F' U2` | `(U2) F R U R' U F' L F' L' F2 U2 F'` |
| T-3 | **13** | 12 | BFLR | BFLR | `L U' R2 U F R2 F' L' B U2 B' U' R2 U2` | `(U2) R2 U B U2 B' L F R2 F' U' R2 U L'` |
| T-4 | **13** | 12 | BFLR | BFLR | `R' U' R U' R' U2 R2 U R' U R U2 R' U2` | `(U2) R U2 R' U' R U' R2 U2 R U R' U R` |
| T-5 | **13** | 32 | BFLR | BFLR | `R' U' R U' R U2 R2 U' R2 U' R2 U R U2` | `(U2) R' U' R2 U R2 U R2 U2 R' U R' U R` |
| T-6 | **13** | 12 | BFLR | BFLR | `R' D F2 R2 U R2 D' F2 R U L2 U' L2 U` | `(U') L2 U L2 U' R' F2 D R2 U' R2 F2 D' R` |
| T-7 | **13** | 32 | BFLR | BFLR | `R U R' U R' U2 R2 U R2 U R2 U' R' U2` | `(U2) R U R2 U' R2 U' R2 U2 R U' R U' R'` |
| T-8 | **12** | 4 | BFLR | BFLR | `L B' R2 F2 R' D R D' F2 R2 B L'` | `L B' R2 F2 D R' D' R F2 R2 B L'` |
| T-9 | **12** | 8 | BFLR | BFLR | `F2 R2 F L F' R2 F2 R' F' L' F R` | `R' F' L F R F2 R2 F L' F' R2 F2` |
| T-10 | **12** | 8 | BFLR | BFLR | `F' U2 F2 R' F' R F' U L' U L F` | `F' L' U' L U' F R' F R F2 U2 F` |
| T-11 | **12** | 4 | BFLR | BFLR | `L' F R2 B2 R D' R' D B2 R2 F' L` | `L' F R2 B2 D' R D R' B2 R2 F' L` |
| T-12 | **13** | 12 | BFLR | BFLR | `R U R' U R U2 R2 U' R U' R' U2 R U'` | `(U) R' U2 R U R' U R2 U2 R' U' R U' R'` |
| T-13 | **10** | 8 | BFLR | BFLR | `R' U R U2 R' L' U R U' L U2` | `(U2) L' U R' U' L R U2 R' U' R` |
| T-14 | **12** | 56 | BFLR | BFLR | `R L' U R U' L U R2 U R U2 R' U` | `(U') R U2 R' U' R2 U' L' U R' U' L R'` |
| T-15 | **10** | 16 | BFLR | BFLR | `R' F R' D2 L B' L' D2 R2 F' U2` | `(U2) F R2 D2 L B L' D2 R F' R` |
| T-16 | **12** | 56 | BFLR | BFLR | `L R' U' R' U L' U' R2 U' R' U2 R U` | `(U') R' U2 R U R2 U L U' R U R L'` |
| T-17 | **13** | 8 | BFLR | BFLR | `L F R U2 R' U R U2 R2 F R F2 L' U2` | `(U2) L F2 R' F' R2 U2 R' U' R U2 R' F' L'` |
| T-18 | **14** | 72 | BFLR | BFLR | `R' U' R U' R' U R F U' R' U2 R U F'` | `F U' R' U2 R U F' R' U' R U R' U R` |
| T-19 | **14** | 72 | BFLR | BFLR | `F U' R' U2 R U F' R' U' R U R' U R U2` | `(U2) R' U' R U' R' U R F U' R' U2 R U F'` |
| T-20 | **14** | 80 | BFLR | BFLR | `L2 D' B' D L2 U' F R2 U2 R2 U2 R2 U F' U2` | `(U2) F U' R2 U2 R2 U2 R2 F' U L2 D' B D L2` |
| T-21 | **14** | 72 | BFLR | BFLR | `R U R' U R U' R' F' U' L' U2 L U F U2` | `(U2) F' U' L' U2 L U F R U R' U' R U' R'` |
| T-22 | **13** | 8 | BFLR | BFLR | `L F2 R' F' R2 U2 R' U' R U2 R' F' L' U2` | `(U2) L F R U2 R' U R U2 R2 F R F2 L'` |
| T-23 | **10** | 8 | BFLR | BFLR | `R U' R' U2 L R U' R' U L'` | `L U' R U R' L' U2 R U R'` |
| T-24 | **14** | 72 | BFLR | BFLR | `L F U' R U' R' U' R U R' U2 F' U' L' U2` | `(U2) L U F U2 R U' R' U R U R' U F' L'` |
| T-25 | **12** | 16 | BFLR | BFLR | `F R2 L' U B U' B' U' L U R2 F' U'` | `(U) F R2 U' L' U B U B' U' L R2 F'` |
| T-26 | **13** | 40 | BFLR | BFLR | `R2 U R2 U' R' U' F R2 U R2 U' F' R' U2` | `(U2) R F U R2 U' R2 F' U R U R2 U' R2` |
| T-27 | **12** | 4 | BFLR | BFLR | `L' U2 F2 D' L D F2 R U' R' U2 L` | `L' U2 R U R' F2 D' L' D F2 U2 L` |
| T-28 | **14** | 104 | BDFLR | BDFLR | `R2 U2 R' U' R F R F' U' R' F' U2 F R' U2` | `(U2) R F' U2 F R U F R' F' R' U R U2 R2` |
| T-29 | **8** | 8 | BFLR | BFLR | `R' F' L' F R F' L F` | `F' L' F R' F' L F R` |
| T-30 | **10** | 4 | BFLR | BFLR | `R' L F2 U' F2 U F2 R U L'` | `L U' R' F2 U' F2 U F2 L' R` |
| T-31 | **12** | 24 | BFLR | BFLR | `F R U R' U' R' F' R U2 R U2 R' U2` | `(U2) R U2 R' U2 R' F R U R U' R' F'` |
| T-32 | **12** | 8 | BFLR | BFLR | `L2 F' R' F R L' F R' F R F' L' U2` | `(U2) L F R' F' R F' L R' F' R F L2` |
| T-33 | **12** | 12 | BFLR | BFLR | `R2 F2 R2 U' F R2 D' F D R2 U F' U2` | `(U2) F U' R2 D' F' D R2 F' U R2 F2 R2` |
| T-34 | **13** | 8 | BFLR | BFLR | `F2 R' U R2 U' F2 U F2 R' F2 R' U' R` | `R' U R F2 R F2 U' F2 U R2 U' R F2` |
| T-35 | **12** | 8 | BFLR | BFLR | `F2 U F2 R D2 L' U2 L D2 R' U' F2 U` | `(U') F2 U R D2 L' U2 L D2 R' F2 U' F2` |
| T-36 | **10** | 12 | BFLR | BFLR | `R U R' F' U' L' U2 L U F U2` | `(U2) F' U' L' U2 L U F R U' R'` |
| T-37 | **8** | 8 | BFLR | BFLR | `L F R F' L' F R' F'` | `F R F' L F R' F' L'` |
| T-38 | **12** | 16 | BFLR | BFLR | `R2 D' L F2 R L' U R' D R U R U` | `(U') R' U' R' D' R U' L R' F2 L' D R2` |
| T-39 | **12** | 24 | BFLR | BFLR | `F' R U2 R' U2 R' F R U R U' R' U2` | `(U2) R U R' U' R' F' R U2 R U2 R' F` |
| T-40 | **12** | 4 | BFLR | BFLR | `R U2 F2 D R' D' F2 L' U L U2 R'` | `R U2 L' U' L F2 D R D' F2 U2 R'` |
| T-41 | **12** | 8 | BFLR | BFLR | `F2 U' F2 L' D2 R U2 R' D2 L U F2 U'` | `(U) F2 U' L' D2 R U2 R' D2 L F2 U F2` |
| T-42 | **14** | 104 | BDFLR | BDFLR | `F2 U2 F U F' R' F' R U F R U2 R' F U'` | `(U) F' R U2 R' F' U' R' F R F U' F' U2 F2` |
| T-43 | **12** | 12 | BFLR | BFLR | `F2 R2 F2 U R' F2 D R' D' F2 U' R U'` | `(U) R' U F2 D R D' F2 R U' F2 R2 F2` |
| T-44 | **13** | 40 | BFLR | BFLR | `F' U2 R U' L' U L R' U L' U2 L F U` | `(U') F' L' U2 L U' R L' U' L U R' U2 F` |
| T-45 | **13** | 8 | BFLR | BFLR | `R2 F U' F2 U R2 U' R2 F R2 F U F' U` | `(U') F U' F' R2 F' R2 U R2 U' F2 U F' R2` |
| T-46 | **10** | 4 | BFLR | BFLR | `R' L F2 U F2 U' F2 L' U' R` | `R' U L F2 U F2 U' F2 L' R` |
| T-47 | **10** | 12 | BFLR | BFLR | `F R U' R' U' R U2 R' U' F' U2` | `(U2) F U R U2 R' U R U R' F'` |
| T-48 | **12** | 8 | BFLR | BFLR | `R2 F L F' R L' F' L F' L' F R U2` | `(U2) R' F' L F L' F L R' F L' F' R2` |
| T-49 | **9** | 4 | BFLR | BFLR | `F2 R2 F L2 F' R2 F L2 F U2` | `(U2) F' L2 F' R2 F L2 F' R2 F2` |
| T-50 | **13** | 60 | BFLR | BFLR | `L U2 R' U' R U2 L2 U R' U' L U' R U2` | `(U2) R' U L' U R U' L2 U2 R' U R U2 L'` |
| T-51 | **13** | 40 | BFLR | BFLR | `R2 F2 R U2 R U R2 F2 R2 U R' F2 R U'` | `(U) R' F2 R U' R2 F2 R2 U' R' U2 R' F2 R2` |
| T-52 | **13** | 24 | BFLR | BFLR | `R U2 F2 R F' R F R2 F' R F' U2 R' U'` | `(U) R U2 F R' F R2 F' R' F R' F2 U2 R'` |
| T-53 | **13** | 60 | BFLR | BFLR | `L' U2 R U R' U2 L2 U' R U L' U R'` | `R U' L U' R' U L2 U2 R U' R' U2 L` |
| T-54 | **12** | 4 | BFLR | BFLR | `F' R' U F U F' U2 F U F' R F U` | `(U') F' R' F U' F' U2 F U' F' U' R F` |
| T-55 | **13** | 4 | BFLR | BFLR | `R' D2 U L U' L' D2 F2 D' L D F2 R U` | `(U') R' F2 D' L' D F2 D2 L U L' U' D2 R` |
| T-56 | **12** | 4 | BFLR | BFLR | `R F U' R' U' R U2 R' U' R F' R'` | `R F R' U R U2 R' U R U F' R'` |
| T-57 | **9** | 4 | BFLR | BFLR | `F2 L2 F' R2 F L2 F' R2 F' U2` | `(U2) F R2 F L2 F' R2 F L2 F2` |
| T-58 | **13** | 4 | BFLR | BFLR | `L D2 U' R' U R D2 F2 D R' D' F2 L' U'` | `(U) L F2 D R D' F2 D2 R' U' R U D2 L'` |
| T-59 | **13** | 40 | BFLR | BFLR | `F2 R2 F' U2 F' U' F2 R2 F2 U' F R2 F' U2` | `(U2) F R2 F' U F2 R2 F2 U F U2 F R2 F2` |
| T-60 | **13** | 40 | BFLR | BFLR | `F R U' R' U R U R' U R U' R' F'` | `F R U R' U' R U' R' U' R U R' F'` |
| T-61 | **13** | 24 | BDFLR | BDFLR | `R D' F2 L' U' L F2 R2 U R U' R2 D U'` | `(U) D' R2 U R' U' R2 F2 L' U L F2 D R'` |
| T-62 | **12** | 4 | BFLR | BFLR | `F2 U F' U' R U2 F U2 F' R' U F' U'` | `(U) F U' R F U2 F' U2 R' U F U' F2` |
| T-63 | **11** | 4 | BFLR | BFLR | `L' U L2 D R' F2 R D' L2 U' L U'` | `(U) L' U L2 D R' F2 R D' L2 U' L` |
| T-64 | **13** | 8 | BFLR | BFLR | `L2 D2 R D2 L' F2 D U' R' U R' D' L' U2` | `(U2) L D R U' R U D' F2 L D2 R' D2 L2` |
| T-65 | **15** | 472 | BFLR | BFLR | `R' U2 R' U2 R2 U R F R U R U' R' F' R2` | `R2 F R U R' U' R' F' R' U' R2 U2 R U2 R` |
| T-66 | **13** | 24 | BDFLR | BDFLR | `R2 U R' U' R2 F2 L' U L F2 D R' D' U'` | `(U) D R D' F2 L' U' L F2 R2 U R U' R2` |
| T-67 | **13** | 8 | BFLR | BFLR | `L D R U' R D' U F2 L D2 R' D2 L2 U2` | `(U2) L2 D2 R D2 L' F2 U' D R' U R' D' L'` |
| T-68 | **11** | 4 | BFLR | BFLR | `R U' R2 D' L F2 L' D R2 U R' U'` | `(U) R U' R2 D' L F2 L' D R2 U R'` |
| T-69 | **12** | 4 | BFLR | BFLR | `F U' R F U2 F' U2 R' U F U' F2 U'` | `(U) F2 U F' U' R U2 F U2 F' R' U F'` |
| T-70 | **13** | 48 | BFLR | BFLR | `R' U2 R U' L' U R' U' L2 U' R U L' U2` | `(U2) L U' R' U L2 U R U' L U R' U2 R` |
| T-71 | **12** | 4 | BFLR | BFLR | `R' U F' R' U2 R U2 F U' R' U R2 U'` | `(U) R2 U' R U F' U2 R' U2 R F U' R` |
| T-72 | **12** | 4 | BFLR | BFLR | `R2 U' R U F' U2 R' U2 R F U' R U'` | `(U) R' U F' R' U2 R U2 F U' R' U R2` |
| U-1 | **12** | 72 | BFLR | BFLR | `R' U' L U2 R' F2 R F2 U' R U2 L' U2` | `(U2) L U2 R' U F2 R' F2 R U2 L' U R` |
| U-2 | **13** | 12 | BFLR | BFLR | `R2 U B U2 B' L F R2 F' U' R2 U L' U` | `(U') L U' R2 U F R2 F' L' B U2 B' U' R2` |
| U-3 | **12** | 8 | BFLR | BFLR | `F R U R' U F' L F' L' F2 U2 F' U2` | `(U2) F U2 F2 L F L' F U' R U' R' F'` |
| U-4 | **13** | 12 | BFLR | BFLR | `R U2 R' U' R U' R2 U2 R U R' U R U2` | `(U2) R' U' R U' R' U2 R2 U R' U R U2 R'` |
| U-5 | **13** | 32 | BFLR | BFLR | `R U R2 U' R2 U' R2 U2 R U' R U' R' U'` | `(U) R U R' U R' U2 R2 U R2 U R2 U' R'` |
| U-6 | **12** | 8 | BFLR | BFLR | `F' L' U' L U' F R' F R F2 U2 F` | `F' U2 F2 R' F' R F' U L' U L F` |
| U-7 | **13** | 32 | BFLR | BFLR | `R' U' R2 U R2 U R2 U2 R' U R' U R U` | `(U') R' U' R U' R U2 R2 U' R2 U' R2 U R` |
| U-8 | **12** | 4 | BFLR | BFLR | `L' F R2 B2 D' R D R' B2 R2 F' L U2` | `(U2) L' F R2 B2 R D' R' D B2 R2 F' L` |
| U-9 | **12** | 8 | BFLR | BFLR | `R' F' L F R F2 R2 F L' F' R2 F2 U2` | `(U2) F2 R2 F L F' R2 F2 R' F' L' F R` |
| U-10 | **13** | 12 | BFLR | BFLR | `L2 U L2 U' R' F2 D R2 U' R2 F2 D' R U2` | `(U2) R' D F2 R2 U R2 D' F2 R U L2 U' L2` |
| U-11 | **12** | 4 | BFLR | BFLR | `L B' R2 F2 D R' D' R F2 R2 B L' U2` | `(U2) L B' R2 F2 R' D R D' F2 R2 B L'` |
| U-12 | **13** | 12 | BFLR | BFLR | `R' U2 R U R' U R2 U2 R' U' R U' R' U'` | `(U) R U R' U R U2 R2 U' R U' R' U2 R` |
| U-13 | **13** | 40 | BDFLR | BDFLR | `R U' R' U F' L F' L' F2 R' F R F' U'` | `(U) F R' F' R F2 L F L' F U' R U R'` |
| U-14 | **13** | 8 | BFLR | BFLR | `R' U L' U' L U' R U R' F2 R' F2 R2 U2` | `(U2) R2 F2 R F2 R U' R' U L' U L U' R` |
| U-15 | **13** | 16 | BFLR | BFLR | `R2 F2 D L' B2 L D' F2 R2 U' R U' R' U2` | `(U2) R U R' U R2 F2 D L' B2 L D' F2 R2` |
| U-16 | **13** | 8 | BFLR | BFLR | `L2 F2 L' F2 L' U L U' R U' R' U L' U` | `(U') L U' R U R' U L' U' L F2 L F2 L2` |
| U-17 | **13** | 24 | BFLR | BFLR | `R' U' R F R2 D' R U R' D R2 U' F' U` | `(U') F U R2 D' R U' R' D R2 F' R' U R` |
| U-18 | **13** | 16 | BFLR | BFLR | `L' U2 R L2 U2 R' U' R U2 L2 U R' L U2` | `(U2) L' R U' L2 U2 R' U R U2 L2 R' U2 L` |
| U-19 | **13** | 16 | BFLR | BFLR | `L' R U' L2 U2 R' U R U2 L2 R' U2 L U'` | `(U) L' U2 R L2 U2 R' U' R U2 L2 U R' L` |
| U-20 | **14** | 48 | BFLR | BFLR | `R' U R U' R' U L' U' L U' F2 R' F2 R2 U2` | `(U2) R2 F2 R F2 U L' U L U' R U R' U' R` |
| U-21 | **13** | 16 | BFLR | BFLR | `L U2 R' L2 U2 R U R' U2 L2 U' R L'` | `L R' U L2 U2 R U' R' U2 L2 R U2 L'` |
| U-22 | **13** | 24 | BFLR | BFLR | `F U R2 D' R U' R' D R2 F' R' U R U` | `(U') R' U' R F R2 D' R U R' D R2 U' F'` |
| U-23 | **13** | 40 | BDFLR | BDFLR | `L' U L U' F R' F R F2 L F' L' F U` | `(U') F' L F L' F2 R' F' R F' U L' U' L` |
| U-24 | **13** | 16 | BFLR | BFLR | `R' L U L2 U2 R U' R' U2 R L2 U2 L' U'` | `(U) L U2 L2 R' U2 R U R' U2 L2 U' L' R` |
| U-25 | **9** | 4 | BFLR | BFLR | `R' U2 R' D' L F2 L' D R2 U` | `(U') R2 D' L F2 L' D R U2 R` |
| U-26 | **11** | 16 | BFLR | BFLR | `R' U2 R U R2 D' R U R' D R2` | `R2 D' R U' R' D R2 U' R' U2 R` |
| U-27 | **13** | 12 | BFLR | BFLR | `F2 L' U2 R' F R U2 F2 U2 F' U2 L F U'` | `(U) F' L' U2 F U2 F2 U2 R' F' R U2 L F2` |
| U-28 | **13** | 24 | BFLR | BFLR | `R' F R' U F' R' F U' R2 F' R U R' U` | `(U') R U' R' F R2 U F' R F U' R F' R` |
| U-29 | **9** | 4 | BFLR | BFLR | `R' U2 R' D' R U2 R' D R2 U` | `(U') R2 D' R U2 R' D R U2 R` |
| U-30 | **14** | 60 | BFLR | BFLR | `L F' U2 F' U' R U' R' F' U F' U' F' L' U` | `(U') L F U F U' F R U R' U F U2 F L'` |
| U-31 | **13** | 56 | BDFLR | BDFLR | `F U' R' U R U2 F' R' F U' F' U2 R U` | `(U') R' U2 F U F' R F U2 R' U' R U F'` |
| U-32 | **13** | 16 | BFLR | BFLR | `R U2 R' F U2 F' U' R F U' F' U2 R'` | `R U2 F U F' R' U F U2 F' R U2 R'` |
| U-33 | **14** | 36 | BFLR | BFLR | `R U' R2 F2 R U2 R U2 R' F2 U2 R U' R' U2` | `(U2) R U R' U2 F2 R U2 R' U2 R' F2 R2 U R'` |
| U-34 | **14** | 60 | BFLR | BFLR | `L' U F R2 U' L U R2 U2 L' U2 F' U2 L U2` | `(U2) L' U2 F U2 L U2 R2 U' L' U R2 F' U' L` |
| U-35 | **13** | 24 | BDFLR | BDFLR | `L U' R' U L' U' R2 U2 R' U' R U' R' U'` | `(U) R U R' U R U2 R2 U L U' R U L'` |
| U-36 | **11** | 4 | BFLR | BFLR | `R2 D' L' F2 L' D2 R' D L2 D2 R' U2` | `(U2) R D2 L2 D' R D2 L F2 L D R2` |
| U-37 | **9** | 4 | BFLR | BFLR | `R U2 R D R' U2 R D' R2 U` | `(U') R2 D R' U2 R D' R' U2 R'` |
| U-38 | **9** | 4 | BFLR | BFLR | `L U2 L D R' F2 R D' L2 U'` | `(U) L2 D R' F2 R D' L' U2 L'` |
| U-39 | **13** | 56 | BDFLR | BDFLR | `R' U F U' F' U2 R F R' U R U2 F'` | `F U2 R' U' R F' R' U2 F U F' U' R` |
| U-40 | **13** | 12 | BFLR | BFLR | `F2 R U2 L F' L' U2 F2 U2 F U2 R' F' U` | `(U') F R U2 F' U2 F2 U2 L F L' U2 R' F2` |
| U-41 | **13** | 24 | BDFLR | BDFLR | `L' U R U' L U R2 U2 R U R' U R U'` | `(U) R' U' R U' R' U2 R2 U' L' U R' U' L` |
| U-42 | **13** | 24 | BFLR | BFLR | `F R' F U' R F R' U F2 R F' U' F` | `F' U F R' F2 U' R F' R' U F' R F'` |
| U-43 | **14** | 36 | BFLR | BFLR | `F2 L' R' U2 R U R2 U' L U F2 R2 U F2 U'` | `(U) F2 U' R2 F2 U' L' U R2 U' R' U2 R L F2` |
| U-44 | **11** | 16 | BFLR | BFLR | `R U2 R2 D' R U' R' D R2 U' R' U2` | `(U2) R U R2 D' R U R' D R2 U2 R'` |
| U-45 | **14** | 60 | BFLR | BFLR | `R U' F' L2 U R' U' L2 U2 R U2 F U2 R' U2` | `(U2) R U2 F' U2 R' U2 L2 U R U' L2 F U R'` |
| U-46 | **14** | 60 | BFLR | BFLR | `R' F U2 F U L' U L F U' F U F R U'` | `(U) R' F' U' F' U F' L' U' L U' F' U2 F' R` |
| U-47 | **11** | 4 | BFLR | BFLR | `L2 D R F2 R D2 L D' R2 D2 L U2` | `(U2) L' D2 R2 D L' D2 R' F2 R' D' L2` |
| U-48 | **13** | 16 | BFLR | BFLR | `F' U2 F R' U2 R U F' R' U R U2 F U` | `(U') F' U2 R' U' R F U' R' U2 R F' U2 F` |
| U-49 | **9** | 4 | BFLR | BFLR | `F R2 F L2 F' R2 F L2 F2 U` | `(U') F2 L2 F' R2 F L2 F' R2 F'` |
| U-50 | **12** | 4 | BFLR | BFLR | `F' R' F U' F' U2 F U' F' U' R F U'` | `(U) F' R' U F U F' U2 F U F' R F` |
| U-51 | **13** | 40 | BFLR | BFLR | `R' F2 R U' R2 F2 R2 U' R' U2 R' F2 R2` | `R2 F2 R U2 R U R2 F2 R2 U R' F2 R` |
| U-52 | **13** | 24 | BFLR | BFLR | `R U2 F R' F R2 F' R' F R' F2 U2 R' U` | `(U') R U2 F2 R F' R F R2 F' R F' U2 R'` |
| U-53 | **12** | 4 | BFLR | BFLR | `R F R' U R U2 R' U R U F' R' U2` | `(U2) R F U' R' U' R U2 R' U' R F' R'` |
| U-54 | **13** | 60 | BFLR | BFLR | `R' U L' U R U' L2 U2 R' U R U2 L' U'` | `(U) L U2 R' U' R U2 L2 U R' U' L U' R` |
| U-55 | **13** | 4 | BFLR | BFLR | `R' F2 D' L' D F2 D2 L U L' D2 U' R U` | `(U') R' U D2 L U' L' D2 F2 D' L D F2 R` |
| U-56 | **13** | 60 | BFLR | BFLR | `R U' L U' R' U L2 U2 R U' R' U2 L U'` | `(U) L' U2 R U R' U2 L2 U' R U L' U R'` |
| U-57 | **9** | 4 | BFLR | BFLR | `F' L2 F' R2 F L2 F' R2 F2 U'` | `(U) F2 R2 F L2 F' R2 F L2 F` |
| U-58 | **13** | 4 | BFLR | BFLR | `L F2 D R D' F2 D2 R' U' R D2 U L' U'` | `(U) L U' D2 R' U R D2 F2 D R' D' F2 L'` |
| U-59 | **13** | 40 | BFLR | BFLR | `F R2 F' U F2 R2 F2 U F U2 F R2 F2 U` | `(U') F2 R2 F' U2 F' U' F2 R2 F2 U' F R2 F'` |
| U-60 | **13** | 40 | BFLR | BFLR | `F R U R' U' R U' R' U' R U R' F' U2` | `(U2) F R U' R' U R U R' U R U' R' F'` |
| U-61 | **11** | 8 | BFLR | BFLR | `R U2 R2 F2 D2 L2 D L2 D F2 R U'` | `(U) R' F2 D' L2 D' L2 D2 F2 R2 U2 R'` |
| U-62 | **10** | 4 | BFLR | BFLR | `R' U F U' F' U' R F U2 F' U` | `(U') F U2 F' R' U F U F' U' R` |
| U-63 | **13** | 112 | BFLR | BFLR | `R' U' R U' L R' U' R U L' R' U2 R U2` | `(U2) R' U2 R L U' R' U R L' U R' U R` |
| U-64 | **13** | 8 | BFLR | BFLR | `R2 L D R' D' R' U' R' U2 F2 U' R L' U'` | `(U) L R' U F2 U2 R U R D R D' L' R2` |
| U-65 | **12** | 56 | BFLR | BFLR | `L' B' R' U R B L F R U' R' F' U'` | `(U) F R U R' F' L' B' R' U' R B L` |
| U-66 | **11** | 8 | BFLR | BFLR | `R' F2 D' L2 D' L2 D2 F2 R2 U2 R' U2` | `(U2) R U2 R2 F2 D2 L2 D L2 D F2 R` |
| U-67 | **13** | 8 | BFLR | BFLR | `R' L U F2 U2 R U R D R D' R2 L'` | `L R2 D R' D' R' U' R' U2 F2 U' L' R` |
| U-68 | **13** | 112 | BFLR | BFLR | `R U2 R' L' U R U' R' L U' R U' R'` | `R U R' U L' R U R' U' L R U2 R'` |
| U-69 | **10** | 4 | BFLR | BFLR | `F U2 F' R' U F U F' U' R U'` | `(U) R' U F U' F' U' R F U2 F'` |
| U-70 | **13** | 96 | BFLR | BFLR | `R U' B2 U' R2 F2 D L2 D' F2 R2 U2 R'` | `R U2 R2 F2 D L2 D' F2 R2 U B2 U R'` |
| U-71 | **10** | 4 | BFLR | BFLR | `R' U2 R F U' R' U' R U F' U'` | `(U) F U' R' U R U F' R' U2 R` |
| U-72 | **10** | 4 | BFLR | BFLR | `F U' R' U R U F' R' U2 R U` | `(U') R' U2 R F U' R' U' R U F'` |
| L-1 | **11** | 8 | BFLR | BFLR | `F' R D2 R' F U2 F' R D2 R' F U2` | `(U2) F' R D2 R' F U2 F' R D2 R' F` |
| L-2 | **13** | 28 | BFLR | BFLR | `F L' R U R' U' L U' R U R' U F' U` | `(U') F U' R U' R' U L' U R U' R' L F'` |
| L-3 | **13** | 12 | BFLR | BFLR | `F U R2 U2 L' R2 U R2 U' L U2 R2 F' U2` | `(U2) F R2 U2 L' U R2 U' R2 L U2 R2 U' F'` |
| L-4 | **13** | 8 | BFLR | BFLR | `F R2 D' F L2 D' B2 D L2 F' D R2 F' U` | `(U') F R2 D' F L2 D' B2 D L2 F' D R2 F'` |
| L-5 | **13** | 28 | BFLR | BFLR | `L' U2 L2 U' R' U F2 R' L' F2 R U' R U` | `(U') R' U R' F2 L R F2 U' R U L2 U2 L` |
| L-6 | **13** | 12 | BFLR | BFLR | `F R2 U2 L' U R2 U' L R2 U2 R2 U' F' U` | `(U') F U R2 U2 R2 L' U R2 U' L U2 R2 F'` |
| L-7 | **13** | 12 | BFLR | BFLR | `F' L2 U2 R U' L2 U R' L2 U2 L2 U F U` | `(U') F' U' L2 U2 L2 R U' L2 U R' U2 L2 F` |
| L-8 | **13** | 12 | BFLR | BFLR | `F' U' L2 U2 L2 R U' L2 U R' U2 L2 F` | `F' L2 U2 R U' L2 U R' L2 U2 L2 U F` |
| L-9 | **13** | 16 | BFLR | BFLR | `L F' D F2 R' F R D' F L' F2 U F' U` | `(U') F U' F2 L F' D R' F' R F2 D' F L'` |
| L-10 | **13** | 28 | BFLR | BFLR | `F U' R U' R' U L' U R U' L R' F' U2` | `(U2) F R L' U R' U' L U' R U R' U F'` |
| L-11 | **13** | 28 | BFLR | BFLR | `R' U R' F2 L R F2 U' R U L2 U2 L` | `L' U2 L2 U' R' U F2 R' L' F2 R U' R` |
| L-12 | **14** | 56 | BFLR | BFLR | `R U R' U R U2 R' L U2 L' U' L U' L' U'` | `(U) L U L' U L U2 L' R U2 R' U' R U' R'` |
| L-13 | **9** | 4 | BFLR | BFLR | `L2 D R' F2 R D' L' U2 L' U2` | `(U2) L U2 L D R' F2 R D' L2` |
| L-14 | **13** | 24 | BDFLR | BDFLR | `R' U' R U' R' U2 R2 U' L' U R' U' L U'` | `(U) L' U R U' L U R2 U2 R U R' U R` |
| L-15 | **13** | 16 | BFLR | BFLR | `F' U2 R' U' R F U' R' U2 R F' U2 F` | `F' U2 F R' U2 R U F' R' U R U2 F` |
| L-16 | **13** | 24 | BFLR | BFLR | `F' U F R' F2 U' R F' R' U F' R F' U'` | `(U) F R' F U' R F R' U F2 R F' U' F` |
| L-17 | **9** | 4 | BFLR | BFLR | `R2 D R' U2 R D' R' U2 R'` | `R U2 R D R' U2 R D' R2` |
| L-18 | **14** | 60 | BFLR | BFLR | `R' F' U' F' U F' L' U' L U' F' U2 F' R U'` | `(U) R' F U2 F U L' U L F U' F U F R` |
| L-19 | **13** | 56 | BDFLR | BDFLR | `F U2 R' U' R F' R' U2 F U F' U' R` | `R' U F U' F' U2 R F R' U R U2 F'` |
| L-20 | **13** | 12 | BFLR | BFLR | `F R U2 F' U2 F2 U2 L F L' U2 R' F2 U` | `(U') F2 R U2 L F' L' U2 F2 U2 F U2 R' F'` |
| L-21 | **11** | 4 | BFLR | BFLR | `L' D2 R2 D L' D2 R' F2 R' D' L2 U'` | `(U) L2 D R F2 R D2 L D' R2 D2 L` |
| L-22 | **14** | 60 | BFLR | BFLR | `R U2 F' U2 R' U2 L2 U R U' L2 F U R'` | `R U' F' L2 U R' U' L2 U2 R U2 F U2 R'` |
| L-23 | **11** | 16 | BFLR | BFLR | `R U R2 D' R U R' D R2 U2 R' U'` | `(U) R U2 R2 D' R U' R' D R2 U' R'` |
| L-24 | **14** | 36 | BFLR | BFLR | `F2 U' R2 F2 U' L' U R2 U' R' U2 R L F2 U2` | `(U2) F2 L' R' U2 R U R2 U' L U F2 R2 U F2` |
| L-25 | **9** | 4 | BFLR | BFLR | `R2 D' L F2 L' D R U2 R U2` | `(U2) R' U2 R' D' L F2 L' D R2` |
| L-26 | **11** | 16 | BFLR | BFLR | `R2 D' R U' R' D R2 U' R' U2 R U'` | `(U) R' U2 R U R2 D' R U R' D R2` |
| L-27 | **13** | 12 | BFLR | BFLR | `F' L' U2 F U2 F2 U2 R' F' R U2 L F2 U'` | `(U) F2 L' U2 R' F R U2 F2 U2 F' U2 L F` |
| L-28 | **13** | 24 | BFLR | BFLR | `R U' R' F R2 U F' R F U' R F' R U2` | `(U2) R' F R' U F' R' F U' R2 F' R U R'` |
| L-29 | **14** | 60 | BFLR | BFLR | `L' U' L2 F' L' F2 R' U2 R2 U R2 U R F' U2` | `(U2) F R' U' R2 U' R2 U2 R F2 L F L2 U L` |
| L-30 | **13** | 56 | BDFLR | BDFLR | `R' U2 F U F' R F U2 R' U' R U F' U` | `(U') F U' R' U R U2 F' R' F U' F' U2 R` |
| L-31 | **14** | 60 | BFLR | BFLR | `L F U F U' F R U R' U F U2 F L' U` | `(U') L F' U2 F' U' R U' R' F' U F' U' F' L'` |
| L-32 | **13** | 16 | BFLR | BFLR | `R U2 F U F' R' U F U2 F' R U2 R' U` | `(U') R U2 R' F U2 F' U' R F U' F' U2 R'` |
| L-33 | **11** | 4 | BFLR | BFLR | `R D2 L2 D' R D2 L F2 L D R2 U` | `(U') R2 D' L' F2 L' D2 R' D L2 D2 R'` |
| L-34 | **9** | 4 | BFLR | BFLR | `R2 D' R U2 R' D R U2 R U2` | `(U2) R' U2 R' D' R U2 R' D R2` |
| L-35 | **13** | 24 | BDFLR | BDFLR | `R U R' U R U2 R2 U L U' R U L' U'` | `(U) L U' R' U L' U' R2 U2 R' U' R U' R'` |
| L-36 | **14** | 36 | BFLR | BFLR | `R U R' U2 F2 R U2 R' U2 R' F2 R2 U R' U'` | `(U) R U' R2 F2 R U2 R U2 R' F2 U2 R U' R'` |
| L-37 | **8** | 8 | BFLR | BFLR | `L F R' F' L' F R F' U'` | `(U) F R' F' L F R F' L'` |
| L-38 | **13** | 40 | BFLR | BFLR | `R F U R2 U' R2 F' U R U R2 U' R2` | `R2 U R2 U' R' U' F R2 U R2 U' F' R'` |
| L-39 | **10** | 12 | BFLR | BFLR | `R' U F U2 F' U' R F U' F' U` | `(U') F U F' R' U F U2 F' U' R` |
| L-40 | **12** | 4 | BFLR | BFLR | `L' U2 R U R' F2 D' L' D F2 U2 L U` | `(U') L' U2 F2 D' L D F2 R U' R' U2 L` |
| L-41 | **14** | 104 | BDFLR | BDFLR | `R F' U2 F R U F R' F' R' U R U2 R2 U2` | `(U2) R2 U2 R' U' R F R F' U' R' F' U2 F R'` |
| L-42 | **12** | 8 | BFLR | BFLR | `F2 U R D2 L' U2 L D2 R' F2 U' F2 U2` | `(U2) F2 U F2 R D2 L' U2 L D2 R' U' F2` |
| L-43 | **10** | 4 | BFLR | BFLR | `L U' R' F2 U' F2 U F2 R L'` | `L R' F2 U' F2 U F2 R U L'` |
| L-44 | **12** | 16 | BFLR | BFLR | `F R2 U' L' U B U B' U' R2 L F'` | `F L' R2 U B U' B' U' L U R2 F'` |
| L-45 | **13** | 8 | BFLR | BFLR | `R' U R F2 R F2 U' F2 U R2 U' R F2 U'` | `(U) F2 R' U R2 U' F2 U F2 R' F2 R' U' R` |
| L-46 | **12** | 12 | BFLR | BFLR | `F U' R2 D' F' D R2 F' U R2 F2 R2` | `R2 F2 R2 U' F R2 D' F D R2 U F'` |
| L-47 | **12** | 24 | BFLR | BFLR | `R U2 R' U2 R' F R U R U' R' F' U'` | `(U) F R U R' U' R' F' R U2 R U2 R'` |
| L-48 | **12** | 8 | BFLR | BFLR | `L F R' F' R F' L R' F' R F L2` | `L2 F' R' F R L' F R' F R F' L'` |
| L-49 | **8** | 8 | BFLR | BFLR | `R' F' L F R F' L' F U` | `(U') F' L F R' F' L' F R` |
| L-50 | **12** | 24 | BFLR | BFLR | `R U R' U' R' F' R U2 R U2 R' F U` | `(U') F' R U2 R' U2 R' F R U R U' R'` |
| L-51 | **12** | 16 | BFLR | BFLR | `R' U' R' D' R U' L R' F2 L' D R2` | `R2 D' L F2 R L' U R' D R U R` |
| L-52 | **12** | 4 | BFLR | BFLR | `R U2 L' U' L F2 D R D' F2 U2 R' U'` | `(U) R U2 F2 D R' D' F2 L' U L U2 R'` |
| L-53 | **12** | 12 | BFLR | BFLR | `R' U F2 D R D' F2 R U' F2 R2 F2 U` | `(U') F2 R2 F2 U R' F2 D R' D' F2 U' R` |
| L-54 | **10** | 4 | BFLR | BFLR | `R' U L F2 U F2 U' F2 R L'` | `L R' F2 U F2 U' F2 L' U' R` |
| L-55 | **12** | 8 | BFLR | BFLR | `F2 U' L' D2 R U2 R' D2 L F2 U F2 U2` | `(U2) F2 U' F2 L' D2 R U2 R' D2 L U F2` |
| L-56 | **10** | 12 | BFLR | BFLR | `F U R U2 R' U R U R' F'` | `F R U' R' U' R U2 R' U' F'` |
| L-57 | **13** | 8 | BFLR | BFLR | `F U' F' R2 F' R2 U R2 U' F2 U F' R2 U2` | `(U2) R2 F U' F2 U R2 U' R2 F R2 F U F'` |
| L-58 | **14** | 104 | BDFLR | BDFLR | `F' R U2 R' F' U' R' F R F U' F' U2 F2 U'` | `(U) F2 U2 F U F' R' F' R U F R U2 R' F` |
| L-59 | **13** | 40 | BFLR | BFLR | `F' R' U' F2 U F2 R U' F' U' F2 U F2 U` | `(U') F2 U' F2 U F U R' F2 U' F2 U R F` |
| L-60 | **12** | 8 | BFLR | BFLR | `R' F' L F L' F L R' F L' F' R2` | `R2 F L F' R L' F' L F' L' F R` |
| L-61 | **11** | 48 | BFLR | BFLR | `F R U R' F R' F' R2 U' R' F'` | `F R U R2 F R F' R U' R' F'` |
| L-62 | **9** | 8 | BFLR | BFLR | `R' F2 L2 D' L' D L' F2 R U2` | `(U2) R' F2 L D' L D L2 F2 R` |
| L-63 | **11** | 8 | BFLR | BFLR | `R' F R2 D2 L B L' D2 R F' R2` | `R2 F R' D2 L B' L' D2 R2 F' R` |
| L-64 | **12** | 16 | BFLR | BFLR | `L2 D F2 R2 D' L' D R' U2 R' D' L' U2` | `(U2) L D R U2 R D' L D R2 F2 D' L2` |
| L-65 | **12** | 8 | BFLR | BFLR | `R B L U2 L' B' R2 F R F2 U2 F` | `F' U2 F2 R' F' R2 B L U2 L' B' R'` |
| L-66 | **13** | 24 | BFLR | BFLR | `B' U2 F B R U' L U' L' U' R' U' F' U` | `(U') F U R U L U L' U R' B' F' U2 B` |
| L-67 | **13** | 24 | BFLR | BFLR | `B U L U R U R' U L' F' B' U2 F` | `F' U2 B F L U' R U' R' U' L' U' B'` |
| L-68 | **11** | 8 | BFLR | BFLR | `L2 F' L D2 R' B R D2 L2 F L' U'` | `(U) L F' L2 D2 R' B' R D2 L' F L2` |
| L-69 | **10** | 8 | BFLR | BFLR | `R' F2 R2 U' L' U R2 L F2 R U` | `(U') R' F2 L' R2 U' L U R2 F2 R` |
| L-70 | **12** | 8 | BFLR | BFLR | `F' L' B' U2 B L F2 R' F' R2 U2 R' U` | `(U') R U2 R2 F R F2 L' B' U2 B L F` |
| L-71 | **9** | 8 | BFLR | BFLR | `L F2 R' D R' D' R2 F2 L' U2` | `(U2) L F2 R2 D R D' R F2 L'` |
| L-72 | **10** | 8 | BFLR | BFLR | `R' F2 L' R2 U' L U R2 F2 R` | `R' F2 R2 U' L' U R2 L F2 R` |
| H-1 | **15** | 112 | BFLR | BFLR | `R F' U R F2 U2 R' U2 F' R U' F' R2 U2 F` | `F' U2 R2 F U R' F U2 R U2 F2 R' U' F R'` |
| H-2 | **11** | 4 | BFLR | BFLR | `R' U' R U' R' U R U' R' U2 R U` | `(U') R' U2 R U R' U' R U R' U R` |
| H-3 | **11** | 4 | BFLR | BFLR | `R U R' U R U' R' U R U2 R' U'` | `(U) R U2 R' U' R U R' U' R U' R'` |
| H-4 | **13** | 32 | BFLR | BFLR | `R U2 R2 U2 R' U2 R U2 R' U2 R2 U2 R U'` | `(U) R' U2 R2 U2 R U2 R' U2 R U2 R2 U2 R'` |
| H-5 | **11** | 4 | BFLR | BFLR | `R U2 R' U' R U R' U' R U' R' U'` | `(U) R U R' U R U' R' U R U2 R'` |
| H-6 | **11** | 4 | BFLR | BFLR | `R' U2 R U R' U' R U R' U R U` | `(U') R' U' R U' R' U R U' R' U2 R` |
| H-7 | **13** | 272 | BFLR | BFLR | `R' U R2 L U' L2 U R2 U' R L2 U L' U'` | `(U) L U' L2 R' U R2 U' L2 U L' R2 U' R` |
| H-8 | **13** | 32 | BFLR | BFLR | `R' U2 R2 U2 R U2 R' U2 R U2 R2 U2 R'` | `R U2 R2 U2 R' U2 R U2 R' U2 R2 U2 R` |
| H-9 | **12** | 8 | BFLR | BFLR | `F U R' U F2 U' F2 U' R F2 U2 F U2` | `(U2) F' U2 F2 R' U F2 U F2 U' R U' F'` |
| H-10 | **13** | 8 | BFLR | BFLR | `R' U2 R U R' F U F' R U F U2 F' U'` | `(U) F U2 F' U' R' F U' F' R U' R' U2 R` |
| H-11 | **14** | 152 | BFLR | BFLR | `F U R U' R' U R U2 R' U' R U R' F'` | `F R U' R' U R U2 R' U' R U R' U' F'` |
| H-12 | **13** | 8 | BFLR | BFLR | `F U2 F' U' F R' U' R F' U' R' U2 R U2` | `(U2) R' U2 R U F R' U R F' U F U2 F'` |
| H-13 | **12** | 4 | BFLR | BFLR | `R2 F2 L F L' F R2 U2 B' R B R' U'` | `(U) R B' R' B U2 R2 F' L F' L' F2 R2` |
| H-14 | **11** | 12 | BFLR | BFLR | `R' F2 U F2 U' F2 U' R L' U2 L U` | `(U') L' U2 L R' U F2 U F2 U' F2 R` |
| H-15 | **12** | 4 | BFLR | BFLR | `F' R' U' F2 U2 F2 U' F2 U' F2 R F U'` | `(U) F' R' F2 U F2 U F2 U2 F2 U R F` |
| H-16 | **14** | 16 | BFLR | BFLR | `F2 D R D' F2 L' D' U2 R' U R D U' L U` | `(U') L' U D' R' U' R U2 D L F2 D R' D' F2` |
| H-17 | **11** | 12 | BFLR | BFLR | `L F2 U' F2 U F2 U L' R U2 R' U'` | `(U) R U2 R' L U' F2 U' F2 U F2 L'` |
| H-18 | **12** | 4 | BFLR | BFLR | `F2 R2 B' R' B R' F2 U2 L F' L' F U2` | `(U2) F' L F L' U2 F2 R B' R B R2 F2` |
| H-19 | **12** | 8 | BFLR | BFLR | `R' U' F U' R2 U R2 U F' R2 U2 R' U'` | `(U) R U2 R2 F U' R2 U' R2 U F' U R` |
| H-20 | **12** | 4 | BFLR | BFLR | `R F U R2 U2 R2 U R2 U R2 F' R' U2` | `(U2) R F R2 U' R2 U' R2 U2 R2 U' F' R'` |
| H-21 | **13** | 8 | BFLR | BFLR | `R2 U' R2 F U R2 U' R2 F' U' R2 U R2 U2` | `(U2) R2 U' R2 U F R2 U R2 U' F' R2 U R2` |
| H-22 | **13** | 8 | BFLR | BFLR | `F2 U F2 R' U' F2 U F2 R U F2 U' F2 U'` | `(U) F2 U F2 U' R' F2 U' F2 U R F2 U' F2` |
| H-23 | **13** | 8 | BFLR | BFLR | `R U2 R2 U' R2 U' R D' L F2 L' D R2 U` | `(U') R2 D' L F2 L' D R' U R2 U R2 U2 R'` |
| H-24 | **13** | 12 | BFLR | BFLR | `R' U2 R2 U R2 U R' D R' U2 R D' R2 U` | `(U') R2 D R' U2 R D' R U' R2 U' R2 U2 R` |
| H-25 | **13** | 16 | BFLR | BFLR | `R' U' R F2 R D' F2 D F2 R2 U R F2` | `F2 R' U' R2 F2 D' F2 D R' F2 R' U R` |
| H-26 | **11** | 4 | BFLR | BFLR | `L' U R U' L U' R' U' R U' R' U'` | `(U) R U R' U R U L' U R' U' L` |
| H-27 | **12** | 4 | BFLR | BFLR | `R U' R' U2 R L U' R2 U L' U' R` | `R' U L U' R2 U L' R' U2 R U R'` |
| H-28 | **13** | 24 | BFLR | BFLR | `F' R2 F L F' R2 L' U2 R' F' R U2 F` | `F' U2 R' F R U2 L R2 F L' F' R2 F` |
| H-29 | **12** | 4 | BFLR | BFLR | `R' U R U2 R' L' U R2 U' L U R' U2` | `(U2) R U' L' U R2 U' L R U2 R' U' R` |
| H-30 | **13** | 16 | BFLR | BFLR | `F U F' R2 F' D R2 D' R2 F2 U' F' R2 U` | `(U') R2 F U F2 R2 D R2 D' F R2 F U' F'` |
| H-31 | **13** | 12 | BFLR | BFLR | `R' U2 R' D R' U R D' R U R2 U2 R'` | `R U2 R2 U' R' D R' U' R D' R U2 R` |
| H-32 | **11** | 4 | BFLR | BFLR | `L U' R' U L' U R U R' U R U'` | `(U) R' U' R U' R' U' L U' R U L'` |
| H-33 | **13** | 8 | BFLR | BFLR | `R U R' U F' U F U' F2 L F L' F U'` | `(U) F' L F' L' F2 U F' U' F U' R U' R'` |
| H-34 | **14** | 184 | BDFLR | BDFLR | `R U' R2 F2 U' R2 U' R2 U F2 U R2 U R' U` | `(U') R U' R2 U' F2 U' R2 U R2 U F2 R2 U R'` |
| H-35 | **14** | 32 | BFLR | BFLR | `R U2 R' F L' U L2 F L2 U' L F2 U F U'` | `(U) F' U' F2 L' U L2 F' L2 U' L F' R U2 R'` |
| H-36 | **11** | 64 | BFLR | BFLR | `R' F2 R2 U2 R' F2 R U2 R2 F2 R U'` | `(U) R' F2 R2 U2 R' F2 R U2 R2 F2 R` |
| H-37 | **13** | 8 | BFLR | BFLR | `L' U' L U' F U' F' U F2 R' F' R F'` | `F R' F R F2 U' F U F' U L' U L` |
| H-38 | **14** | 32 | BFLR | BFLR | `L' U2 L F' R U' R2 F' R2 U R' F2 U' F' U'` | `(U) F U F2 R U' R2 F R2 U R' F L' U2 L` |
| H-39 | **14** | 184 | BDFLR | BDFLR | `R U' R2 U' F2 U' R2 U R2 U F2 R2 U R' U2` | `(U2) R U' R2 F2 U' R2 U' R2 U F2 U R2 U R'` |
| H-40 | **11** | 16 | BFLR | BFLR | `F2 R D2 R' F2 U2 F2 L B2 L' F2` | `F2 L B2 L' F2 U2 F2 R D2 R' F2` |
| Pi-1 | **14** | 32 | BFLR | BFLR | `L2 F2 L2 U' R' L D R2 U2 R D' R2 U2 L' U` | `(U') L U2 R2 D R' U2 R2 D' L' R U L2 F2 L2` |
| Pi-2 | **13** | 24 | BFLR | BFLR | `F2 R2 F2 R' U2 R2 U' L' U2 R2 U' L R' U'` | `(U) R L' U R2 U2 L U R2 U2 R F2 R2 F2` |
| Pi-3 | **13** | 24 | BFLR | BFLR | `F2 L2 F2 L U2 L2 U R U2 L2 U L R' U'` | `(U) R L' U' L2 U2 R' U' L2 U2 L' F2 L2 F2` |
| Pi-4 | **12** | 4 | BFLR | BFLR | `B' U' R' U R B F' L' U' L U F` | `F' U' L' U L F B' R' U' R U B` |
| Pi-5 | **9** | 4 | BFLR | BFLR | `R' U2 R2 U R2 U R2 U2 R' U'` | `(U) R U2 R2 U' R2 U' R2 U2 R` |
| Pi-6 | **13** | 24 | BFLR | BFLR | `R L' U' L2 U2 R' U' L2 U2 L' F2 L2 F2 U` | `(U') F2 L2 F2 L U2 L2 U R U2 L2 U L R'` |
| Pi-7 | **11** | 4 | BFLR | BFLR | `F U R U' R' F2 L' U' L U F U'` | `(U) F' U' L' U L F2 R U R' U' F'` |
| Pi-8 | **11** | 4 | BFLR | BFLR | `F' U' L' U L F2 R U R' U' F' U'` | `(U) F U R U' R' F2 L' U' L U F` |
| Pi-9 | **11** | 16 | BFLR | BFLR | `F' R D2 R' F U' F' R D2 R' F U` | `(U') F' R D2 R' F U F' R D2 R' F` |
| Pi-10 | **13** | 24 | BFLR | BFLR | `R L' U R2 U2 L U R2 U2 R F2 R2 F2 U` | `(U') F2 R2 F2 R' U2 R2 U' L' U2 R2 U' L R'` |
| Pi-11 | **9** | 4 | BFLR | BFLR | `R U2 R2 U' R2 U' R2 U2 R U` | `(U') R' U2 R2 U R2 U R2 U2 R'` |
| Pi-12 | **12** | 4 | BFLR | BFLR | `B U L U' L' B' F R U R' U' F' U` | `(U') F U R U' R' F' B L U L' U' B'` |
| Pi-13 | **13** | 4 | BFLR | BFLR | `R' U' R' F2 R2 U R' F2 R U' R2 F2 R2 U` | `(U') R2 F2 R2 U R' F2 R U' R2 F2 R U R` |
| Pi-14 | **13** | 76 | BFLR | BFLR | `R U2 R F2 R2 U' R U' R' U R2 F2 R2 U'` | `(U) R2 F2 R2 U' R U R' U R2 F2 R' U2 R'` |
| Pi-15 | **13** | 8 | BFLR | BFLR | `L' U' L' F2 R2 D R' B2 R D' R2 F2 L2 U'` | `(U) L2 F2 R2 D R' B2 R D' R2 F2 L U L` |
| Pi-16 | **13** | 76 | BFLR | BFLR | `R2 F2 R2 U2 R U2 R F2 R2 U' R U' R' U'` | `(U) R U R' U R2 F2 R' U2 R' U2 R2 F2 R2` |
| Pi-17 | **13** | 8 | BFLR | BFLR | `R2 D R2 D' L F2 D' F2 D L' F2 U F2` | `F2 U' F2 L D' F2 D F2 L' D R2 D' R2` |
| Pi-18 | **12** | 12 | BFLR | BFLR | `L' U' L U' F2 R' F2 R U2 R U2 R'` | `R U2 R' U2 R' F2 R F2 U L' U L` |
| Pi-19 | **12** | 4 | BFLR | BFLR | `R' F2 L2 D' L2 U' L D L' U F2 R U2` | `(U2) R' F2 U' L D' L' U L2 D L2 F2 R` |
| Pi-20 | **13** | 8 | BFLR | BFLR | `R B2 R' U R2 F2 L' D' L' D2 L2 F2 R2 U2` | `(U2) R2 F2 L2 D2 L D L F2 R2 U' R B2 R'` |
| Pi-21 | **12** | 4 | BFLR | BFLR | `L F2 U R' D R U' R2 D' R2 F2 L' U'` | `(U) L F2 R2 D R2 U R' D' R U' F2 L'` |
| Pi-22 | **12** | 32 | BFLR | BFLR | `F R' F2 L F' L' F' R F' R U2 R'` | `R U2 R' F R' F L F L' F2 R F'` |
| Pi-23 | **13** | 4 | BFLR | BFLR | `F2 R2 F2 U' F R2 F' U F2 R2 F' U' F' U2` | `(U2) F U F R2 F2 U' F R2 F' U F2 R2 F2` |
| Pi-24 | **12** | 12 | BFLR | BFLR | `R' F2 R U2 R U2 R' F2 U' R U' R' U'` | `(U) R U R' U F2 R U2 R' U2 R' F2 R` |
| Pi-25 | **12** | 8 | BFLR | BFLR | `R U2 R2 F U' R2 U' R2 U F' U R` | `R' U' F U' R2 U R2 U F' R2 U2 R'` |
| Pi-26 | **12** | 8 | BFLR | BFLR | `F' U2 F2 R' U F2 U F2 U' R U' F' U` | `(U') F U R' U F2 U' F2 U' R F2 U2 F` |
| Pi-27 | **14** | 16 | BFLR | BFLR | `L' D' U R' U' R D U2 L F2 D R' D' F2 U` | `(U') F2 D R D' F2 L' U2 D' R' U R U' D L` |
| Pi-28 | **13** | 8 | BFLR | BFLR | `F U2 F' U' R' F U' F' R U' R' U2 R U'` | `(U) R' U2 R U R' F U F' R U F U2 F'` |
| Pi-29 | **12** | 4 | BFLR | BFLR | `F' L F L' U2 F2 R B' R B R2 F2 U'` | `(U) F2 R2 B' R' B R' F2 U2 L F' L' F` |
| Pi-30 | **11** | 12 | BFLR | BFLR | `R U2 R' L U' F2 U' F2 U F2 L' U` | `(U') L F2 U' F2 U F2 U L' R U2 R'` |
| Pi-31 | **12** | 4 | BFLR | BFLR | `R F R2 U' R2 U' R2 U2 R2 U' F' R'` | `R F U R2 U2 R2 U R2 U R2 F' R'` |
| Pi-32 | **14** | 152 | BFLR | BFLR | `F R U' R' U R U2 R' U' R U R' U' F' U2` | `(U2) F U R U' R' U R U2 R' U' R U R' F'` |
| Pi-33 | **12** | 4 | BFLR | BFLR | `F' R' F2 U F2 U F2 U2 F2 U R F U` | `(U') F' R' U' F2 U2 F2 U' F2 U' F2 R F` |
| Pi-34 | **12** | 4 | BFLR | BFLR | `R B' R' B U2 R2 F' L F' L' F2 R2 U2` | `(U2) R2 F2 L F L' F R2 U2 B' R B R'` |
| Pi-35 | **13** | 8 | BFLR | BFLR | `R' U2 R U F R' U R F' U F U2 F' U2` | `(U2) F U2 F' U' F R' U' R F' U' R' U2 R` |
| Pi-36 | **11** | 12 | BFLR | BFLR | `L' U2 L R' U F2 U F2 U' F2 R U'` | `(U) R' F2 U F2 U' F2 U' R L' U2 L` |
| Pi-37 | **13** | 16 | BFLR | BFLR | `F2 R' U' R2 F2 D' F2 D R' F2 R' U R U` | `(U') R' U' R F2 R D' F2 D F2 R2 U R F2` |
| Pi-38 | **13** | 8 | BFLR | BFLR | `F2 U F2 U' R' F2 U' F2 U R F2 U' F2 U` | `(U') F2 U F2 R' U' F2 U F2 R U F2 U' F2` |
| Pi-39 | **11** | 4 | BFLR | BFLR | `R' U' R U' R' U' L U' R U L' U'` | `(U) L U' R' U L' U R U R' U R` |
| Pi-40 | **13** | 8 | BFLR | BFLR | `R2 D' L F2 L' D R' U R2 U R2 U2 R'` | `R U2 R2 U' R2 U' R D' L F2 L' D R2` |
| Pi-41 | **13** | 12 | BFLR | BFLR | `R' U2 R2 U R D' R U R' D R' U2 R' U` | `(U') R U2 R D' R U' R' D R' U' R2 U2 R` |
| Pi-42 | **13** | 12 | BFLR | BFLR | `R2 D' R U2 R' D R' U R2 U R2 U2 R'` | `R U2 R2 U' R2 U' R D' R U2 R' D R2` |
| Pi-43 | **11** | 4 | BFLR | BFLR | `R U R' U R U L' U R' U' L U'` | `(U) L' U R U' L U' R' U' R U' R'` |
| Pi-44 | **13** | 8 | BFLR | BFLR | `R2 U' R2 U F R2 U R2 U' F' R2 U R2` | `R2 U' R2 F U R2 U' R2 F' U' R2 U R2` |
| Pi-45 | **13** | 16 | BFLR | BFLR | `R2 F U F2 R2 D R2 D' F R2 F U' F'` | `F U F' R2 F' D R2 D' R2 F2 U' F' R2` |
| Pi-46 | **12** | 4 | BFLR | BFLR | `R U' L' U R2 U' L R U2 R' U' R U'` | `(U) R' U R U2 R' L' U R2 U' L U R'` |
| Pi-47 | **12** | 4 | BFLR | BFLR | `R' U L U' R2 U R' L' U2 R U R' U'` | `(U) R U' R' U2 L R U' R2 U L' U' R` |
| Pi-48 | **13** | 24 | BFLR | BFLR | `F' U2 R' F R U2 R2 L F L' F' R2 F` | `F' R2 F L F' L' R2 U2 R' F' R U2 F` |
| Pi-49 | **12** | 32 | BFLR | BFLR | `R U2 R' F R' F L F L' F2 R F' U` | `(U') F R' F2 L F' L' F' R F' R U2 R'` |
| Pi-50 | **12** | 12 | BFLR | BFLR | `R U2 R' U2 R' F2 R F2 U L' U L U` | `(U') L' U' L U' F2 R' F2 R U2 R U2 R'` |
| Pi-51 | **13** | 4 | BFLR | BFLR | `R2 F2 R2 U R' F2 R U' R2 F2 R U R U'` | `(U) R' U' R' F2 R2 U R' F2 R U' R2 F2 R2` |
| Pi-52 | **13** | 8 | BFLR | BFLR | `R2 F2 L2 D2 L D L F2 R2 U' R B2 R' U'` | `(U) R B2 R' U R2 F2 L' D' L' D2 L2 F2 R2` |
| Pi-53 | **12** | 4 | BFLR | BFLR | `L F2 R2 D R2 U R' D' R U' F2 L' U2` | `(U2) L F2 U R' D R U' R2 D' R2 F2 L'` |
| Pi-54 | **12** | 4 | BFLR | BFLR | `R' F2 U' L D' L' U L2 D L2 F2 R U` | `(U') R' F2 L2 D' L2 U' L D L' U F2 R` |
| Pi-55 | **13** | 76 | BFLR | BFLR | `R2 F2 R2 U' R U R' U R2 F2 R' U2 R' U` | `(U') R U2 R F2 R2 U' R U' R' U R2 F2 R2` |
| Pi-56 | **12** | 12 | BFLR | BFLR | `R U R' U F2 R U2 R' U2 R' F2 R` | `R' F2 R U2 R U2 R' F2 U' R U' R'` |
| Pi-57 | **13** | 8 | BFLR | BFLR | `L2 D' L2 D R' F2 D F2 D' R F2 U' F2` | `F2 U F2 R' D F2 D' F2 R D' L2 D L2` |
| Pi-58 | **13** | 76 | BFLR | BFLR | `R U R' U R2 F2 R' U2 R' U2 R2 F2 R2 U2` | `(U2) R2 F2 R2 U2 R U2 R F2 R2 U' R U' R'` |
| Pi-59 | **13** | 4 | BFLR | BFLR | `F U F R2 F2 U' F R2 F' U F2 R2 F2` | `F2 R2 F2 U' F R2 F' U F2 R2 F' U' F'` |
| Pi-60 | **13** | 8 | BFLR | BFLR | `L2 F2 R2 D R' B2 R D' R2 F2 L U L U` | `(U') L' U' L' F2 R2 D R' B2 R D' R2 F2 L2` |
| Pi-61 | **13** | 24 | BFLR | BFLR | `F R2 U' R U2 R U R' U R' U R2 F' U2` | `(U2) F R2 U' R U' R U' R' U2 R' U R2 F'` |
| Pi-62 | **12** | 4 | BFLR | BFLR | `R F' U' R2 U' F U F' R2 U F R' U2` | `(U2) R F' U' R2 F U' F' U R2 U F R'` |
| Pi-63 | **14** | 16 | BFLR | BFLR | `R' L U' R U' R' U' L' U' R U' L U' L' U'` | `(U) L U L' U R' U L U R U R' U L' R` |
| Pi-64 | **14** | 24 | BFLR | BFLR | `R' U2 R2 U R2 U L' R U R U' L U2 R' U` | `(U') R U2 L' U R' U' R' L U' R2 U' R2 U2 R` |
| Pi-65 | **13** | 48 | BFLR | BFLR | `F R' F' L F2 R F L' F U2 F U2 F' U2` | `(U2) F U2 F' U2 F' L F' R' F2 L' F R F'` |
| Pi-66 | **14** | 24 | BFLR | BFLR | `R U2 R2 U' R2 U' R' L U' R' U L' U2 R` | `R' U2 L U' R U L' R U R2 U R2 U2 R'` |
| Pi-67 | **13** | 24 | BFLR | BFLR | `F R2 U' R U' R U' R' U2 R' U R2 F' U` | `(U') F R2 U' R U2 R U R' U R' U R2 F'` |
| Pi-68 | **14** | 16 | BFLR | BFLR | `R' U' R U' L U' R' U' L' U' L U' R L' U'` | `(U) L R' U L' U L U R U L' U R' U R` |
| Pi-69 | **12** | 4 | BFLR | BFLR | `F' R U F2 U R' U' R F2 U' R' F` | `F' R U F2 R' U R U' F2 U' R' F` |
| Pi-70 | **13** | 32 | BFLR | BFLR | `L' U' L F R' U2 R2 U R2 U R U' F' U'` | `(U) F U R' U' R2 U' R2 U2 R F' L' U L` |
| Pi-71 | **12** | 4 | BFLR | BFLR | `F' R U F2 R' U R U' F2 U' R' F U` | `(U') F' R U F2 U R' U' R F2 U' R' F` |
| Pi-72 | **12** | 4 | BFLR | BFLR | `R F' U' R2 F U' F' U R2 U F R' U` | `(U') R F' U' R2 U' F U F' R2 U F R'` |
| S-1 | **12** | 8 | BFLR | BFLR | `R' U L D' U' F2 D R2 U2 L' U R' U` | `(U') R U' L U2 R2 D' F2 U D L' U' R` |
| S-2 | **7** | 4 | BFLR | BFLR | `R' U' R U' R' U2 R` | `R' U2 R U R' U R` |
| S-3 | **11** | 12 | BFLR | BFLR | `R U2 R2 U2 R2 U R2 U R2 U' R' U'` | `(U) R U R2 U' R2 U' R2 U2 R2 U2 R'` |
| S-4 | **13** | 64 | BFLR | BFLR | `R2 U R U R2 U' R' U' R2 U' R U' R' U` | `(U') R U R' U R2 U R U R2 U' R' U' R2` |
| S-5 | **10** | 4 | BFLR | BFLR | `R' U L' U' L2 D F2 D' L' R U` | `(U') R' L D F2 D' L2 U L U' R` |
| S-6 | **10** | 4 | BFLR | BFLR | `L R' D' F2 D R2 U' R' U L' U` | `(U') L U' R U R2 D' F2 D R L'` |
| S-7 | **12** | 8 | BFLR | BFLR | `L' U' L F2 R U R' U' F2 L' U L U'` | `(U) L' U' L F2 U R U' R' F2 L' U L` |
| S-8 | **7** | 4 | BFLR | BFLR | `R U2 R' U' R U' R' U2` | `(U2) R U R' U R U2 R'` |
| S-9 | **11** | 8 | BFLR | BFLR | `R U R' U R' U' R2 U' R2 U2 R U2` | `(U2) R' U2 R2 U R2 U R U' R U' R'` |
| S-10 | **12** | 8 | BFLR | BFLR | `R U R' F2 U' L' U L F2 R U' R' U` | `(U') R U R' F2 L' U' L U F2 R U' R'` |
| S-11 | **11** | 12 | BFLR | BFLR | `R' U' R2 U R2 U R2 U2 R2 U2 R U2` | `(U2) R' U2 R2 U2 R2 U' R2 U' R2 U R` |
| S-12 | **12** | 4 | BFLR | BFLR | `F R U R' U' F' R' U' F' U F R` | `R' F' U' F U R F U R U' R' F'` |
| S-13 | **11** | 8 | BFLR | BFLR | `R' L F2 U F2 U' F2 U' R U L'` | `L U' R' U F2 U F2 U' F2 L' R` |
| S-14 | **13** | 4 | BFLR | BFLR | `R' U2 R' D' R U R' D R2 U' R' U2 R U2` | `(U2) R' U2 R U R2 D' R U' R' D R U2 R` |
| S-15 | **11** | 4 | BFLR | BFLR | `R' U' R U' R2 D' L F2 L' D R2 U` | `(U') R2 D' L F2 L' D R2 U R' U R` |
| S-16 | **10** | 8 | BFLR | BFLR | `R U2 R' U2 L' U R U' R' L U'` | `(U) L' R U R' U' L U2 R U2 R'` |
| S-17 | **13** | 28 | BFLR | BFLR | `F U R' U' R F' R' U2 L U' R U L'` | `L U' R' U L' U2 R F R' U R U' F'` |
| S-18 | **13** | 16 | BFLR | BFLR | `R' L' U2 R U R' U L2 D F2 D' R L'` | `L R' D F2 D' L2 U' R U' R' U2 L R` |
| S-19 | **11** | 4 | BFLR | BFLR | `R' F U F2 U F2 U2 F2 U F R U'` | `(U) R' F' U' F2 U2 F2 U' F2 U' F' R` |
| S-20 | **14** | 48 | BFLR | BFLR | `R2 U R U2 R2 U L R' U' R2 U L' U R` | `R' U' L U' R2 U R L' U' R2 U2 R' U' R2` |
| S-21 | **12** | 4 | BFLR | BFLR | `R L U2 R2 D' F2 D R2 U L' U R' U2` | `(U2) R U' L U' R2 D' F2 D R2 U2 L' R'` |
| S-22 | **13** | 16 | BFLR | BFLR | `R2 D' R U2 R' D R U' L U' R U L' U2` | `(U2) L U' R' U L' U R' D' R U2 R' D R2` |
| S-23 | **12** | 16 | BFLR | BFLR | `F2 U F2 L' U R U R' U L U' F2 U2` | `(U2) F2 U L' U' R U' R' U' L F2 U' F2` |
| S-24 | **11** | 4 | BFLR | BFLR | `R' U' R U' R2 D' R U2 R' D R2 U` | `(U') R2 D' R U2 R' D R2 U R' U R` |
| S-25 | **11** | 8 | BFLR | BFLR | `R' U L U' F2 U' F2 U F2 R L' U` | `(U') L R' F2 U' F2 U F2 U L' U' R` |
| S-26 | **12** | 16 | BFLR | BFLR | `F2 U' R U L' U L U R' F2 U F2` | `F2 U' F2 R U' L' U' L U' R' U F2` |
| S-27 | **14** | 48 | BFLR | BFLR | `R U L' U R2 U' L R' U R2 U2 R U R2 U` | `(U') R2 U' R' U2 R2 U' R L' U R2 U' L U' R'` |
| S-28 | **10** | 8 | BFLR | BFLR | `F U R' U' R F' U' R' U2 R U2` | `(U2) R' U2 R U F R' U R U' F'` |
| S-29 | **13** | 28 | BFLR | BFLR | `R' U' R F R' U2 L U' R U L' U2 F' U` | `(U') F U2 L U' R' U L' U2 R F' R' U R` |
| S-30 | **13** | 16 | BFLR | BFLR | `R' L D' F2 D R2 U L' U L U2 R' L' U` | `(U') L R U2 L' U' L U' R2 D' F2 D L' R` |
| S-31 | **11** | 4 | BFLR | BFLR | `F R U R2 U2 R2 U R2 U R F'` | `F R' U' R2 U' R2 U2 R2 U' R' F'` |
| S-32 | **11** | 4 | BFLR | BFLR | `L2 D R' F2 R D' L2 U' L U' L'` | `L U L' U L2 D R' F2 R D' L2` |
| S-33 | **11** | 4 | BFLR | BFLR | `R2 D R' U2 R D' R2 U' R U' R' U2` | `(U2) R U R' U R2 D R' U2 R D' R2` |
| S-34 | **13** | 16 | BFLR | BFLR | `L' U R U' L U' R D R' U2 R D' R2` | `R2 D R' U2 R D' R' U L' U R' U' L` |
| S-35 | **13** | 4 | BFLR | BFLR | `R U2 R' U' R2 D R' U R D' R' U2 R' U'` | `(U) R U2 R D R' U' R D' R2 U R U2 R'` |
| S-36 | **12** | 4 | BFLR | BFLR | `L' U R' U L2 D F2 D' L2 U2 L R U` | `(U') R' L' U2 L2 D F2 D' L2 U' R U' L` |
| S-37 | **10** | 4 | BFLR | BFLR | `R' F U2 F' R F R' U2 R F' U'` | `(U) F R' U2 R F' R' F U2 F' R` |
| S-38 | **11** | 4 | BFLR | BFLR | `R U2 R F2 D L' B2 L D' F2 R2 U` | `(U') R2 F2 D L' B2 L D' F2 R' U2 R'` |
| S-39 | **12** | 12 | BFLR | BFLR | `L' R' U2 R U R' U2 L2 U' R U L'` | `L U' R' U L2 U2 R U' R' U2 R L` |
| S-40 | **13** | 8 | BFLR | BFLR | `L F' L2 R2 D2 R' B' R D2 L' R2 F L2` | `L2 F' R2 L D2 R' B R D2 R2 L2 F L'` |
| S-41 | **12** | 12 | BFLR | BFLR | `F U R2 U F2 U' F2 U' R2 F2 U2 F U` | `(U') F' U2 F2 R2 U F2 U F2 U' R2 U' F'` |
| S-42 | **12** | 12 | BFLR | BFLR | `R U2 R2 F2 U' R2 U' R2 U F2 U R U` | `(U') R' U' F2 U' R2 U R2 U F2 R2 U2 R'` |
| S-43 | **12** | 4 | BFLR | BFLR | `F' D F2 U R2 U R2 F' U2 F2 D' F2 U'` | `(U) F2 D F2 U2 F R2 U' R2 U' F2 D' F` |
| S-44 | **11** | 4 | BFLR | BFLR | `L2 F2 D' R B2 R' D F2 L U2 L` | `L' U2 L' F2 D' R B2 R' D F2 L2` |
| S-45 | **12** | 28 | BFLR | BFLR | `R' F U' F' U' R F U' R' U' R F' U'` | `(U) F R' U R U F' R' U F U F' R` |
| S-46 | **12** | 4 | BFLR | BFLR | `R2 D' R2 U2 R' F2 U F2 U R2 D R' U` | `(U') R D' R2 U' F2 U' F2 R U2 R2 D R2` |
| S-47 | **12** | 12 | BFLR | BFLR | `L' U R U' L2 U2 R' U R U2 L' R'` | `R L U2 R' U' R U2 L2 U R' U' L` |
| S-48 | **13** | 8 | BFLR | BFLR | `F2 R2 U R' D U' R' D' F2 U F2 R' F2` | `F2 R F2 U' F2 D R U D' R U' R2 F2` |
| S-49 | **7** | 4 | BFLR | BFLR | `R' U L U' R U L' U'` | `(U) L U' R' U L' U' R` |
| S-50 | **12** | 4 | BFLR | BFLR | `R2 D' F2 L D R D' L2 U2 L D R` | `R' D' L' U2 L2 D R' D' L' F2 D R2` |
| S-51 | **12** | 4 | BFLR | BFLR | `F' U R2 D2 L2 F' D2 R2 U F U B' U2` | `(U2) B U' F' U' R2 D2 F L2 D2 R2 U' F` |
| S-52 | **13** | 8 | BFLR | BFLR | `F2 D R' D' U R' U' R2 F2 U' F2 R' F2 U'` | `(U) F2 R F2 U F2 R2 U R U' D R D' F2` |
| S-53 | **13** | 24 | BFLR | BFLR | `F U2 F' R' U F U F' L U' R U L' U` | `(U') L U' R' U L' F U' F' U' R F U2 F'` |
| S-54 | **13** | 24 | BFLR | BFLR | `R' U L U' R F' U F U L' F' U2 F U2` | `(U2) F' U2 F L U' F' U' F R' U L' U' R` |
| S-55 | **12** | 8 | BFLR | BFLR | `R' U2 R' F' R U R U' R' F U2 R U2` | `(U2) R' U2 F' R U R' U' R' F R U2 R` |
| S-56 | **12** | 4 | BFLR | BFLR | `L D R U2 R2 D' L D R F2 D' L2 U'` | `(U) L2 D F2 R' D' L' D R2 U2 R' D' L'` |
| S-57 | **13** | 12 | BFLR | BFLR | `F2 R2 F2 U' L' U R U R2 U2 L U R' U` | `(U') R U' L' U2 R2 U' R' U' L U F2 R2 F2` |
| S-58 | **12** | 8 | BFLR | BFLR | `F U2 F' R' U' R F R' U R U2 F' U2` | `(U2) F U2 R' U' R F' R' U R F U2 F'` |
| S-59 | **12** | 4 | BFLR | BFLR | `L' U R U F2 D2 R' B2 D2 F2 U R' U` | `(U') R U' F2 D2 B2 R D2 F2 U' R' U' L` |
| S-60 | **10** | 8 | BFLR | BFLR | `L' U F2 D2 R' B2 D' R D' F2` | `F2 D R' D B2 R D2 F2 U' L` |
| S-61 | **13** | 24 | BFLR | BFLR | `R2 D R F2 L D' L' F2 D2 F2 D F2 R U2` | `(U2) R' F2 D' F2 D2 F2 L D L' F2 R' D' R2` |
| S-62 | **11** | 8 | BFLR | BFLR | `R' U' R U' L U' R' U L' U2 R U'` | `(U) R' U2 L U' R U L' U R' U R` |
| S-63 | **13** | 4 | BFLR | BFLR | `R2 U' R U' R U R U2 L D' F2 D L' U'` | `(U) L D' F2 D L' U2 R' U' R' U R' U R2` |
| S-64 | **13** | 40 | BFLR | BFLR | `F R U' R2 U' R U' R' U2 R2 U R' F' U` | `(U') F R U' R2 U2 R U R' U R2 U R' F'` |
| S-65 | **13** | 16 | BFLR | BFLR | `L' U2 F R' F' R U2 L F U2 F2 U2 F U2` | `(U2) F' U2 F2 U2 F' L' U2 R' F R F' U2 L` |
| S-66 | **14** | 96 | BFLR | BFLR | `F R U R' U' R U R' F R' F' R U' F' U` | `(U') F U R' F R F' R U' R' U R U' R' F'` |
| S-67 | **13** | 24 | BFLR | BFLR | `R U' R2 D' U' R U' R' D U2 R2 U R' U` | `(U') R U' R2 U2 D' R U R' U D R2 U R'` |
| S-68 | **13** | 4 | BFLR | BFLR | `R' D F2 D' R U2 L U L U' L U' L2 U` | `(U') L2 U L' U L' U' L' U2 R' D F2 D' R` |
| S-69 | **13** | 16 | BFLR | BFLR | `R U2 R2 F2 R F2 R U2 R' U L' U L U` | `(U') L' U' L U' R U2 R' F2 R' F2 R2 U2 R'` |
| S-70 | **12** | 8 | BFLR | BFLR | `L R B R' F R F' B' L' F R' F' U` | `(U') F R F' L B F R' F' R B' R' L'` |
| S-71 | **11** | 8 | BFLR | BFLR | `R U2 L' U R' U' L U' R U' R' U'` | `(U) R U R' U L' U R U' L U2 R'` |
| S-72 | **13** | 16 | BFLR | BFLR | `R U R' U L' U2 L F2 R' F2 R2 U2 R' U'` | `(U) R U2 R2 F2 R F2 L' U2 L U' R U' R'` |
| AS-1 | **12** | 8 | BFLR | BFLR | `R U' L U2 R2 D' F2 D U L' U' R` | `R' U L U' D' F2 D R2 U2 L' U R'` |
| AS-2 | **11** | 12 | BFLR | BFLR | `R U R2 U' R2 U' R2 U2 R2 U2 R' U2` | `(U2) R U2 R2 U2 R2 U R2 U R2 U' R'` |
| AS-3 | **7** | 4 | BFLR | BFLR | `R' U2 R U R' U R U2` | `(U2) R' U' R U' R' U2 R` |
| AS-4 | **13** | 64 | BFLR | BFLR | `R U R' U R2 U R U R2 U' R' U' R2 U'` | `(U) R2 U R U R2 U' R' U' R2 U' R U' R'` |
| AS-5 | **12** | 8 | BFLR | BFLR | `L' U' L F2 U R U' R' F2 L' U L U` | `(U') L' U' L F2 R U R' U' F2 L' U L` |
| AS-6 | **12** | 8 | BFLR | BFLR | `R U R' F2 L' U' L U F2 R U' R' U'` | `(U) R U R' F2 U' L' U L F2 R U' R'` |
| AS-7 | **10** | 4 | BFLR | BFLR | `L R' D F2 D' L2 U L U' R U` | `(U') R' U L' U' L2 D F2 D' R L'` |
| AS-8 | **11** | 12 | BFLR | BFLR | `R' U2 R2 U2 R2 U' R2 U' R2 U R U` | `(U') R' U' R2 U R2 U R2 U2 R2 U2 R` |
| AS-9 | **11** | 8 | BFLR | BFLR | `R' U2 R2 U R2 U R U' R U' R' U2` | `(U2) R U R' U R' U' R2 U' R2 U2 R` |
| AS-10 | **10** | 4 | BFLR | BFLR | `L U' R U R2 D' F2 D L' R U` | `(U') R' L D' F2 D R2 U' R' U L'` |
| AS-11 | **7** | 4 | BFLR | BFLR | `R U R' U R U2 R'` | `R U2 R' U' R U' R'` |
| AS-12 | **12** | 4 | BFLR | BFLR | `R' F' U' F U R F U R U' R' F' U2` | `(U2) F R U R' U' F' R' U' F' U F R` |
| AS-13 | **11** | 8 | BFLR | BFLR | `L U' R' U F2 U F2 U' F2 R L' U'` | `(U) L R' F2 U F2 U' F2 U' R U L'` |
| AS-14 | **13** | 4 | BFLR | BFLR | `R' U2 R U R2 D' R U' R' D R U2 R U'` | `(U) R' U2 R' D' R U R' D R2 U' R' U2 R` |
| AS-15 | **11** | 4 | BFLR | BFLR | `R2 D' L F2 L' D R2 U R' U R` | `R' U' R U' R2 D' L F2 L' D R2` |
| AS-16 | **10** | 8 | BFLR | BFLR | `R L' U R' U' L U2 R U2 R'` | `R U2 R' U2 L' U R U' L R'` |
| AS-17 | **13** | 16 | BFLR | BFLR | `L U' R' U L' U R' D' R U2 R' D R2 U2` | `(U2) R2 D' R U2 R' D R U' L U' R U L'` |
| AS-18 | **11** | 4 | BFLR | BFLR | `R' F' U' F2 U2 F2 U' F2 U' F' R U` | `(U') R' F U F2 U F2 U2 F2 U F R` |
| AS-19 | **13** | 16 | BFLR | BFLR | `R' L D F2 D' L2 U' R U' R' U2 R L U'` | `(U) L' R' U2 R U R' U L2 D F2 D' L' R` |
| AS-20 | **14** | 48 | BFLR | BFLR | `R' U' L U' R2 U R L' U' R2 U2 R' U' R2 U` | `(U') R2 U R U2 R2 U L R' U' R2 U L' U R` |
| AS-21 | **11** | 4 | BFLR | BFLR | `R2 D' R U2 R' D R2 U R' U R` | `R' U' R U' R2 D' R U2 R' D R2` |
| AS-22 | **13** | 28 | BFLR | BFLR | `L U' R' U L' U2 R F R' U R U' F' U'` | `(U) F U R' U' R F' R' U2 L U' R U L'` |
| AS-23 | **12** | 16 | BFLR | BFLR | `F2 U L' U' R U' R' U' L F2 U' F2` | `F2 U F2 L' U R U R' U L U' F2` |
| AS-24 | **12** | 4 | BFLR | BFLR | `R U' L U' R2 D' F2 D R2 U2 L' R' U'` | `(U) R L U2 R2 D' F2 D R2 U L' U R'` |
| AS-25 | **11** | 8 | BFLR | BFLR | `L R' F2 U' F2 U F2 U L' U' R` | `R' U L U' F2 U' F2 U F2 R L'` |
| AS-26 | **12** | 16 | BFLR | BFLR | `F2 U' F2 R U' L' U' L U' R' U F2 U2` | `(U2) F2 U' R U L' U L U R' F2 U F2` |
| AS-27 | **14** | 48 | BFLR | BFLR | `R2 U' R' U2 R2 U' L' R U R2 U' L U' R' U2` | `(U2) R U L' U R2 U' R' L U R2 U2 R U R2` |
| AS-28 | **10** | 8 | BFLR | BFLR | `R' U2 R U F R' U R U' F'` | `F U R' U' R F' U' R' U2 R` |
| AS-29 | **13** | 16 | BFLR | BFLR | `R2 D R' U2 R D' R' U L' U R' U' L` | `L' U R U' L U' R D R' U2 R D' R2` |
| AS-30 | **11** | 4 | BFLR | BFLR | `F R' U' R2 U' R2 U2 R2 U' R' F' U2` | `(U2) F R U R2 U2 R2 U R2 U R F'` |
| AS-31 | **13** | 16 | BFLR | BFLR | `L R U2 L' U' L U' R2 D' F2 D L' R` | `R' L D' F2 D R2 U L' U L U2 R' L'` |
| AS-32 | **11** | 4 | BFLR | BFLR | `L U L' U L2 D R' F2 R D' L2 U'` | `(U) L2 D R' F2 R D' L2 U' L U' L'` |
| AS-33 | **12** | 4 | BFLR | BFLR | `L' R' U2 L2 D F2 D' L2 U' R U' L U2` | `(U2) L' U R' U L2 D F2 D' L2 U2 R L` |
| AS-34 | **13** | 28 | BFLR | BFLR | `F U2 L U' R' U L' U2 R F' R' U R U` | `(U') R' U' R F R' U2 L U' R U L' U2 F'` |
| AS-35 | **13** | 4 | BFLR | BFLR | `R U2 R D R' U' R D' R2 U R U2 R'` | `R U2 R' U' R2 D R' U R D' R' U2 R'` |
| AS-36 | **11** | 4 | BFLR | BFLR | `R U R' U R2 D R' U2 R D' R2 U` | `(U') R2 D R' U2 R D' R2 U' R U' R'` |
| AS-37 | **7** | 4 | BFLR | BFLR | `R U' L' U R' U' L U'` | `(U) L' U R U' L U R'` |
| AS-38 | **12** | 4 | BFLR | BFLR | `B U' F' U' R2 D2 F L2 D2 R2 U' F` | `F' U R2 D2 L2 F' D2 R2 U F U B'` |
| AS-39 | **12** | 4 | BFLR | BFLR | `R' D' L' U2 L2 D R' D' L' F2 D R2 U` | `(U') R2 D' F2 L D R D' L2 U2 L D R` |
| AS-40 | **13** | 8 | BFLR | BFLR | `F2 R F2 U F2 R2 U R D U' R D' F2` | `F2 D R' U D' R' U' R2 F2 U' F2 R' F2` |
| AS-41 | **12** | 8 | BFLR | BFLR | `R' U2 F' R U R' U' R' F R U2 R U'` | `(U) R' U2 R' F' R U R U' R' F U2 R` |
| AS-42 | **12** | 8 | BFLR | BFLR | `F U2 R' U' R F' R' U R F U2 F' U'` | `(U) F U2 F' R' U' R F R' U R U2 F'` |
| AS-43 | **13** | 24 | BFLR | BFLR | `L U' R' U L' F U' F' U' R F U2 F' U2` | `(U2) F U2 F' R' U F U F' L U' R U L'` |
| AS-44 | **12** | 4 | BFLR | BFLR | `R U' F2 D2 B2 R D2 F2 U' R' U' L U'` | `(U) L' U R U F2 D2 R' B2 D2 F2 U R'` |
| AS-45 | **13** | 12 | BFLR | BFLR | `R U' L' U2 R2 U' R' U' L U F2 R2 F2` | `F2 R2 F2 U' L' U R U R2 U2 L U R'` |
| AS-46 | **13** | 24 | BFLR | BFLR | `F' U2 F L U' F' U' F R' U L' U' R U'` | `(U) R' U L U' R F' U F U L' F' U2 F` |
| AS-47 | **12** | 4 | BFLR | BFLR | `L2 D F2 R' D' L' D R2 U2 R' D' L'` | `L D R U2 R2 D' L D R F2 D' L2` |
| AS-48 | **10** | 8 | BFLR | BFLR | `F2 D R' D B2 R D2 F2 U' L U` | `(U') L' U F2 D2 R' B2 D' R D' F2` |
| AS-49 | **10** | 4 | BFLR | BFLR | `F R' U2 R F' R' F U2 F' R U2` | `(U2) R' F U2 F' R F R' U2 R F'` |
| AS-50 | **12** | 12 | BFLR | BFLR | `L U' R' U L2 U2 R U' R' U2 R L U2` | `(U2) L' R' U2 R U R' U2 L2 U' R U L'` |
| AS-51 | **11** | 4 | BFLR | BFLR | `R2 F2 D L' B2 L D' F2 R' U2 R'` | `R U2 R F2 D L' B2 L D' F2 R2` |
| AS-52 | **13** | 8 | BFLR | BFLR | `L2 F' R2 L D2 R' B R D2 R2 L2 F L' U2` | `(U2) L F' L2 R2 D2 R' B' R D2 L' R2 F L2` |
| AS-53 | **12** | 4 | BFLR | BFLR | `F2 D F2 U2 F R2 U' R2 U' F2 D' F` | `F' D F2 U R2 U R2 F' U2 F2 D' F2` |
| AS-54 | **12** | 4 | BFLR | BFLR | `R D' R2 U' F2 U' F2 R U2 R2 D R2 U2` | `(U2) R2 D' R2 U2 R' F2 U F2 U R2 D R'` |
| AS-55 | **12** | 12 | BFLR | BFLR | `F' U2 F2 R2 U F2 U F2 U' R2 U' F'` | `F U R2 U F2 U' F2 U' R2 F2 U2 F` |
| AS-56 | **12** | 12 | BFLR | BFLR | `R L U2 R' U' R U2 L2 U R' U' L U2` | `(U2) L' U R U' L2 U2 R' U R U2 L' R'` |
| AS-57 | **12** | 28 | BFLR | BFLR | `F2 R2 F2 R U' L' U2 R2 U' R' U' L U2` | `(U2) L' U R U R2 U2 L U R' F2 R2 F2` |
| AS-58 | **12** | 12 | BFLR | BFLR | `R' U' F2 U' R2 U R2 U F2 R2 U2 R'` | `R U2 R2 F2 U' R2 U' R2 U F2 U R` |
| AS-59 | **11** | 4 | BFLR | BFLR | `L' U2 L' F2 D' R B2 R' D F2 L2 U'` | `(U) L2 F2 D' R B2 R' D F2 L U2 L` |
| AS-60 | **13** | 8 | BFLR | BFLR | `F2 R F2 U' F2 D R D' U R U' R2 F2 U'` | `(U) F2 R2 U R' U' D R' D' F2 U F2 R' F2` |
| AS-61 | **13** | 24 | BFLR | BFLR | `R' F2 D' F2 D2 F2 L D L' F2 R' D' R2 U2` | `(U2) R2 D R F2 L D' L' F2 D2 F2 D F2 R` |
| AS-62 | **11** | 8 | BFLR | BFLR | `R' U2 L U' R U L' U R' U R` | `R' U' R U' L U' R' U L' U2 R` |
| AS-63 | **13** | 4 | BFLR | BFLR | `L D' F2 D L' U2 R' U' R' U R' U R2 U2` | `(U2) R2 U' R U' R U R U2 L D' F2 D L'` |
| AS-64 | **13** | 40 | BFLR | BFLR | `F R U' R2 U2 R U R' U R2 U R' F' U2` | `(U2) F R U' R2 U' R U' R' U2 R2 U R' F'` |
| AS-65 | **12** | 8 | BFLR | BFLR | `F R F' L F B R' F' R B' L' R' U` | `(U') R L B R' F R B' F' L' F R' F'` |
| AS-66 | **13** | 24 | BFLR | BFLR | `R U' R2 D' U2 R U R' D U R2 U R' U2` | `(U2) R U' R2 U' D' R U' R' U2 D R2 U R'` |
| AS-67 | **14** | 96 | BFLR | BFLR | `F U R' F R F' R U' R' U R U' R' F'` | `F R U R' U' R U R' F R' F' R U' F'` |
| AS-68 | **13** | 4 | BFLR | BFLR | `L2 U L' U L' U' L' U2 R' D F2 D' R` | `R' D F2 D' R U2 L U L U' L U' L2` |
| AS-69 | **13** | 16 | BFLR | BFLR | `R U2 R2 F2 R F2 L' U2 L U' R U' R'` | `R U R' U L' U2 L F2 R' F2 R2 U2 R'` |
| AS-70 | **13** | 16 | BFLR | BFLR | `R U2 F' L F L' U2 R' F' U2 F2 U2 F'` | `F U2 F2 U2 F R U2 L F' L' F U2 R'` |
| AS-71 | **11** | 8 | BFLR | BFLR | `R U R' U L' U R U' L U2 R'` | `R U2 L' U R' U' L U' R U' R'` |
| AS-72 | **13** | 16 | BFLR | BFLR | `L' U' L U' R U2 R' F2 R' F2 R2 U2 R' U2` | `(U2) R U2 R2 F2 R F2 R U2 R' U L' U L` |
| PLL-1 | **9** | 4 | BFLR | BFLR | `F2 U' L R' F2 L' R U' F2 U` | `(U') F2 U R' L F2 R L' U F2` |
| PLL-2 | **9** | 4 | BFLR | BFLR | `F2 U L R' F2 L' R U F2 U` | `(U') F2 U' R' L F2 R L' U' F2` |
| PLL-3 | **12** | 448 | BDFLR | BDFLR | `R2 U R2 U' R2 F2 R2 U' F2 U R2 F2 U'` | `(U) F2 R2 U' F2 U R2 F2 R2 U R2 U' R2` |
| PLL-4 | **9** | 32 | BFLR | BFLR | `B2 R2 L2 F2 D' B2 R2 L2 F2` | `F2 L2 R2 B2 D F2 L2 R2 B2` |
| PLL-5 | **10** | 48 | BFLR | BFLR | `R' U L' U2 R U' R' U2 R L U'` | `(U) L' R' U2 R U R' U2 L U' R` |
| PLL-6 | **13** | 56 | BFLR | BFLR | `R2 F2 U R U R' U' R' U' F2 R' U R' U2` | `(U2) R U' R F2 U R U R U' R' U' F2 R2` |
| PLL-7 | **10** | 24 | BFLR | BFLR | `L2 U' L2 D F2 R2 U R2 D' F2 U2` | `(U2) F2 D R2 U' R2 F2 D' L2 U L2` |
| PLL-8 | **13** | 56 | BFLR | BFLR | `R' U2 R U2 R' F R U R' U' R' F' R2 U'` | `(U) R2 F R U R U' R' F' R U2 R' U2 R` |
| PLL-9 | **9** | 8 | BFLR | BFLR | `F2 L2 F' R' F L2 F' R F' U'` | `(U) F R' F L2 F' R F L2 F2` |
| PLL-10 | **12** | 24 | BFLR | BFLR | `F2 D' L U' L U L' D F2 R U' R' U'` | `(U) R U R' F2 D' L U' L' U L' D F2` |
| PLL-11 | **12** | 24 | BFLR | BFLR | `R U R' F2 D' L U' L' U L' D F2 U2` | `(U2) F2 D' L U' L U L' D F2 R U' R'` |
| PLL-12 | **13** | 32 | BFLR | BFLR | `R B' L U2 R' F R' L2 D2 L2 F' R L' U2` | `(U2) L R' F L2 D2 L2 R F' R U2 L' B R'` |
| PLL-13 | **12** | 24 | BFLR | BFLR | `F2 L2 R2 U R2 U' R2 D R2 D' L2 F2 U` | `(U') F2 L2 D R2 D' R2 U R2 U' R2 L2 F2` |
| PLL-14 | **9** | 8 | BFLR | BFLR | `F R' F L2 F' R F L2 F2` | `F2 L2 F' R' F L2 F' R F'` |
| PLL-15 | **10** | 48 | BFLR | BFLR | `R L U2 R' U' R U2 L' U R' U2` | `(U2) R U' L U2 R' U R U2 L' R'` |
| PLL-16 | **12** | 24 | BFLR | BFLR | `L' U' L F2 D R' U R U' R D' F2 U2` | `(U2) F2 D R' U R' U' R D' F2 L' U L` |
| PLL-17 | **14** | 176 | BFLR | BFLR | `F' R' F' R U' R U R2 F R U F U' F U2` | `(U2) F' U F' U' R' F' R2 U' R' U R' F R F` |
| PLL-18 | **13** | 112 | BFLR | BFLR | `R2 U' R2 U' R2 U F U F' R2 F U' F' U'` | `(U) F U F' R2 F U' F' U' R2 U R2 U R2` |
| PLL-19 | **13** | 24 | BFLR | BFLR | `R U' R2 F2 U' R F2 R' U F2 R2 U R'` | `R U' R2 F2 U' R F2 R' U F2 R2 U R'` |
| PLL-20 | **14** | 208 | BDFLR | BDFLR | `F2 U' R2 U2 R2 F2 U' R2 U' R2 U2 F2 U F2` | `F2 U' F2 U2 R2 U R2 U F2 R2 U2 R2 U F2` |
| PLL-21 | **13** | 24 | BFLR | BFLR | `F2 R2 U2 R2 U' R2 F2 U2 F2 U' R2 U2 F2` | `F2 U2 R2 U F2 U2 F2 R2 U R2 U2 R2 F2` |

## Reproducing

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
cd build && ./ll_optimal zbll --near 7 > zbll.md   # ~50 min on 4 cores, ~6 GB RAM
```
