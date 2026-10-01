#ifndef PLAYGROUND_TETRIS_AI_H
#define PLAYGROUND_TETRIS_AI_H
#include "board.h"
#include "weights.h"
#include <stdint.h>
/* A bounded breadth-first search over (x, y, rotation). */
#define TETRIS_AI_MAX_STATES ((BOARD_WIDTH + 4) * (BOARD_HEIGHT + 4) * 4)
typedef struct { int x, y, piece, rotation; } TetrisPose;
typedef enum { TETRIS_LEFT, TETRIS_RIGHT, TETRIS_ROTATE, TETRIS_DOWN, TETRIS_DROP } TetrisAction;
typedef struct {
    TetrisAction actions[TETRIS_AI_MAX_STATES];
    int count, candidates;
    double value;
    TetrisPose landing;
} TetrisPlan;
/* Pure planner: never changes state or start. Returns 0 for an invalid start. */
int tetris_ai_plan(const TetrisBoard *state, TetrisPose start, TetrisPlan *plan);
int tetris_ai_plan_weighted(const TetrisBoard *state, TetrisPose start, const TetrisWeights *weights, TetrisPlan *plan);
/* Apply a legal movement only; caller handles locking/scoring after DROP.
   Returns -1 on failure, otherwise vertical distance moved (0 for lateral/rotation). */
int tetris_step(const TetrisBoard *state, TetrisPose *pose, TetrisAction action);
/* Identical deterministic piece stream for graphical and headless play. Seed must be nonzero. */
int tetris_spawn(const TetrisBoard *state, uint32_t *random_state, TetrisPose *pose);
#endif
