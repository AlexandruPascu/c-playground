/* The workshop polynomial hash uses explicitly defined 32-bit wrapping. */
#include "pin.h"
#include <stdio.h>
uint32_t pg_pin_hash(const char *pin) {
    uint32_t hash = 191;
    while (*pin) { uint32_t c = (unsigned char)*pin++; hash = 5u * hash * c + c; }
    return hash;
}
int pg_find_pin(uint32_t hash, char pin[5]) {
    for (int candidate = 0; candidate <= 9999; ++candidate) {
        snprintf(pin, 5, "%04d", candidate);
        if (pg_pin_hash(pin) == hash) return 1;
    }
    pin[0] = '\0';
    return 0;
}
int pg_prime_below(int limit) {
    for (int candidate = limit - 1; candidate >= 2; --candidate) {
        int prime = 1;
        for (int divisor = 2; divisor <= candidate / divisor; ++divisor)
            if (candidate % divisor == 0) { prime = 0; break; }
        if (prime) return candidate;
    }
    return 0;
}
