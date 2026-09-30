/* Skeleton sprite-sheet exercise: 9 frames per direction, each 64 by 64. */
#include "../../graphics/window.h"
#include <stdio.h>
#include <string.h>
#define WIDTH 800
#define HEIGHT 600
#define FRAME_SIZE 64
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

int main(int argc, char **argv) {
    PlaygroundWindow window;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_EVENT event;
    float x = (WIDTH - FRAME_SIZE) / 2.0f, y = (HEIGHT - FRAME_SIZE) / 2.0f;
    double elapsed, animation_time = 0;
    int smoke = 0, frame = 0, direction = DOWN;
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        puts("Usage: skeleton [--smoke-test]\nMove with WASD or arrow keys; combine directions for diagonal movement. Esc closes.");
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "--smoke-test") == 0) smoke = 1;
    else if (argc != 1) { fputs("Usage: skeleton [--smoke-test]\n", stderr); return 1; }
    if (!pg_window_open(&window, "Skeleton - WASD/arrows to move, Esc to close", WIDTH, HEIGHT, 1, smoke)) return 1;
    sprite = pg_load_bitmap("skelly.png", 576, 256);
    if (!sprite) { pg_window_close(&window); return 1; }
    while (pg_window_next(&window, &event, &elapsed)) {
        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_KEYBOARD_STATE keys;
            int dx, dy;
            float distance;
            al_get_keyboard_state(&keys);
            dx = (al_key_down(&keys, ALLEGRO_KEY_D) || al_key_down(&keys, ALLEGRO_KEY_RIGHT)) -
                 (al_key_down(&keys, ALLEGRO_KEY_A) || al_key_down(&keys, ALLEGRO_KEY_LEFT));
            dy = (al_key_down(&keys, ALLEGRO_KEY_S) || al_key_down(&keys, ALLEGRO_KEY_DOWN)) -
                 (al_key_down(&keys, ALLEGRO_KEY_W) || al_key_down(&keys, ALLEGRO_KEY_UP));
            if (smoke) { dx = 1; dy = 0; } /* Exercise animation in the automated window check. */
            distance = 128.0f * (float)elapsed * (dx && dy ? 0.70710678f : 1.0f);
            x += dx * distance;
            y += dy * distance;
            if (dx) direction = dx > 0 ? RIGHT : LEFT;
            if (dy) direction = dy > 0 ? DOWN : UP;
            if (dx || dy) {
                animation_time += elapsed;
                while (animation_time >= 0.1) { frame = (frame + 1) % 9; animation_time -= 0.1; }
            } else { frame = 0; animation_time = 0; }
            if (x > WIDTH - FRAME_SIZE) x = 0;
            if (x < 0) x = WIDTH - FRAME_SIZE;
            if (y > HEIGHT - FRAME_SIZE) y = 0;
            if (y < 0) y = HEIGHT - FRAME_SIZE;
            al_clear_to_color(al_map_rgb(12, 18, 28));
            al_draw_bitmap_region(sprite, frame * FRAME_SIZE, direction * FRAME_SIZE,
                                  FRAME_SIZE, FRAME_SIZE, x, y, 0);
            al_flip_display();
        }
    }
    al_destroy_bitmap(sprite);
    pg_window_close(&window);
    return 0;
}
