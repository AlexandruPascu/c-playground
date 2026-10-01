/* Completed Allegro exercises share resource handling and timer-based movement. */
#include "demos.h"
#include "window.h"
#include <allegro5/allegro_ttf.h>
#include <string.h>
#include <stdio.h>
int pg_run_demo(int argc, char **argv, enum PgDemo demo) {
    PlaygroundWindow window;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *sprite = NULL;
    const char *font_path = NULL;
    const char *titles[] = {"Hello, C Playground", "Primitive art", "Two moving primitives", "Window events", "Event logger", "Keyboard movement", "Spinning coin", "Character animation"};
    double elapsed, elapsed_total = 0, animation = 0;
    float x = 350, y = 250, motion_x = 100, motion_y = 100;
    int smoke = 0, frame = 0, direction = 0, x_direction = 1, y_direction = 1, ttf = 0;
    int images = demo == PG_COIN || demo == PG_CHARACTER;
    ALLEGRO_COLOR white = al_map_rgb(225, 235, 245), cyan = al_map_rgb(90, 210, 230);
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) {
            printf("Usage: %s [--smoke-test]%s\nEsc closes. WASD/arrows move the square or character.\nHello closes after 2 seconds; two-primitives animation after 5 seconds.\n", argv[0], demo == PG_HELLO ? " [--font font.ttf]" : ""); return 0;
        }
        if (!strcmp(argv[i], "--smoke-test")) smoke = 1;
        else if (demo == PG_HELLO && !strcmp(argv[i], "--font") && i + 1 < argc && !font_path) font_path = argv[++i];
        else { fputs("Invalid argument. Use --help.\n", stderr); return 1; }
    }
    if (!pg_window_open(&window, titles[demo], 800, 600, images, smoke)) return 1;
    if (demo == PG_EVENTS && !pg_window_mouse(&window)) { pg_window_close(&window); return 1; }
    if (font_path) {
        ALLEGRO_FONT *font;
        ttf = al_init_ttf_addon();
        font = ttf ? al_load_ttf_font(font_path, 24, 0) : NULL;
        if (!font) { fputs("Could not load the requested TTF font.\n", stderr); if (ttf) al_shutdown_ttf_addon(); pg_window_close(&window); return 1; }
        al_destroy_font(window.font);
        window.font = font;
    }
    if (demo == PG_COIN) sprite = pg_load_bitmap("coin.png", 700, 200);
    if (demo == PG_CHARACTER) sprite = pg_load_bitmap("trump_run.png", 600, 400);
    if (images && !sprite) { pg_window_close(&window); return 1; }
    while (pg_window_next(&window, &event, &elapsed)) {
        if (demo == PG_EVENTS) {
            if (event.type == ALLEGRO_EVENT_TIMER) puts("timer");
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) puts("key down");
            if (event.type == ALLEGRO_EVENT_KEY_UP) puts("key up");
            if (event.type == ALLEGRO_EVENT_MOUSE_AXES) puts("mouse moved");
        }
        if (event.type != ALLEGRO_EVENT_TIMER) continue;
        elapsed_total += elapsed;
        if ((demo == PG_HELLO && elapsed_total >= 2) || (demo == PG_MOTION && elapsed_total >= 5)) break;
        if (demo == PG_SQUARE || demo == PG_CHARACTER) {
            ALLEGRO_KEYBOARD_STATE keys;
            int dx, dy;
            float distance;
            al_get_keyboard_state(&keys);
            dx = (al_key_down(&keys, ALLEGRO_KEY_D) || al_key_down(&keys, ALLEGRO_KEY_RIGHT)) -
                 (al_key_down(&keys, ALLEGRO_KEY_A) || al_key_down(&keys, ALLEGRO_KEY_LEFT));
            dy = (al_key_down(&keys, ALLEGRO_KEY_S) || al_key_down(&keys, ALLEGRO_KEY_DOWN)) -
                 (al_key_down(&keys, ALLEGRO_KEY_W) || al_key_down(&keys, ALLEGRO_KEY_UP));
            if (smoke) dx = 1;
            distance = (float)elapsed * 150 * (dx && dy ? 0.70710678f : 1);
            x += dx * distance; y += dy * distance;
            if (x > 700) x = 0;
            if (x < 0) x = 700;
            if (y > 500) y = 60;
            if (y < 60) y = 500;
            if (dx) direction = dx > 0 ? 1 : 3;
            if (dy) direction = dy > 0 ? 0 : 2;
            if (dx || dy) animation += elapsed;
            else { animation = 0; frame = 0; }
        } else animation += elapsed;
        while (animation >= 0.15) { animation -= 0.15; frame = (frame + 1) % 6; }
        motion_x += x_direction * 180 * (float)elapsed;
        motion_y += y_direction * 120 * (float)elapsed;
        if (motion_x > 650) { motion_x = 650; x_direction = -1; }
        if (motion_x < 50) { motion_x = 50; x_direction = 1; }
        if (motion_y > 480) { motion_y = 480; y_direction = -1; }
        if (motion_y < 100) { motion_y = 100; y_direction = 1; }
        al_clear_to_color(al_map_rgb(12, 18, 28));
        pg_text(&window, 24, 20, font_path ? 1 : 2, white, "%s", titles[demo]);
        if (demo == PG_HELLO) pg_text(&window, 120, 270, font_path ? 1 : 3, cyan, "Hello, World!");
        if (demo == PG_ART) {
            al_draw_filled_rectangle(80, 120, 480, 440, al_map_rgb(235, 120, 65));
            al_draw_filled_triangle(240, 460, 600, 80, 710, 500, cyan);
            al_draw_circle(390, 300, 150, white, 12);
            al_draw_line(80, 510, 730, 150, al_map_rgb(240, 210, 85), 8);
        }
        if (demo == PG_MOTION) {
            al_draw_filled_rectangle(motion_x, 170, motion_x + 80, 250, cyan);
            al_draw_filled_circle(400, motion_y, 45, al_map_rgb(255, 175, 110));
        }
        if (demo == PG_CLOSE || demo == PG_EVENTS) pg_text(&window, 70, 280, 2, cyan, "Esc / close button exits");
        if (demo == PG_SQUARE) al_draw_filled_rectangle(x, y, x + 80, y + 80, cyan);
        if (demo == PG_COIN) {
            /* The sixth 120px frame has only 100px of source pixels in this asset. */
            int width = frame == 5 ? 100 : 120;
            al_draw_bitmap_region(sprite, frame * 120, 0, width, 200, 340, 200, 0);
        }
        if (demo == PG_CHARACTER) al_draw_bitmap_region(sprite, frame * 100, direction * 100, 100, 100, x, y, 0);
        al_flip_display();
    }
    if (demo == PG_EVENTS) puts("exiting...");
    if (sprite) al_destroy_bitmap(sprite);
    if (ttf) { al_destroy_font(window.font); window.font = NULL; al_shutdown_ttf_addon(); }
    pg_window_close(&window);
    return 0;
}
