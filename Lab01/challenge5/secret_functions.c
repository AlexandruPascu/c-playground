#include "../../workshop/pin.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
int secret_hash(unsigned char *text) {
    uint32_t hash = pg_pin_hash((const char *)text);
    return hash <= INT_MAX ? (int)hash : -1 - (int)(UINT32_MAX - hash);
}
char *pin_to_string(int pin) {
    char *result;
    if (pin < 0 || pin > 9999) return NULL;
    result = malloc(5);
    if (result) snprintf(result, 5, "%04d", pin);
    return result;
}
