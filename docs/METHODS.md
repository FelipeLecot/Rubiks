# How many moves does each solving method need?

A step-optimal comparison of CFOP (Friedrich), CFOP with a one-look last
layer, ZB, ZZ, Petrus, Roux and a corners-first method on the **same 100
uniformly random cube states**, in both the slice turn metric (STM: `M`
counts 1) and the half turn metric (HTM: face turns only, `M` counts 2).
Computed by `method_stats` (`src/tools/method_stats.cpp`); raw per-scramble
data in [`data/methods_stm.csv`](data/methods_stm.csv) and
[`data/methods_htm.csv`](data/methods_htm.csv).

## Headline (STM)

| Method | Mean moves | vs CFOP |
|---|--:|--:|
| **ZZ** (EOLine, blocks, ZBLL) | **34.3** | −13.0 |
| **Roux** (blocks, CMLL, LSE) | **34.3** | −13.0 |
| Corners first (corners, L/R edges, last six edges) | 36.8 | −10.5 |
| Petrus (2x2x2, 2x2x3, EO, F2L, ZBLL) | 37.2 | −10.1 |
| CFOP + one-look last layer (1LLL) | 40.3 | −7.0 |
| ZB (CFOP with ZBLS + ZBLL) | 41.0 | −6.4 |
| **CFOP** (cross, 4 pairs, OLL, PLL) | **47.3** | — |

