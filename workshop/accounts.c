#include "apps.h"
#include "pin.h"
#include "../common/input.h"
#include <string.h>
#ifndef PLAYGROUND_ACCOUNTS_FILE
#define PLAYGROUND_ACCOUNTS_FILE "challenge5.txt"
#endif
typedef struct { char name[100]; uint32_t hash; int balance; } Client;
static int is_pin(const char *text) {
    if (strlen(text) != 4) return 0;
    for (int i = 0; i < 4; ++i) if (text[i] < '0' || text[i] > '9') return 0;
    return 1;
}
int pg_accounts(int argc, char **argv) {
    Client *clients;
    FILE *input;
    int count, status = 0, recover = 0, target = -1;
    const char *path = PLAYGROUND_ACCOUNTS_FILE;
    char token[100];
    if (argc == 2 && !strcmp(argv[1], "--help")) {
        puts("Usage: pin_lab [--recover] [accounts.txt]\nLogin with a fixture username and four-digit PIN; see_balance displays its balance; exit logs out.\n--recover prints the four-digit PIN matching Razvan's workshop hash."); return 0;
    }
    if (argc > 1 && !strcmp(argv[1], "--recover")) recover = 1;
    if (argc > 1 + recover) path = argv[1 + recover];
    if (argc > 2 + recover) { fputs("Too many arguments. Use --help.\n", stderr); return 1; }
    input = fopen(path, "r");
    if (!input) { fputs("Could not open the accounts file.\n", stderr); return 1; }
    if (pg_read_int(input, &count) != 1 || count < 1 || count > 10000) { fclose(input); fputs("Invalid account count.\n", stderr); return 1; }
    clients = calloc((size_t)count, sizeof(*clients));
    if (!clients) { fclose(input); return 1; }
    for (int i = 0; i < count; ++i) {
        int hash;
        if (pg_read_token(input, clients[i].name, sizeof(clients[i].name)) != 1 ||
            pg_read_int(input, &hash) != 1 || pg_read_int(input, &clients[i].balance) != 1 || clients[i].balance < 0) {
            status = 1; break;
        }
        clients[i].hash = (uint32_t)hash;
        for (int j = 0; j < i; ++j) if (!strcmp(clients[j].name, clients[i].name)) status = 1;
        if (!strcmp(clients[i].name, "Razvan")) target = i;
    }
    if (!status && pg_read_token(input, token, sizeof(token)) != 0) status = 1;
    fclose(input);
    if (status) { free(clients); fputs("Malformed account data.\n", stderr); return 1; }
    if (!recover) {
        for (;;) {
            int id = -1, read_status;
            puts("Type your user or exit to exit:"); fflush(stdout);
            read_status = pg_read_token(stdin, token, sizeof(token));
            if (read_status == 0 || (read_status == 1 && !strcmp(token, "exit"))) break;
            if (read_status < 0) { status = 1; break; }
            for (int i = 0; i < count; ++i) if (!strcmp(clients[i].name, token)) id = i;
            if (id < 0) { puts("User not found!"); continue; }
            puts("Type your PIN:"); fflush(stdout);
            if (pg_read_token(stdin, token, sizeof(token)) != 1) { status = 1; break; }
            if (!is_pin(token) || pg_pin_hash(token) != clients[id].hash) { puts("Incorrect PIN!"); continue; }
            puts("Type see_balance to see your balance or exit to log out:"); fflush(stdout);
            while ((read_status = pg_read_token(stdin, token, sizeof(token))) == 1 && strcmp(token, "exit")) {
                if (!strcmp(token, "see_balance")) printf("%s's balance is %d bitcoin.\n", clients[id].name, clients[id].balance);
                else puts("Unknown command.");
                fflush(stdout);
            }
            if (read_status < 0) status = 1;
            if (read_status <= 0) break;
        }
    }
    if (!status) {
        char pin[5];
        if (target >= 0 && pg_find_pin(clients[target].hash, pin)) printf("Razvan's pin is %s\n", pin);
        else puts("No pin found!");
    } else fputs("Malformed input.\n", stderr);
    free(clients);
    return status;
}
