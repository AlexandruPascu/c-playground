#include <allegro5/allegro.h>

#define WIDTH  800
#define HEIGHT 600

int main() {
    al_init();
    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);

    al_rest(2.0);
    al_destroy_display(display);
    return 0;
}
