/* Board logic extracted from the workshop reference solution. */
#include "board.h"
#include "pieces.h"
#include <string.h>
TetrisBoard tetris_board;
void init_board(void) { memset(board, 0, sizeof(board)); }
int is_free_block(int x, int y) {
    return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT &&
           board[y][x] == POS_FREE;
}
int tetris_board_can_move(const TetrisBoard *state, int x, int y, int piece, int rotation) {
    if (piece < 0 || piece >= 7 || rotation < 0 || rotation >= 4 ||
        x < -4 || x >= BOARD_WIDTH || y < -4 || y >= BOARD_HEIGHT) return 0;
    for (int row = 0; row < 5; ++row)
        for (int column = 0; column < 5; ++column)
            if (get_block(piece, rotation, column, row) &&
                (x + column < 0 || x + column >= BOARD_WIDTH || y + row < 0 ||
                 y + row >= BOARD_HEIGHT || state->cells[y + row][x + column] != POS_FREE)) return 0;
    return 1;
}
int tetris_board_place(TetrisBoard *state, int x, int y, int piece, int rotation) {
    if (!tetris_board_can_move(state, x, y, piece, rotation)) return 0;
    for (int row = 0; row < 5; ++row)
        for (int column = 0; column < 5; ++column)
            if (get_block(piece, rotation, column, row))
                state->cells[y + row][x + column] = POS_FILLED;
    return 1;
}
int tetris_board_over(const TetrisBoard *state) {
    for (int x = 0; x < BOARD_WIDTH; ++x)
        if (state->cells[BOARD_HEIGHT - 1][x] != POS_FREE) return 1;
    return 0;
}
void delete_line(int line) {
    if (line < 0 || line >= BOARD_HEIGHT) return;
    for (int row = line; row < BOARD_HEIGHT - 1; ++row)
        memcpy(board[row], board[row + 1], sizeof(board[row]));
    memset(board[BOARD_HEIGHT - 1], 0, sizeof(board[BOARD_HEIGHT - 1]));
}
int can_delete_line(int line) {
    if (line < 0 || line >= BOARD_HEIGHT) return 0;
    for (int x = 0; x < BOARD_WIDTH; ++x)
        if (board[line][x] == POS_FREE) return 0;
    return 1;
}
int tetris_board_clear(TetrisBoard *state) {
    int destination = 0, cleared = 0;
    for (int row = 0; row < BOARD_HEIGHT; ++row) {
        int full = 1;
        for (int x = 0; x < BOARD_WIDTH; ++x)
            if (state->cells[row][x] == POS_FREE) full = 0;
        if (full) ++cleared;
        else {
            if (destination != row) memcpy(state->cells[destination], state->cells[row], sizeof(state->cells[row]));
            ++destination;
        }
    }
    while (destination < BOARD_HEIGHT) memset(state->cells[destination++], 0, sizeof(state->cells[0]));
    return cleared;
}
int is_possible_movement(int x, int y, int piece, int rotation) {
    return tetris_board_can_move(&tetris_board, x, y, piece, rotation);
}
int place_piece(int x, int y, int piece, int rotation) {
    return tetris_board_place(&tetris_board, x, y, piece, rotation);
}
int game_over(void) { return tetris_board_over(&tetris_board); }
int delete_possible_lines(void) { return tetris_board_clear(&tetris_board); }
