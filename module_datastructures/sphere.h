#ifndef DATA_STRUCTURES_SPHERE_H
#define DATA_STRUCTURES_SPHERE_H

#include "tuples.h"
#include "ray.h"
#include "material.h"

typedef struct SPHERE_Sphere {
    TUPLES_Point origin;
    double radius;
    MATRIX_Matrix* transform;
    MATERIAL_Material* material;
} SPHERE_Sphere;

SPHERE_Sphere* SPHERE_new();
void SPHERE_init(SPHERE_Sphere* sphere);
void SPHERE_destroy(SPHERE_Sphere* sphere);
void SPHERE_delete(SPHERE_Sphere* sphere);

/**
 * Constructs a set of intersections, must call RAY_delete_intersections() on returned ptr.
 * If other_intersections == NULL, a new RAY_Intersections is allocated
 * Otherwise, additional intersections are added to the other_intersections and no new RAY_Intersections
 * is allocated.
 * @return
 */
RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere*, const RAY_Ray*, RAY_Intersections* other_intersections);

/**
 * Sets dest to a normal vector at surface of the sphere.
 * @param dest
 * @param sphere
 * @param point
 * @return
 */
TUPLES_Vector SPHERE_normal_at(const SPHERE_Sphere* sphere, TUPLES_Point point);
void SPHERE_set_transform(SPHERE_Sphere* sphere, const MATRIX_Matrix* matrix);
const MATRIX_Matrix* SPHERE_get_transform(const SPHERE_Sphere* sphere);

void SPHERE_set_material(SPHERE_Sphere* sphere, const MATERIAL_Material* material);
const MATERIAL_Material* SPHERE_get_material(const SPHERE_Sphere* sphere);

#endif //DATA_STRUCTURES_SPHERE_H
