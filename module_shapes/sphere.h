#ifndef DATA_STRUCTURES_SPHERE_H
#define DATA_STRUCTURES_SPHERE_H

#include "ray.h"
#include "material.h"
#include "shape.h"

typedef SHAPE_Shape SPHERE_Sphere;

#define SPHERE_new() (SPHERE_Sphere*)SHAPE_new(SHAPE_SPHERE)
#define SPHERE_init(sphere) SHAPE_init((SHAPE_Shape*)sphere, SHAPE_SPHERE)
#define SPHERE_destroy(sphere) SHAPE_destroy((SHAPE_Shape*)sphere)
#define SPHERE_delete(sphere) SHAPE_delete((SHAPE_Shape*)sphere)

void SPHERE_local_intersect(RAY_Intersections* intersections, SPHERE_Sphere* sphere, const RAY_Ray* local_ray);

void SPHERE_local_normal_at(TUPLES_Vector* local_normal, const SPHERE_Sphere* sphere, const TUPLES_Point* local_point);

#define SPHERE_set_material(sphere, material) SHAPE_set_material((SHAPE_Shape*)sphere, material)
#define SPHERE_get_material(sphere) SHAPE_get_material((SHAPE_Shape*)sphere)
#define SPHERE_set_transform(sphere, transform) SHAPE_set_transform((SHAPE_Shape*)sphere, transform)
#define SPHERE_get_transform(sphere) SHAPE_get_transform((SHAPE_Shape*)sphere)
#endif //DATA_STRUCTURES_SPHERE_H
