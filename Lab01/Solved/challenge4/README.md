# Challenge 4
Se da o imagine sub forma de matrice ce contine culorile alb `0` si negru `1` din fisierul `challenge4.txt`.
Sa se determine numarul de pixeli din cea mai mare portiune neagra. (recursiv)

# Exemplu
## Casual input:
```
5 6
1 0 1 0 1 1
1 1 1 0 0 1
1 0 0 0 1 1
1 0 1 1 0 0
1 1 1 1 0 1
```
## Casual output (stdout):
```
Maximum area: 13
```

## Maintained implementation

The root CMake target is `components`. It uses an explicit DFS stack instead of recursion so large regions do not exhaust the call stack. The original exercise statement above is preserved. See the root README for input limits and build commands.
