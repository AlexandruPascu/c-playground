#include "score.h"
static void add_saturated(uint64_t *value, uint64_t amount) {
    *value = UINT64_MAX - *value < amount ? UINT64_MAX : *value + amount;
}
void tetris_score_reset(TetrisScore *score) { score->points = score->lines = 0; score->level = 1; }
void tetris_score_clear(TetrisScore *score, int lines) {
    static const unsigned int awards[] = {0, 100, 300, 500, 800};
    if (lines < 1 || lines > 4) return;
    add_saturated(&score->points, (uint64_t)awards[lines] * (unsigned int)score->level);
    add_saturated(&score->lines, (unsigned int)lines);
    score->level = score->lines >= 190 ? 20 : 1 + (int)(score->lines / 10);
}
void tetris_score_drop(TetrisScore *score, int cells, int hard) {
    if (cells > 0) add_saturated(&score->points, (uint64_t)(unsigned int)cells * (hard ? 2u : 1u));
}
double tetris_fall_interval(const TetrisScore *score) { return 1.0 / (1.0 + 0.5 * (score->level - 1)); }
