#include <allegro5/allegro.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define TARGET_FPS 60.0

int main() {
    al_init();

    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);

    int run = 1;
    while (run) {

    }

    al_destroy_display(display);
    return 0;
}
