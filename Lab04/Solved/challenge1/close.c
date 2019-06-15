#include <allegro5/allegro.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

int main() {
    al_init();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    int run = 1;
    while (run) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            run = 0;
        } 

        if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
            event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            run = 0;
        }
    }


    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}
