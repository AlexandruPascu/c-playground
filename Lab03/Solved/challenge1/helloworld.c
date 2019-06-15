#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define WIDTH  800
#define HEIGHT 600

int main() {
    al_init();
    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);

    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_FONT *font = al_load_font("orbitron-black.ttf", 36, 0);
    al_draw_text(font, al_map_rgb(50, 100, 226), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Hello, \nWorld!");
    al_flip_display();

    al_rest(2.0);
    al_destroy_display(display);
    return 0;
}
