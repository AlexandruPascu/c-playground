# Challenge 1
TODO link-urile la fiecare function call
## Instalarea bibliotecii Allegro

Urmariti instructiunile de
[aici](https://github.com/liballeg/allegro_wiki/wiki/Install-Allegro-from-Ubuntu-PPAs)
pentru a instala biblioteca Allegro.

## Hello, World!

Scopul acestui challenge este sa va familiarizati cu modul in care sunt
initializate modulele in Allegro si sa invatati cum se deseneaza text.

Incepeti prin a citi si a compila codul din `examples/helloworld.c` prin rularea
comenzii `gcc -o helloworld helloworld.c -Wall -Wextra -lallegro`

La rularea `./helloworld` ar trebui sa apara pe ecran o fereastra pentru 2
secunde.

Descarcati fontul **Orbitron Black** de
[aici](https://fonts2u.com/orbitron-black.font). Dupa dezarhivare, mutati
fisierul `.ttf` in acelasi director cu fisierul `helloworld.c`.

Pentru desenarea de text, Allegro foloseste un modul numit `allegro_font`.
Acesta trebuie inclus in sursa folosind `#include <allegro5/allegro_font.h>`.
Pentru ca fontul pe care il vom folosi este de tip `ttf`, trebuie de asemenea sa
incarcam si sa initializam modulul `allegro_ttf` (care se include folosind
`#include <allegro5/allegro_ttf.h>`). Pentru a initializa modulele precedente,
apelati functiile `al_init_font_addon()` si `al_init_ttf_addon()` dupa ce ati
creat display-ul.

Pentru a incarca fontul pe care l-ati descarcat, folositi functia
`al_load_font`. Aceasta primeste trei parametri (in aceasta ordine):

 * `char const *filename` - numele fisierului din care sa incarce fontul
 * `int size` - marimea la care sa incarce fontul
 * `int flags` - diferite flag-uri pentru incarcarea fontului. In cazul nostru,
   puteti sa setati valoarea acestui parametru la `0`.

`al_load_font()` intoarce un un pointer catre un `ALLEGRO_FONT`.

Pentru a desena text, folositi functia `al_draw_text`. Aceasta primeste sase
parametri (in aceasta ordine):

 * `const ALLEGRO_FONT *font` - un font, spre exemplu cel incarcat de
   `al_load_font`
 * `ALLEGRO_COLOR color` - o culoare (puteti crea o culoare folosind
   `al_map_rgb`)
 * `float x` - coordonata x
 * `float y` - coordonata y
 * `int flags` - acest parametru poate avea trei valori:
   * `ALLEGRO_ALIGN_LEFT` - punctul (x, y) va fi imediat in stanga textului
     desenat
   * `ALLEGRO_ALIGN_CENTRE` - punctul (x, y) va fi in centrul textului desenat
   * `ALLEGRO_ALIGN_RIGHT` - punctul (x, y) va fi imediat in dreapta textului
     desenat
 * `char const *text` - textul pe care sa il deseneze

Cand compilati, va trebui sa pasati si flagurile `-lallegro_font` si
`-lallegro_ttf`.

Nu uitati sa apelati `al_flip_display()` dupa ce ati desenat textul si inainte
de `al_rest()`!
