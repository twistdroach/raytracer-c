#ifndef DATA_STRUCTURES_TESTSHAPE_H
#define DATA_STRUCTURES_TESTSHAPE_H

#include "shape.h"
#include "ray.h"

typedef struct TESTSHAPE_TestShape {
    SHAPE_Shape parent;  /** parent class */
    double size;
    RAY_Ray saved_ray;
} TESTSHAPE_TestShape;

TESTSHAPE_TestShape* TESTSHAPE_new();
void TESTSHAPE_delete(TESTSHAPE_TestShape* shape);
void TESTSHAPE_local_intercept(RAY_Intersections* dest_intersections, TESTSHAPE_TestShape* shape, const RAY_Ray* local_ray);
void TESTSHAPE_local_normal_at(TUPLES_Vector* local_normal, TESTSHAPE_TestShape* shape, const TUPLES_Vector* local_point);




#endif //DATA_STRUCTURES_TESTSHAPE_H
