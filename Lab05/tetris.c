#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"

#define WIN_WIDTH      600
#define WIN_HEIGHT     800

#define BOARD_WIDTH     10
#define BOARD_HEIGHT    20

#define POS_FREE         0
#define POS_FILLED       1

#define BLOCK_SIZE 30
#define BORDER_SIZE 15
#define BLOCK_BORDER_SIZE 2

#define red    al_map_rgb(255,   0,   0)
#define green  al_map_rgb(  0, 255,   0)
#define blue   al_map_rgb(  0,   0, 255)
#define black  al_map_rgb(  0,   0,   0)


int board[BOARD_HEIGHT][BOARD_WIDTH];

void init_board() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = POS_FREE;
        }
    }
}

void place_piece(int x, int y, int piece, int rotation) {
    // TODO
}

int game_over() {
    // TODO
}

void delete_line(int line) {
    // TODO
}

int can_delete_line(int line) {
    // TODO
}

void delete_possible_lines() {
    // TODO
}

int is_free_block(int x, int y) {
    return board[y][x] == POS_FREE;
}

int is_possible_movement(int x, int y, int piece, int rotation) {
    // TODO
}

int rand_int(int a, int b) {
    return rand() % (b - a + 1) + a;
}

void draw_background() {
    al_draw_filled_rectangle(0,
                             WIN_HEIGHT,
                             BOARD_WIDTH * BLOCK_SIZE + 2 * BORDER_SIZE,
                             WIN_HEIGHT - BORDER_SIZE,
                             blue);

    al_draw_filled_rectangle(0,
                             WIN_HEIGHT - BORDER_SIZE,
                             BORDER_SIZE,
                             WIN_HEIGHT - (BOARD_HEIGHT * BLOCK_SIZE) - BORDER_SIZE,
                             blue);

    al_draw_filled_rectangle(BOARD_WIDTH * BLOCK_SIZE + BORDER_SIZE,
                             WIN_HEIGHT - BORDER_SIZE,
                             BOARD_WIDTH * BLOCK_SIZE + 2 * BORDER_SIZE,
                             WIN_HEIGHT - (BOARD_HEIGHT * BLOCK_SIZE) - BORDER_SIZE,
                             blue);
}

void draw_rect_on_board(int x, int y, ALLEGRO_COLOR color) {
    int start_x = BORDER_SIZE + x * BLOCK_SIZE;
    int start_y = WIN_HEIGHT - (BORDER_SIZE + y * BLOCK_SIZE);

    int end_x = BORDER_SIZE + (x + 1) * BLOCK_SIZE;
    int end_y = WIN_HEIGHT - (BORDER_SIZE + (y + 1) * BLOCK_SIZE);

    al_draw_filled_rectangle(start_x, start_y,
                             end_x, end_y,
                             al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(start_x + BLOCK_BORDER_SIZE, start_y - BLOCK_BORDER_SIZE,
                             end_x - BLOCK_BORDER_SIZE, end_y + BLOCK_BORDER_SIZE,
                             color);
}

void draw_board() {
    // TODO
}

void draw_piece(int x, int y, int piece, int rotation) {
    // TODO
}

int main() {
    srand(time(0));
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    ALLEGRO_DISPLAY *display = al_create_display(WIN_WIDTH, WIN_HEIGHT);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // prima piesa
    int piece = rand_int(0, 6);
    int rotation = rand_int(0, 3);
    int pos_x = (BOARD_WIDTH / 2) + get_x_displacement(piece, rotation);
    int pos_y = BOARD_HEIGHT - 1 + get_y_displacement(piece, rotation);

    double start_time = 0;
    double end_time = 0;
    double accumulated_time = 0;
    double tick_time = 1;
    int draw = 1;

    while (!game_over()) {
        double delta_time = end_time - start_time;
        start_time = al_get_time();

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            draw = 1;
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_D:
                // TODO miscati piesa la dreapta (daca se poate)
                break;
            case ALLEGRO_KEY_A:
                // TODO miscati piesa la stanga (daca se poate)
                break;
            case ALLEGRO_KEY_W:
                // TODO rotiti piesa (daca se poate)
                break;
            case ALLEGRO_KEY_S:
                // TODO miscati piesa in jos (daca se poate)
                break;
            }
        }

        accumulated_time += delta_time;
        if (accumulated_time > tick_time) {
            draw = 1;
            accumulated_time -= tick_time;

            // TODO miscarea in jos a piesei (daca se poate).
            // Daca piesa nu mai poate sa se miste in jos,
            // plasati-i cubuletele pe tabla de joc si verificati
            // daca a fost creata o linie. Nu uitati sa generati
            // o piesa noua
        }

        // drawing code
        if (draw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_background();
            draw_board();
            draw_piece(pos_x, pos_y, piece, rotation);
            al_flip_display();
            draw = 0;
        }

        end_time = al_get_time();
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}
