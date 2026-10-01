#include "image_ops.h"
void pg_gray(unsigned char *rgba, size_t pixels, int weighted) {
    for (size_t i = 0; i < pixels; ++i) {
        unsigned char *pixel = rgba + 4 * i;
        unsigned int gray = weighted ? (2126u * pixel[0] + 7152u * pixel[1] + 722u * pixel[2] + 5000u) / 10000u
                                     : ((unsigned int)pixel[0] + pixel[1] + pixel[2]) / 3u;
        pixel[0] = pixel[1] = pixel[2] = (unsigned char)gray;
    }
}
void pg_xor_decode(unsigned char *bytes, size_t length, unsigned int passes) {
    /* The forward in-place cipher reads the already encoded previous byte.
       Undo each pass backwards so that both operands remain encoded. */
    for (unsigned int pass = 0; pass < passes; ++pass)
        for (size_t i = length; i > 1; --i) bytes[i - 1] ^= bytes[i - 2];
}
