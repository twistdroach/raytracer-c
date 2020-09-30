#ifndef DATA_STRUCTURES_CONE_H
#define DATA_STRUCTURES_CONE_H

#include "cylinder.h"

typedef CYLINDER_Cylinder CONE_Cone;

const SHAPE_vtable CONE_vtable;

CONE_Cone* CONE_new();
void CONE_init(CONE_Cone* cone);
#define CONE_destroy(cone) CYLINDER_destroy(cone)
#define CONE_delete(cone) CYLINDER_delete(cone)

void CONE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cone, const TUPLES_Point* local_point, const RAY_Xs* hit);
void CONE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* cone, const RAY_Ray* local_ray);

#define CONE_set_material(cone, material) SHAPE_set_material((SHAPE_Shape*)cone, material)
#define CONE_get_material(cone) SHAPE_get_material((SHAPE_Shape*)cone)
#define CONE_set_transform(cone, transform) SHAPE_set_transform((SHAPE_Shape*)cone, transform)

#endif //DATA_STRUCTURES_CONE_H
