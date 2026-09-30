/* Playable driver for the workshop Tetris board and piece definitions. */
#include "board.h"
#include "../../graphics/window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 30
#define BORDER_SIZE 15
#define WIN_WIDTH (BOARD_WIDTH * BLOCK_SIZE + 2 * BORDER_SIZE)
#define WIN_HEIGHT (BOARD_HEIGHT * BLOCK_SIZE + 2 * BORDER_SIZE)

static void draw_block(int x, int y, ALLEGRO_COLOR color) {
    float left = (float)(BORDER_SIZE + x * BLOCK_SIZE);
    float top = (float)(WIN_HEIGHT - BORDER_SIZE - (y + 1) * BLOCK_SIZE);
    al_draw_filled_rectangle(left + 2, top + 2, left + BLOCK_SIZE - 2, top + BLOCK_SIZE - 2, color);
}
static void draw_game(int x, int y, int piece, int rotation, int ended) {
    al_clear_to_color(al_map_rgb(12, 18, 28));
    al_draw_rectangle(BORDER_SIZE - 2, BORDER_SIZE - 2, WIN_WIDTH - BORDER_SIZE + 2,
                      WIN_HEIGHT - BORDER_SIZE + 2, al_map_rgb(80, 105, 130), 2);
    for (int row = 0; row < BOARD_HEIGHT; ++row)
        for (int column = 0; column < BOARD_WIDTH; ++column)
            if (!is_free_block(column, row)) draw_block(column, row, al_map_rgb(210, 110, 80));
    if (!ended)
        for (int row = 0; row < 5; ++row)
            for (int column = 0; column < 5; ++column)
                if (get_block(piece, rotation, column, row))
                    draw_block(x + column, y + row, al_map_rgb(90, 210, 230));
    al_flip_display();
}
static int spawn_piece(int *x, int *y, int *piece, int *rotation) {
    *piece = rand() % 7;
    *rotation = rand() % 4;
    *x = BOARD_WIDTH / 2 + get_x_displacement(*piece, *rotation);
    *y = BOARD_HEIGHT - 1 + get_y_displacement(*piece, *rotation);
    return is_possible_movement(*x, *y, *piece, *rotation);
}
int main(int argc, char **argv) {
    PlaygroundWindow window;
    ALLEGRO_EVENT event;
    int smoke = 0, piece, rotation, x, y, ended, redraw = 1;
    double elapsed, accumulated = 0;
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        puts("Usage: tetris [--smoke-test]\nA/D or Left/Right: move; W/Up: rotate; S/Down: lower; R: restart; Esc: close.");
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "--smoke-test") == 0) smoke = 1;
    else if (argc != 1) { fputs("Usage: tetris [--smoke-test]\n", stderr); return 1; }
    if (!pg_window_open(&window, "Tetris - arrows/WASD, R restart, Esc close", WIN_WIDTH, WIN_HEIGHT, 0, smoke)) return 1;
    srand(smoke ? 1u : (unsigned int)time(NULL));
    init_board();
    ended = !spawn_piece(&x, &y, &piece, &rotation);
    while (pg_window_next(&window, &event, &elapsed)) {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            int key = event.keyboard.keycode;
            if (key == ALLEGRO_KEY_R) {
                init_board();
                ended = !spawn_piece(&x, &y, &piece, &rotation);
                accumulated = 0;
                al_set_window_title(window.display, "Tetris - arrows/WASD, R restart, Esc close");
            } else if (!ended) {
                if ((key == ALLEGRO_KEY_D || key == ALLEGRO_KEY_RIGHT) && is_possible_movement(x + 1, y, piece, rotation)) ++x;
                if ((key == ALLEGRO_KEY_A || key == ALLEGRO_KEY_LEFT) && is_possible_movement(x - 1, y, piece, rotation)) --x;
                if ((key == ALLEGRO_KEY_W || key == ALLEGRO_KEY_UP) && is_possible_movement(x, y, piece, next_rotation(rotation)))
                    rotation = next_rotation(rotation);
                if ((key == ALLEGRO_KEY_S || key == ALLEGRO_KEY_DOWN) && is_possible_movement(x, y - 1, piece, rotation)) --y;
            }
            redraw = 1;
        }
        if (event.type == ALLEGRO_EVENT_TIMER && !ended) {
            accumulated += elapsed;
            if (accumulated >= 1.0) {
                accumulated -= 1.0;
                if (is_possible_movement(x, y - 1, piece, rotation)) --y;
                else {
                    if (!place_piece(x, y, piece, rotation)) ended = 1;
                    delete_possible_lines();
                    if (game_over() || !spawn_piece(&x, &y, &piece, &rotation)) ended = 1;
                }
                redraw = 1;
                if (ended) al_set_window_title(window.display, "Tetris - game over! R restart, Esc close");
            }
        }
        if (redraw) { draw_game(x, y, piece, rotation, ended); redraw = 0; }
    }
    pg_window_close(&window);
    return 0;
}
