# Challenge 3
Se citeste de la tastatura un intreg n si n elemente ale unui vector de float-uri, reprezentand frametimeuri.
Frametime este distanta de timp dintre doua frameuri.
Sa se scrie doua functii:
1. `int* get_fps(int n, float *frametimes)`
   - Numarul de frameuri dintr-o secunda
   - Atentie! Un frametime poate dura mai mult de o secunda.
2. `int get_average_fps(int n, int *fps)`
   - Intoarce media aritmetica a frameurilor din cele n secunde.

# Example
## Casual input:
```
6
0.5 0.3 0.6 0.2 0.8 0.1
```
## Casual output (stdout):
```
Fps array: 3 3 2
Average fps: 2
```
## Explanation:
* Pentru prima secunda incap 3 frame-uri (`0.5, 0.3, 0.6`).
* Pentru a doua secunda incap 3 frame-uri (`0.6, 0.2, 0.8`).
* Pentru ultima secunda incap 2 frame-uri (`0.8, 0.1`).
## Tricky input:
```
6
0.7 1 0.1 3 0.2 0.5
```
## Tricky output (stdout):
```
Fps array: 2 3 1 1 2 1
Average fps: 1
```

## Explanation:
* Se poate observa ca al patrulea frametime dureaza `3` secunde, de aceea in
secundele 2 si 3 avem un singur frame in fiecare.
