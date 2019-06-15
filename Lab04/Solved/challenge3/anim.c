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

    ALLEGRO_KEYBOARD_STATE keystate;

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

        switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            run = 0;
            break;
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_UP:
            al_get_keyboard_state(&keystate);
            break;
        case ALLEGRO_EVENT_TIMER:
            al_get_keyboard_state(&keystate);
            draw = 1;
            break;
        }

        if (al_key_down(&keystate, ALLEGRO_KEY_W)) {
            y_pos -= speed * delta_time;
        } else if (al_key_down(&keystate, ALLEGRO_KEY_S)) {
            y_pos += speed * delta_time;
        }
        if (al_key_down(&keystate, ALLEGRO_KEY_A)) {
            x_pos -= speed * delta_time;
        } else if (al_key_down(&keystate, ALLEGRO_KEY_D)) {
            x_pos += speed * delta_time;
        }

        if (x_pos + 100 > WIDTH)
            x_pos = 0;
        if (x_pos < 0)
            x_pos = WIDTH - 100;
        if (y_pos + 100 > HEIGHT)
            y_pos = 0;
        if (y_pos < 0)
            y_pos = HEIGHT - 100;

        if (draw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_rectangle(x_pos, y_pos, x_pos + 100, y_pos + 100, al_map_rgb(255, 128, 64), 10);

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
