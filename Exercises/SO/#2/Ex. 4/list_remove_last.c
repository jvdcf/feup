// Note: Solution not provided by the teachers
#include "list.h"
#include <stddef.h>
#include <stdlib.h>

void list_remove_last(list *l) {
    node *n = l->first;
    node *previous = NULL;
    while (n->next != NULL) {
        previous = n;
        n = n->next;
    }
    previous->next = NULL;
    free(n);
    l->size--;
}