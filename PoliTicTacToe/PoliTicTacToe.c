// Copyright 2018 Alexandru Pascu (alexandru.pascu99@gmail.com)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "../common/input.h"

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
void tic_line_scores(char macroboard[NMAX1][NMAX1], int n, int *x, int *zero) {
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
            if (macroboard[l][k] == 'X') {
                ++count_x;
            }
            if (macroboard[l][k] == '0') {
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
    *x = wins_x;
    *zero = wins_0;
}
int end_game(char macroboard[NMAX1][NMAX1], int n) {
    int x, zero;
    tic_line_scores(macroboard, n, &x, &zero);
    return x == zero ? 0 : x > zero ? 1 : 2;
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

/* Interactive play rejects illegal cells and leaves the turn unchanged. */
int tic_start(TicSession *game, int n) {
    if (n < 1 || n > NMAX1) return 0;
    memset(game, 0, sizeof(*game));
    memset(game->cells, '-', sizeof(game->cells));
    memset(game->macro, '-', sizeof(game->macro));
    game->n = n;
    game->player = 'X';
    return 1;
}
int tic_move(TicSession *game, int row, int column) {
    int index;
    if (game->finished || game->n < 1 || game->n > NMAX1 || row < 0 || column < 0 ||
        row >= game->n * game->n || column >= game->n * game->n || game->cells[row][column] != '-') return 0;
    index = game->player == 'X' ? 0 : 1;
    game->cells[row][column] = game->player;
    ++game->turns[index];
    if (verify_win(game->cells, game->player, row, column, game->n, game->macro)) ++game->claims[index];
    ++game->occupied;
    game->finished = game->occupied == game->n * game->n * game->n * game->n;
    game->player = game->player == 'X' ? '0' : 'X';
    return 1;
}

/* Invalid coordinates and occupied cells use the workshop's diagonal
 * fallback order. Only manually placed winning moves earn attention credit. */
#ifndef PLAYGROUND_NO_MAIN
static int play_moves(int n, int moves, char matrix[NMAX][NMAX]) {
    int wins_x = 0, wins_0 = 0, turns_x = 0, turns_0 = 0;
    int occupied = 0, full_reported = 0;
    const int capacity = n * n * n * n;
    char expected = 'X', macroboard[NMAX1][NMAX1];
    memset(macroboard, '-', sizeof(macroboard));
    for (int turn = 0; turn < moves; ++turn) {
        char token[8], player;
        int row, column, automatic = 0;
        if (pg_read_token(stdin, token, sizeof(token)) != 1 ||
            token[1] != '\0' || (token[0] != 'X' && token[0] != '0') ||
            pg_read_int(stdin, &row) != 1 || pg_read_int(stdin, &column) != 1) {
            fprintf(stderr, "Invalid move %d: expected X or 0 followed by two integers.\n", turn + 1);
            return EXIT_FAILURE;
        }
        player = token[0];
        if (occupied == capacity) {
            if (!full_reported) puts("FULL BOARD");
            full_reported = 1;
            continue;
        }
        if (player != expected) { puts("NOT YOUR TURN"); continue; }
        expected = player == 'X' ? '0' : 'X';
        if (player == 'X') ++turns_x; else ++turns_0;
        if (row < 0 || column < 0 || row >= n * n || column >= n * n) {
            puts("INVALID INDEX");
            automatic = 1;
        } else if (matrix[row][column] != '-') {
            puts("NOT AN EMPTY CELL");
            automatic = 1;
        }
        if (automatic) {
            if (round_robin(matrix, player, n, &row, &column)) {
                puts("FULL BOARD");
                continue;
            }
        } else matrix[row][column] = player;
        ++occupied;
        if (verify_win(matrix, player, row, column, n, macroboard) && !automatic) {
            if (player == 'X') ++wins_x; else ++wins_0;
        }
    }
    print_results(macroboard, n, wins_x, wins_0, turns_x, turns_0,
                  (char)('0' + end_game(macroboard, n)));
    return EXIT_SUCCESS;
}

static void print_interactive_board(const TicSession *game) {
    int side = game->n * game->n;
    printf("\n    ");
    for (int column = 0; column < side; ++column) printf("%3d", column);
    putchar('\n');
    for (int row = 0; row < side; ++row) {
        printf("%3d ", row);
        for (int column = 0; column < side; ++column) printf("%3c", game->cells[row][column]);
        putchar('\n');
        if ((row + 1) % game->n == 0) putchar('\n');
    }
}
static int play_interactive(int n) {
    TicSession game;
    char line[256];
    if (!tic_start(&game, n)) { fputs("Board size must be 1..10.\n", stderr); return 1; }
    puts("Two-player nested tic-tac-toe. Enter row column (zero-based), r to restart, or q to finish and score.\nThe winner has the most complete macroboard lines at the end.");
    while (!game.finished) {
        char first[64], second[64], extra[2];
        int row, column, fields;
        print_interactive_board(&game);
        printf("%c to move > ", game.player); fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        if (!strchr(line, '\n') && !feof(stdin)) {
            int c; while ((c = getchar()) != '\n' && c != EOF) { }
            puts("Input is too long. Try again."); continue;
        }
        fields = sscanf(line, "%63s %63s %1s", first, second, extra);
        if (fields == 1 && (!strcmp(first, "q") || !strcmp(first, "quit"))) break;
        if (fields == 1 && !strcmp(first, "r")) { tic_start(&game, n); continue; }
        if (fields != 2 || !pg_parse_int(first, &row) || !pg_parse_int(second, &column) || !tic_move(&game, row, column))
            puts("Invalid or occupied cell. Your turn is unchanged.");
    }
    print_interactive_board(&game);
    print_results(game.macro, n, game.claims[0], game.claims[1], game.turns[0], game.turns[1],
                  (char)('0' + end_game(game.macro, n)));
    return ferror(stdin) ? 1 : 0;
}

int main(int argc, char **argv) {
    int n, moves;
    char matrix[NMAX][NMAX];
    if (argc >= 2 && strcmp(argv[1], "--play") == 0) {
        n = 3;
        if (argc > 3 || (argc == 3 && !pg_parse_int(argv[2], &n))) { fputs("Use tic_tac_toe --play [n].\n", stderr); return 1; }
        return play_interactive(n);
    }
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        puts("Usage: tic_tac_toe < moves.txt\n       tic_tac_toe --play [n] (interactive two-player terminal game)\n"
             "Input: n move_count, then move_count lines: X|0 row column.\n"
             "n must be 1..10; the playing grid is (n*n) by (n*n).\n"
             "X starts. Coordinates are zero-based. Invalid cells use a diagonal fallback.");
        return EXIT_SUCCESS;
    }
    if (argc != 1 || pg_read_int(stdin, &n) != 1 || n < 1 || n > NMAX1 ||
        pg_read_int(stdin, &moves) != 1 || moves < 0) {
        fputs("Expected board size 1..10 and a nonnegative move count. Use --help.\n", stderr);
        return EXIT_FAILURE;
    }
    memset(matrix, '-', sizeof(matrix));
    return play_moves(n, moves, matrix);
}
#endif
