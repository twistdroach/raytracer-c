#ifndef DATA_STRUCTURES_CUBE_H
#define DATA_STRUCTURES_CUBE_H

#include "ray.h"
#include "shape.h"

/**
 * \extends SHAPE_Shape
 */
typedef SHAPE_Shape CUBE_Cube;
const SHAPE_vtable CUBE_vtable;

#define CUBE_new() (CUBE_Cube*)SHAPE_new(&CUBE_vtable)
#define CUBE_init(cube) SHAPE_init((SHAPE_Shape*)cube, &CUBE_vtable)
#define CUBE_destroy(cube) SHAPE_destroy((SHAPE_Shape*)cube)
#define CUBE_delete(cube) SHAPE_delete((SHAPE_Shape*)cube)

void CUBE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cube, const TUPLES_Point* local_point, const RAY_Xs* hit);
void CUBE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* cube, const RAY_Ray* local_ray);
void CUBE_check_axis(double* tmin, double* tmax, double origin, double direction, double min, double max);

#define CUBE_set_material(cube, material) SHAPE_set_material((SHAPE_Shape*)cube, material)
#define CUBE_get_material(cube) SHAPE_get_material((SHAPE_Shape*)cube)
#define CUBE_set_transform(cube, transform) SHAPE_set_transform((SHAPE_Shape*)cube, transform)

#endif //DATA_STRUCTURES_CUBE_H
