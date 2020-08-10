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
} SPHERE_Sphere;

void SPHERE_init(SPHERE_Sphere* sphere, double x, double y, double z, double radius);
RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere*, const RAY_Ray*);

#endif //DATA_STRUCTURES_SPHERE_H
