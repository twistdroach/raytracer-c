#ifndef DATA_STRUCTURES_TESTSHAPE_H
#define DATA_STRUCTURES_TESTSHAPE_H

#include "ray.h"
#include "shape.h"

const SHAPE_vtable TESTSHAPE_vtable;

/**
 * \extends SHAPE_Shape
 */
typedef struct TESTSHAPE_TestShape {
  SHAPE_Shape parent; /** parent class */
  double size;
  RAY_Ray saved_ray;
  bool ray_set;
} TESTSHAPE_TestShape;

TESTSHAPE_TestShape *TESTSHAPE_new();
void TESTSHAPE_delete(TESTSHAPE_TestShape *shape);
void TESTSHAPE_local_intersect(RAY_Intersections *dest_intersections, SHAPE_Shape *shape, const RAY_Ray *local_ray);
void TESTSHAPE_local_normal_at(TUPLES_Vector *local_normal, SHAPE_Shape *shape, const TUPLES_Vector *local_point, const RAY_Xs *hit);

#endif // DATA_STRUCTURES_TESTSHAPE_H
