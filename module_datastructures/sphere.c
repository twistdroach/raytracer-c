//
// Created by zrowitsch on 8/7/20.
//

#include "sphere.h"
#include <math.h>
#include <exceptions.h>
#include <assert.h>

SPHERE_Sphere* SPHERE_new(double x, double y, double z, double radius) {
    SPHERE_Sphere* s = malloc(sizeof(SPHERE_Sphere));
    if (!s)
        Throw(E_MALLOC_FAILED);
    SPHERE_init(s, x, y, z, radius);
    return s;
}

void SPHERE_init(SPHERE_Sphere* sphere, double x, double y, double z, double radius) {
    assert(sphere);
    TUPLES_init_point(&sphere->origin, x, y , z);
    sphere->radius = radius;
    sphere->transform = MATRIX_new_identity(4);
}

void SPHERE_destroy(SPHERE_Sphere* sphere) {
    assert(sphere);
    TUPLES_destroy(&sphere->origin);
    MATRIX_delete(sphere->transform);
}

void SPHERE_delete(SPHERE_Sphere* sphere) {
    assert(sphere);
    SPHERE_destroy(sphere);
    free(sphere);
}

RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere* sphere, const RAY_Ray* original_ray) {
    assert(sphere);
    assert(original_ray);
    assert(MATRIX_is_invertible(sphere->transform));
    RAY_Ray ray;
    MATRIX_Matrix* inv_transform = MATRIX_inverse(sphere->transform);
    RAY_transform(&ray, original_ray, inv_transform);
    MATRIX_delete(inv_transform);

    RAY_Intersections* intersections = RAY_new_intersections();
    TUPLES_Vector sphere_to_ray;

    //compute discriminant
    TUPLES_subtract(&sphere_to_ray, &ray.origin, &sphere->origin);
    double a = TUPLES_dot(&ray.direction, &ray.direction);
    double b = 2 * TUPLES_dot(&ray.direction, &sphere_to_ray);
    double c = TUPLES_dot(&sphere_to_ray, &sphere_to_ray) - 1.0;
    double discriminant = pow(b, 2.0) - 4.0 * a *c;
    if (discriminant < 0) {
        //no hit
        return intersections;
    }

    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    RAY_add_intersection(intersections, t1, sphere);
    RAY_add_intersection(intersections, t2, sphere);
    RAY_destroy(&ray);
    return intersections;
}

void SPHERE_set_transform(SPHERE_Sphere* sphere, MATRIX_Matrix* matrix) {
    assert(sphere);
    assert(matrix);
    assert(matrix->width == 4);
    assert(matrix->height == 4);
    MATRIX_destroy(sphere->transform);
    MATRIX_copy(sphere->transform, matrix);
}

const MATRIX_Matrix* SPHERE_get_transform(const SPHERE_Sphere* sphere) {
    assert(sphere);
    return  sphere->transform;
}

