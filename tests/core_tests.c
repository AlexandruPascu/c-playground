#include "PoliTicTacToe/game.h"
#include "Lab05/Solved/board.h"
#include "algorithms/fibonacci.h"
#include "algorithms/flood_fill.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #condition); return 1; } } while (0)
static int tic_tac_toe_checks(void) {
    char macro[NMAX1][NMAX1], cells[NMAX][NMAX];
    for (int direction = 0; direction < 4; ++direction) {
        for (int player = 0; player < 2; ++player) {
            memset(macro, '-', sizeof(macro));
            for (int i = 0; i < 3; ++i) {
                int row = direction == 0 ? 1 : i;
                int column = direction == 1 ? 1 : direction == 2 ? i : direction == 3 ? 2 - i : i;
                macro[row][column] = player ? '0' : 'X';
            }
            CHECK(end_game(macro, 3) == (player ? 2 : 1));
        }
    }
    memset(macro, '-', sizeof(macro));
    for (int i = 0; i < 3; ++i) { macro[0][i] = 'X'; macro[1][i] = '0'; }
    CHECK(end_game(macro, 3) == 0);
    memset(macro, '-', sizeof(macro)); memset(cells, '-', sizeof(cells));
    cells[8][6] = cells[8][7] = cells[8][8] = 'X';
    CHECK(verify_win(cells, 'X', 8, 8, 3, macro) == 1);
    CHECK(macro[2][2] == 'X');
    CHECK(verify_win(cells, 'X', 8, 8, 3, macro) == 0);
    memset(cells, '-', sizeof(cells));
    cells[0][0] = 'X';
    { int row = -100, column = INT_MAX;
      CHECK(round_robin(cells, '0', 2, &row, &column) == 0);
      CHECK(row == 1 && column == 1 && cells[1][1] == '0'); }
    memset(cells, 'X', sizeof(cells));
    { int row = 99, column = 99;
      CHECK(round_robin(cells, '0', 10, &row, &column) == 1);
      CHECK(row == 99 && column == 99); }
    puts("PASS tic-tac-toe rows, columns, diagonals, ties, ownership and fallback");
    return 0;
}
static int tetris_checks(void) {
    init_board();
    for (int x = 0; x < BOARD_WIDTH; ++x) board[0][x] = board[1][x] = POS_FILLED;
    board[2][3] = POS_FILLED; board[BOARD_HEIGHT - 1][0] = POS_FILLED;
    delete_possible_lines();
    CHECK(board[0][3] == POS_FILLED);
    CHECK(board[BOARD_HEIGHT - 3][0] == POS_FILLED);
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        CHECK(board[BOARD_HEIGHT - 1][x] == POS_FREE);
        CHECK(board[BOARD_HEIGHT - 2][x] == POS_FREE);
    }
    for (int y = 0; y < BOARD_HEIGHT; ++y)
        for (int x = 0; x < BOARD_WIDTH; ++x) board[y][x] = POS_FILLED;
    delete_possible_lines();
    for (int y = 0; y < BOARD_HEIGHT; ++y)
        for (int x = 0; x < BOARD_WIDTH; ++x) CHECK(board[y][x] == POS_FREE);
    for (int piece = 0; piece < 7; ++piece)
        for (int rotation = 0; rotation < 4; ++rotation) {
            int count = 0;
            init_board();
            { int x = BOARD_WIDTH / 2 + get_x_displacement(piece, rotation);
              int y = BOARD_HEIGHT - 1 + get_y_displacement(piece, rotation);
              if (!is_possible_movement(x, y, piece, rotation))
                  fprintf(stderr, "Invalid spawn: piece %d, rotation %d\n", piece, rotation);
              CHECK(is_possible_movement(x, y, piece, rotation));
              CHECK(place_piece(x, y, piece, rotation));
              CHECK(!is_possible_movement(x, y, piece, rotation)); }
            init_board();
            CHECK(place_piece(2, 5, piece, rotation));
            for (int y = 0; y < BOARD_HEIGHT; ++y)
                for (int x = 0; x < BOARD_WIDTH; ++x) count += board[y][x] != POS_FREE;
            CHECK(count == 4);
            CHECK(!place_piece(2, 5, piece, rotation));
            CHECK(!is_possible_movement(-5, 5, piece, rotation));
            CHECK(!is_possible_movement(2, BOARD_HEIGHT, piece, rotation));
            CHECK(!is_possible_movement(INT_MAX, INT_MIN, piece, rotation));
        }
    CHECK(!place_piece(0, 0, 7, 0)); CHECK(!place_piece(0, 0, 0, 4));
    puts("PASS Tetris consecutive/top row clearing, all piece rotations and collisions");
    return 0;
}
static int algorithm_checks(void) {
    const int known[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
    for (unsigned int i = 0; i < sizeof(known) / sizeof(known[0]); ++i) {
        CHECK(fibo_recursive(i) == known[i]);
        CHECK(fibo_iterative(i) == known[i]);
        CHECK(fibo_logarithmic(i) == known[i]);
    }
    for (unsigned int i = 0; i < 2000; ++i) CHECK(fibo_iterative(i) == fibo_logarithmic(i));
    CHECK(fibo_logarithmic(40) == 102334155 % FIBONACCI_MODULUS);
    { unsigned char edge[] = {0, 0, 0, 0, 0, 1, 0, 1, 1}; size_t area;
      CHECK(largest_component(edge, 3, 3, &area) && area == 3); }
    { unsigned char diagonal[] = {1, 0, 1, 0, 1, 0, 1, 0, 1}; size_t area;
      CHECK(largest_component(diagonal, 3, 3, &area) && area == 1); }
    { unsigned char one = 1; size_t area; CHECK(largest_component(&one, 1, 1, &area) && area == 1); }
    { unsigned char zero = 0; size_t area; CHECK(largest_component(&zero, 1, 1, &area) && area == 0); }
    { unsigned char invalid = 9; size_t area; CHECK(!largest_component(&invalid, 1, 1, &area)); }
    { size_t count = 100000, area; unsigned char *large = malloc(count);
      CHECK(large); memset(large, 1, count);
      CHECK(largest_component(large, 1, count, &area) && area == count); free(large); }
    puts("PASS Fibonacci base cases/method agreement and flood-fill edges/large components");
    return 0;
}
int main(void) {
    if (tic_tac_toe_checks() || tetris_checks() || algorithm_checks()) return 1;
    return 0;
}
