#include "vector.h"
vector* vector_vprod(vector* a, vector* b) {
    // Cross Product of two vectors
    //          |   i  j   k  |         {a.y * b.z - b.y * a.z} 
    // a x b =  | a.x a.y a.z |    =    {b.x * a.z - a.x * b.z}
    //          | b.x b.y b.z |         {a.x * b.y - b.x * a.y}
    return vector_new(a->y * b->z - a->z * b->y,
                      a->z * b->x - a->x * b->z,
                      a->x * b->y - a->y * b->x);
}