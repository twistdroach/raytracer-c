#ifndef DATA_STRUCTURES_SHAPE_H
#define DATA_STRUCTURES_SHAPE_H

#include <material.h>
#include "matrix.h"
#include "material.h"
#include "ray.h"
//typedef struct RAY_Ray RAY_Ray;
//typedef struct RAY_Intersections RAY_Intersections;
//typedef struct MATERIAL_Material MATERIAL_Material;

typedef struct SHAPE_Shape SHAPE_Shape;
typedef struct SHAPE_vtable {
    void (*local_intersect)(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* ray);
    void (*delete)(SHAPE_Shape*);
    void (*local_normal_at)(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point);
} SHAPE_vtable;

/**
 * Shapes that derive from this struct by including it as it's first member
 * (there are places where we downcast to shape from sub-shapes (spheres,planes,etc)
 *
 * Be sure to add a new type to SHAPE_Type...
 */
typedef struct SHAPE_Shape {
    const SHAPE_vtable* vtable;
    MATRIX_Matrix*      transform;
    MATRIX_Matrix*      inverse;
    MATRIX_Matrix*      inverse_transpose;
    MATERIAL_Material*  material;
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
/**
 * Calculates ray in object coordinates to that shape can handle intersection code locally.
 * @param local_ray The local ray to be calculated
 * @param ray ray in world coordinates
 * @param shape
 */
void SHAPE_calc_local_ray(RAY_Ray* local_ray, const RAY_Ray* ray, SHAPE_Shape* shape);

/**
 * Calculates local point in object coordinates given a point in world coords
 * @param local_point
 * @param point
 * @param point
 */
void SHAPE_calc_local_point(TUPLES_Point* local_point, SHAPE_Shape* shape, const TUPLES_Point* point);

void SHAPE_calc_world_normal(TUPLES_Vector* world_normal, SHAPE_Shape* shape, const TUPLES_Vector* local_normal);

void SHAPE_delete_any_type(SHAPE_Shape* shape);
void SHAPE_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* ray);
void SHAPE_normal_at(TUPLES_Vector* world_normal, SHAPE_Shape* shape, const TUPLES_Point* point);

#endif //DATA_STRUCTURES_SHAPE_H
