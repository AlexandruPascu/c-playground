# Challenge 2

În acest challenge veți crea un program care să afișeze la ieșirea standard
(`stdout`) de fiecare dată când primește un eveniment din una din următoarele
surse:


 * timer
   
   Se va afișa `timer` atunci când se primește un eveniment de tipul
   [`ALLEGRO_EVENT_TIMER`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_timer).

 * tastatura

   Se va afișa `key up` atunci când se primește un eveniment de tipul
   [`ALLEGRO_EVENT_KEY_UP`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_up)
   și `key down` atunci când se primește un eveniment de tipul
   [`ALLEGRO_EVENT_KEY_DOWN`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_key_down).
   Nu uitați să apelați funcția
   [`al_install_keyboard`](https://liballeg.org/a5docs/trunk/keyboard.html#al_install_keyboard)
   pentru a inițializa inputul de la tastatură.

 * mouse

  Se va afișa `mouse moved` atunci când se primește un eveniment de tipul
  [`ALLEGRO_EVENT_MOUSE_AXES`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_mouse_axeshttps://liballeg.org/a5docs/trunk/events.html#allegro_event_mouse_axes).
  Nu uitați să apelați funcția
  [`al_install_mouse`](https://liballeg.org/a5docs/trunk/mouse.html#al_install_mouse)
  pentru a inițializa inputul de la mouse și să înregistrați mouse-ul că o
  sursă de evenimente folosind
  [`al_get_mouse_event_source`](https://liballeg.org/a5docs/trunk/mouse.html#al_get_mouse_event_source).

 * display

   Se va afișa `exiting...` atunci când se primește un eveniment de tipul
   [`ALLEGRO_EVENT_DISPLAY_CLOSE`](https://liballeg.org/a5docs/trunk/events.html#allegro_event_display_close),
   după care se va închide programul.

Pentru a lucra cu un timer, mai întâi acesta trebuie creat; se folosește
funcția
[`al_create_timer`](https://liballeg.org/a5docs/trunk/timer.html#al_create_timer).
__Atentie__, funcția primește ca parametru un `double`, aveți grijă să nu
pasați valori de tip `int` cum ar fi `1 / 60`. Puteți în schimb să pasați `1.0
/ 60`. După creare, trebuie să adăugați în coadă de evenimente sursa de
evenimente a timerului care poate fi obținută cu funcția
[`al_get_timer_event_source`](https://liballeg.org/a5docs/trunk/timer.html#al_get_timer_event_source).
Înainte ca timerul să înceapă să emită evenimente acesta trebuie pornit cu
funcția
[`al_start_timer`](https://liballeg.org/a5docs/trunk/timer.html#al_start_timer).
După ce ați terminat de lucrat cu un timer, acesta trebuie distrus cu funcția
[`al_destroy_timer`](https://liballeg.org/a5docs/trunk/timer.html#al_destroy_timer).
