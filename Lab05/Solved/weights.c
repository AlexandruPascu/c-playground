#include "weights.h"
#include "../../common/input.h"
#include <math.h>
#include <string.h>
const TetrisWeights tetris_default_weights = {{100, -5, -80, -3, -2}};
int tetris_weights_valid(const TetrisWeights *weights) {
    for (int i = 0; i < TETRIS_FEATURE_COUNT; ++i)
        if (!isfinite(weights->values[i]) || weights->values[i] < -1000 || weights->values[i] > 1000) return 0;
    if (weights->values[0] <= 0) return 0;
    for (int i = 1; i < TETRIS_FEATURE_COUNT; ++i) if (weights->values[i] > 0) return 0;
    return 1;
}
int tetris_weights_load(const char *path, TetrisWeights *weights, char *error, size_t capacity) {
    static const char *names[] = {"lines", "height", "holes", "bumpiness", "max_height"};
    TetrisWeights parsed = {{0}};
    char token[128], *end;
    FILE *file = fopen(path, "r");
    int success = 0;
    if (!file) { snprintf(error, capacity, "Could not open weights: %s", path); return 0; }
    if (pg_read_token(file, token, sizeof(token)) != 1 || strcmp(token, "TETRIS_WEIGHTS_V1")) goto done;
    for (int i = 0; i < TETRIS_FEATURE_COUNT; ++i) {
        if (pg_read_token(file, token, sizeof(token)) != 1 || strcmp(token, names[i])) goto done;
        if (pg_read_token(file, token, sizeof(token)) != 1) goto done;
        errno = 0;
        parsed.values[i] = strtod(token, &end);
        if (!*token || *end || errno == ERANGE) goto done;
    }
    if (pg_read_token(file, token, sizeof(token)) != 0 || !tetris_weights_valid(&parsed)) goto done;
    success = 1;
done:
    if (fclose(file)) success = 0;
    if (success) { *weights = parsed; if (capacity) error[0] = '\0'; }
    else snprintf(error, capacity, "Invalid weights: expected TETRIS_WEIGHTS_V1 and lines/height/holes/bumpiness/max_height; finite values within +/-1000, positive lines, nonpositive penalties.");
    return success;
}
