#ifndef DATA_STRUCTURES_SPHERE_H
#define DATA_STRUCTURES_SPHERE_H

#include "material.h"
#include "ray.h"
#include "shape.h"

/**
 * \extends SHAPE_Shape
 */
typedef SHAPE_Shape SPHERE_Sphere;

const SHAPE_vtable SPHERE_vtable;

#define SPHERE_new() (SPHERE_Sphere *)SHAPE_new(&SPHERE_vtable)
#define SPHERE_init(sphere) SHAPE_init((SHAPE_Shape *)sphere, &SPHERE_vtable)
#define SPHERE_destroy(sphere) SHAPE_destroy((SHAPE_Shape *)sphere)
#define SPHERE_delete(sphere) SHAPE_delete((SHAPE_Shape *)sphere)
#define SPHERE_delete_all(...) Fn_apply(SPHERE_Sphere, SPHERE_delete, __VA_ARGS__);

void SPHERE_local_intersect(RAY_Intersections *intersections, SHAPE_Shape *shape, const RAY_Ray *local_ray);
void SPHERE_local_normal_at(TUPLES_Vector *local_normal, SHAPE_Shape *shape, const TUPLES_Point *local_point, const RAY_Xs *hit);

#define SPHERE_set_material(sphere, material) SHAPE_set_material((SHAPE_Shape *)sphere, material)
#define SPHERE_get_material(sphere) SHAPE_get_material((SHAPE_Shape *)sphere)
#define SPHERE_set_transform(sphere, transform) SHAPE_set_transform((SHAPE_Shape *)sphere, transform)
#define SPHERE_get_transform(sphere) SHAPE_get_transform((SHAPE_Shape *)sphere)

SPHERE_Sphere *SPHERE_make_glassy(SPHERE_Sphere *sphere);
#endif // DATA_STRUCTURES_SPHERE_H
