#ifndef DATA_STRUCTURES_SHAPE_H
#define DATA_STRUCTURES_SHAPE_H

#include "matrix.h"
#include "material.h"
//#include "ray.h"
typedef struct RAY_Ray RAY_Ray;

typedef struct SHAPE_Shape {
    MATRIX_Matrix* transform;
    MATERIAL_Material* material;
} SHAPE_Shape;

/**
 * Should be used by subclasses only...
 * @return
 */
SHAPE_Shape* SHAPE_new();
void SHAPE_init(SHAPE_Shape* shape);
void SHAPE_destroy(SHAPE_Shape* shape);
/**
 * Should be used by subclasses only...
 * @return
 */
void SHAPE_delete(SHAPE_Shape* shape);
void SHAPE_set_transform(SHAPE_Shape* shape, MATRIX_Matrix* transformation);
void SHAPE_set_material(SHAPE_Shape* shape, MATERIAL_Material* material);
MATERIAL_Material* SHAPE_get_material(const SHAPE_Shape* shape);
MATRIX_Matrix* SHAPE_get_transform(const SHAPE_Shape* shape);
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
#endif //DATA_STRUCTURES_SHAPE_H
