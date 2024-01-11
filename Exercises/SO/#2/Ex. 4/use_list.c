// Note: Solution not provided by the teachers
#include "list.h"
#include <stdio.h>

int main(int argc, char** argv) {
    printf("Testing list.h\n");

    printf("1. List creation and print\n");
    list* l1 = list_new();
    list* l2 = list_new_random(3, 10);
    list_print(l1);
    list_print(l2);

    printf("2. Adders, removers, getters and size\n");
    list* l3 = list_new();
    list_add_first(1, l3);
    list_add_first(2, l3);
    list_add_last(3, l3);
    list_add_last(4, l3);
    list_add_last(5, l3);
    list_remove_first(l3);
    list_remove_last(l3);
    printf("First element: %d\n",list_get_first(l3)); // 1
    printf("Last element: %d\n",list_get_last(l3));   // 4
    printf("Size: %d\n",list_size(l3));               // 3

    return 0;
}