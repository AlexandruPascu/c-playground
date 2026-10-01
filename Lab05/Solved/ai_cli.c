#include "ai.h"
#include "score.h"
#include "../../common/input.h"
#include <string.h>
int main(int argc, char **argv) {
    int seed = 1, limit = 1000, games = 1;
    for (int i = 1; i < argc; i += 2) {
        int value;
        if (!strcmp(argv[i], "--help")) {
            puts("Usage: tetris_ai [--seed 1..2147483647] [--pieces 1..100000] [--games 1..100]\nRuns the same heuristic planner without a display. Games use consecutive seeds.\nCSV columns: seed,pieces,lines,score,level,status (limit or game_over)."); return 0;
        }
        if (i + 1 >= argc || !pg_parse_int(argv[i + 1], &value) || value < 1) goto invalid;
        if (!strcmp(argv[i], "--seed")) seed = value;
        else if (!strcmp(argv[i], "--pieces") && value <= 100000) limit = value;
        else if (!strcmp(argv[i], "--games") && value <= 100) games = value;
        else goto invalid;
    }
    if (seed > INT_MAX - (games - 1)) goto invalid;
    puts("seed,pieces,lines,score,level,status");
    for (int game = 0; game < games; ++game) {
        TetrisBoard state = {0};
        TetrisScore score;
        TetrisPose pose;
        TetrisPlan plan;
        uint32_t initial_seed = (uint32_t)seed + (uint32_t)game, random_state = initial_seed;
        int placed = 0, ended;
        tetris_score_reset(&score);
        ended = !tetris_spawn(&state, &random_state, &pose);
        while (!ended && placed < limit) {
            if (!tetris_ai_plan(&state, pose, &plan)) { fputs("Planner failed for a valid piece.\n", stderr); return 1; }
            for (int i = 0; i < plan.count; ++i) {
                int distance = tetris_step(&state, &pose, plan.actions[i]);
                if (distance < 0) { fputs("Planner produced an illegal action.\n", stderr); return 1; }
                if (plan.actions[i] == TETRIS_DOWN || plan.actions[i] == TETRIS_DROP)
                    tetris_score_drop(&score, distance, plan.actions[i] == TETRIS_DROP);
            }
            if (!tetris_board_place(&state, pose.x, pose.y, pose.piece, pose.rotation)) return 1;
            ++placed;
            tetris_score_clear(&score, tetris_board_clear(&state));
            ended = tetris_board_over(&state) || !tetris_spawn(&state, &random_state, &pose);
        }
        printf("%u,%d,%llu,%llu,%d,%s\n", (unsigned int)initial_seed, placed,
               (unsigned long long)score.lines, (unsigned long long)score.points, score.level,
               ended ? "game_over" : "limit");
    }
    return 0;
invalid:
    fputs("Invalid arguments. Use tetris_ai --help.\n", stderr);
    return 1;
}
