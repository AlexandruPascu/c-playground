#ifndef PLAYGROUND_TIC_TAC_TOE_H
#define PLAYGROUND_TIC_TAC_TOE_H
#define NMAX 100
#define NMAX1 10
int end_game(char macroboard[NMAX1][NMAX1], int n);
int verify_win(char matrix[NMAX][NMAX], char player, int row, int column, int n,
               char macroboard[NMAX1][NMAX1]);
int round_robin(char matrix[NMAX][NMAX], char player, int n, int *row, int *column);
typedef struct {
    char cells[NMAX][NMAX], macro[NMAX1][NMAX1];
    int n, occupied, finished, turns[2], claims[2];
    char player;
} TicSession;
void tic_line_scores(char macro[NMAX1][NMAX1], int n, int *x, int *zero);
int tic_start(TicSession *game, int n);
int tic_move(TicSession *game, int row, int column);
#endif
