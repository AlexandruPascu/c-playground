#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int secret_hash(unsigned char *str);
char* pin_to_string(int pin);

void read_clients(int *n, char ***users, int **pins, int **balances) {
    FILE *in = fopen("challenge5.txt", "r");
    fscanf(in, "%d ", n);
    
    *users = (char**) malloc((*n) * sizeof(char*));
    
    // TODO 1: Aloca memorie pentru vectorul de pin-uri si de bitcoin
    *pins = 0;
    *balances = 0;

    char user[100];
    int current_pin;
    int current_balance;

    for (int i = 0; i < *n; i++) {
        fscanf(in, "%s %d %d", user, &current_pin, &current_balance);
        users[0][i] = strdup(user);
        pins[0][i] = current_pin;
        balances[0][i] = current_balance;
    }
}
int search_user(char *user_to_search, int n, char **users) {
    // TODO 2: Cauta utilizatorul indicat in vectorul de utilizatori
    // Returneaza -1 daca nu l-ai gasit
    
    return -1;
}
void logged_in(int id, char *user, int *balance) {
    char command[100];
    while(1) {
        printf("Type see_balance to see your balance or exit to exit:\n");
        scanf("%s", command);
        if(strcmp(command, "see_balance") == 0) {
            printf("%s's balance is %d bitcoin.\n", user, *balance);
        }
        if(strcmp(command, "exit") == 0) {
            return;
        }
    }
}

void break_pin(int n, char **users, int *pins) {
    // TODO 4: Afisaza pin-ul utilizatorului Razvan cu formatul: "Razvan's pin is %s\n"
    // HINT 1: Functia pin_to_string primeste un numar de 4 cifre si intoarce sirul de
    //       caractere corespunzator. Ex: pin_to_string(1234) = "1234"
    // HINT 2: Functia secret_hash primeste un string cu caracterele pinului si
    //       intoarce un numar ce reprezinta hash-ul (criptarea) acelor caractere.
    // Daca pin-ul nu a fost gasit, afisati "No pin found!".
    
    printf("No pin found!\n");
}
int main() {
    int n;
    char **users;
    int *pins;
    int *balances;

    read_clients(&n, &users, &pins, &balances);

    for(;;) {
        char user[100];
        char pin[100];
        printf("Type your user or exit to exit:\n");
        scanf("%s", user);
        if(strcmp(user, "exit") == 0) {
            break;
        }
        int id = search_user(user, n, users);
        if(id >= 0) {
            printf("Type your PIN:\n");
            scanf("%s", pin);
            
            // TODO 3: Verifica daca hash-ul pinurilor se potrivesc
            if(0) {
                logged_in(id, users[id], &(balances[id]));
                break;
            } else {
                printf("Incorrect PIN!\n");
            }
        } else {
            printf("User not found!\n");
        }
    }

    break_pin(n, users, pins);

    return 0;
}
