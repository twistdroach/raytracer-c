#ifndef DATA_STRUCTURES_SPHERE_H
#define DATA_STRUCTURES_SPHERE_H

#include "tuples.h"
#include "ray.h"
#include "material.h"
#include "shape.h"

typedef struct SPHERE_Sphere {
    SHAPE_Shape parent;  /** parent class */
} SPHERE_Sphere;

SPHERE_Sphere* SPHERE_new();
void SPHERE_init(SPHERE_Sphere* sphere);
void SPHERE_destroy(SPHERE_Sphere* sphere);
void SPHERE_delete(SPHERE_Sphere* sphere);

void SPHERE_local_intersect(RAY_Intersections* intersections, SPHERE_Sphere* sphere, const RAY_Ray* local_ray);

void SPHERE_local_normal_at(TUPLES_Vector* world_normal, const SPHERE_Sphere* sphere, const TUPLES_Point* world_point);

//convenience - probably should just refactor
#define SPHERE_set_material(sphere, material) SHAPE_set_material((SHAPE_Shape*)sphere, material)
#define SPHERE_set_transform(sphere, transform) SHAPE_set_transform((SHAPE_Shape*)sphere, transform)
#define SPHERE_get_material(sphere) SHAPE_get_material((SHAPE_Shape*)sphere)

#endif //DATA_STRUCTURES_SPHERE_H
