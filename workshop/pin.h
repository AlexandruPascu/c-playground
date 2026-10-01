#ifndef PLAYGROUND_PIN_H
#define PLAYGROUND_PIN_H
#include <stdint.h>
uint32_t pg_pin_hash(const char *pin);
int pg_find_pin(uint32_t hash, char pin[5]);
int pg_prime_below(int limit);
#endif
