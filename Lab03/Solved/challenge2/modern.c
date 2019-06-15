#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define WIDTH  800
#define HEIGHT 600

int main() {
    al_init();
    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);

    al_init_primitives_addon();

    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    al_clear_to_color(al_map_rgb(100, 10, 25));
    al_draw_line(-10, HEIGHT/3, WIDTH/2, -10, white, 10);
    al_draw_line(WIDTH/8, -10, WIDTH/4, HEIGHT + 10, white, 10);
    al_draw_line(WIDTH/4, HEIGHT + 10, WIDTH/4 + WIDTH / 16, -10,  white, 10);
    al_draw_line(-10, HEIGHT/3, 2 * WIDTH/3, HEIGHT + 10, white, 10);
    al_draw_line(-10, 2 * HEIGHT/3, WIDTH - 10, -10, white, 10);
    al_draw_line(8 * WIDTH / 10, -10, 9 * WIDTH / 10, HEIGHT + 10, white, 10);

    al_flip_display();
    al_rest(2.0);
    al_destroy_display(display);
    return 0;
}
