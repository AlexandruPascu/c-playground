# Workshop guide

Build instructions are in the [README](../README.md). Run commands below from the repository root. Each program supports `--help`, except the two original `linked_list` and `structs` demos. On Visual Studio builds, use `build/Release/*.exe`.

## Console exercises

| Exercise | Executable | What you interact with / learn |
| --- | --- | --- |
| Lab01/1 | `numbers` | Enter numbers until a negative value or EOF. Integers go to stdout; fractions go to stderr, rounded to two decimals. Stream handling and numeric classification. |
| Lab01/2 | `vector_sum` | Enter a count, then that many integers. Dynamically allocates the array, sums using a wide integer, and frees memory. |
| Lab01/3 | `fps` | Enter a count, then frame durations in seconds. Counts frames overlapping each one-second interval, including a frame in each interval it spans; prints the integer average. This follows the workshop's overlap metric, rather than completed-frame throughput. Durations must be at least 0.000001 seconds; total duration is capped at 1,000,000 seconds. |
| Lab02/1 | `fibonacci [n] [all\|recursive\|iterative\|matrix]` | Compare three ways of calculating the same sequence. Results are modulo 666013. Recursion repeats work, iteration keeps the last two values, and matrix exponentiation handles large indices efficiently. |
| Lab01/5 | `pin_lab [--recover] [accounts.txt]` | A toy account/PIN exercise. `--recover` brute-forces the shipped Razvan hash; normal mode accepts account names, PINs, and `see_balance`/`exit`. Demonstrates parsing and exhaustive search; the hash is not secure authentication. |
| Lab01/6 | `highlight` | Enter a text line, then a search word. Matching substrings are uppercased, ignoring case when matching. |
| Lab01/4 | `components [grid.txt]` | Finds the largest connected group of 1s using four-neighbour flood fill. File format: rows, columns, then the binary cells. No arguments uses the supplied grid. |
| Lab02/2 | `list_ops` | Commands: `push_front N`, `push_back N`, `pop_front`, `pop_back`, `find N`, `min`, `max`, `print`, `quit`. Explore pointer-linked nodes and their ownership. |
| Lab02/3 | `insertion_sort` | Enter a count, then nonnegative integers. Each value is inserted into its ordered position in a linked list; prints the sorted list. Worst-case quadratic time. |
| Lab02/4 | `image_convert [input output.bmp]` | Decode a JPEG/PNG/BMP and write BMP. No arguments converts the supplied JPEG. |
| Lab02/5 | `image_grayscale [input average.png weighted.png]` | Produce mean-RGB and weighted grayscale versions, preserving alpha. Compare their treatment of red, green, and blue. |
| Lab02/6 | `image_decode [input output.png [PIN]]` | Reverse the workshop's rolling-XOR encoding. The default recovers PIN 4903, finds prime 199, and performs 127 inverse passes. No arguments decodes the supplied image. |
| Lab05 AI | `tetris_ai --seed 1 --games 20 --pieces 1000` | Run repeatable Tetris games without a display; CSV reports pieces, lines, score, level, and why each game stopped. Use `--weights models/tetris/cem-v1.weights` for the learned policy. See [the planner guide](TETRIS_AI.md) and [cross-entropy training](TETRIS_TRAINING.md). |
| Lab02/Demo | `linked_list`, `structs` | Small fixed demonstrations of node operations and structures. |
| PoliTicTacToe | `tic_tac_toe --play 2` | Play a nested board in the terminal. Omit `--play` to evaluate a supplied move sequence. See the [complete rules](../PoliTicTacToe/README). |

Try the supplied inputs:

```sh
./build/numbers < examples/numbers.txt
./build/vector_sum < examples/vector_sum.txt
./build/fps < examples/fps.txt
./build/highlight < examples/highlight.txt
./build/list_ops < examples/list_ops.txt
./build/insertion_sort < examples/insertion_sort.txt
./build/pin_lab --recover
./build/fibonacci 30
./build/components
./build/image_convert
./build/image_grayscale
./build/image_decode
```

Image defaults are resolved at configuration time and write into `output/` inside that executable's build directory (for example, `build/output/`). Open those files in your image viewer to compare results. Explicit paths let you process other images; the decoder expects the workshop's RGBA byte encoding, not arbitrary encrypted files.

## Windowed exercises

Use `./build-graphics/<executable>`. Escape and the close button exit. `--smoke-test` runs briefly and closes automatically.

| Exercise | Executable | Interaction / purpose |
| --- | --- | --- |
| Lab03/1 | `hello_window` | Displays text for two seconds. Uses a built-in font by default; `--font path/to/font.ttf` loads your own TTF. |
| Lab03/2 | `primitive_art` | A composition made from drawing primitives. |
| Lab03/3 | `sierpinski_triangle [depth]` | Recursive triangle. Left/Right changes depth from 0–9. |
| Lab03/4 | `moving_shapes` | Two bouncing primitives, animated for five seconds. |
| Lab03/5 | `sierpinski_animation [depth]` | Builds successive recursion depths over time. |
| Lab04/1 | `window_events` | Minimal close/Escape event handling. |
| Lab04/2 | `event_logger` | Move the mouse or press keys; events are printed in the launching terminal. |
| Lab04/3 | `moving_square` | Move with held WASD/arrows; diagonal speed is normalized. |
| Lab04/4 | `coin_animation` | Cycles through a coin sprite sheet. |
| Lab04/5 | `character_animation` | Move the original character sprite with WASD/arrows; animation follows direction and stops when idle. |
| Lab05 | `tetris` | Play with arrows/WASD, Space to hard-drop, P to pause, R to restart. F2 toggles the autoplayer or returns control. `--ai --seed 1` starts a repeatable demonstration. Score, lines, level, and session best are visible. See [scoring rules](../README.md#graphics-with-allegro-5). |
| Exam/1 | `skeleton` | Move the animated skeleton with WASD/arrows, including diagonals. |
| Exam/2 | `sierpinski_carpet [depth]` | Recursive carpet. Left/Right changes depth from 0–5. |
| PoliTicTacToe | `tic_tac_toe_play [size]` | Click to play, arrows + Enter/Space also work. F scores early; R restarts. Default size 3, supported 1–5. |

These are individual exercises rather than a single application launcher. Original challenge statements and assets remain beside their entry points; shared code avoids divergent copies of the same solution.
