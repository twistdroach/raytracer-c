#ifndef DATA_STRUCTURES_CYLINDER_H
#define DATA_STRUCTURES_CYLINDER_H

#include "ray.h"
#include "shape.h"

typedef SHAPE_Shape CYLINDER_Cylinder;
const SHAPE_vtable CYLINDER_vtable;

#define CYLINDER_new() (CYLINDER_Cylinder*)SHAPE_new(&CYLINDER_vtable)
#define CYLINDER_init(cylinder) SHAPE_init((SHAPE_Shape*)cylinder, &CYLINDER_vtable)
#define CYLINDER_destroy(cylinder) SHAPE_destroy((SHAPE_Shape*)cylinder)
#define CYLINDER_delete(cylinder) SHAPE_delete((SHAPE_Shape*)cylinder)

void CYLINDER_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cylinder, const TUPLES_Point* local_point);
void CYLINDER_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* cylinder, const RAY_Ray* local_ray);

#define CYLINDER_set_material(cylinder, material) SHAPE_set_material((SHAPE_Shape*)cylinder, material)
#define CYLINDER_get_material(cylinder) SHAPE_get_material((SHAPE_Shape*)cylinder)
#define CYLINDER_set_transform(cylinder, transform) SHAPE_set_transform((SHAPE_Shape*)cylinder, transform)

#endif //DATA_STRUCTURES_CYLINDER_H
