# Challenge 5
Convertirea imaginilor este usoara. Se pot gasi si tool-uri online. Si mai interesant este procesul de convertire a imaginilor din
`RGBA` in `greyscale`. 
### Average greyscale
  * O abordare naturala este cea de a calcula media celor 3 componente si a stabili fiecarei componente aceasta valoare.
  * Aceasta are ca rezultat un efect ce nu pare asa de natural.
  * Formula este:
   ```
   R = G = B = (R + G + B) / 3
   ```
  * Implementati aceasta metoda.
### Luminosity greyscale
   * Este o metoda mai sofisticata ce implica o medie ponderata.
   * Layer-ul alpha nu este modificat.
   * Imaginea devine mai soft
   * Formula pentru este:
   ```
   R = G = B = 0.2126 R + 0.7152 G + 0.0722 B
   ```
   * Implementati si observati diferentele.

### Hint
  * Exportati pozele cu formatul `png`.
