# Learning Tetris weights with cross-entropy

The game includes an offline-trained profile alongside the original fixed-weight baseline. Both use exactly the same reachable-placement search, movement rules, piece generator, and scoring. Training changes only how candidate boards are ranked.

## Play the learned policy

From the repository root, after building with graphics enabled:

```sh
./build-graphics/tetris --ai --seed 2 --weights models/tetris/cem-v1.weights
```

F2 hands control back to you; P pauses and R restarts the same seeded sequence. The sidebar shows `Strategy: loaded weights`. Omitting `--weights` keeps the original baseline. A saved profile does not keep learning while you watch; its choices remain deterministic.

Compare the two on identical piece sequences without opening a window:

```sh
./build/tetris_ai --seed 2000001 --games 100 --pieces 1000
./build/tetris_ai --seed 2000001 --games 100 --pieces 1000 --weights models/tetris/cem-v1.weights
```

On seed 2, the learned profile reaches the 1,000-piece cap with 393 lines; the original baseline ends at 52 lines. Improvement is not universal: on seed 1, both clear 13 lines.

## What was learned

The evaluation remains a linear combination of five board features. Multiplying every weight by the same positive constant does not change the chosen action, so the line-clear coefficient is fixed at 100 to set the scale. Cross-entropy search learns the four relative penalty magnitudes. Their signs remain a prior: lines are rewarded, and height, holes, unevenness, and maximum height are penalized.

| Feature | Baseline | Learned (rounded) |
| --- | ---: | ---: |
| Cleared lines | 100 | 100 |
| Sum of column heights | -5 | -7.86754 |
| Holes | -80 | -22.83073 |
| Adjacent height differences | -3 | -6.39231 |
| Maximum height | -2 | -2.22172 |

The learned policy puts less relative emphasis on avoiding holes and more on keeping the surface low and even. That describes its coefficients; it does not guarantee a better move in every position. Top-out receives a separate fixed penalty of one billion, which dominates every accepted weight vector. Drop bonuses are excluded from the training objective.

## Training loop

[tools/train_tetris.py](../tools/train_tetris.py) uses only the Python standard library and launches the compiled C simulator in batches. It needs no GPU or ML packages.

1. Initialize a diagonal Gaussian around the logarithms of the baseline's four penalty magnitudes, with standard deviation 1.0.
2. Sample 32 policies per generation. Include the distribution mean and, when distinct, the best validation policy retained so far. Convert log magnitudes to negative weights, bounded between -1,000 and -0.001.
3. Evaluate every candidate on the same 12 seeds, capped at 1,000 pieces per game. Rank by mean cleared lines, then median and 10th percentile to break ties.
4. Fit the mean and variance to the best 25% of candidates. Smooth each update with coefficient 0.7 and retain a minimum log standard deviation of 0.2 to prevent premature collapse.
5. Evaluate the generation winner on the fixed validation set. Retain a new profile only when it beats the current validation ranking. Use a fresh training-seed batch in the next generation.
6. After 12 generations, freeze the selected profile and evaluate it and the baseline on the untouched test set. Test results never update the weights.

The cross-entropy method for feature-weight policy search is described in [Scherrer et al., JMLR 2015](https://jmlr.org/papers/volume16/scherrer15a/scherrer15a.pdf). The log parameterization, smoothing, bounds, objective, and evaluation split above are this project's implementation choices.

## Reproduce or retrain

Build the Release console executable, then run:

```sh
python3 tools/train_tetris.py --engine build/tetris_ai --output training-runs/my-cem-run \
  --iterations 12 --population 32 --train-games 12 \
  --validation-games 32 --test-games 100 --pieces 1000 --seed 23 --workers 4
```

For Visual Studio, use `build/Release/tetris_ai.exe`. The optimizer seed controls candidate sampling; game seeds come from the separate ranges below. Worker completion order does not affect candidate selection. The output directory must be new, so an existing experiment cannot be overwritten accidentally. `--help` lists bounded options. Use a Release build for actual training; sanitizer builds are useful for small verification runs.

Outputs include:

- `best.weights`: selected loadable profile; the baseline is retained if no candidate improves validation performance.
- `checkpoints/generation-NNN.weights`: the best profile retained at that generation, ready to load in the game.
- Matching checkpoint JSON: distribution, optimizer RNG state, configuration, and metrics for inspection. Automatic resume is not implemented.
- `candidates/` and checkpoint CSV files: individual candidate profiles and per-seed training/validation results.
- `history.json`, `validation.csv`, `test.csv`, and `report.json`: progress and final comparison, including engine/trainer hashes and a paired bootstrap interval.

Run directories are ignored by Git. The first run's selected profile and compact results are published under [models/tetris](../models/tetris/README.md).

## Evaluation of the bundled profile

The bundled run used optimizer seed 23. Training used 144 distinct seeds (1001–1144), with 4,608 candidate games in total. Validation reused seeds 1000001–1000032 for model selection; generation 11 supplied the selected profile. Only after selection, both policies were evaluated on test seeds 2000001–2000100.

| Metric, 100 test seeds, 1,000-piece cap | Baseline | Learned |
| --- | ---: | ---: |
| Mean cleared lines | 116.60 | 248.47 |
| Median cleared lines | 83.5 | 259.5 |
| 10th percentile cleared lines | 17.9 | 75.6 |
| Games reaching the piece cap | 5 | 30 |

The learned policy cleared more lines on 76 seeds, tied on 1, and cleared fewer on 23. The paired mean difference was +131.87 lines; a 2,000-resample paired bootstrap gives a 95% interval of approximately +99.1 to +162.8 lines. Per-seed outcomes are in [cem-v1-test.csv](../models/tetris/cem-v1-test.csv); the full metrics and configuration are in [cem-v1-report.json](../models/tetris/cem-v1-report.json).

These are capped-game results from one optimizer run. Games reaching the cap are truncated, so the results do not estimate their full survival length. Reusing these published test seeds to choose future models would turn them into development data; a subsequent tuning round should reserve a fresh final test set.

## Weight-file format

Files contain the version token followed by exactly these five names and values, in this order:

```text
TETRIS_WEIGHTS_V1
lines 100
height -5
holes -80
bumpiness -3
max_height -2
```

All values must be finite and within -1,000 to +1,000; `lines` must be positive and the other values nonpositive. Missing, duplicate, extra, nonfinite, or invalid tokens are rejected before play begins. Values are saved with enough digits to round-trip a C double. Profiles work in both `tetris` and `tetris_ai` through `--weights file`.
