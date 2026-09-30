/* Board logic extracted from the workshop reference solution. */
#include "board.h"
#include "pieces.h"
#include <string.h>
int board[BOARD_HEIGHT][BOARD_WIDTH];
void init_board(void) { memset(board, 0, sizeof(board)); }
int is_free_block(int x, int y) {
    return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT &&
           board[y][x] == POS_FREE;
}
int is_possible_movement(int x, int y, int piece, int rotation) {
    if (piece < 0 || piece >= 7 || rotation < 0 || rotation >= 4 ||
        x < -4 || x >= BOARD_WIDTH || y < -4 || y >= BOARD_HEIGHT) return 0;
    for (int row = 0; row < 5; ++row)
        for (int column = 0; column < 5; ++column)
            if (get_block(piece, rotation, column, row) &&
                !is_free_block(x + column, y + row)) return 0;
    return 1;
}
int place_piece(int x, int y, int piece, int rotation) {
    if (!is_possible_movement(x, y, piece, rotation)) return 0;
    for (int row = 0; row < 5; ++row)
        for (int column = 0; column < 5; ++column)
            if (get_block(piece, rotation, column, row))
                board[y + row][x + column] = POS_FILLED;
    return 1;
}
int game_over(void) {
    for (int x = 0; x < BOARD_WIDTH; ++x)
        if (board[BOARD_HEIGHT - 1][x] != POS_FREE) return 1;
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
void delete_possible_lines(void) {
    for (int row = 0; row < BOARD_HEIGHT; ++row)
        while (can_delete_line(row)) delete_line(row);
}
