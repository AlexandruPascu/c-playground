#include "Lab05/Solved/ai.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#define CHECK(value) do { if (!(value)) { fprintf(stderr, "FAIL %d: %s\n", __LINE__, #value); return 1; } } while (0)
/* Check geometry directly so a planner/step bug cannot validate itself. */
static int legal(const TetrisBoard *state, TetrisPose pose) {
    for (int y = 0; y < 5; ++y)
        for (int x = 0; x < 5; ++x)
            if (get_block(pose.piece, pose.rotation, x, y)) {
                int row = pose.y + y, column = pose.x + x;
                if (row < 0 || row >= BOARD_HEIGHT || column < 0 || column >= BOARD_WIDTH || state->cells[row][column]) return 0;
            }
    return 1;
}
static TetrisPose start(int piece, int rotation) {
    TetrisPose pose;
    pose.piece = piece; pose.rotation = rotation;
    pose.x = BOARD_WIDTH / 2 + get_x_displacement(piece, rotation);
    pose.y = BOARD_HEIGHT - 1 + get_y_displacement(piece, rotation);
    return pose;
}
static int replay(const TetrisBoard *state, TetrisPose pose, const TetrisPlan *plan) {
    CHECK(plan->count > 0 && plan->count <= TETRIS_AI_MAX_STATES && plan->candidates > 0);
    CHECK(plan->actions[plan->count - 1] == TETRIS_DROP);
    for (int i = 0; i < plan->count; ++i) {
        TetrisPose expected = pose;
        switch (plan->actions[i]) {
            case TETRIS_LEFT: --expected.x; break;
            case TETRIS_RIGHT: ++expected.x; break;
            case TETRIS_ROTATE: expected.rotation = (expected.rotation + 1) % 4; break;
            case TETRIS_DOWN: --expected.y; break;
            case TETRIS_DROP:
                CHECK(i == plan->count - 1);
                do { --expected.y; } while (legal(state, expected));
                ++expected.y;
                break;
            default: CHECK(0);
        }
        CHECK(legal(state, expected));
        CHECK(tetris_step(state, &pose, plan->actions[i]) >= 0);
        CHECK(pose.x == expected.x && pose.y == expected.y && pose.rotation == expected.rotation);
    }
    CHECK(pose.x == plan->landing.x && pose.y == plan->landing.y && pose.rotation == plan->landing.rotation);
    --pose.y; CHECK(!legal(state, pose));
    return 0;
}
static int placements(void) {
    TetrisBoard state = {0}, saved;
    TetrisPlan plan, repeated;
    for (int layout = 0; layout < 4; ++layout) {
        memset(&state, 0, sizeof(state));
        for (int x = 0; x < BOARD_WIDTH; ++x)
            for (int y = 0; y < (layout * (x + 2)) % 6; ++y) state.cells[y][x] = POS_FILLED;
        saved = state;
        for (int piece = 0; piece < 7; ++piece)
            for (int rotation = 0; rotation < 4; ++rotation) {
                TetrisPose pose = start(piece, rotation);
                CHECK(tetris_ai_plan(&state, pose, &plan));
                CHECK(!memcmp(&state, &saved, sizeof(state)));
                CHECK(!replay(&state, pose, &plan));
                CHECK(tetris_ai_plan(&state, pose, &repeated));
                CHECK(plan.count == repeated.count && plan.value == repeated.value);
                CHECK(!memcmp(plan.actions, repeated.actions, (size_t)plan.count * sizeof(plan.actions[0])));
            }
    }
    { TetrisPose pose = {INT_MAX, INT_MIN, 0, 0};
      CHECK(!tetris_ai_plan(&state, pose, &plan) && !plan.count);
      CHECK(tetris_step(&state, &pose, TETRIS_LEFT) == -1 && pose.x == INT_MAX); }
    { TetrisPose pose = start(0, 0); pose.piece = 7; CHECK(!tetris_ai_plan(&state, pose, &plan)); }
    memset(&state, 1, sizeof(state));
    CHECK(!tetris_ai_plan(&state, start(0, 0), &plan));
    return 0;
}
static int tactical_positions(void) {
    TetrisBoard state = {0};
    TetrisPlan plan;
    TetrisPose pose = start(1, 0);
    /* Only a reachable vertical I can complete all four rows. */
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < BOARD_WIDTH; ++x) state.cells[y][x] = x != 4;
    CHECK(tetris_ai_plan(&state, pose, &plan) && !replay(&state, pose, &plan));
    CHECK(tetris_board_place(&state, plan.landing.x, plan.landing.y, 1, plan.landing.rotation));
    CHECK(tetris_board_clear(&state) == 4);
    /* Slide an O left beneath a roof: dropping directly at the target is impossible. */
    memset(&state, 0, sizeof(state));
    for (int y = 0; y < 2; ++y)
        for (int x = 4; x < BOARD_WIDTH; ++x) state.cells[y][x] = POS_FILLED;
    state.cells[2][0] = state.cells[2][1] = POS_FILLED;
    pose = start(0, 0);
    CHECK(tetris_ai_plan(&state, pose, &plan) && !replay(&state, pose, &plan));
    CHECK(plan.landing.x == -2 && plan.landing.y == -2);
    { int descends_before_sliding = 0;
      for (int i = 0; i < plan.count - 1; ++i) if (plan.actions[i] == TETRIS_DOWN) descends_before_sliding = 1;
      CHECK(descends_before_sliding); }
    /* Seal that cavity's side. The attractive empty space is no longer reachable. */
    state.cells[0][2] = state.cells[1][2] = POS_FILLED;
    CHECK(tetris_ai_plan(&state, pose, &plan) && !replay(&state, pose, &plan));
    CHECK(plan.landing.x != -2 || plan.landing.y != -2);
    return 0;
}
int main(void) {
    if (placements() || tactical_positions()) return 1;
    puts("PASS AI legal paths, deterministic choices, no mutation, line clearing, overhangs and sealed cavities");
    return 0;
}
