#include "shape.h"
#include "material.h"
#include "ray.h"
#include <assert.h>
#include <exceptions.h>

SHAPE_Shape *SHAPE_new(const SHAPE_vtable *vtable) {
  assert(vtable);
  SHAPE_Shape *s = malloc(sizeof(SHAPE_Shape));
  if (!s)
    Throw(E_MALLOC_FAILED);
  SHAPE_init(s, vtable);
  return s;
}

void SHAPE_init(SHAPE_Shape *shape, const SHAPE_vtable *vtable) {
  assert(vtable);
  assert(shape);
  shape->transform = MATRIX_new_identity(4);
  shape->inverse = MATRIX_new_identity(4);
  shape->inverse_transpose = MATRIX_new_identity(4);
  MATRIX_inverse(shape->inverse, shape->transform);
  MATRIX_copy(shape->inverse_transpose, shape->inverse);
  MATRIX_transpose(shape->inverse_transpose);
  shape->material = MATERIAL_new();
  shape->vtable = vtable;
  shape->parent = NULL;
}

void SHAPE_destroy(SHAPE_Shape *shape) {
  assert(shape);
  MATRIX_delete(shape->transform);
  MATRIX_delete(shape->inverse);
  MATRIX_delete(shape->inverse_transpose);
  MATERIAL_delete(shape->material);
}

void SHAPE_delete(SHAPE_Shape *shape) {
  assert(shape);
  SHAPE_destroy(shape);
  free(shape);
}

void SHAPE_set_transform(SHAPE_Shape *shape, const MATRIX_Matrix *transformation) {
  assert(shape);
  assert(shape->transform);
  assert(transformation);
  assert(transformation->width == 4);
  assert(transformation->height == 4);
  MATRIX_copy(shape->transform, transformation);
  MATRIX_inverse(shape->inverse, transformation);
  MATRIX_copy(shape->inverse_transpose, shape->inverse);
  MATRIX_transpose(shape->inverse_transpose);
}

void SHAPE_set_material(SHAPE_Shape *shape, const MATERIAL_Material *material) {
  assert(shape);
  assert(shape->material);
  assert(material);
  MATERIAL_delete(shape->material);
  shape->material = MATERIAL_new_copy(material);
}

MATERIAL_Material *SHAPE_get_material(const SHAPE_Shape *shape) {
  assert(shape);
  return shape->material;
}

MATRIX_Matrix *SHAPE_get_transform(const SHAPE_Shape *shape) {
  assert(shape);
  return shape->transform;
}

MATRIX_Matrix *SHAPE_get_inverse_transform(const SHAPE_Shape *shape) {
  assert(shape);
  return shape->inverse;
}

MATRIX_Matrix *SHAPE_get_transpose_inverse_transform(const SHAPE_Shape *shape) {
  assert(shape);
  return shape->inverse_transpose;
}

void SHAPE_calc_local_ray(RAY_Ray *local_ray, const RAY_Ray *ray, SHAPE_Shape *shape) {
  assert(local_ray);
  assert(ray);
  assert(shape);
  RAY_transform(local_ray, ray, shape->inverse);
}

void SHAPE_normal_at(TUPLES_Vector *world_normal, SHAPE_Shape *shape, const TUPLES_Point *point, const RAY_Xs *hit) {
  assert(world_normal);
  assert(shape);
  assert(point);
  TUPLES_Point local_point;
  SHAPE_world_to_object(&local_point, shape, point);
  TUPLES_Vector local_normal;
  shape->vtable->local_normal_at(&local_normal, shape, &local_point, hit);
  SHAPE_normal_to_world(world_normal, shape, &local_normal);
}

void SHAPE_delete_any_type(SHAPE_Shape *shape) {
  assert(shape);
  shape->vtable->delete (shape);
}

void SHAPE_intersect(RAY_Intersections *intersections, SHAPE_Shape *shape, const RAY_Ray *ray) {
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

void SHAPE_world_to_object(TUPLES_Point *result, const SHAPE_Shape *shape, const TUPLES_Point *world_point) {
  assert(result);
  assert(shape);
  assert(world_point);

  const SHAPE_Shape *parent = SHAPE_get_parent(shape);
  TUPLES_Point tmp_point;
  if (parent) {
    SHAPE_world_to_object(&tmp_point, parent, world_point);
  } else {
    TUPLES_copy(&tmp_point, world_point);
  }

  MATRIX_multiply_tuple(result, shape->inverse, &tmp_point);
}

void SHAPE_normal_to_world(TUPLES_Vector *result, const SHAPE_Shape *shape, const TUPLES_Vector *normal) {
  assert(result);
  assert(shape);
  assert(normal);
  MATRIX_multiply_tuple(result, shape->inverse_transpose, normal);
  result->w = 0;
  TUPLES_normalize(result);

  const SHAPE_Shape *parent = SHAPE_get_parent(shape);
  if (parent) {
    SHAPE_normal_to_world(result, parent, result);
  }
}

bool SHAPE_default_shape_contains(const SHAPE_Shape *a, const SHAPE_Shape *b) {
  assert(a);
  assert(b);
  return a == b;
}

void SHAPE_parent_space_bounds_of(BOUND_Box *dest_box, const SHAPE_Shape *shape) {
  assert(dest_box);
  assert(shape);
  const MATRIX_Matrix *transform = SHAPE_get_transform(shape);
  BOUND_Box shape_bounds;
  shape->vtable->bounds_of(shape, &shape_bounds);
  BOUND_transform(dest_box, &shape_bounds, transform);
}

void SHAPE_divide(SHAPE_Shape *shape, unsigned int minimum_size) {
  assert(shape);
  if (shape->vtable->divide) {
    shape->vtable->divide(shape, minimum_size);
  }
}
