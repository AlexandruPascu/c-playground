# Challenge 1


Pentru acest challenge va trebui să creați o fereastră care să poată fi închisă
ca o fereastră obișnuită (prin apăsarea butonului _x_) sau prin apăsarea tastei
`Escape`.

Mai întâi , trebuie să activați tastatura prin apelarea funcției
[`al_install_keyboard`](https://liballeg.org/a5docs/trunk/keyboard.html#al_install_keyboard).
Creați o coadă de evenimente folosind funcția
[`al_create_event_queue`](https://liballeg.org/a5docs/trunk/events.html#al_create_event_queue).
Folosiți
[`al_register_event_source`](https://liballeg.org/a5docs/trunk/events.html#al_register_event_source)
pentru a primi evenimente de la tastatură și fereastră . Pentru a obține sursa
de evenimente a tastaturii folosiți
[`al_get_keyboard_event_source`](https://liballeg.org/a5docs/trunk/keyboard.html#al_get_keyboard_event_source).
Pentru a obține sursa de evenimente a ferestrei folosiți
[`al_get_display_event_source`](https://liballeg.org/a5docs/trunk/display.html#al_get_display_event_source).

În main loop-ul programului, așteptați primirea unui eveniment folosind
[`al_wait_for_event`](https://liballeg.org/a5docs/trunk/events.html#al_wait_for_event)
și decideți dacă trebuie să închideți fereastra în funcție de `event.type` și
`event.keyboard.keycode`. Găsiți o listă cu toate `keycode`-urile posibile
[aici](https://liballeg.org/a5docs/trunk/keyboard.html#key-codes). O listă cu
valori posibile ale `event.type` găsiți
[aici](https://liballeg.org/a5docs/trunk/events.html).

Nu uitați să distrugeți coada de evenimente la sfârșitul programului folosind
[`al_destroy_event_queue`](https://liballeg.org/a5docs/trunk/events.html#al_destroy_event_queue).
