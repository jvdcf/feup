#include "vector.h"
double vector_sprod(vector* a, vector* b) {
    // Scalar product of two vectors
    // a . b = |a| * |b| * cos(theta) = a.x * b.x + a.y * b.y + a.z * b.z
    return a->x * b->x + a->y * b->y + a->z * b->z;
}