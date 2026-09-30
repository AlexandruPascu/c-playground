#ifndef PLAYGROUND_TIC_TAC_TOE_H
#define PLAYGROUND_TIC_TAC_TOE_H
#define NMAX 100
#define NMAX1 10
int end_game(char macroboard[NMAX1][NMAX1], int n);
int verify_win(char matrix[NMAX][NMAX], char player, int row, int column, int n,
               char macroboard[NMAX1][NMAX1]);
int round_robin(char matrix[NMAX][NMAX], char player, int n, int *row, int *column);
#endif
