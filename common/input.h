#ifndef PLAYGROUND_INPUT_H
#define PLAYGROUND_INPUT_H
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* 1: token read, 0: EOF, -1: token too long or I/O error. */
static inline int pg_read_token(FILE *input, char *text, size_t capacity) {
    int c;
    size_t length = 0;
    int overflow = 0;
    do { c = fgetc(input); } while (c != EOF && isspace((unsigned char)c));
    if (c == EOF) return ferror(input) ? -1 : 0;
    do {
        if (length + 1 < capacity) text[length++] = (char)c;
        else overflow = 1;
        c = fgetc(input);
    } while (c != EOF && !isspace((unsigned char)c));
    if (capacity) text[length] = '\0';
    return overflow || ferror(input) ? -1 : 1;
}
static inline int pg_parse_int(const char *text, int *value) {
    char *end;
    long parsed;
    errno = 0;
    parsed = strtol(text, &end, 10);
    if (!*text || *end || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX) return 0;
    *value = (int)parsed;
    return 1;
}
static inline int pg_read_int(FILE *input, int *value) {
    char token[64];
    int status = pg_read_token(input, token, sizeof(token));
    return status == 1 ? (pg_parse_int(token, value) ? 1 : -1) : status;
}
#endif
