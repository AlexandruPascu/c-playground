#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int secret_hash(unsigned char *str);
char* pin_to_string(int pin);

void read_clients(int *n, char ***users, int **pins, int **balances) {
    FILE *in = fopen("challenge5.txt", "r");
    fscanf(in, "%d ", n);

    *users = (char**) malloc((*n) * sizeof(char*));
    *pins = (int*) malloc((*n) * sizeof(int));
    *balances = (int*) malloc((*n) * sizeof(int));

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
    for (int i = 0; i < n; i++) {
        if(strcmp(user_to_search, users[i]) == 0)
            return i;
    }
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
/*
    TODO1: Get id of user "Razvan"
    TODO2: Take all pins from 0000 to 9999
    TODO3: Convert each pin to string using
           char* pin_to_string(int pin)
    TODO4: Hash the string
    TODO5: Compare hash to the one in pins
    TODO6: Print the pin string
 */
void break_pin(int n, char **users, int *pins) {
    int id = search_user("Razvan", n, users);

    for (int pin = 0; pin <= 9999; pin++) {

        char *pin_string = pin_to_string(pin);

        int hash = secret_hash(pin_string);

        if(hash == pins[id]) {
            printf("Razvan's pin is %s\n", pin_string);
            return;
        }
    }
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

            if(secret_hash(pin) == pins[id]) {
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
