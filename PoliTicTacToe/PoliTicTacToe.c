// Copyright 2018 Alexandru Pascu (alexandru.pascu99@gmail.com)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NMAX 100
#define NMAX1 10

// function prints the macroboard and the attention coeffiecient of each player
void print_results(char macroboard[NMAX1][NMAX1], int n,
                    int win_moves_x, int win_moves_0,
                    int total_moves_x, int total_moves_0, char end) {
    double attention_x, attention_0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%c", macroboard[i][j]);
        }
        printf("\n");
    }
    if (end == '0') {
        printf("Draw again! Let's play darts!\n");
    }
    if (end == '1') {
        printf("X won\n");
    }
    if (end == '2') {
        printf("0 won\n");
    }

    if (total_moves_x) {
        attention_x = 1.0 * win_moves_x / total_moves_x;
        printf("X %.10f\n", attention_x);
    } else {
        printf("X N/A\n");
    }
    if (total_moves_0) {
        attention_0 = 1.0 * win_moves_0 / total_moves_0;
        printf("0 %.10f\n", attention_0);
    } else {
        printf("0 N/A\n");
    }
}

// function verifies the winner of the macroboard
int end_game(char macroboard[NMAX1][NMAX1], int n) {
int count_x = 0, count_0 = 0,
        wins_x = 0, wins_0 = 0;
    // linie
    for (int i = 0; i < n; ++i) {
        count_x = 0,
        count_0 = 0;
        for (int j = 0; j < n; ++j) {
            if (macroboard[i][j] == 'X') {
                ++count_x;
            }
            if (macroboard[i][j] == '0') {
                ++count_0;
            }
        }
        if (count_x == n)
          ++wins_x;
        else if (count_0 == n)
          ++wins_0;
    }

    count_0 = 0;
    count_x = 0;
    for (int k = 0; k < n; ++k) {
        count_x = 0,
        count_0 = 0;
        for (int l = 0; l < n; ++l) {
            if (macroboard[k][l] == 'X') {
                ++count_x;
            }
            if (macroboard[k][l] == '0') {
                ++count_0;
            }
        }
        if (count_x == n)
          ++wins_x;
        else if (count_0 == n)
          ++wins_0;
    }
    count_0 = 0;
    count_x = 0;
    for (int m = 0; m < n; ++m) {
        if (macroboard[m][m] == 'X') {
            ++count_x;
        }
        if (macroboard[m][m] == '0') {
            ++count_0;
        }
    }
    if (count_x == n)
      ++wins_x;
    else if (count_0 == n)
      ++wins_0;
    count_0 = 0;
    count_x = 0;
    for (int i = 0, j = n -1; i < n; ++i, --j) {
        if (macroboard[i][j] == 'X') {
            ++count_x;
        }
        if (macroboard[i][j] == '0') {
            ++count_0;
        }
    }
    if (count_x == n)
      ++wins_x;
    else if (count_0 == n)
      ++wins_0;
    if (wins_x == wins_0)
      return 0;
    else if (wins_x > wins_0)
      return 1;
    else
      return 2;
}

// function verifies in the microboard coresponding to the move if the player
// who made the move wins with this one and then completes the macroboard
int verify_win(char matrix[NMAX][NMAX], char player, const int x, int y, int n,
                                char macroboard[NMAX1][NMAX1]) {
    int x_macro, y_macro, count = 0;
    x_macro = x / n;
    y_macro = y / n;
    if (macroboard[x_macro][y_macro] != '-') {
        return 0;
    } else {
        // line
        for (int j = n * y_macro; j < n + n * y_macro; ++j) {
            if (matrix[x][j] == player) {
                ++count;
            }
        }
            if (count == n) {
                macroboard[x_macro][y_macro] = player;
                return 1;
            }
        // column
        count = 0;
        for (int i = n * x_macro; i < n +  n * x_macro; ++i) {
            if (matrix[i][y] == player) {
                ++count;
            }
        }
            if (count == n) {
                macroboard[x_macro][y_macro] = player;
                return 1;
            }
        count = 0;
        for (int i = x_macro * n, j = y_macro * n; i < (x_macro + 1) * n;
             ++i, ++j) {
            if (matrix[i][j] == player) {
                ++count;
            }
        }
        if (count == n) {
            macroboard[x_macro][y_macro] = player;
            return 1;
        }

        count = 0;
        for (int i = x_macro *n, j = (y_macro + 1) * n -1;
        i < (x_macro + 1) * n; ++i, --j) {
            if (matrix[i][j] == player) {
                ++count;
            }
        }
        if (count == n) {
            macroboard[x_macro][y_macro] = player;
            return 1;
        }
    }

    return 0;
}

