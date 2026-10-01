# Tetris autoplayer

Watch it play:

```sh
./build-graphics/tetris --ai --seed 1
```

F2 switches between AI and human control, including mid-piece. P pauses, R restarts, and Escape exits. A green outline shows the chosen landing position. With an explicit seed, R repeats the same piece sequence. Without one, a fresh game is seeded from the clock and restarts continue that sequence. Human controls remain arrows/WASD, with Space for a hard drop.

AI playback takes one action every 80 milliseconds. It owns the movement clock, so extra gravity cannot invalidate a route between actions; human play retains normal level-dependent gravity. AI and human play share collision checks, rotations, row clearing, drop bonuses, and scoring. The session-best score includes both modes.

A learned profile is also available: add `--weights models/tetris/cem-v1.weights`. It uses the same search with coefficients optimized offline by cross-entropy. See the [training guide and results](TETRIS_TRAINING.md). Without that option, the original baseline below remains active.

## How it decides

The agent reads the actual board array. It uses a deterministic heuristic search, with no screen capture, external service, neural network, or training dependency.

1. Search from the current `(x, y, rotation)` using breadth-first search. Edges are legal left, right, rotation, or downward moves under the existing workshop rules. There are no wall kicks or upward moves.
2. Every reachable position that cannot move down is a candidate landing. This includes slides beneath overhangs; a geometrically empty but sealed cavity is never a candidate.
3. Place each candidate on a private board copy, clear full rows, then measure the remaining stack.
4. Choose the highest-valued candidate. Equal values retain the first BFS result, giving deterministic, shortest-path tie-breaking.
5. Replay its movement path. Replace the final uninterrupted descent with a hard drop. Earlier downward moves remain when needed to reach an overhang.

The hand-selected evaluation is:

```text
100 * cleared_lines
- 5 * sum_of_column_heights
- 80 * holes
- 3 * adjacent_column_height_differences
- 2 * maximum_column_height
- 1,000,000,000 if the result has a filled cell in the top row
```

A hole is an empty cell below an occupied cell in the same column. All stack features are measured after line clearing. Drop points do not influence the choice; they are awarded normally during execution. Planning never mutates the live board.

The search is bounded to 1,344 possible position/orientation states. It considers only the current piece. Random pieces and starting rotations follow the workshop's existing independent draws, using an explicit xorshift32 generator so seeds behave consistently across platforms; there is no seven-piece bag or next-piece preview.

## Evaluate without a window

The console build includes the same planner and board rules:

```sh
./build/tetris_ai --seed 1 --games 20 --pieces 1000
./build/tetris_ai --seed 101 --games 20 --pieces 1000 > results.csv
```

Output columns are `seed,pieces,lines,score,level,status`. Each game uses the next consecutive seed. `limit` means the requested piece cap was reached while still alive; `game_over` means the stack or next spawn ended play. `--pieces` allows 1–100,000, `--games` allows 1–100, and all seeds must fit 1–2,147,483,647. Defaults are seed 1, one game, and a 1,000-piece cap.

For the baseline weights above, seeds 1–20 with a 1,000-piece cap cleared 7–253 lines, with a median of 79. All 20 games eventually ended before that cap. These are reproducible examples, not a claim of optimal play or a general performance guarantee.

The tests independently replay planned geometry, verify that planning leaves the board unchanged, check four-line clears and overhang/closed-cavity cases, exercise every piece/orientation, and run repeatable seeded games. A graphical smoke test also executes the AI through actual piece locks.

## Limits and possible extensions

This is a one-piece heuristic baseline. Greedy choices can create problems that only become apparent several pieces later. A next-piece queue would enable lookahead; cross-entropy weight optimization is now implemented with separate training, validation, and test seeds. Reinforcement learning would require a simulation API with observations, actions, resets, rewards, and a separate training pipeline.

The implementation lives in [ai.c](../Lab05/Solved/ai.c), with the headless runner in [ai_cli.c](../Lab05/Solved/ai_cli.c). The GUI and runner use the same board and movement functions.
