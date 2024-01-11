#include "vector.h"
#include <math.h>
double vector_mod(vector* a) {
    // Distance from origin
    return sqrt(pow(a->x, 2) + pow(a->y, 2) + pow(a->z, 2));
}