# Challenge 6

Lui Razvan ii plac pozele despre facultate, insa nu doreste le dea share studentilor in mod direct. Asa
ca le cripteaza. Imaginea `input.png` a fost criptata de Razvan cu un algoritm de tip `rolling xor`. 
Gasiti un algoritm de decriptare pentru a putea vizualiza poza.

## Special Rolling XOR (Crypting)
  * Se citeste poza cu 4 componente (`RGBA`)
    #### Algoritm
    * PAS 1: Primul byte este lasat in pace.
    * PAS 2: Se itereaza prin restul de bytes ai pozei.
    * PAS 3: Byte-ului curent i se aplica `XOR` cu byte-ul precedent.
    * PAS 4: Pentru ca poza sa fie si mai obfuscata, Razvan a aplicat acest algoritm de `K` ori.
  * Se exporteaza poza cu 4 componente (`RGBA`).
### Valoarea lui `K`
  * Este restul impartirii `PIN`-ului lui Razvan (determinat laboratorul anterior) la cel mai mare numar prim mai mic decat `200`. 

