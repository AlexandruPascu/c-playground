#include "game.h"
#include "../graphics/window.h"
#include "../common/input.h"
#include <string.h>
#define LEFT 24
#define TOP 76
#define SIZE 600
#define WIDTH 960
#define HEIGHT 730
static ALLEGRO_COLOR player_color(char player) {
    return player == 'X' ? al_map_rgb(90, 210, 230) : al_map_rgb(255, 175, 110);
}
static void draw_mark(char mark, float x, float y, float size) {
    float padding = size * 0.25f, thickness = size > 40 ? 3 : 2;
    if (mark == 'X') {
        al_draw_line(x + padding, y + padding, x + size - padding, y + size - padding, player_color(mark), thickness);
        al_draw_line(x + size - padding, y + padding, x + padding, y + size - padding, player_color(mark), thickness);
    } else if (mark == '0') al_draw_circle(x + size / 2, y + size / 2, size * 0.27f, player_color(mark), thickness);
}
static void draw(PlaygroundWindow *window, TicSession *game, int cursor_row, int cursor_column, const char *message) {
    int side = game->n * game->n, x_score, zero_score;
    float cell = SIZE / (float)side, macro = SIZE / (float)game->n;
    ALLEGRO_COLOR white = al_map_rgb(225, 235, 245), dim = al_map_rgb(90, 110, 135);
    al_clear_to_color(al_map_rgb(12, 18, 28));
    pg_text(window, LEFT, 20, 2.4f, white, "NESTED TIC-TAC-TOE");
    pg_text(window, 650, 30, 1.4f, white, "%d x %d microboards", game->n, game->n);
    for (int row = 0; row < side; ++row)
        for (int column = 0; column < side; ++column) {
            char owner = game->macro[row / game->n][column / game->n];
            float x = LEFT + column * cell, y = TOP + row * cell;
            if (owner != '-') al_draw_filled_rectangle(x, y, x + cell, y + cell,
                owner == 'X' ? al_map_rgb(18, 55, 67) : al_map_rgb(67, 40, 25));
            draw_mark(game->cells[row][column], x, y, cell);
        }
    for (int i = 0; i <= side; ++i) {
        float position = i * cell;
        float thickness = i % game->n == 0 ? 3 : 1;
        al_draw_line(LEFT + position, TOP, LEFT + position, TOP + SIZE, dim, thickness);
        al_draw_line(LEFT, TOP + position, LEFT + SIZE, TOP + position, dim, thickness);
    }
    if (!game->finished) al_draw_rectangle(LEFT + cursor_column * cell + 2, TOP + cursor_row * cell + 2,
            LEFT + (cursor_column + 1) * cell - 2, TOP + (cursor_row + 1) * cell - 2, al_map_rgb(245, 220, 115), 2);
    tic_line_scores(game->macro, game->n, &x_score, &zero_score);
    if (game->finished) {
        int winner = end_game(game->macro, game->n);
        pg_text(window, 650, 90, 2.5f, white, winner == 0 ? "DRAW" : winner == 1 ? "X WINS" : "0 WINS");
    } else pg_text(window, 650, 90, 2.5f, player_color(game->player), "%c TO MOVE", game->player);
    pg_text(window, 650, 142, 1.4f, white, "Macro lines: X %d / 0 %d", x_score, zero_score);
    pg_text(window, 650, 180, 1.2f, white, "Claimed microboards");
    macro = 180.0f / game->n;
    for (int row = 0; row < game->n; ++row)
        for (int column = 0; column < game->n; ++column) {
            float x = 650 + column * macro, y = 205 + row * macro;
            al_draw_rectangle(x, y, x + macro, y + macro, dim, 1);
            draw_mark(game->macro[row][column], x, y, macro);
        }
    pg_text(window, 650, 415, 1.2f, white, "Click a free cell to play");
    pg_text(window, 650, 439, 1.2f, white, "Arrows + Enter also work");
    pg_text(window, 650, 474, 1, white, "First local line claims a microboard.");
    pg_text(window, 650, 494, 1, white, "Most macro lines at the end wins.");
    pg_text(window, 650, 514, 1, white, "Any empty cell is allowed.");
    al_draw_filled_rounded_rectangle(650, 552, 930, 594, 6, 6, al_map_rgb(35, 60, 82));
    pg_text(window, 662, 565, 1.5f, white, "F: finish and score");
    al_draw_filled_rounded_rectangle(650, 606, 930, 648, 6, 6, al_map_rgb(35, 60, 82));
    pg_text(window, 662, 619, 1.5f, white, "R: restart");
    pg_text(window, LEFT, 697, 1.2f, white, "%s", message);
    al_flip_display();
}
int main(int argc, char **argv) {
    PlaygroundWindow window;
    ALLEGRO_EVENT event;
    TicSession game;
    int n = 3, smoke = 0, depth_arg = 0, row = 0, column = 0;
    double elapsed;
    const char *message = "Click a cell. X starts. Esc closes.";
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) { puts("Usage: tic_tac_toe_play [n] [--smoke-test]\nn=1..5 (default 3); playing grid is n*n by n*n. Two local players.\nClick a cell or use arrows/Enter; F finishes and scores; R restarts; Esc closes."); return 0; }
        if (!strcmp(argv[i], "--smoke-test")) smoke = 1;
        else if (depth_arg++ || !pg_parse_int(argv[i], &n) || n < 1 || n > 5) { fputs("Board size must be 1..5.\n", stderr); return 1; }
    }
    tic_start(&game, n);
    if (!pg_window_open(&window, "Nested tic-tac-toe - two players", WIDTH, HEIGHT, 0, smoke)) return 1;
    if (!pg_window_mouse(&window)) { pg_window_close(&window); return 1; }
    while (pg_window_next(&window, &event, &elapsed)) {
        int place = 0, restart = 0, finish = 0, side = n * n;
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
            int x = event.mouse.x, y = event.mouse.y;
            if (x >= LEFT && x < LEFT + SIZE && y >= TOP && y < TOP + SIZE) {
                row = (y - TOP) * side / SIZE; column = (x - LEFT) * side / SIZE; place = 1;
            }
            if (x >= 650 && x < 930 && y >= 552 && y < 594) finish = 1;
            if (x >= 650 && x < 930 && y >= 606 && y < 648) restart = 1;
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            int key = event.keyboard.keycode;
            if (key == ALLEGRO_KEY_LEFT && column > 0) --column;
            if (key == ALLEGRO_KEY_RIGHT && column + 1 < side) ++column;
            if (key == ALLEGRO_KEY_UP && row > 0) --row;
            if (key == ALLEGRO_KEY_DOWN && row + 1 < side) ++row;
            place = key == ALLEGRO_KEY_ENTER || key == ALLEGRO_KEY_SPACE;
            restart = key == ALLEGRO_KEY_R;
            finish = key == ALLEGRO_KEY_F;
        }
        if (restart) { tic_start(&game, n); row = column = 0; message = "New game. X starts."; }
        else if (finish) { game.finished = 1; message = "Final score. Press R to play again."; }
        else if (place) message = tic_move(&game, row, column) ? "Move accepted." : "Choose an empty cell, or restart if the game has ended.";
        if (smoke && event.type == ALLEGRO_EVENT_TIMER && window.frames == 1) tic_move(&game, 0, 0);
        if (event.type == ALLEGRO_EVENT_TIMER || place || restart || finish) draw(&window, &game, row, column, message);
    }
    pg_window_close(&window);
    return 0;
}
