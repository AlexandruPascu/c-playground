#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int value;
	struct Node *next;
} Node;

typedef struct {
	Node *first, *last;
} List;

List *create_list() {
	List *l = malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	return l;
}

void add_first(List *l, int value) {
	Node *new = malloc(sizeof(Node));
	new->value = value;
	new->next = l->first;
	if (l->first == NULL) {
		l->last = new;
	}
	l->first = new;
}

void add_last(List *l, int value) {
	Node *new = malloc(sizeof(Node));
	new->value = value;
	new->next = NULL;
	if (l->last == NULL) {
		l->first = l->last = new;
	} else {
		l->last->next = new;
		l->last = new;
	}
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

int main() {
	List *l1;
	l1 = create_list();
	add_first(l1, 31);
	add_first(l1, 50);
	add_first(l1, 10);
	add_last(l1, 100);
	print_linked_list(l1);
	remove_last(l1);
	remove_last(l1);
	print_linked_list(l1);
	return 0;
}
