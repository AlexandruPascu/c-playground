# Tetris

Astăzi veți implementa un joc funcțional de tetris pornind de la scheletul de
cod dat.

## Stocarea pieselor și rotiri

Pentru a ne simplifica munca, vom considera că fiecare piesă este reprezentată de o matrice 5x5.
O valoare nenula dintr-o matrice ca aceasta reprezintă un bloculeț din piesă. În fișierul `pieces.h`
există o matrice patru-dimensională numită `pieces`, definită astfel:

```C++
    char pieces [7 /*kind */ ][4 /* rotation */ ][5 /* vertical blocks */ ][5 /* horizontal blocks */ ]
```

Practic, pentru fiecare piesă stocăm bloculețele pentru fiecare rotație. Un dezavantaj al acestui mod de stocare
este faptul că atunci când punem o piesă în scenă aceasta nu va fi poziționată fix unde trebuie. Pentru a repara
asta, am mai definit (tot în `pieces.h`) o matrice numită `initial_displacement`:

```C++
    int initial_displacement  [7 /*kind */ ][4 /* r2otation */ ][2 /* position */]
```

`initial_displacement[0][0][0]` reprezintă cu cât de mult trebuie să mutăm o piesă de tip `Square`, nerotită, pe axa x.
Pentru a vedea cât de mult trebuie să mutăm aceeași piesă pe axa y, accesăm `initial_displacement[0][0][1]`.

În partea de jos a fișierului `pieces.h` există definite câteva funcții de ajutor:

 - ```C++
    int get_block(int piece, int rotation, int x, int y)
    ```
    Întoarce bloculețul de la poziția (x, y) a piesei `piece`, rotită la `rotation`.
    
 - ```C++
    int get_x_displacement(int piece, int rotation)
    ```
    Întoarce cât de mult trebuie să mutăm o piesă `piece`, rotită la `rotation` pe axa x.
    
 - ```C++
    int get_y_displacement(int piece, int rotation)
    ```
    Întoarce cât de mult trebuie să mutăm o piesă `piece`, rotită la `rotation` pe axa y.
    
- ```C++
    int next_rotation(int rotation)
    ```
    Întoarce următoarea rotație după `rotation`.
    
## Implementare

Pentru a face jocul să funcționeze, aveți de implementat o serie de funcții din fișierul `tetris.c`.
Starea curentă a tablei de joc este stocată în variabila globală `board`
(definită că `int board[BOARD_HEIGHT][BOARD_WIDTH]`).

- ```C++
  void place_piece(int x, int y, int piece, int rotation)
  ```
  Această funcție trebuie să pună bloculetele piesei `piece`, rotită la `rotation` pe tabla de joc. Atenție, trebuie
  să puneți doar bloculetele, nu copiați și valorile de `0`.
  
- ```C++
  int game_over()
  ```
  Această funcție trebuie să întoarcă 1 dacă s-a terminat jocul și 0 dacă nu s-a terminat. Vom considera că
  s-a terminat jocul dacă pe rândul cel mai de sus din tablă există cel puțin un bloculet.
  
- ```C++
  void delete_line(int line)
  ```
  Această funcție trebuie să elimine linia `line` de pe tabla de joc. Puteți face acest lucru prin copierea
  fiecărei linii de deasupra liniei line cu o linie mai jos.
  
- ```C++
  int can_delete_line(int line)
  ```
  Această funcție trebuie să întoarcă 1 dacă linia `line` poate fi eliminată (aceasta este plină de bloculete)
  și 0 dacă nu.
  
- ```C++
  void delete_possible_lines()
  ```
  Această funcție trebuie să elimine toate liniile pline din tablă de joc.
  
- ```C++
  int is_possible_movement(int x, int y, int piece, int rotation)
  ```
  Această funcție trebuie să întoarcă 1 dacă piesa `piece`, la rotația `rotation` poate fi pusă
  pe tabla de joc la poziția (x, y). Altfel, întoarceți 0.
  
- ```C++
  void draw_board()
  ```
  Această funcție trebuie să deseneze starea curentă a tablei de joc. Vă puteți folosi de funcția
  `draw_rect_on_board`.
  
- ```C++
  void draw_piece(int x, int y, int piece, int rotation)
  ```
  Această funcție trebuie să deseneze piesa `piece`, rotită la `rotation`, la poziția (x, y).
  
 - În `main`, în locurile marcate cu `TODO` va trebui să implementați mișcarea pieselor și răspunsul la comenzille 
   utilizatorului. Vom folosi următoarele taste:
    - `D` mișcă piesa la dreapta
    - `A` mișcă piesa la stânga
    - `W` rotește piesa
    - `S` face piesa să coboare mai repede
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
