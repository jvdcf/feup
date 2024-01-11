// Note: Solution not provided by the teachers
#include "list.h"
#include <stddef.h>

int list_get_last(list *l) {
    node *n = l->first;
    while (n->next != NULL) {
        n = n->next;
    }
    return n->val;
}