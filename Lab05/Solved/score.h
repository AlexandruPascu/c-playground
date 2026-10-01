#ifndef PLAYGROUND_TETRIS_SCORE_H
#define PLAYGROUND_TETRIS_SCORE_H
#include <stdint.h>
typedef struct { uint64_t points, lines; int level; } TetrisScore;
void tetris_score_reset(TetrisScore *score);
void tetris_score_clear(TetrisScore *score, int lines);
void tetris_score_drop(TetrisScore *score, int cells, int hard);
double tetris_fall_interval(const TetrisScore *score);
#endif
