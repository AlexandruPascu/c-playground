#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int value;
	struct Node *next;
} Node;

typedef struct {
	Node *first, *last;
} List;

List *create_list(void) {
	List *l = malloc(sizeof(List));
	if (!l) return NULL;
	l->first = NULL;
	l->last = NULL;
	return l;
}

int add_first(List *l, int value) {
	Node *new = malloc(sizeof(Node));
	if (!new) return 0;
	new->value = value;
	new->next = l->first;
	if (l->first == NULL) {
		l->last = new;
	}
	l->first = new;
	return 1;
}

int add_last(List *l, int value) {
	Node *new = malloc(sizeof(Node));
	if (!new) return 0;
	new->value = value;
	new->next = NULL;
	if (l->last == NULL) {
		l->first = l->last = new;
	} else {
		l->last->next = new;
		l->last = new;
	}
	return 1;
}

void remove_last(List *l) {
	Node *tmp = l->last;
	Node *i;
	//printf("%d %d\n", l->first->value, l->last->value);
	if (l->first == l->last) {
		i = l->first;
		l->first = l->last = NULL;
		free(i);
		return;
	}
	for (i = l->first; i->next != l->last; i = i->next);
	l->last = i;
	l->last->next = NULL;
	free(tmp);
}

void print_linked_list(List *l) {
	for (Node *i = l->first; i; i = i->next) {
		printf("%d ", i->value);
	}
	printf("\n");
}

void destroy_list(List *list) {
    Node *node = list->first;
    while (node) { Node *next = node->next; free(node); node = next; }
    free(list);
}
int main(void) {
    List *list = create_list();
    if (!list) return 1;
    if (!add_first(list, 31) || !add_first(list, 50) || !add_first(list, 10) || !add_last(list, 100)) {
        destroy_list(list); return 1;
    }
    print_linked_list(list);
    remove_last(list); remove_last(list);
    print_linked_list(list);
    remove_last(list); remove_last(list); remove_last(list); /* Empty-list removal is safe. */
    if (list->first || list->last) { destroy_list(list); return 1; }
    destroy_list(list);
    return 0;
}
