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
 * @return
 */
RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere*, const RAY_Ray*);

/**
 * Sets dest to a normal vector at surface of the sphere.
 * @param dest
 * @param sphere
 * @param point
 * @return
 */
void SPHERE_normal_at(TUPLES_Vector* dest, const SPHERE_Sphere* sphere, const TUPLES_Point* point);
void SPHERE_set_transform(SPHERE_Sphere* sphere, const MATRIX_Matrix* matrix);
const MATRIX_Matrix* SPHERE_get_transform(const SPHERE_Sphere* sphere);

void SPHERE_set_material(SPHERE_Sphere* sphere, const MATERIAL_Material* material);
const MATERIAL_Material* SPHERE_get_material(const SPHERE_Sphere* sphere);

#endif //DATA_STRUCTURES_SPHERE_H
