#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>

#define WIDTH  800
#define HEIGHT 600

#define BACKGROUND_COL al_map_rgb(0, 0, 0)
#define FOREGROUND_COL al_map_rgb(255, 255, 255)

void draw_hole(int x1, int y1, int x2, int y2, int x3, int y3, int depth) {
    if (depth < 0)
        return;

    // draw a hole in the middle of the triangle
    al_draw_filled_triangle(
            (x1 + x2) / 2, (y1 + y2) / 2,
            (x1 + x3) / 2, (y1 + y3) / 2,
            (x3 + x2) / 2, (y3 + y2) / 2,
            BACKGROUND_COL);

    // recurse to the next triangles
    draw_hole(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2, depth - 1);
    draw_hole(x2, y2, (x2 + x1) / 2, (y2 + y1) / 2, (x2 + x3) / 2, (y2 + y3) / 2, depth - 1);
    draw_hole(x3, y3, (x3 + x2) / 2, (y3 + y2) / 2, (x3 + x1) / 2, (y3 + y1) / 2, depth - 1);
}

void sierpinski(int depth) {
    int x1, y1, x2, y2, x3, y3;

    // initialize the coordinates of the triangle
    x1 = WIDTH / 9;
    y1 = HEIGHT;

    x2 = 8 * WIDTH / 9;
    y2 = HEIGHT;

    x3 = WIDTH / 2;
    y3 = 0;

    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, FOREGROUND_COL);
    draw_hole(x1, y1, x2, y2, x3, y3, depth);
}

int main(void) {
    al_init();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);

    al_clear_to_color(BACKGROUND_COL);
    sierpinski(atoi(argv[1]));

    al_flip_display();
    al_rest(2.0);
    al_destroy_display(display);
    return 0;
}
