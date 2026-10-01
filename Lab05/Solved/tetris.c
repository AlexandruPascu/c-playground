/* Playable driver for the workshop Tetris board and piece definitions. */
#include "board.h"
#include "score.h"
#include "ai.h"
#include "../../common/input.h"
#include "../../graphics/window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 30
#define BORDER_SIZE 15
#define BOARD_RIGHT (BOARD_WIDTH * BLOCK_SIZE + 2 * BORDER_SIZE)
#define WIN_WIDTH (BOARD_RIGHT + 260)
#define WIN_HEIGHT (BOARD_HEIGHT * BLOCK_SIZE + 2 * BORDER_SIZE)

static void draw_block(int x, int y, ALLEGRO_COLOR color) {
    float left = (float)(BORDER_SIZE + x * BLOCK_SIZE);
    float top = (float)(WIN_HEIGHT - BORDER_SIZE - (y + 1) * BLOCK_SIZE);
    al_draw_filled_rectangle(left + 2, top + 2, left + BLOCK_SIZE - 2, top + BLOCK_SIZE - 2, color);
}
static void draw_target(TetrisPose pose) {
    for (int row = 0; row < 5; ++row)
        for (int column = 0; column < 5; ++column)
            if (get_block(pose.piece, pose.rotation, column, row)) {
                float left = (float)(BORDER_SIZE + (pose.x + column) * BLOCK_SIZE);
                float top = (float)(WIN_HEIGHT - BORDER_SIZE - (pose.y + row + 1) * BLOCK_SIZE);
                al_draw_rectangle(left + 4, top + 4, left + BLOCK_SIZE - 4, top + BLOCK_SIZE - 4,
                                  al_map_rgb(120, 200, 130), 2);
            }
}
static void draw_game(PlaygroundWindow *window, int x, int y, int piece, int rotation, int ended, int paused, const TetrisScore *score, uint64_t best, int autoplay, const TetrisPlan *plan) {
    al_clear_to_color(al_map_rgb(12, 18, 28));
    al_draw_rectangle(BORDER_SIZE - 2, BORDER_SIZE - 2, BOARD_RIGHT - BORDER_SIZE + 2,
                      WIN_HEIGHT - BORDER_SIZE + 2, al_map_rgb(80, 105, 130), 2);
    for (int row = 0; row < BOARD_HEIGHT; ++row)
        for (int column = 0; column < BOARD_WIDTH; ++column)
            if (!is_free_block(column, row)) draw_block(column, row, al_map_rgb(210, 110, 80));
    if (autoplay && !ended && plan->count) draw_target(plan->landing);
    if (!ended)
        for (int row = 0; row < 5; ++row)
            for (int column = 0; column < 5; ++column)
                if (get_block(piece, rotation, column, row))
                    draw_block(x + column, y + row, al_map_rgb(90, 210, 230));
    {
        float left = BOARD_RIGHT + 12;
        ALLEGRO_COLOR white = al_map_rgb(225, 235, 245), cyan = al_map_rgb(90, 210, 230);
        pg_text(window, left, 25, 2.5f, cyan, "TETRIS");
        pg_text(window, left, 82, 1.5f, white, "SCORE");
        pg_text(window, left, 106, 2, cyan, "%llu", (unsigned long long)score->points);
        pg_text(window, left, 152, 1.5f, white, "LINES  %llu", (unsigned long long)score->lines);
        pg_text(window, left, 184, 1.5f, white, "LEVEL  %d / 20", score->level);
        pg_text(window, left, 230, 1, white, "BEST THIS SESSION: %llu", (unsigned long long)best);
        pg_text(window, left, 286, 1.5f, cyan, ended ? "GAME OVER" : paused ? "PAUSED" : autoplay ? "AI PLAYING" : "YOUR TURN");
        pg_text(window, left, 315, 1, white, autoplay ? "F2: take over from AI" : "F2: enable AI");
        pg_text(window, left, 344, 1.3f, white, "Arrows / WASD");
        pg_text(window, left, 370, 1.3f, white, "Up / W: rotate");
        pg_text(window, left, 396, 1.3f, white, "Down / S: soft drop");
        pg_text(window, left, 422, 1.3f, white, "Space: hard drop");
        pg_text(window, left, 448, 1.3f, white, "P: pause   R: restart");
        pg_text(window, left, 474, 1.3f, white, "Esc: close");
        pg_text(window, left, 512, 1, white, autoplay ? "Green outline: AI target" : "F2: watch the AI play");
        pg_text(window, left, 535, 1, white, "Clear 1/2/3/4 lines:");
        pg_text(window, left, 555, 1, white, "100/300/500/800 x level");
        pg_text(window, left, 582, 1, white, "Level up every 10 lines");
    }
    al_flip_display();
}
static int lock_piece(TetrisPose *pose, uint32_t *random_state, TetrisScore *score) {
    if (!place_piece(pose->x, pose->y, pose->piece, pose->rotation)) return 1;
    tetris_score_clear(score, delete_possible_lines());
    return game_over() || !tetris_spawn(&tetris_board, random_state, pose);
}
int main(int argc, char **argv) {
    PlaygroundWindow window;
    ALLEGRO_EVENT event;
    int smoke = 0, ended, paused = 0, redraw = 1, autoplay = 0, seeded = 0, seed = 1;
    int next_action = 0, ai_placed = 0, failed = 0;
    TetrisPose pose;
    TetrisPlan plan = {0};
    TetrisScore score;
    uint32_t random_state;
    uint64_t best = 0;
    double elapsed, accumulated = 0, ai_elapsed = 0;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) {
            puts("Usage: tetris [--ai] [--seed 1..2147483647] [--smoke-test]\nA/D or Left/Right: move; W/Up: rotate; S/Down: soft drop; Space: hard drop;\nF2: toggle AI / take over; P: pause; R: restart; Esc: close.\nAI advances through legal actions at its own viewing pace. --seed makes restarts repeatable.");
            return 0;
        }
        if (!strcmp(argv[i], "--smoke-test")) smoke = 1;
        else if (!strcmp(argv[i], "--ai")) autoplay = 1;
        else if (!strcmp(argv[i], "--seed") && i + 1 < argc && pg_parse_int(argv[i + 1], &seed) && seed > 0) { seeded = 1; ++i; }
        else { fputs("Invalid arguments. Use tetris --help.\n", stderr); return 1; }
    }
    random_state = seeded || smoke ? (uint32_t)seed : (uint32_t)time(NULL);
    if (!pg_window_open(&window, "Tetris - F2 AI, P pause, R restart", WIN_WIDTH, WIN_HEIGHT, 0, smoke)) return 1;
    init_board();
    tetris_score_reset(&score);
    ended = !tetris_spawn(&tetris_board, &random_state, &pose);
    while (pg_window_next(&window, &event, &elapsed)) {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            int key = event.keyboard.keycode;
            if (key == ALLEGRO_KEY_R) {
                init_board(); tetris_score_reset(&score);
                if (seeded || smoke) random_state = (uint32_t)seed;
                paused = 0;
                ended = !tetris_spawn(&tetris_board, &random_state, &pose);
                accumulated = ai_elapsed = 0; plan.count = 0;
            } else if (key == ALLEGRO_KEY_F2) {
                autoplay = !autoplay;
                accumulated = ai_elapsed = 0; plan.count = 0;
            } else if (key == ALLEGRO_KEY_P && !ended) { paused = !paused; accumulated = ai_elapsed = 0; }
            else if (!ended && !paused && !autoplay) {
                TetrisAction action = TETRIS_LEFT;
                int move = 1, distance;
                switch (key) {
                    case ALLEGRO_KEY_A: case ALLEGRO_KEY_LEFT: action = TETRIS_LEFT; break;
                    case ALLEGRO_KEY_D: case ALLEGRO_KEY_RIGHT: action = TETRIS_RIGHT; break;
                    case ALLEGRO_KEY_W: case ALLEGRO_KEY_UP: action = TETRIS_ROTATE; break;
                    case ALLEGRO_KEY_S: case ALLEGRO_KEY_DOWN: action = TETRIS_DOWN; break;
                    case ALLEGRO_KEY_SPACE: action = TETRIS_DROP; break;
                    default: move = 0; break;
                }
                if (move && (distance = tetris_step(&tetris_board, &pose, action)) >= 0) {
                    if (action == TETRIS_DOWN || action == TETRIS_DROP)
                        tetris_score_drop(&score, distance, action == TETRIS_DROP);
                    if (action == TETRIS_DROP) {
                        ended = lock_piece(&pose, &random_state, &score);
                        accumulated = 0;
                    }
                }
            }
            redraw = 1;
        }
        if (event.type == ALLEGRO_EVENT_TIMER && !ended && !paused) {
            if (autoplay) {
                /* AI owns the movement clock. Extra gravity between planned actions could
                   invalidate a route; human play keeps the usual level-dependent gravity. */
                ai_elapsed += elapsed;
                if (!plan.count) {
                    if (!tetris_ai_plan(&tetris_board, pose, &plan)) { failed = 1; break; }
                    next_action = 0;
                    redraw = 1;
                }
                if (smoke || ai_elapsed >= 0.08) {
                    TetrisAction action = plan.actions[next_action++];
                    int distance = tetris_step(&tetris_board, &pose, action);
                    if (distance < 0) { failed = 1; break; }
                    ai_elapsed = 0;
                    if (action == TETRIS_DOWN || action == TETRIS_DROP)
                        tetris_score_drop(&score, distance, action == TETRIS_DROP);
                    if (action == TETRIS_DROP) {
                        ended = lock_piece(&pose, &random_state, &score);
                        ++ai_placed;
                        plan.count = 0;
                    }
                    redraw = 1;
                }
            } else {
                accumulated += elapsed;
                if (accumulated >= tetris_fall_interval(&score)) {
                    accumulated -= tetris_fall_interval(&score);
                    if (tetris_step(&tetris_board, &pose, TETRIS_DOWN) < 0)
                        ended = lock_piece(&pose, &random_state, &score);
                    redraw = 1;
                }
            }
        }
        if (score.points > best) best = score.points;
        if (redraw) {
            draw_game(&window, pose.x, pose.y, pose.piece, pose.rotation, ended, paused, &score, best, autoplay, &plan);
            redraw = 0;
        }
    }
    pg_window_close(&window);
    if (failed) { fputs("AI could not execute its planned move.\n", stderr); return 1; }
    if (smoke && autoplay) {
        printf("AI smoke test: %d pieces placed.\n", ai_placed);
        if (!ai_placed) return 1;
    }
    return 0;
}
