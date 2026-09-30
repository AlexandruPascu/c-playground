#ifndef PLAYGROUND_TETRIS_BOARD_H
#define PLAYGROUND_TETRIS_BOARD_H
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define POS_FREE 0
#define POS_FILLED 1
extern int board[BOARD_HEIGHT][BOARD_WIDTH];
void init_board(void);
int place_piece(int x, int y, int piece, int rotation);
int game_over(void);
void delete_line(int line);
int can_delete_line(int line);
void delete_possible_lines(void);
int is_free_block(int x, int y);
int is_possible_movement(int x, int y, int piece, int rotation);
int get_block(int piece, int rotation, int x, int y);
int get_x_displacement(int piece, int rotation);
int get_y_displacement(int piece, int rotation);
int next_rotation(int rotation);
#endif
