# Challenge 3


În acest challenge veți anima mișcarea unui pătrat. Pătratul va fi controlat de
tastele W, A, S și D.

Pentru a determina starea unei taste, veți folosi un `struct` numit
[`ALLEGRO_KEYBOARD_STATE`](https://liballeg.org/a5docs/trunk/keyboard.html#allegro_keyboard_state),
funcția
[`al_get_keyboard_state`](https://liballeg.org/a5docs/trunk/keyboard.html#al_get_keyboard_state),
funcția
[`al_key_down`](https://liballeg.org/a5docs/trunk/keyboard.html#al_key_down),
evenimentul
[`ALLEGRO_EVENT_KEY_DOWN`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_down),
evenimentul
[`ALLEGRO_EVENT_KEY_UP`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_up)
și evenimentul
[`ALLEGRO_EVENT_TIMER`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_timer).
De fiecare dată când se primește unul din evenimentele anterioare, se va apela
[`al_get_keyboard_state`](https://liballeg.org/a5docs/trunk/keyboard.html#al_get_keyboard_state)
pentru a salva starea curentă a tastaturii. Folosiți
[`al_key_down`](https://liballeg.org/a5docs/trunk/keyboard.html#al_key_down)
pentru a afla dacă o tasta este apăsată.
