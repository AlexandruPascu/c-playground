#include "window.h"
#include <stdio.h>
#include <string.h>
#ifndef PLAYGROUND_ASSET_DIR
#define PLAYGROUND_ASSET_DIR "."
#endif
void pg_window_close(PlaygroundWindow *window) {
    if (window->timer) al_destroy_timer(window->timer);
    if (window->events) al_destroy_event_queue(window->events);
    if (window->display) al_destroy_display(window->display);
    if (window->images) al_shutdown_image_addon();
    if (al_is_system_installed()) {
        al_shutdown_primitives_addon();
        al_uninstall_system();
    }
    memset(window, 0, sizeof(*window));
}
int pg_window_open(PlaygroundWindow *window, const char *title, int width, int height, int images, int smoke_test) {
    memset(window, 0, sizeof(*window));
    window->smoke_test = smoke_test;
    if (!al_init() || !al_init_primitives_addon() || !al_install_keyboard()) {
        fputs("Could not initialize Allegro and its keyboard/graphics support.\n", stderr);
        pg_window_close(window); return 0;
    }
    if (images) {
        if (!al_init_image_addon()) { fputs("Could not initialize image loading.\n", stderr); pg_window_close(window); return 0; }
        window->images = 1;
    }
    window->display = al_create_display(width, height);
    window->events = al_create_event_queue();
    window->timer = al_create_timer(1.0 / 60.0);
    if (!window->display || !window->events || !window->timer) {
        fputs("Could not create the game window. Run in a graphical desktop (or Xvfb for tests).\n", stderr);
        pg_window_close(window); return 0;
    }
    al_set_window_title(window->display, title);
    al_register_event_source(window->events, al_get_display_event_source(window->display));
    al_register_event_source(window->events, al_get_keyboard_event_source());
    al_register_event_source(window->events, al_get_timer_event_source(window->timer));
    window->previous_time = al_get_time();
    al_start_timer(window->timer);
    return 1;
}
int pg_window_next(PlaygroundWindow *window, ALLEGRO_EVENT *event, double *elapsed) {
    al_wait_for_event(window->events, event);
    *elapsed = 0;
    if (event->type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
        (event->type == ALLEGRO_EVENT_KEY_DOWN && event->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) return 0;
    if (event->type == ALLEGRO_EVENT_TIMER) {
        double now = al_get_time();
        *elapsed = now - window->previous_time;
        window->previous_time = now;
        if (*elapsed < 0) *elapsed = 0;
        if (*elapsed > 0.1) *elapsed = 0.1;
        if (window->smoke_test && ++window->frames > 90) return 0;
    }
    return 1;
}
ALLEGRO_BITMAP *pg_load_bitmap(const char *name, int width, int height) {
    char path[4096];
    ALLEGRO_BITMAP *bitmap;
    int length = snprintf(path, sizeof(path), "%s/%s", PLAYGROUND_ASSET_DIR, name);
    if (length < 0 || (size_t)length >= sizeof(path)) { fputs("Asset path is too long.\n", stderr); return NULL; }
    bitmap = al_load_bitmap(path);
    if (!bitmap) { fprintf(stderr, "Could not load sprite: %s\n", path); return NULL; }
    if (al_get_bitmap_width(bitmap) != width || al_get_bitmap_height(bitmap) != height) {
        fprintf(stderr, "Unexpected sprite dimensions for %s; expected %dx%d.\n", name, width, height);
        al_destroy_bitmap(bitmap); return NULL;
    }
    return bitmap;
}
