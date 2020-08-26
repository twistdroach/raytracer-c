#ifndef DATA_STRUCTURES_PLANE_H
#define DATA_STRUCTURES_PLANE_H

#include <ray.h>
#include "shape.h"

typedef SHAPE_Shape PLANE_Plane;

#define PLANE_new() (PLANE_Plane*)SHAPE_new(SHAPE_PLANE)
#define PLANE_init(plane) SHAPE_init((SHAPE_Shape*)plane, SHAPE_PLANE)
#define PLANE_destroy(plane) SHAPE_destroy((SHAPE_Shape*)plane)
#define PLANE_delete(plane) SHAPE_delete((SHAPE_Shape*)plane)

void PLANE_local_normal_at(TUPLES_Vector* local_normal, const PLANE_Plane* plane, const TUPLES_Point* local_point);
void PLANE_local_intersect(RAY_Intersections* intersections, PLANE_Plane* plane, const RAY_Ray* local_ray);

#define PLANE_set_material(plane, material) SHAPE_set_material((SHAPE_Shape*)plane, material)
#define PLANE_get_material(plane) SHAPE_get_material((SHAPE_Shape*)plane)
#define PLANE_set_transform(plane, transform) SHAPE_set_transform((SHAPE_Shape*)plane, transform)
#define PLANE_get_transform(plane) SHAPE_get_transform((SHAPE_Shape*)plane)

#endif //DATA_STRUCTURES_PLANE_H
