/* Recursive subdivision follows the workshop triangle/carpet exercises. */
#include "window.h"
#include "../common/input.h"
#include <string.h>
static void triangle_holes(float x1, float y1, float x2, float y2, float x3, float y3, int depth) {
    float ax, ay, bx, by, cx, cy;
    if (!depth) return;
    ax = (x1 + x2) / 2; ay = (y1 + y2) / 2;
    bx = (x1 + x3) / 2; by = (y1 + y3) / 2;
    cx = (x2 + x3) / 2; cy = (y2 + y3) / 2;
    al_draw_filled_triangle(ax, ay, bx, by, cx, cy, al_map_rgb(12, 18, 28));
    triangle_holes(x1, y1, ax, ay, bx, by, depth - 1);
    triangle_holes(x2, y2, ax, ay, cx, cy, depth - 1);
    triangle_holes(x3, y3, bx, by, cx, cy, depth - 1);
}
static void carpet_holes(float x, float y, float size, int depth) {
    float third;
    if (!depth) return;
    third = size / 3;
    al_draw_filled_rectangle(x + third, y + third, x + 2 * third, y + 2 * third, al_map_rgb(12, 18, 28));
    for (int row = 0; row < 3; ++row)
        for (int column = 0; column < 3; ++column)
            if (row != 1 || column != 1)
                carpet_holes(x + column * third, y + row * third, third, depth - 1);
}
static void draw_fractal(int carpet, int depth) {
    al_clear_to_color(al_map_rgb(12, 18, 28));
    if (carpet) {
        al_draw_filled_rectangle(130, 30, 670, 570, al_map_rgb(90, 210, 230));
        carpet_holes(130, 30, 540, depth);
    } else {
        al_draw_filled_triangle(80, 570, 720, 570, 400, 30, al_map_rgb(90, 210, 230));
        triangle_holes(80, 570, 720, 570, 400, 30, depth);
    }
    al_flip_display();
}
int pg_run_fractal(int argc, char **argv, int carpet, int animated) {
    int limit = carpet ? 5 : 9, target = 3, smoke = 0, have_depth = 0;
    PlaygroundWindow window;
    ALLEGRO_EVENT event;
    double elapsed, animation_time = 0;
    int depth, redraw = 1;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [depth] [--smoke-test]\nDepth: 0..%d. Left/Right change depth; Esc closes.\n", argv[0], limit);
            return 0;
        }
        if (strcmp(argv[i], "--smoke-test") == 0) smoke = 1;
        else if (have_depth || !pg_parse_int(argv[i], &target) || target < 0 || target > limit) {
            fprintf(stderr, "Depth must be an integer from 0 to %d.\n", limit); return 1;
        } else have_depth = 1;
    }
    if (!pg_window_open(&window, carpet ? "Sierpinski carpet" : "Sierpinski triangle", 800, 600, 0, smoke)) return 1;
    depth = animated ? 0 : target;
    while (pg_window_next(&window, &event, &elapsed)) {
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && depth < limit) { ++depth; animated = 0; redraw = 1; }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && depth > 0) { --depth; animated = 0; redraw = 1; }
        }
        if (event.type == ALLEGRO_EVENT_TIMER && animated && depth < target) {
            animation_time += elapsed;
            if (animation_time >= 0.5) { ++depth; animation_time -= 0.5; redraw = 1; }
        }
        if (redraw) {
            char title[100];
            snprintf(title, sizeof(title), "Sierpinski %s - depth %d (Left/Right, Esc)", carpet ? "carpet" : "triangle", depth);
            al_set_window_title(window.display, title);
            draw_fractal(carpet, depth);
            redraw = 0;
        }
    }
    pg_window_close(&window);
    return 0;
}
