#include "workshop/list.h"
#include "workshop/pin.h"
#include "workshop/image_ops.h"
#include "PoliTicTacToe/game.h"
#include "Lab05/Solved/board.h"
#include "Lab05/Solved/score.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#define CHECK(value) do { if (!(value)) { fprintf(stderr, "FAIL %d: %s\n", __LINE__, #value); return 1; } } while (0)
static int lists(void) {
    PgList list = {0};
    int value, minimum, maximum;
    const int input[] = {7, 1, 7, 0, INT_MAX, 3};
    const int sorted[] = {0, 1, 3, 7, 7, INT_MAX};
    CHECK(!pg_list_pop(&list, &value, 0));
    CHECK(!pg_list_extrema(&list, &minimum, &maximum));
    for (int i = 0; i < 6; ++i) CHECK(pg_list_insert_sorted(&list, input[i]));
    CHECK(list.size == 6 && list.tail->value == INT_MAX);
    CHECK(pg_list_contains(&list, 7) && !pg_list_contains(&list, -1));
    CHECK(pg_list_extrema(&list, &minimum, &maximum) && minimum == 0 && maximum == INT_MAX);
    for (int i = 0; i < 6; ++i) { CHECK(pg_list_pop(&list, &value, 1)); CHECK(value == sorted[i]); }
    CHECK(!list.head && !list.tail && !list.size);
    CHECK(pg_list_push(&list, 10, 0)); CHECK(pg_list_push(&list, -3, 1)); CHECK(pg_list_push(&list, 5, 0));
    CHECK(pg_list_pop(&list, &value, 0) && value == 5 && list.tail->value == 10);
    CHECK(pg_list_pop(&list, &value, 0) && value == 10 && list.head == list.tail);
    pg_list_clear(&list); pg_list_clear(&list);
    CHECK(!list.head && !list.tail && !list.size);
    return 0;
}
static int images_and_pin(void) {
    char pin[5];
    unsigned char gray[] = {255,0,0,13, 0,255,0,27, 0,0,255,0, 255,255,255,255};
    unsigned char weighted[sizeof(gray)], original[] = {0, 1, 2, 3, 255, 17, 0, 0, 128, 64, 32, 16};
    unsigned char encrypted[sizeof(original)];
    CHECK(pg_pin_hash("1234") == (uint32_t)(int32_t)-946452264);
    CHECK(pg_find_pin((uint32_t)(int32_t)-503189101, pin) && !strcmp(pin, "4903"));
    CHECK(pg_prime_below(200) == 199 && pg_prime_below(3) == 2 && pg_prime_below(2) == 0);
    memcpy(weighted, gray, sizeof(gray));
    pg_gray(gray, 4, 0); pg_gray(weighted, 4, 1);
    CHECK(gray[0] == 85 && gray[4] == 85 && gray[8] == 85 && gray[12] == 255);
    CHECK(weighted[0] == 54 && weighted[4] == 182 && weighted[8] == 18 && weighted[12] == 255);
    for (int i = 0; i < 4; ++i) {
        CHECK(gray[i*4] == gray[i*4+1] && gray[i*4] == gray[i*4+2]);
        CHECK(weighted[i*4] == weighted[i*4+1] && weighted[i*4] == weighted[i*4+2]);
        CHECK(gray[i*4+3] == weighted[i*4+3]);
    }
    memcpy(encrypted, original, sizeof(original));
    for (int pass = 0; pass < 127; ++pass)
        for (size_t i = 1; i < sizeof(encrypted); ++i) encrypted[i] ^= encrypted[i-1];
    pg_xor_decode(encrypted, sizeof(encrypted), 127);
    CHECK(!memcmp(original, encrypted, sizeof(original)));
    pg_xor_decode(encrypted, 0, 127); pg_xor_decode(encrypted, 1, 127); pg_xor_decode(encrypted, sizeof(encrypted), 0);
    CHECK(!memcmp(original, encrypted, sizeof(original)));
    return 0;
}
static int scores(void) {
    TetrisScore score;
    tetris_score_reset(&score);
    CHECK(score.points == 0 && score.lines == 0 && score.level == 1 && tetris_fall_interval(&score) == 1);
    for (int count = 1; count <= 4; ++count) tetris_score_clear(&score, count);
    CHECK(score.points == 1700 && score.lines == 10 && score.level == 2);
    tetris_score_clear(&score, 1);
    CHECK(score.points == 1900 && score.lines == 11);
    tetris_score_drop(&score, 3, 0); tetris_score_drop(&score, 5, 1);
    CHECK(score.points == 1913);
    tetris_score_clear(&score, 0); tetris_score_clear(&score, 5); tetris_score_drop(&score, -10, 1);
    CHECK(score.points == 1913 && score.lines == 11);
    score.lines = 189; score.level = 19;
    tetris_score_clear(&score, 1);
    CHECK(score.level == 20 && score.lines == 190 && tetris_fall_interval(&score) > 0);
    score.points = UINT64_MAX - 1;
    tetris_score_drop(&score, 2, 1);
    CHECK(score.points == UINT64_MAX);
    tetris_score_reset(&score);
    init_board();
    for (int row = 0; row < 4; ++row)
        for (int column = 0; column < BOARD_WIDTH; ++column) board[row][column] = POS_FILLED;
    tetris_score_clear(&score, delete_possible_lines());
    CHECK(score.points == 800 && score.lines == 4 && !game_over());
    CHECK(delete_possible_lines() == 0);
    return 0;
}
static int interactive_game(void) {
    TicSession game = {0};
    CHECK(!tic_start(&game, 0) && !tic_start(&game, 11));
    CHECK(tic_start(&game, 2));
    CHECK(!tic_move(&game, -1, 0) && game.player == 'X' && game.occupied == 0);
    CHECK(tic_move(&game, 0, 0));
    CHECK(!tic_move(&game, 0, 0) && game.player == '0' && game.occupied == 1);
    CHECK(tic_move(&game, 2, 0)); CHECK(tic_move(&game, 0, 1));
    CHECK(game.macro[0][0] == 'X' && game.claims[0] == 1 && game.turns[0] == 2);
    game.finished = 1;
    CHECK(!tic_move(&game, 3, 3));
    CHECK(tic_start(&game, 1) && game.occupied == 0 && game.turns[0] == 0);
    CHECK(tic_move(&game, 0, 0) && game.finished && end_game(game.macro, 1) == 1);
    CHECK(!tic_move(&game, 0, 0));
    return 0;
}
int main(void) {
    if (lists() || images_and_pin() || scores() || interactive_game()) return 1;
    puts("PASS list ownership/sorting, pixel transforms/XOR/PIN, Tetris scoring, and interactive game rules");
    return 0;
}
