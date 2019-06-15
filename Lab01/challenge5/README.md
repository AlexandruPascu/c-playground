# Challenge 5
Un bancomat primitiv foloseste ca baza de date fisierul `challenge5.txt`, ce are parolele utilizatorilor criptate cu un algoritm de hash-ing polinomial.
Programul functioneaza in felul urmator:
  * Se citesc numele utilizatorilor, pin-urile hash-uite si numarul de monede din contul lor.
  * Se citesc comenzi de la tastatura:
    * Daca se citeste un nume valid de utilizator se va citi pin-ul.
    * Pin-ul se trece prin functia `secret_hash(char *pin)` ce returneaza un int care reprezinta hash-ul pin-ului.
    * Se compara valoarea hash-ul curent cu hash-ul din fisier pentru a autentifica utilizatorul.
  * Odata autentificat, daca se citeste comanda `see_balance` se afisaza numarul de bitcoin din contul curent.
  * Comanda `exit` termina programul.
Utilizatorul `Laur` are pin-ul `1234`. Logati-va pe contul lui si notati cate bitcoin are.
Dupa ce ati inteles cum functioneaza sistemul, trebuie sa scrieti functia `break_pin` pentru a afisa pin-ul utilizatorului `Razvan`.
Notati si cate bitcoin are acest utilizator.
# Exemplu
## Casual example:
```
Type your user or exit to exit:
Laur
Type your PIN:
1234
Type see_balance to see your balance or exit to exit:
see_balance
Laur's balance is ? bitcoin.
Razvan's pin is ?.
```
