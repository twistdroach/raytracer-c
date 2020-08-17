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

RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere* sphere, const RAY_Ray* original_ray, RAY_Intersections* other_intersections) {
    assert(sphere);
    assert(original_ray);
    assert(MATRIX_is_invertible(sphere->transform));
    RAY_Ray ray;
    MATRIX_Matrix inv_transform;
    MATRIX_inverse(&inv_transform, SPHERE_get_transform(sphere));
    RAY_transform(&ray, original_ray, &inv_transform);

    if (!other_intersections) {
        other_intersections = RAY_new_intersections();
    }
    TUPLES_Vector sphere_to_ray;

    //compute discriminant
    TUPLES_subtract(&sphere_to_ray, &ray.origin, &sphere->origin);
    double a = TUPLES_dot(&ray.direction, &ray.direction);
    double b = 2 * TUPLES_dot(&ray.direction, &sphere_to_ray);
    double c = TUPLES_dot(&sphere_to_ray, &sphere_to_ray) - 1.0;
    double discriminant = pow(b, 2.0) - 4.0 * a *c;
    if (discriminant < 0) {
        //no hit
        return other_intersections;
    }

    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    RAY_add_intersection(other_intersections, t1, sphere);
    RAY_add_intersection(other_intersections, t2, sphere);
    RAY_destroy(&ray);
    return other_intersections;
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

void SPHERE_normal_at(TUPLES_Vector* world_normal, const SPHERE_Sphere* sphere, const TUPLES_Point* world_point) {
    assert(sphere);
    assert(world_point);
    assert(world_normal);

    MATRIX_Matrix inverse;
    TUPLES_Point object_point;
    TUPLES_Vector object_normal;

    MATRIX_inverse(&inverse, SPHERE_get_transform(sphere));
    MATRIX_multiply_tuple(&object_point, &inverse, world_point);
    TUPLES_subtract(&object_normal, &object_point, &sphere->origin);
    MATRIX_transpose(&inverse);
    MATRIX_multiply_tuple(world_normal, &inverse, &object_normal);
    world_normal->w = 0;
    TUPLES_normalize(world_normal);
}
