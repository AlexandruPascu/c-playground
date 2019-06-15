#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define WIDTH  800
#define HEIGHT 600

int main() {
    al_init();
    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);

    al_init_primitives_addon();
    double start_time, end_time;
    
    // scrieti codul de initializare aici
    double rect_x = 10, rect_y = 10, rect_speed = 50;
    double circle_x = WIDTH - 60, circle_y = 60, circle_speed = 100;

    start_time = al_get_time();
    end_time = al_get_time();
    while (al_get_time() < 5.0) {
        double delta_time = end_time - start_time;
        start_time = al_get_time();

        // scrieti codul de animare aici
        al_draw_filled_rectangle(rect_x, rect_y, rect_x + 100, rect_y + 100, al_map_rgb(255, 186, 52));
        rect_x += rect_speed * delta_time;

        al_draw_filled_circle(circle_x, circle_y, 50, al_map_rgb(186, 255, 52));
        circle_y += circle_speed * delta_time;
        circle_x -= circle_speed * delta_time;

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        end_time = al_get_time();
    }

    al_destroy_display(display);
    return 0;
}
