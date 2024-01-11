// Note: Solution not provided by the teachers
#include "list.h"
#include <stdio.h>

void list_add_first(int val, list *l) {
    node* n = node_new(val, l->first);
    l->first = n;
    l->size++;
}