// function searches to complete the invalid move in the matrix, when it does
// not find any place where to put it, we will know to ignore all the other
// moves
int round_robin(char matrix[NMAX][NMAX], char player, int n, int *x, int *y) {
    for (int i = 0; i < n * n; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k + i < n * n; ++k) {
                if (matrix[k + i * (j % 2)][k + i * ((j + 1) % 2)] == '-') {
                    matrix[k + i * (j % 2)][k + i * ((j + 1) % 2)] = player;
                    *x = k + i * (j % 2);
                    *y = k + i * ((j + 1) % 2);
                    return 0;
                }
            }
        }
    }
    return 1;
}

// function verifies each player's move conform to the game and completes the
// matrix with each valid move
int verify_data(int n, int m, char matrix[NMAX][NMAX]) {
    int x, y, move_x = 0, move_0 = 0, win_moves_x = 0, win_moves_0 = 0;
    int total_moves_x = 0, total_moves_0 = 0;
    char player, macroboard[NMAX1][NMAX1], end = 0;
    memset(macroboard, '-', sizeof(macroboard));
    for (int i = 0; i < m; ++i) {
        scanf("\n%c %d %d", &player, &x, &y);
        if (player == 'X') {
            ++move_x;
        } else {
            ++move_0;
        }
        if (move_x > move_0 + 1) {
            printf("NOT YOUR TURN\n");
            move_x -= 1;
        } else if (move_x < move_0) {
                printf("NOT YOUR TURN\n");
                move_0 -= 1;
        } else {
            if (player == 'X') {
            ++total_moves_x;
        } else {
            ++total_moves_0;
        }
            if (x >= 0 && y >= 0 && x < n * n && y < n * n) {
                if (matrix[x][y] != '-') {
                    printf("NOT AN EMPTY CELL\n");
                    if (round_robin(matrix, player, n, &x, &y) == 1 &&
                        i == m - 1) {
                    printf("FULL BOARD\n");
                }
                    verify_win(matrix, player, x, y, n, macroboard);
                } else {
                    matrix[x][y] = player;
                    if (verify_win(matrix, player, x, y, n, macroboard) == 1) {
                        if (player == 'X') {
                            ++win_moves_x;
                        } else {
                            ++win_moves_0;
                        }
                    }
                }
            } else {
                printf("INVALID INDEX\n");
                if (round_robin(matrix, player, n, &x, &y) == 1 &&
                    i == m - 1) {
                    printf("FULL BOARD\n");
                }
                verify_win(matrix, player, x, y, n, macroboard);
            }
        }
    }
    if (end_game(macroboard, n) == 0) {
        end = '0';
    }
    if (end_game(macroboard, n) == 1) {
        end = '1';
    }
    if (end_game(macroboard, n) == 2) {
        end = '2';
    }
    print_results(macroboard, n, win_moves_x, win_moves_0,
                   total_moves_x, total_moves_0, end);
    return 0;
}

int main() {
    int n, m;
    char matrix[NMAX][NMAX];
    memset(matrix, '-', sizeof(matrix));
    scanf("%d %d", &n, &m);
    verify_data(n, m, matrix);
    return 0;
}
