#ifndef PLAYGROUND_TETRIS_WEIGHTS_H
#define PLAYGROUND_TETRIS_WEIGHTS_H
#include <stddef.h>
#define TETRIS_FEATURE_COUNT 5
typedef struct { double values[TETRIS_FEATURE_COUNT]; } TetrisWeights;
extern const TetrisWeights tetris_default_weights;
int tetris_weights_valid(const TetrisWeights *weights);
/* Exact versioned format; rejects nonfinite/out-of-range values and extra tokens.
   On failure the destination is unchanged. */
int tetris_weights_load(const char *path, TetrisWeights *weights, char *error, size_t capacity);
#endif
