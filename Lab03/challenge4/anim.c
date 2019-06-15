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

    start_time = al_get_time();
    end_time = al_get_time();
    while (al_get_time() < 5.0) {
        double delta_time = end_time - start_time;
        start_time = al_get_time();

        // scrieti codul de animare aici

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        end_time = al_get_time();
    }

    al_destroy_display(display);
    return 0;
}
