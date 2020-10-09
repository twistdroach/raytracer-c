#ifndef DATA_STRUCTURES_SHAPE_H
#define DATA_STRUCTURES_SHAPE_H

#include <material.h>
#include "matrix.h"
#include "material.h"
#include "ray.h"
#include "bound.h"

typedef struct SHAPE_Shape SHAPE_Shape;
typedef struct SHAPE_vtable {
    void (*local_intersect)(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* ray);
    void (*delete)(SHAPE_Shape*);
    void (*local_normal_at)(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point, const RAY_Xs* hit);
    bool (*contains)(const SHAPE_Shape* a, const SHAPE_Shape* b);
    void (*bounds_of)(const SHAPE_Shape* shape, BOUND_Box* box); /** bounds_of sets the bounds of the shape in the box provided */
    void (*divide)(SHAPE_Shape* shape, unsigned int minimum_size);
} SHAPE_vtable;

/**
 * Shapes that derive from this struct by including it as it's first member
 * (there are places where we downcast to shape from sub-shapes (spheres,planes,etc)
 */
typedef struct SHAPE_Shape {
    const SHAPE_vtable* vtable;
    MATRIX_Matrix*      transform;
    MATRIX_Matrix*      inverse;
    MATRIX_Matrix*      inverse_transpose;
    MATERIAL_Material*  material;
    SHAPE_Shape*        parent;
} SHAPE_Shape;

/**
 * Should be used by subclasses only...
 * @return
 */
SHAPE_Shape* SHAPE_new(const SHAPE_vtable* vtable);
void SHAPE_init(SHAPE_Shape* shape, const SHAPE_vtable* vtable);
void SHAPE_destroy(SHAPE_Shape* shape);
/**
 * Should be used by subclasses only...
 * @return
 */
void SHAPE_delete(SHAPE_Shape* shape);
void SHAPE_set_transform(SHAPE_Shape* shape, const MATRIX_Matrix* transformation);
void SHAPE_set_material(SHAPE_Shape* shape, const MATERIAL_Material* material);
MATERIAL_Material* SHAPE_get_material(const SHAPE_Shape* shape);
MATRIX_Matrix* SHAPE_get_transform(const SHAPE_Shape* shape);
MATRIX_Matrix* SHAPE_get_inverse_transform(const SHAPE_Shape* shape);
MATRIX_Matrix* SHAPE_get_transpose_inverse_transform(const SHAPE_Shape* shape);

#define SHAPE_bounds_of(shape, box) ((SHAPE_Shape*)(shape))->vtable->bounds_of(((SHAPE_Shape*)(shape)), (box))
#define SHAPE_set_parent(child, parent_shape) ((((SHAPE_Shape*)(child))->parent) = (SHAPE_Shape*)(parent_shape))
#define SHAPE_get_parent(shape) (((SHAPE_Shape*)(shape))->parent)

/**
 * Calculates ray in object coordinates to that shape can handle intersection code locally.
 * @param local_ray The local ray to be calculated
 * @param ray ray in world coordinates
 * @param shape
 */
void SHAPE_calc_local_ray(RAY_Ray* local_ray, const RAY_Ray* ray, SHAPE_Shape* shape);

void SHAPE_delete_any_type(SHAPE_Shape* shape);

void SHAPE_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* ray);
void SHAPE_normal_at(TUPLES_Vector* world_normal, SHAPE_Shape* shape, const TUPLES_Point* point, const RAY_Xs* hit);
void SHAPE_world_to_object(TUPLES_Point* result, const SHAPE_Shape* shape, const TUPLES_Point* world_point);
void SHAPE_normal_to_world(TUPLES_Vector* result, const SHAPE_Shape* shape, const TUPLES_Vector* normal);

bool SHAPE_default_shape_contains(const SHAPE_Shape* a, const SHAPE_Shape* b);
void SHAPE_parent_space_bounds_of(BOUND_Box* dest_box, const SHAPE_Shape* shape);

void SHAPE_divide(SHAPE_Shape* shape, unsigned int minimum_size);

#endif //DATA_STRUCTURES_SHAPE_H
