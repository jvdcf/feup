// Note: Solution not provided by the teachers
#include "list.h"
#include <stdio.h>

void list_print(list* l) {
    printf("List at %p with %d nodes:\n", l, l->size);
    printf("--------------------------------------------\n");

    node* n = l->first;
    for (int i = 0; i < l->size; i++) {
        printf("%d | %d\n", i, n->val);
        n = n->next;
    }
    printf("\n");
}