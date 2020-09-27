#include <assert.h>
#include <exceptions.h>
#include "triangle.h"

const SHAPE_vtable TRIANGLE_vtable = {
        &TRIANGLE_local_intersect,
        &TRIANGLE_delete_shape,
        &TRIANGLE_local_normal_at
};

TRIANGLE_Triangle* TRIANGLE_new_from_points(const TUPLES_Point* p1, const TUPLES_Point* p2, const TUPLES_Point* p3) {
    return TRIANGLE_new(p1->x, p1->y, p1->z,
                        p2->x, p2->y, p2->z,
                        p3->x, p3->y, p3->z);
}

TRIANGLE_Triangle* TRIANGLE_new(double p1x, double p1y, double p1z,
                                double p2x, double p2y, double p2z,
                                double p3x, double p3y, double p3z) {
    TRIANGLE_Triangle* t = malloc(sizeof(TRIANGLE_Triangle));
    if (!t) {
        Throw(E_MALLOC_FAILED);
    }
    TRIANGLE_init(t, p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z);
    return t;
}

void TRIANGLE_init_from_points(TRIANGLE_Triangle* triangle,
                               const TUPLES_Point* p1,
                               const TUPLES_Point* p2,
                               const TUPLES_Point* p3) {
    assert(triangle);
    TRIANGLE_init(triangle,
                  p1->x, p1->y, p1->z,
                  p2->x, p2->y, p2->z,
                  p3->x, p3->y, p3->z);
}

void TRIANGLE_init(TRIANGLE_Triangle* triangle,
                   double p1x, double p1y, double p1z,
                   double p2x, double p2y, double p2z,
                   double p3x, double p3y, double p3z) {
    assert(triangle);
    SHAPE_init(&triangle->shape, &TRIANGLE_vtable);
    TUPLES_init_point(&triangle->p1, p1x, p1y, p1z);
    TUPLES_init_point(&triangle->p2, p2x, p2y, p2z);
    TUPLES_init_point(&triangle->p3, p3x, p3y, p3z);
    TUPLES_subtract(&triangle->e1, &triangle->p2, &triangle->p1);
    TUPLES_subtract(&triangle->e2, &triangle->p3, &triangle->p1);
    TUPLES_cross(&triangle->normal, &triangle->e2, &triangle->e1);
    TUPLES_normalize(&triangle->normal);
}

void TRIANGLE_destroy(TRIANGLE_Triangle* triangle) {
    assert(triangle);
    SHAPE_destroy(&triangle->shape);
}

void TRIANGLE_delete(TRIANGLE_Triangle* triangle) {
    assert(triangle);
    TRIANGLE_destroy(triangle);
    free(triangle);
}

void TRIANGLE_delete_shape(SHAPE_Shape* shape) {
    assert(shape);
    TRIANGLE_delete((TRIANGLE_Triangle*)shape);
}

void TRIANGLE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    TRIANGLE_Triangle* triangle = (TRIANGLE_Triangle*) shape;
    TUPLES_copy(local_normal, &triangle->normal);
}

void TRIANGLE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);
    TRIANGLE_Triangle* triangle = (TRIANGLE_Triangle*) shape;

    TUPLES_Vector dir_cross_e2;
    TUPLES_cross(&dir_cross_e2, &local_ray->direction, &triangle->e2);

    double det = TUPLES_dot(&triangle->e1, &dir_cross_e2);

    // if det is close to zero, ray is parallel
    if (double_equal(0, det)) {
        return;
    }

    double f = 1.0 / det;

    TUPLES_Vector p1_to_origin;
    TUPLES_subtract(&p1_to_origin, &local_ray->origin, &triangle->p1);

    double u = f * TUPLES_dot(&p1_to_origin, &dir_cross_e2);

    if (u < 0 || u > 1) {
        return;
    }

    TUPLES_Vector origin_cross_e1;
    TUPLES_cross(&origin_cross_e1, &p1_to_origin, &triangle->e1);

    double v = f * TUPLES_dot(&local_ray->direction, &origin_cross_e1);

    if (v < 0 || (u + v) > 1) {
        return;
    }

    double t = f * TUPLES_dot(&triangle->e2, &origin_cross_e1);
    RAY_add_intersection(intersections, t, triangle);
}
