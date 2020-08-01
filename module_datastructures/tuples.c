#include "tuples.h"
#include "utilities.h"
#include "proj_exceptions.h"

#include <assert.h>
#include <stdlib.h>
#include <CException.h>

#define VECTOR 0.0
#define POINT 1.0

//may want to expose this later
void TUPLES_init_point(TUPLES_Point* t, double x, double y, double z) {
    assert(t);
    t->x = x;
    t->y = y;
    t->z = z;
    t->w = POINT;
}

TUPLES_Tuple* TUPLES_new() {
    TUPLES_Tuple* new_t = (TUPLES_Tuple*)malloc(sizeof(TUPLES_Tuple));
    if (!new_t)
        Throw(E_MALLOC_FAILED);
    return new_t;
}

TUPLES_Point* TUPLES_new_point(double x, double y, double z) {
    TUPLES_Point* new_point = TUPLES_new();
    TUPLES_init_point(new_point, x, y, z);
    return new_point;
}

//may want to expose this later
void TUPLES_init_vector(TUPLES_Vector* v, double x, double y, double z) {
    assert(v);
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = VECTOR;
}

TUPLES_Vector* TUPLES_new_vector(double x, double y, double z) {
    TUPLES_Vector* new_vector = TUPLES_new();
    TUPLES_init_vector(new_vector, x, y, z);
    return new_vector;
}

bool TUPLES_is_point(const TUPLES_Tuple *tuple) {
    assert(tuple);
    return double_equal(tuple->w, POINT);
}

bool TUPLES_is_vector(const TUPLES_Tuple *tuple) {
    assert(tuple);
    return double_equal(tuple->w, VECTOR);
}

void TUPLES_add(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, const TUPLES_Tuple* t2) {
    assert(t1);
    assert(t2);
    assert(dest);
    dest->x = t1->x + t2->x;
    dest->y = t1->y + t2->y;
    dest->z = t1->z + t2->z;
    dest->w = t1->w + t2->w;
    assert(TUPLES_is_vector(dest) || TUPLES_is_point(dest));
}
void TUPLES_subtract(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, const TUPLES_Tuple* t2) {
    assert(t1);
    assert(t2);
    assert(dest);
    dest->x = t1->x - t2->x;
    dest->y = t1->y - t2->y;
    dest->z = t1->z - t2->z;
    dest->w = t1->w - t2->w;
    assert(TUPLES_is_vector(dest) || TUPLES_is_point(dest));
}

void TUPLES_destroy(TUPLES_Tuple* tuple) {
    if (tuple) {
        free(tuple);
    }
}
