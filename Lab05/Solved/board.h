#ifndef PLAYGROUND_TETRIS_BOARD_H
#define PLAYGROUND_TETRIS_BOARD_H
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define POS_FREE 0
#define POS_FILLED 1
typedef struct { int cells[BOARD_HEIGHT][BOARD_WIDTH]; } TetrisBoard;
extern TetrisBoard tetris_board;
/* Retain the workshop's array interface for its examples and tests. */
#define board (tetris_board.cells)
int tetris_board_can_move(const TetrisBoard *state, int x, int y, int piece, int rotation);
int tetris_board_place(TetrisBoard *state, int x, int y, int piece, int rotation);
int tetris_board_clear(TetrisBoard *state);
int tetris_board_over(const TetrisBoard *state);
void init_board(void);
int place_piece(int x, int y, int piece, int rotation);
int game_over(void);
void delete_line(int line);
int can_delete_line(int line);
int delete_possible_lines(void);
int is_free_block(int x, int y);
int is_possible_movement(int x, int y, int piece, int rotation);
int get_block(int piece, int rotation, int x, int y);
int get_x_displacement(int piece, int rotation);
int get_y_displacement(int piece, int rotation);
int next_rotation(int rotation);
#endif
