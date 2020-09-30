#include <exceptions.h>
#include <assert.h>
#include "testshape.h"

const SHAPE_vtable TESTSHAPE_vtable = {
        &TESTSHAPE_local_intersect,
        &SHAPE_delete,
        &TESTSHAPE_local_normal_at
};

void TESTSHAPE_init(TESTSHAPE_TestShape* shape) {
    assert(shape);
    shape->size = 5.0;
    SHAPE_init(&shape->parent, &TESTSHAPE_vtable);
}

TESTSHAPE_TestShape* TESTSHAPE_new() {
    TESTSHAPE_TestShape* testshape = malloc(sizeof(TESTSHAPE_TestShape));
    if (!testshape) {
        Throw(E_MALLOC_FAILED);
    }
    TESTSHAPE_init(testshape);
    return testshape;
}

void TESTSHAPE_delete(TESTSHAPE_TestShape* testshape) {
    assert(testshape);
    SHAPE_destroy(&testshape->parent);
    free(testshape);
}

void TESTSHAPE_local_intersect(RAY_Intersections* dest, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(dest);
    assert(shape);
    assert(local_ray);
    UNUSED(dest);
    ((TESTSHAPE_TestShape*)shape)->saved_ray = *local_ray;
}

void TESTSHAPE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Vector* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(hit);
    UNUSED(shape);
    TUPLES_init_vector(local_normal, local_point->x, local_point->y, local_point->z);
}
