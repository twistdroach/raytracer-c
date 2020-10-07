#include <assert.h>
#include <exceptions.h>
#include "triangle.h"

void TRIANGLE_bounds_of(const SHAPE_Shape* shape, BOUND_Box* box) {
    assert(shape);
    assert(box);
    TRIANGLE_Triangle* triangle = (TRIANGLE_Triangle*) shape;
    BOUND_init(box);
    BOUND_add_point_from_tuple(box, &triangle->p1);
    BOUND_add_point_from_tuple(box, &triangle->p2);
    BOUND_add_point_from_tuple(box, &triangle->p3);
}

const SHAPE_vtable TRIANGLE_vtable = {
        &TRIANGLE_local_intersect,
        &TRIANGLE_delete_shape,
        &TRIANGLE_local_normal_at,
        &SHAPE_default_shape_contains,
        &TRIANGLE_bounds_of
};

const SHAPE_vtable TRIANGLE_smooth_vtable = {
       &TRIANGLE_local_intersect,
       &TRIANGLE_delete_shape,
       &TRIANGLE_smooth_local_normal_at,
       &SHAPE_default_shape_contains,
       &TRIANGLE_bounds_of
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

static void init_base_triangle_info(TRIANGLE_Triangle* triangle,
                       double p1x, double p1y, double p1z,
                       double p2x, double p2y, double p2z,
                       double p3x, double p3y, double p3z) {
    assert(triangle);
    TUPLES_init_point(&triangle->p1, p1x, p1y, p1z);
    TUPLES_init_point(&triangle->p2, p2x, p2y, p2z);
    TUPLES_init_point(&triangle->p3, p3x, p3y, p3z);
    TUPLES_subtract(&triangle->e1, &triangle->p2, &triangle->p1);
    TUPLES_subtract(&triangle->e2, &triangle->p3, &triangle->p1);
    TUPLES_cross(&triangle->normal, &triangle->e2, &triangle->e1);
    TUPLES_normalize(&triangle->normal);
}

void TRIANGLE_init(TRIANGLE_Triangle* triangle,
                   double p1x, double p1y, double p1z,
                   double p2x, double p2y, double p2z,
                   double p3x, double p3y, double p3z) {
    assert(triangle);
    SHAPE_init(&triangle->shape, &TRIANGLE_vtable);
    init_base_triangle_info(triangle, p1x, p1y, p1z,
                                      p2x, p2y, p2z,
                                      p3x, p3y, p3z);
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

void TRIANGLE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(hit);
    UNUSED(local_point);
    TRIANGLE_Triangle* triangle = (TRIANGLE_Triangle*) shape;
    TUPLES_copy(local_normal, &triangle->normal);
}

void TRIANGLE_smooth_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(local_point);
    TRIANGLE_SmoothTriangle* st = (TRIANGLE_SmoothTriangle*) shape;
    TUPLES_Vector partial1, partial2, partial3;

    TUPLES_multiply(&partial2, &st->n2, hit->u);
    TUPLES_multiply(&partial3, &st->n3, hit->v);
    TUPLES_multiply(&partial1, &st->n1, (1 - hit->u - hit->v));

    TUPLES_add(local_normal, &partial1, &partial2);
    TUPLES_add(local_normal, local_normal, &partial3);
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
    RAY_add_intersection_tri(intersections, t, triangle, u, v);
}

void TRIANGLE_init_smooth_from_points(TRIANGLE_SmoothTriangle* triangle,
                                      const TUPLES_Point* p1, const TUPLES_Point* p2, const TUPLES_Point* p3,
                                      const TUPLES_Vector* v1, const TUPLES_Vector* v2, const TUPLES_Vector* v3) {
    assert(triangle);
    assert(p1);
    assert(p2);
    assert(p3);
    assert(v1);
    assert(v2);
    assert(v3);
    SHAPE_init(&triangle->shape, &TRIANGLE_smooth_vtable);
    init_base_triangle_info(&triangle->tri, p1->x, p1->y, p1->z, p2->x, p2->y, p2->z, p3->x, p3->y, p3->z);
    TUPLES_copy(&triangle->n1, v1);
    TUPLES_copy(&triangle->n2, v2);
    TUPLES_copy(&triangle->n3, v3);
}

TRIANGLE_SmoothTriangle* TRIANGLE_new_smooth_from_points(const TUPLES_Point* p1, const TUPLES_Point* p2, const TUPLES_Point* p3,
                                                         const TUPLES_Vector* v1, const TUPLES_Vector* v2, const TUPLES_Vector* v3) {
    assert(p1);
    assert(p2);
    assert(p3);
    assert(v1);
    assert(v2);
    assert(v3);
    TRIANGLE_SmoothTriangle* st = malloc(sizeof(TRIANGLE_SmoothTriangle));
    if (!st) {
        Throw(E_MALLOC_FAILED);
    }
    TRIANGLE_init_smooth_from_points(st, p1, p2, p3, v1, v2, v3);
    return st;
}
