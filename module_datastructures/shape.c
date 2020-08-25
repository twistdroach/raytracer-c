#include <assert.h>
#include "shape.h"
#include "ray.h"

void SHAPE_init(SHAPE_Shape* shape) {
    assert(shape);
    shape->transform = MATRIX_new_identity(4);
    shape->material = MATERIAL_new();
}

void SHAPE_destroy(SHAPE_Shape* shape) {
    assert(shape);
    MATRIX_delete(shape->transform);
    MATERIAL_delete(shape->material);
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

MATERIAL_Material* SHAPE_get_material(SHAPE_Shape* shape) {
    assert(shape);
    return shape->material;
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
