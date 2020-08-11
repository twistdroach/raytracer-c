//
// Created by zrowitsch on 8/7/20.
//

#ifndef DATA_STRUCTURES_SPHERE_H
#define DATA_STRUCTURES_SPHERE_H

#include "tuples.h"
#include "ray.h"

typedef struct {
    TUPLES_Point origin;
    double radius;
    MATRIX_Matrix* transform;
} SPHERE_Sphere;

SPHERE_Sphere* SPHERE_new(double x, double y, double z, double radius);
void SPHERE_init(SPHERE_Sphere* sphere, double x, double y, double z, double radius);
void SPHERE_destroy(SPHERE_Sphere* sphere);
void SPHERE_delete(SPHERE_Sphere* sphere);
/**
 * Constructs a set of intersections, must call RAY_delete_intersections() on returned ptr.
 * @return
 */
RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere*, const RAY_Ray*);
void SPHERE_set_transform(SPHERE_Sphere* sphere, MATRIX_Matrix* matrix);
const MATRIX_Matrix* SPHERE_get_transform(const SPHERE_Sphere* sphere);

#endif //DATA_STRUCTURES_SPHERE_H
