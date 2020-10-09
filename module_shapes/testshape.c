#include <exceptions.h>
#include <assert.h>
#include "testshape.h"

void TESTSHAPE_bounds_of(const SHAPE_Shape* shape, BOUND_Box* box) {
    assert(shape);
    assert(box);
    UNUSED(shape);
    BOUND_init(box);
    BOUND_add_point(box, -1, -1, -1);
    BOUND_add_point(box, 1, 1, 1);
}

const SHAPE_vtable TESTSHAPE_vtable = {
        &TESTSHAPE_local_intersect,
        &SHAPE_delete,
        &TESTSHAPE_local_normal_at,
        &SHAPE_default_shape_contains,
        &TESTSHAPE_bounds_of,
        NULL
};

void TESTSHAPE_init(TESTSHAPE_TestShape* shape) {
    assert(shape);
    shape->size = 5.0;
    RAY_init(&shape->saved_ray, 0, 0, 0, 0, 0, 0);
    shape->ray_set = false;
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
    TESTSHAPE_TestShape* ts = (TESTSHAPE_TestShape*) shape;
    ts->saved_ray = *local_ray;
    ts->ray_set = true;
}

void TESTSHAPE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Vector* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(hit);
    UNUSED(shape);
    TUPLES_init_vector(local_normal, local_point->x, local_point->y, local_point->z);
}
