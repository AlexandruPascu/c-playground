#ifndef PLAYGROUND_LIST_H
#define PLAYGROUND_LIST_H
#include <stddef.h>
typedef struct PgNode { int value; struct PgNode *next; } PgNode;
typedef struct { PgNode *head, *tail; size_t size; } PgList;
int pg_list_push(PgList *list, int value, int front);
int pg_list_pop(PgList *list, int *value, int front);
int pg_list_insert_sorted(PgList *list, int value);
int pg_list_contains(const PgList *list, int value);
int pg_list_extrema(const PgList *list, int *minimum, int *maximum);
void pg_list_clear(PgList *list);
#endif
