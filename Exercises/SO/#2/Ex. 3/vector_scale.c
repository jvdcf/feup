#include "vector.h"
vector* vector_scale(double n, vector* a) {
    return vector_new(n * a->x, n * a->y, n * a->z);
}