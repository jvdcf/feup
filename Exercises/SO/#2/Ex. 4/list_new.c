#include "list.h"
#include <stddef.h>
#include <stdlib.h>
list* list_new() {
list* l = (list*) malloc(sizeof(list));
l->size = 0;
l->first = NULL;
return l;
}