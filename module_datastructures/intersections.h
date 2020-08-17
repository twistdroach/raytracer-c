#ifndef DATA_STRUCTURES_INTERSECTIONS_H
#define DATA_STRUCTURES_INTERSECTIONS_H

#include "sphere.h"

typedef struct INTERSECTION_Intersection {
    double t;
    const SPHERE_Sphere* object;
    TUPLES_Point point;
    TUPLES_Vector eyev;
    TUPLES_Vector normalv;
    bool inside;
} INTERSECTION_Intersection;

INTERSECTION_Intersection* INTERSECTION_prepare_computations(const RAY_Xs* hit, const RAY_Ray* ray);
void INTERSECTION_delete( INTERSECTION_Intersection* intersection);

#endif //DATA_STRUCTURES_INTERSECTIONS_H