For scale: the shortest possible solution of a random cube averages about
17.7 face turns (God's algorithm), and good human solves are much longer than
any of these (commonly quoted: CFOP ~55–60 STM, Roux ~45–50, ZZ ~45–55,
Petrus ~45–50; see the sources at the end).

What the numbers say:

1. **Block building with pre-oriented edges wins.** ZZ and Roux tie at 34.3 STM
   (paired difference −0.03 ± 0.52). Both beat CFOP on all 100 scrambles, by 13
   moves on average (~28%).
2. **Most of CFOP's deficit is the two-look last layer.** OLL + PLL cost 19.8
   STM, and a single optimal last-layer step costs 12.8, so 7 of CFOP's 13-move
   gap to ZZ/Roux is the last layer. The rest is F2L: cross + 4 pairs = 27.6 STM,
   against 22.1 for ZZ's EOLine + blocks.
3. **ZB is not shorter than CFOP + 1LLL.** Orienting edges while inserting the
   last pair (ZBLS) costs 1.3 moves more than a plain last pair, and ZBLL saves
   only 0.7 against a full one-look last layer: net +0.6 ± 0.5 STM (+0.75 ± 0.40
   HTM). ZB's real advantage is 493 algorithms instead of 3,915.
4. **Slices matter most for methods that finish with M-slice work.** Going from
   HTM to STM saves 5.7 moves for corners first, 2.9 for Roux and 2.8 for CFOP
   (mostly in its last layer), but only ~1.9 for Petrus, ZB and CFOP + 1LLL.
   In HTM, ZZ (36.6) edges out Roux (37.2).
5. **Corners first is competitive in STM** (36.8, level with Petrus), but its
   edge steps are long, and it is the most variable method (SD 2.6 STM, 3.6 HTM).

## What "step-optimal" means

Every step is solved in the fewest possible moves for its goal (IDA* with
pattern databases built for that goal). Every move counts, AUF included.
Where a solver has a free choice, the tool takes the shortest option:
the next F2L slot, and for Roux / corners first the M-slice offset, which only
has to be fixed in the last step.

- **Optimistic:** humans rarely find the optimal cross, pair or block.
- **Pessimistic:** there is no planning across steps (XCross, keyhole,
  last-layer influence, choosing the cross color / block location), and each
  step takes its *first* optimal solution without trying to leave a better
  next step. The cross and blocks are always built in the same place (cross on
  D, Roux first block on L), with no color neutrality.
- **Same scrambles everywhere**, so differences between methods are paired per
  scramble and much tighter than the per-method spreads.

Sanity checks: the optimal cross averages **5.80 HTM**, matching the
known ~5.8. Every one of the 1,400 simulated solves was replayed on the repo's
independent sticker model and ends solved.

Method definitions (piece sets, in `method_stats.cpp`):
- **CFOP:** cross → 4 pairs (shortest remaining slot first) → OLL → PLL.
- **CFOP + 1LLL:** the same F2L, then the last layer in one step.
- **ZB:** cross → 3 pairs → last pair with all edges oriented → ZBLL.
- **ZZ:** EOLine (all edges oriented + DF, DB) → left 1x2x3 → right 1x2x3,
  keeping EO → ZBLL.
- **Petrus:** 2x2x2 (DBL) → 2x2x3 → EO → rest of F2L → ZBLL.
- **Roux:** first block (left 1x2x3) → second block → CMLL → LSE. Blocks
  and corners only need to agree with each other; the M slice stays free.
- **Corners first:** all corners → left-layer edges → right-layer edges →
  last six edges (M-slice free until the end).

## Slice turn metric (STM)

### Totals (STM, n = 100 random states)

| Method | Mean ± 95% CI | SD | Min | Median | Max |
|---|--:|--:|--:|--:|--:|
| ZZ (ZBLL) | **34.29** ± 0.39 | 1.98 | 30 | 34 | 38 |
| Roux | **34.32** ± 0.38 | 1.96 | 29 | 34 | 39 |
| Corners first | **36.83** ± 0.51 | 2.61 | 30 | 37 | 42 |
| Petrus (ZBLL) | **37.24** ± 0.44 | 2.27 | 30 | 37 | 42 |
| CFOP + 1LLL | **40.33** ± 0.44 | 2.24 | 34 | 40 | 45 |
| ZB (ZBLS + ZBLL) | **40.95** ± 0.48 | 2.45 | 32 | 41 | 48 |
| CFOP | **47.32** ± 0.71 | 3.63 | 34 | 47.5 | 54 |

### Paired differences vs CFOP (STM)

Same scrambles for every method, so differences are measured per scramble.

| Method | Moves saved vs CFOP (mean ± 95% CI) | Beats CFOP on |
|---|--:|--:|
| ZZ (ZBLL) | +13.03 ± 0.82 | 100/100 |
| Roux | +13.00 ± 0.75 | 100/100 |
| Corners first | +10.49 ± 0.89 | 98/100 |
| Petrus (ZBLL) | +10.08 ± 0.80 | 97/100 |
| CFOP + 1LLL | +6.99 ± 0.51 | 96/100 |
| ZB (ZBLS + ZBLL) | +6.37 ± 0.69 | 94/100 |

### Distribution (STM)

```
ZZ (ZBLL)           :--+@#*-:                
Roux               ..--@##**..               
Corners first       :::..:#@=#-..            
Petrus (ZBLL)       .  ::-#@*=--.            
CFOP + 1LLL             ...-++@%*-=.         
ZB (ZBLS + ZBLL)      .   .-:*@@@=--. .      
CFOP                    ..   .. ::*@-@*@%==::
                   29                      54
```

Per step:

| Method | Mean | SD | Median | Min | Max | Mean per step |
|---|--:|--:|--:|--:|--:|---|
| CFOP | **47.32** | 3.63 | 47.5 | 34 | 54 | cross 5.32 · pair 1 4.72 · pair 2 5.20 · pair 3 5.57 · pair 4 6.75 · OLL 8.79 · PLL 10.97 |
| CFOP + 1LLL | **40.33** | 2.24 | 40.0 | 34 | 45 | cross 5.32 · pair 1 4.72 · pair 2 5.20 · pair 3 5.57 · pair 4 6.75 · 1LLL 12.77 |
| ZB (ZBLS + ZBLL) | **40.95** | 2.45 | 41.0 | 32 | 48 | cross 5.32 · pair 1 4.72 · pair 2 5.20 · pair 3 5.57 · ZBLS 8.09 · ZBLL 12.05 |
| ZZ (ZBLL) | **34.29** | 1.98 | 34.0 | 30 | 38 | EOLine 5.46 · left block 7.70 · right block 8.96 · ZBLL 12.17 |
| Petrus (ZBLL) | **37.24** | 2.27 | 37.0 | 30 | 42 | 2x2x2 5.66 · 2x2x3 6.07 · EO 4.26 · F2L 8.97 · ZBLL 12.28 |
| Roux | **34.32** | 1.96 | 34.0 | 29 | 39 | first block 6.28 · second block 8.58 · CMLL 9.40 · LSE 10.06 |
| Corners first | **36.83** | 2.61 | 37.0 | 30 | 42 | corners 8.77 · L edges 8.00 · R edges 10.25 · L6E 9.81 |

## Half turn metric (HTM)

### Totals (HTM, n = 100 random states)

| Method | Mean ± 95% CI | SD | Min | Median | Max |
|---|--:|--:|--:|--:|--:|
| ZZ (ZBLL) | **36.64** ± 0.41 | 2.07 | 31 | 37 | 41 |
| Roux | **37.18** ± 0.56 | 2.88 | 29 | 37.5 | 43 |
| Petrus (ZBLL) | **39.13** ± 0.49 | 2.51 | 31 | 39 | 44 |
| CFOP + 1LLL | **42.14** ± 0.53 | 2.70 | 34 | 42 | 49 |
| Corners first | **42.56** ± 0.71 | 3.62 | 28 | 43 | 48 |
| ZB (ZBLS + ZBLL) | **42.89** ± 0.51 | 2.62 | 37 | 43 | 48 |
| CFOP | **50.12** ± 0.64 | 3.25 | 41 | 51 | 57 |

### Paired differences vs CFOP (HTM)

Same scrambles for every method, so differences are measured per scramble.

| Method | Moves saved vs CFOP (mean ± 95% CI) | Beats CFOP on |
|---|--:|--:|
| ZZ (ZBLL) | +13.48 ± 0.79 | 100/100 |
| Roux | +12.94 ± 0.83 | 100/100 |
| Petrus (ZBLL) | +10.99 ± 0.85 | 99/100 |
| CFOP + 1LLL | +7.98 ± 0.39 | 100/100 |
| Corners first | +7.56 ± 0.97 | 92/100 |
| ZB (ZBLS + ZBLL) | +7.23 ± 0.58 | 99/100 |

### Distribution (HTM)

```
ZZ (ZBLL)             . -+*@%@%-.                
Roux                ...---*#*%@-+-.              
Petrus (ZBLL)         . ..:+-*@@%+:.             
CFOP + 1LLL              . .:-:=@@#+*-...        
Corners first      .    .: :-.:-=*@%**+.         
ZB (ZBLS + ZBLL)            :::=*=@##:=:         
CFOP                            ...::.:#++@++=:..
                   28                          57
```

Per step:

| Method | Mean | SD | Median | Min | Max | Mean per step |
|---|--:|--:|--:|--:|--:|---|
| CFOP | **50.12** | 3.25 | 51.0 | 41 | 57 | cross 5.80 · pair 1 5.08 · pair 2 5.37 · pair 3 5.84 · pair 4 6.84 · OLL 9.33 · PLL 11.86 |
| CFOP + 1LLL | **42.14** | 2.70 | 42.0 | 34 | 49 | cross 5.80 · pair 1 5.08 · pair 2 5.37 · pair 3 5.84 · pair 4 6.84 · 1LLL 13.21 |
| ZB (ZBLS + ZBLL) | **42.89** | 2.62 | 43.0 | 37 | 48 | cross 5.80 · pair 1 5.08 · pair 2 5.37 · pair 3 5.84 · ZBLS 8.34 · ZBLL 12.46 |
| ZZ (ZBLL) | **36.64** | 2.07 | 37.0 | 31 | 41 | EOLine 6.11 · left block 8.31 · right block 9.37 · ZBLL 12.85 |
| Petrus (ZBLL) | **39.13** | 2.51 | 39.0 | 31 | 44 | 2x2x2 6.06 · 2x2x3 6.15 · EO 4.84 · F2L 9.50 · ZBLL 12.58 |
| Roux | **37.18** | 2.88 | 37.5 | 29 | 43 | first block 6.81 · second block 9.07 · CMLL 9.35 · LSE 11.95 |
| Corners first | **42.56** | 3.62 | 43.0 | 28 | 48 | corners 8.77 · L edges 10.17 · R edges 11.83 · L6E 11.79 |

## Reproducing

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
cd build
./method_stats 100 --csv methods_htm.csv          # ~30 min on 4 cores after the databases are built
./method_stats 100 --stm --csv methods_stm.csv    # ~50 min
```
The first run builds ~160 goal databases (a few GB, cached in the working
directory) and uses up to ~8 GB of RAM. `--seed` changes the random states.

## Sources for human move counts

- [SpeedSolving: Method pros and cons: CFOP vs Roux vs ZZ](https://www.speedsolving.com/threads/method-pros-and-cons-cfop-vs-roux-vs-zz.75425/)
- [SpeedSolving: Average moves per solve?](https://www.speedsolving.com/threads/average-moves-per-solve.74166/)
- [Guide to Choosing a Speedsolving Method (speedcubing.org)](https://speedcubing.org/pages/guide-to-choosing-a-speedsolving-method)
