#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define TARGET_FPS 60.0

int main() {
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_TIMER *timer = al_create_timer(1 / TARGET_FPS);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);


    double start_time = 0.f;
    double end_time = 0.f;

    float speed = 50.f;
    float x_pos = 0;
    float y_pos = 0;

    int draw = 0;
    int run = 1;
    while (run) {
        double delta_time = end_time - start_time;
        start_time = al_get_time();
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // updatati starea tastaturii
        switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            run = 0;
            break;
        case ALLEGRO_KEY_DOWN:
            break;
        case ALLEGRO_KEY_UP:
            break;
        case ALLEGRO_EVENT_TIMER:
            draw = 1;
            break;
        }

        // bazat pe starea tastaturii, updatati pozitia patratului

        if (draw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            // desenati aici

            al_flip_display();
            draw = 0;
        }
        end_time = al_get_time();
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}
