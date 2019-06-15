#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define TARGET_FPS 60.0
#define DOWN 0
#define RIGHT 1
#define UP 2
#define LEFT 3

int main() {
    al_init();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    ALLEGRO_BITMAP *trump_spritesheet = al_load_bitmap("trump_run.bmp");
    printf("%dx%d\n", al_get_bitmap_width(trump_spritesheet), al_get_bitmap_height(trump_spritesheet));

    ALLEGRO_TIMER *timer = al_create_timer(1 / TARGET_FPS);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    int i = 0;
    float accumulated_time = 0.f;
    float time_between_transitions = 0.15f;
    float start_time = 0;
    float end_time = 0;

    int run = 1;
    int draw = 1;

    float trump_x = 0;
    float trump_y = 0;
    int trump_dir = DOWN;
    int trump_moving = false;
    float trump_speed = 100.f;

    ALLEGRO_KEYBOARD_STATE keystate;

    while (run) {
        double delta_time = end_time - start_time;
        start_time = al_get_time();

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        switch (event.type) {
        case ALLEGRO_EVENT_MOUSE_AXES:
            break;
        case ALLEGRO_EVENT_TIMER:
            al_get_keyboard_state(&keystate);
            draw = 1;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            run = 0;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            al_get_keyboard_state(&keystate);
            break;
        case ALLEGRO_EVENT_KEY_UP:
            al_get_keyboard_state(&keystate);
            break;
        }

        accumulated_time += delta_time;
        /*printf("%f\n", accumulated_time);*/
        if (trump_moving) {
            while ((accumulated_time / time_between_transitions >= 1)) {
                i = (i + 1) % 6;
                accumulated_time -= time_between_transitions;
            }
        } else {
            i = 0;
            accumulated_time = 0;
        }


        if (al_key_down(&keystate, ALLEGRO_KEY_S)) {
            trump_dir = DOWN;
            trump_moving = 1;
            trump_y += trump_speed * delta_time;
        } else if (al_key_down(&keystate, ALLEGRO_KEY_W)) {
            trump_dir = UP;
            trump_moving = 1;
            trump_y -= trump_speed * delta_time;
        } else if (al_key_down(&keystate, ALLEGRO_KEY_A)) {
            trump_dir = LEFT;
            trump_moving = 1;
            trump_x -= trump_speed * delta_time;
        } else if (al_key_down(&keystate, ALLEGRO_KEY_D)) {
            trump_dir = RIGHT;
            trump_moving = 1;
            trump_x += trump_speed * delta_time;
        } else {
            trump_moving = 0;
        }

        if (trump_y + 100 > HEIGHT)
            trump_y = 0.f;
        if (trump_y < 0)
            trump_y = HEIGHT - 100;
        if (trump_x + 100 > WIDTH)
            trump_x = 0.f;
        if (trump_x < 0)
            trump_x = WIDTH - 100;

        if (draw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(trump_spritesheet, i * 100, trump_dir * 100, 100, 100, trump_x, trump_y, 0);

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
