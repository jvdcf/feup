// Note: Solution not provided by the teachers
#include "complex.h"

complex* complex_div(complex* z, complex* w) {
    // z = a + bi; w = c + di:
    // z        z * conj(w)         (a + bi) * (c - di)         ac + bd   bc - ad
    // -    =   -----------     =   -------------------     =   ------- + ------- i
    // w           |w|²                   c² + d²               c² + d²   c² + d²

    complex* result = complex_new(0, 0);
    result->x = (z->x * w->x + z->y * w->y) / (w->x * w->x + w->y * w->y);
    result->y = (z->y * w->x - z->x * w->y) / (w->x * w->x + w->y * w->y);
    return result;
}