#include "ai.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct { TetrisPose pose; int parent; TetrisAction action; } SearchNode;
static int valid(const TetrisBoard *state, TetrisPose pose) {
    return tetris_board_can_move(state, pose.x, pose.y, pose.piece, pose.rotation);
}
int tetris_step(const TetrisBoard *state, TetrisPose *pose, TetrisAction action) {
    TetrisPose next = *pose;
    if (!valid(state, next)) return -1;
    switch (action) {
        case TETRIS_LEFT: --next.x; break;
        case TETRIS_RIGHT: ++next.x; break;
        case TETRIS_ROTATE: next.rotation = next_rotation(next.rotation); break;
        case TETRIS_DOWN: --next.y; break;
        case TETRIS_DROP:
            while (tetris_board_can_move(state, next.x, next.y - 1, next.piece, next.rotation)) --next.y;
            break;
        default: return -1;
    }
    if (!valid(state, next)) return -1;
    { int distance = pose->y - next.y; *pose = next; return distance; }
}
static uint32_t random_next(uint32_t *state) {
    /* xorshift32: specified unsigned operations give reproducible streams on every platform. */
    if (!*state) *state = 1;
    *state ^= *state << 13; *state ^= *state >> 17; *state ^= *state << 5;
    return *state;
}
int tetris_spawn(const TetrisBoard *state, uint32_t *random_state, TetrisPose *pose) {
    pose->piece = (int)(random_next(random_state) % 7u);
    pose->rotation = (int)(random_next(random_state) % 4u);
    pose->x = BOARD_WIDTH / 2 + get_x_displacement(pose->piece, pose->rotation);
    pose->y = BOARD_HEIGHT - 1 + get_y_displacement(pose->piece, pose->rotation);
    return valid(state, *pose);
}
static int evaluate(const TetrisBoard *state, TetrisPose landing) {
    TetrisBoard result = *state;
    int heights[BOARD_WIDTH] = {0}, total = 0, holes = 0, bumpiness = 0, maximum = 0, lines;
    if (!tetris_board_place(&result, landing.x, landing.y, landing.piece, landing.rotation)) return INT_MIN;
    lines = tetris_board_clear(&result);
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
            if (result.cells[y][x] != POS_FREE && !heights[x]) heights[x] = y + 1;
            if (result.cells[y][x] == POS_FREE && heights[x]) ++holes;
        }
        total += heights[x];
        if (heights[x] > maximum) maximum = heights[x];
        if (x) bumpiness += abs(heights[x] - heights[x - 1]);
    }
    /* Hand-selected baseline weights, not a trained model. Do not reward drop points:
       survival and line clearing are the objective. Top-out dominates all other terms. */
    return 100 * lines - 5 * total - 80 * holes - 3 * bumpiness - 2 * maximum
           - (tetris_board_over(&result) ? 1000000 : 0);
}
static int index_of(TetrisPose pose) {
    return ((pose.y + 4) * (BOARD_WIDTH + 4) + pose.x + 4) * 4 + pose.rotation;
}
int tetris_ai_plan(const TetrisBoard *state, TetrisPose start, TetrisPlan *plan) {
    SearchNode nodes[TETRIS_AI_MAX_STATES];
    unsigned char seen[TETRIS_AI_MAX_STATES] = {0};
    int count = 1, best = -1;
    memset(plan, 0, sizeof(*plan));
    plan->value = INT_MIN;
    if (!valid(state, start)) return 0;
    nodes[0].pose = start; nodes[0].parent = -1;
    seen[index_of(start)] = 1;
    for (int head = 0; head < count; ++head) {
        TetrisPose pose = nodes[head].pose;
        if (!tetris_board_can_move(state, pose.x, pose.y - 1, pose.piece, pose.rotation)) {
            int value = evaluate(state, pose);
            ++plan->candidates;
            /* BFS and strict comparison prefer a shorter path on equal values. */
            if (value > plan->value) { plan->value = value; best = head; }
        }
        for (int action = TETRIS_LEFT; action <= TETRIS_DOWN; ++action) {
            TetrisPose next = pose;
            int index;
            if (tetris_step(state, &next, (TetrisAction)action) < 0) continue;
            index = index_of(next);
            if (seen[index]) continue;
            seen[index] = 1;
            nodes[count].pose = next; nodes[count].parent = head;
            nodes[count].action = (TetrisAction)action;
            ++count;
        }
    }
    if (best < 0) return 0;
    plan->landing = nodes[best].pose;
    for (int node = best; nodes[node].parent >= 0; node = nodes[node].parent)
        plan->actions[plan->count++] = nodes[node].action;
    for (int i = 0; i < plan->count / 2; ++i) {
        TetrisAction swap = plan->actions[i];
        plan->actions[i] = plan->actions[plan->count - i - 1];
        plan->actions[plan->count - i - 1] = swap;
    }
    /* Replace the final uninterrupted descent with the game's hard drop, then lock.
       Paths involving a slide beneath an overhang still retain their earlier DOWN steps. */
    while (plan->count && plan->actions[plan->count - 1] == TETRIS_DOWN) --plan->count;
    plan->actions[plan->count++] = TETRIS_DROP;
    return 1;
}
