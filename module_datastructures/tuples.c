#include "tuples.h"
#include "utilities.h"
#include "proj_exceptions.h"

#include <assert.h>
#include <stdlib.h>
#include <CException.h>

#define VECTOR 0.0
#define POINT 1.0

//may want to expose this later
void TUPLES_init_point(TUPLES_Point t, double x, double y, double z) {
    assert(t);
    t->x = x;
    t->y = y;
    t->z = z;
    t->w = POINT;
}

TUPLES_Point TUPLES_new_point(double x, double y, double z) {
    TUPLES_Point new_point = (TUPLES_Point)malloc(sizeof(TUPLES_Tuple));
    if (!new_point)
        Throw(E_MALLOC_FAILED);
    TUPLES_init_point(new_point, x, y, z);
    return new_point;
}

//may want to expose this later
void TUPLES_init_vector(TUPLES_Vector v, double x, double y, double z) {
    assert(v);
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = VECTOR;
}

TUPLES_Vector TUPLES_new_vector(double x, double y, double z) {
    TUPLES_Vector new_vector = (TUPLES_Vector)malloc(sizeof(TUPLES_Tuple));
    if (!new_vector)
        Throw(E_MALLOC_FAILED);
    TUPLES_init_vector(new_vector, x, y, z);
    return new_vector;
}

bool TUPLES_is_point(TUPLES_Tuple *tuple) {
    assert(tuple);
    return double_equal(tuple->w, POINT);
}

bool TUPLES_is_vector(TUPLES_Tuple *tuple) {
    assert(tuple);
    return double_equal(tuple->w, VECTOR);
}

void TUPLES_destroy(TUPLES_Tuple* tuple) {
    if (tuple) {
        free(tuple);
    }
}
