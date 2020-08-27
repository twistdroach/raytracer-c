#include <assert.h>
#include <exceptions.h>
#include "shape.h"
#include "ray.h"

SHAPE_Shape* SHAPE_new(const SHAPE_vtable* vtable) {
    assert(vtable);
    SHAPE_Shape* s = malloc(sizeof(SHAPE_Shape));
    if (!s)
        Throw(E_MALLOC_FAILED);
    SHAPE_init(s, vtable);
    return s;
}

void SHAPE_init(SHAPE_Shape* shape, const SHAPE_vtable* vtable) {
    assert(vtable);
    assert(shape);
    shape->transform = MATRIX_new_identity(4);
    shape->material = MATERIAL_new();
    shape->vtable = vtable;
}

void SHAPE_destroy(SHAPE_Shape* shape) {
    assert(shape);
    MATRIX_delete(shape->transform);
    MATERIAL_delete(shape->material);
}

void SHAPE_delete(SHAPE_Shape* shape) {
    assert(shape);
    SHAPE_destroy(shape);
    free(shape);
}

void SHAPE_set_transform(SHAPE_Shape* shape, MATRIX_Matrix* transformation) {
    assert(shape);
    assert(shape->transform);
    assert(transformation);
    assert(transformation->width == 4);
    assert(transformation->height == 4);
    MATRIX_destroy(shape->transform);
    MATRIX_copy(shape->transform, transformation);
}

void SHAPE_set_material(SHAPE_Shape* shape, MATERIAL_Material* material) {
    assert(shape);
    assert(shape->material);
    assert(material);
    MATERIAL_copy(shape->material, material);
}

MATERIAL_Material* SHAPE_get_material(const SHAPE_Shape* shape) {
    assert(shape);
    return shape->material;
}

MATRIX_Matrix* SHAPE_get_transform(const SHAPE_Shape* shape) {
    assert(shape);
    return shape->transform;
}

void SHAPE_calc_local_ray(RAY_Ray* local_ray, const RAY_Ray* ray, SHAPE_Shape* shape) {
    assert(local_ray);
    assert(ray);
    assert(shape);
    MATRIX_Matrix inverse;
    MATRIX_inverse(&inverse, shape->transform);
    RAY_transform(local_ray, ray, &inverse);
}

void SHAPE_calc_local_point(TUPLES_Point* local_point, SHAPE_Shape* shape, const TUPLES_Point* point) {
    assert(local_point);
    assert(shape);
    assert(point);
    MATRIX_Matrix inverse;
    MATRIX_inverse(&inverse, shape->transform);
    MATRIX_multiply_tuple(local_point, &inverse, point);
}

void SHAPE_calc_world_normal(TUPLES_Vector* world_normal, SHAPE_Shape* shape, const TUPLES_Vector* local_normal) {
    assert(world_normal);
    assert(shape);
    assert(local_normal);
    MATRIX_Matrix inverse;
    MATRIX_inverse(&inverse, shape->transform);
    MATRIX_transpose(&inverse);
    MATRIX_multiply_tuple(world_normal, &inverse, local_normal);
    world_normal->w = 0;
    TUPLES_normalize(world_normal);
}

void SHAPE_normal_at(TUPLES_Vector* world_normal, SHAPE_Shape* shape, const TUPLES_Point* point) {
    assert(world_normal);
    assert(shape);
    assert(point);
    TUPLES_Point local_point;
    SHAPE_calc_local_point(&local_point, shape, point);
    TUPLES_Vector local_normal;
    shape->vtable->local_normal_at(&local_normal, shape, &local_point);
    SHAPE_calc_world_normal(world_normal, shape, &local_normal);
}

void SHAPE_delete_any_type(SHAPE_Shape* shape) {
    assert(shape);
    shape->vtable->delete(shape);
}

void SHAPE_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* ray) {
    assert(shape);
    assert(ray);

    // intersections can be appended for multiple shapes, so on the first call, we might get NULL here.
    if (!intersections) {
        intersections = RAY_new_intersections();
    }

    RAY_Ray local_ray;
    SHAPE_calc_local_ray(&local_ray, ray, shape);
    shape->vtable->local_intersect(intersections, shape, &local_ray);
}
