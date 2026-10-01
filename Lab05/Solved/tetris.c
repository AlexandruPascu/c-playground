/* Playable driver for the workshop Tetris board and piece definitions. */
#include "board.h"
#include "score.h"
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
static void draw_game(PlaygroundWindow *window, int x, int y, int piece, int rotation, int ended, int paused, const TetrisScore *score, uint64_t best) {
    al_clear_to_color(al_map_rgb(12, 18, 28));
    al_draw_rectangle(BORDER_SIZE - 2, BORDER_SIZE - 2, BOARD_RIGHT - BORDER_SIZE + 2,
                      WIN_HEIGHT - BORDER_SIZE + 2, al_map_rgb(80, 105, 130), 2);
    for (int row = 0; row < BOARD_HEIGHT; ++row)
        for (int column = 0; column < BOARD_WIDTH; ++column)
            if (!is_free_block(column, row)) draw_block(column, row, al_map_rgb(210, 110, 80));
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
        pg_text(window, left, 286, 1.5f, cyan, ended ? "GAME OVER" : paused ? "PAUSED" : "KEEP CLEARING!");
        pg_text(window, left, 344, 1.3f, white, "Arrows / WASD");
        pg_text(window, left, 370, 1.3f, white, "Up / W: rotate");
        pg_text(window, left, 396, 1.3f, white, "Down / S: soft drop");
        pg_text(window, left, 422, 1.3f, white, "Space: hard drop");
        pg_text(window, left, 448, 1.3f, white, "P: pause   R: restart");
        pg_text(window, left, 474, 1.3f, white, "Esc: close");
        pg_text(window, left, 535, 1, white, "Clear 1/2/3/4 lines:");
        pg_text(window, left, 555, 1, white, "100/300/500/800 x level");
        pg_text(window, left, 582, 1, white, "Level up every 10 lines");
    }
    al_flip_display();
}
static int spawn_piece(int *x, int *y, int *piece, int *rotation) {
    *piece = rand() % 7;
    *rotation = rand() % 4;
    *x = BOARD_WIDTH / 2 + get_x_displacement(*piece, *rotation);
    *y = BOARD_HEIGHT - 1 + get_y_displacement(*piece, *rotation);
    return is_possible_movement(*x, *y, *piece, *rotation);
}
static int lock_piece(int *x, int *y, int *piece, int *rotation, TetrisScore *score) {
    if (!place_piece(*x, *y, *piece, *rotation)) return 1;
    tetris_score_clear(score, delete_possible_lines());
    return game_over() || !spawn_piece(x, y, piece, rotation);
}
int main(int argc, char **argv) {
    PlaygroundWindow window;
    ALLEGRO_EVENT event;
    int smoke = 0, piece, rotation, x, y, ended, paused = 0, redraw = 1;
    TetrisScore score;
    uint64_t best = 0;
    double elapsed, accumulated = 0;
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        puts("Usage: tetris [--smoke-test]\nA/D or Left/Right: move; W/Up: rotate; S/Down: soft drop; Space: hard drop; P: pause; R: restart; Esc: close.");
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "--smoke-test") == 0) smoke = 1;
    else if (argc != 1) { fputs("Usage: tetris [--smoke-test]\n", stderr); return 1; }
    if (!pg_window_open(&window, "Tetris - arrows/WASD, R restart, Esc close", WIN_WIDTH, WIN_HEIGHT, 0, smoke)) return 1;
    srand(smoke ? 1u : (unsigned int)time(NULL));
    init_board();
    tetris_score_reset(&score);
    ended = !spawn_piece(&x, &y, &piece, &rotation);
    while (pg_window_next(&window, &event, &elapsed)) {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            int key = event.keyboard.keycode;
            if (key == ALLEGRO_KEY_R) {
                init_board();
                tetris_score_reset(&score);
                paused = 0;
                ended = !spawn_piece(&x, &y, &piece, &rotation);
                accumulated = 0;
                al_set_window_title(window.display, "Tetris - arrows/WASD, R restart, Esc close");
            } else if (key == ALLEGRO_KEY_P && !ended) { paused = !paused; accumulated = 0; }
            else if (!ended && !paused) {
                if ((key == ALLEGRO_KEY_D || key == ALLEGRO_KEY_RIGHT) && is_possible_movement(x + 1, y, piece, rotation)) ++x;
                if ((key == ALLEGRO_KEY_A || key == ALLEGRO_KEY_LEFT) && is_possible_movement(x - 1, y, piece, rotation)) --x;
                if ((key == ALLEGRO_KEY_W || key == ALLEGRO_KEY_UP) && is_possible_movement(x, y, piece, next_rotation(rotation)))
                    rotation = next_rotation(rotation);
                if ((key == ALLEGRO_KEY_S || key == ALLEGRO_KEY_DOWN) && is_possible_movement(x, y - 1, piece, rotation)) {
                    --y; tetris_score_drop(&score, 1, 0);
                }
                if (key == ALLEGRO_KEY_SPACE) {
                    int distance = 0;
                    while (is_possible_movement(x, y - 1, piece, rotation)) { --y; ++distance; }
                    tetris_score_drop(&score, distance, 1);
                    ended = lock_piece(&x, &y, &piece, &rotation, &score);
                    accumulated = 0;
                }
            }
            redraw = 1;
        }
        if (event.type == ALLEGRO_EVENT_TIMER && !ended && !paused) {
            accumulated += elapsed;
            if (accumulated >= tetris_fall_interval(&score)) {
                accumulated -= tetris_fall_interval(&score);
                if (is_possible_movement(x, y - 1, piece, rotation)) --y;
                else ended = lock_piece(&x, &y, &piece, &rotation, &score);
                redraw = 1;
                if (ended) al_set_window_title(window.display, "Tetris - game over! R restart, Esc close");
            }
        }
        if (score.points > best) best = score.points;
        if (redraw) { draw_game(&window, x, y, piece, rotation, ended, paused, &score, best); redraw = 0; }
    }
    pg_window_close(&window);
    return 0;
}
