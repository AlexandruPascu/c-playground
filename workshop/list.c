#include "list.h"
#include <stdlib.h>
int pg_list_push(PgList *list, int value, int front) {
    PgNode *node = malloc(sizeof(*node));
    if (!node) return 0;
    node->value = value;
    node->next = front ? list->head : NULL;
    if (!list->head) list->head = list->tail = node;
    else if (front) list->head = node;
    else { list->tail->next = node; list->tail = node; }
    ++list->size;
    return 1;
}
int pg_list_pop(PgList *list, int *value, int front) {
    PgNode *node;
    if (!list->head) return 0;
    node = list->head;
    if (front || list->size == 1) {
        list->head = node->next;
        if (!list->head) list->tail = NULL;
    } else {
        PgNode *previous = list->head;
        while (previous->next != list->tail) previous = previous->next;
        node = list->tail;
        previous->next = NULL;
        list->tail = previous;
    }
    if (value) *value = node->value;
    free(node);
    --list->size;
    return 1;
}
int pg_list_insert_sorted(PgList *list, int value) {
    PgNode **position = &list->head, *node;
    while (*position && (*position)->value <= value) position = &(*position)->next;
    node = malloc(sizeof(*node));
    if (!node) return 0;
    node->value = value;
    node->next = *position;
    *position = node;
    if (!node->next) list->tail = node;
    ++list->size;
    return 1;
}
int pg_list_contains(const PgList *list, int value) {
    for (const PgNode *node = list->head; node; node = node->next)
        if (node->value == value) return 1;
    return 0;
}
int pg_list_extrema(const PgList *list, int *minimum, int *maximum) {
    if (!list->head) return 0;
    *minimum = *maximum = list->head->value;
    for (const PgNode *node = list->head->next; node; node = node->next) {
        if (node->value < *minimum) *minimum = node->value;
        if (node->value > *maximum) *maximum = node->value;
    }
    return 1;
}
void pg_list_clear(PgList *list) {
    while (pg_list_pop(list, NULL, 1)) { }
}
