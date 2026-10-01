#ifndef PLAYGROUND_IMAGE_OPS_H
#define PLAYGROUND_IMAGE_OPS_H
#include <stddef.h>
void pg_gray(unsigned char *rgba, size_t pixels, int weighted);
void pg_xor_decode(unsigned char *bytes, size_t length, unsigned int passes);
#endif
