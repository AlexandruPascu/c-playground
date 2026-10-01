#ifndef PLAYGROUND_WINDOW_H
#define PLAYGROUND_WINDOW_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
typedef struct {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *events;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *font;
    double previous_time;
    int smoke_test, frames, images;
} PlaygroundWindow;
int pg_window_open(PlaygroundWindow *window, const char *title, int width, int height, int images, int smoke_test);
int pg_window_next(PlaygroundWindow *window, ALLEGRO_EVENT *event, double *elapsed);
int pg_window_mouse(PlaygroundWindow *window);
void pg_text(PlaygroundWindow *window, float x, float y, float scale, ALLEGRO_COLOR color, const char *format, ...);
void pg_window_close(PlaygroundWindow *window);
ALLEGRO_BITMAP *pg_load_bitmap(const char *name, int width, int height);
int pg_run_fractal(int argc, char **argv, int carpet, int animated);
#endif
