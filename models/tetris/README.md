# Tetris policies

- `baseline.weights`: the original hand-selected policy, also built into the game.
- `cem-v1.weights`: selected by validation after a 12-generation cross-entropy run (optimizer seed 23).
- `cem-v1-report.json`: configuration, provenance hashes, validation and held-out test metrics.
- `cem-v1-history.json`: progress by generation; each generation uses a different training batch, so its training score is not directly comparable to the preceding generation's score.
- `cem-v1-test.csv`: paired per-seed outcomes for both policies.

Use a profile with `tetris --ai --weights models/tetris/cem-v1.weights` or the headless `tetris_ai` command. Omit `--weights` to use the baseline. See the [training guide](../../docs/TETRIS_TRAINING.md) for the method, reproduction commands, results, and limitations. The saved profile is frozen; playing it does not update its weights.
