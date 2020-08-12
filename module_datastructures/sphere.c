//
// Created by zrowitsch on 8/7/20.
//

#include "sphere.h"
#include <math.h>
#include <exceptions.h>
#include <assert.h>

SPHERE_Sphere* SPHERE_new() {
    SPHERE_Sphere* s = malloc(sizeof(SPHERE_Sphere));
    if (!s)
        Throw(E_MALLOC_FAILED);
    SPHERE_init(s);
    return s;
}

void SPHERE_init(SPHERE_Sphere* sphere) {
    assert(sphere);
    TUPLES_init_point(&sphere->origin, 0, 0 , 0);
    sphere->radius = 1.0;
    sphere->transform = MATRIX_new_identity(4);
    sphere->material = MATERIAL_new();
}

void SPHERE_destroy(SPHERE_Sphere* sphere) {
    assert(sphere);
    TUPLES_destroy(&sphere->origin);
    MATRIX_delete(sphere->transform);
    MATERIAL_delete(sphere->material);
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
    MATRIX_Matrix* inv_transform = MATRIX_inverse(SPHERE_get_transform(sphere));
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

void SPHERE_set_material(SPHERE_Sphere* sphere, const MATERIAL_Material* material) {
    assert(sphere);
    assert(material);
    MATERIAL_copy(sphere->material, material);
}

const MATERIAL_Material* SPHERE_get_material(const SPHERE_Sphere* sphere) {
    assert(sphere);
    return sphere->material;
}

void SPHERE_set_transform(SPHERE_Sphere* sphere, const MATRIX_Matrix* matrix) {
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

TUPLES_Vector* SPHERE_normal_at(const SPHERE_Sphere* sphere, const TUPLES_Point* world_point) {
    assert(sphere);
    assert(world_point);

    MATRIX_Matrix* inverse = MATRIX_inverse(SPHERE_get_transform(sphere));
    TUPLES_Point* object_point = MATRIX_multiply_tuple(inverse, world_point);
    TUPLES_Vector object_normal;
    TUPLES_subtract(&object_normal, object_point, &sphere->origin);
    MATRIX_transpose(inverse);
    TUPLES_Vector* world_normal = MATRIX_multiply_tuple(inverse, &object_normal);
    world_normal->w = 0;
    TUPLES_normalize(world_normal);

    TUPLES_destroy(&object_normal);
    TUPLES_delete(object_point);
    MATRIX_delete_all(inverse);

    return world_normal;

}