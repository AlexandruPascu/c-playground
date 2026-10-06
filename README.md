# C Playground

[![Build and test](https://github.com/AlexandruPascu/c-playground/actions/workflows/build.yml/badge.svg)](https://github.com/AlexandruPascu/c-playground/actions/workflows/build.yml)

C programming exercises covering algorithms, data structures, graphics, and small games. The original workshop layout is preserved, with a shared CMake build and completed exercises and regression tests. Start with the games below or browse the [workshop guide](docs/WORKSHOP.md) for every runnable exercise.

## Build and run

The console examples need a C99 compiler and CMake 3.20+. Python 3.8+ enables the command-line regression tests.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure

./build/fibonacci 30
./build/fibonacci 1000000 matrix
./build/components
./build/linked_list
./build/structs
./build/tic_tac_toe --play 2
./build/tic_tac_toe < examples/tic_tac_toe.txt
```

These commands work from the repository root. With a multi-configuration generator such as Visual Studio, executables are under `build/Release/` and have the `.exe` suffix. The CLI programs also accept `--help`, except for the two small data-structure demos.

### Graphics with Allegro 5

The windowed examples additionally need Allegro's core, primitives, image, font, and TTF development libraries, discoverable through `pkg-config`. On Ubuntu/Debian:

```sh
sudo apt install build-essential cmake python3 pkg-config liballegro5-dev liballegro-image5-dev liballegro-ttf5-dev
cmake -S . -B build-graphics -DPLAYGROUND_BUILD_GRAPHICS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-graphics --parallel 2

./build-graphics/tetris
./build-graphics/tetris --ai --seed 1
./build-graphics/tetris --ai --seed 2 --weights models/tetris/cem-v1.weights
./build-graphics/tic_tac_toe_play 2
./build-graphics/skeleton
./build-graphics/sierpinski_triangle 5
./build-graphics/sierpinski_animation 5
./build-graphics/sierpinski_carpet 3
```

- **Tetris:** A/D or Left/Right move, W/Up rotates, S/Down soft-drops, Space hard-drops, P pauses, R restarts. Clear 1/2/3/4 rows for 100/300/500/800 points multiplied by the current level. Soft drops earn 1 point per cell and hard drops 2. Every 10 cleared lines raises the level and falling speed, up to level 20. The sidebar shows score, lines, level, and the best score for this session. Restart resets the game but keeps the session best. F2 toggles the AI or hands control back to you mid-piece; a green outline marks its planned landing. Start with `--ai --seed 1` for a repeatable demonstration. [How the AI works](docs/TETRIS_AI.md). The optional [learned weights](docs/TETRIS_TRAINING.md) more than doubled mean cleared lines on a 100-seed test with a 1,000-piece cap. Omitting `--weights` retains the original baseline.
- **Nested tic-tac-toe:** Click an empty cell, or use arrows and Enter/Space. X and digit 0 take turns on the same computer. F finishes and scores the board; R restarts. Start with size 2 for a quick game; the window supports sizes 1–5. See the [rules and evaluator](PoliTicTacToe/README).
- **Skeleton:** WASD or arrows move; simultaneous directions allow diagonal movement at the same speed. The sprite wraps around the window.
- **Fractals:** Left/Right change recursion depth. Triangle depth is limited to 0–9 and carpet depth to 0–5 to keep drawing responsive. The animation adds a depth every half-second.
- **All windows:** Escape or the close button exits. `--smoke-test` runs briefly and exits automatically.

CMake copies the sprite sheets into `build-graphics/assets/`; programs can be launched from any working directory. Keep the configured build directory in place, or rerun CMake after moving it. Graphics are checked on Linux; the console build is checked on Linux, macOS, and Windows.

## Maintained examples and algorithms

| Example | Where to read | What it demonstrates |
| --- | --- | --- |
| Nested tic-tac-toe | [PoliTicTacToe](PoliTicTacToe/README) | An `n × n` macroboard of `n × n` microboards. Line checks claim microboards; the final score compares complete macroboard lines. Includes a clickable game, terminal play, and a batch move evaluator. Only the batch evaluator uses a diagonal fallback for invalid cells. No AI opponent. |
| Tetris | [board rules](Lab05/Solved/board.c), [game loop](Lab05/Solved/tetris.c) | Grid collisions, four precomputed orientations of each piece, full-row removal and downward compaction, timer-driven falling, spawn checks, line/drop scoring, level progression, and a heuristic autoplayer that searches reachable placements. |
| Fibonacci | [three implementations](algorithms/fibonacci.c) | Naive recursion repeats subproblems (exponential time), iteration takes `O(n)` time and constant space, and 2×2 matrix exponentiation takes `O(log n)` time. All return `F(n) mod 666013`, with `F(0)=0`. |
| Largest connected region | [iterative flood fill](algorithms/flood_fill.c) | Depth-first search through four-neighbour cells in a binary grid. Each cell is visited once: `O(rows × columns)` time and worst-case auxiliary space. An explicit stack avoids recursive stack overflow. |
| Linked list and structs | [list demo](Lab02/Demo/demo_linkedlist.c), [struct demo](Lab02/Demo/demo_struct.c) | Node allocation, pointer links, insertion/removal, and releasing owned memory; named fields group related data. |
| Sierpinski triangle and carpet | [recursive drawing](graphics/fractals.c) | Divide a shape, remove its middle, then recurse into 3 triangles or 8 squares. Work grows as `O(3^depth)` or `O(8^depth)`. Depth zero is a solid shape. |
| Sprite animation | [skeleton](Exam/challenge1/skelly.c), [window runtime](graphics/window.c) | Sample held keys each timer tick, move by speed × elapsed time, and cycle across nine sprite-sheet frames in the current direction. |

The Fibonacci CLI caps the recursive method at 35 and the iterative method at 10,000,000. `all` mode skips methods above those limits; `matrix` accepts nonnegative indices through `INT_MAX`. Timings are demonstration measurements, not a benchmarking framework.

`components [grid.txt]` expects positive row and column counts followed by exactly that many binary cells. It accepts up to 1,000,000 cells and defaults to the workshop's sample grid. Its library function marks visited cells in place.

## Original exercises and reference material

All Lab01–Lab05 exercises now have runnable implementations, alongside the two Exam graphics examples and the nested tic-tac-toe evaluator. The [workshop guide](docs/WORKSHOP.md) maps every challenge to its executable, input, and purpose.

The original Romanian statements are retained. Original and `Solved` entry points share maintained implementations in `workshop/`, `algorithms/`, and `graphics/`; the earlier starter versions remain in Git history. Small Makefiles delegate to the root CMake build and place executables in `build/` or `build-graphics/`. `make clean` cleans that shared build directory.

## Checks

```sh
cmake -S . -B build -DPLAYGROUND_WARNINGS_AS_ERRORS=ON -DPLAYGROUND_REQUIRE_INTEGRATION_TESTS=ON
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure

# GCC/Clang on Unix: address, leak, and undefined-behavior checks
cmake -S . -B build-sanitized -DCMAKE_BUILD_TYPE=Debug -DPLAYGROUND_SANITIZERS=ON -DPLAYGROUND_REQUIRE_INTEGRATION_TESTS=ON
cmake --build build-sanitized --parallel 2
ctest --test-dir build-sanitized --output-on-failure

# On Linux without a desktop, install xvfb and xauth, then:
xvfb-run -a ctest --test-dir build-graphics --output-on-failure
```

Tests cover weight validation, cross-entropy updates, training reproducibility and seed separation, AI paths through overhangs, sealed cavities, seeded games and deterministic replay, interactive and batch tic-tac-toe rules, linked-list ownership, PIN recovery, numeric/text input, image pixel values and XOR round trips, Tetris scoring/levels/drop points and row clearing and every piece's rotations/spawn/collision, Fibonacci base cases and method agreement, flood-fill borders and large components, malformed input, and graphics startup/animation/shutdown. The window checks do not replace interactive playtesting. GitHub Actions runs the console matrix, Linux sanitizers, and Linux Allegro/Xvfb checks.

## Background and credit

This repository grew from C workshop and coursework exercises, with starter code, reference solutions, assets, and later personal work kept together. The exact division of the original implementation between workshop assistance and personal contributions was not recorded. Original notices and exercise statements are retained; the current maintenance pass adds correctness fixes, shared build support, tests, and documentation. The image exercises use a shared, pinned copy of [stb image codecs](third_party/stb/README.md), with their author and license notices retained.

## License

My own work here is under the [MIT License](LICENSE). It covers everything except:

- **Original course files:** the files in `Lab01`–`Lab05` and `Exam` that come from the 2019 uploads, including my later changes to them. They mix the course's statements, starter code, reference solutions and assets with my work, and the split was not recorded. `git ls-tree -r --name-only 5a18928 -- Lab01 Lab02 Lab03 Lab04 Lab05 Exam` lists them, including a few since removed. Files added to those folders since, such as the Makefiles and the Tetris AI in `Lab05/Solved/`, are covered.
- **`third_party/stb`:** keeps its own license, MIT or public domain ([LICENSE](third_party/stb/LICENSE)).
