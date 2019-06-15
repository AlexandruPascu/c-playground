#include <stdlib.h>

int secret_hash(unsigned char *str) {

    int hash = 191;
    int c;

    while (c = *str++)
        hash = ((hash << 2) * c + hash * c) + c;

    return hash;
}
char* pin_to_string(int pin) {
    char *result = malloc(5 * sizeof(char));
    result[0] = '0' + pin / 1000;
    result[1] = '0' + (pin % 1000) / 100;
    result[2] = '0' + (pin % 100) / 10;
    result[3] = '0' +  pin % 10;
    result[4] = 0;
    return result;
}
