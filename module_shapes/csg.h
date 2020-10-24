#ifndef SIMPLE_RAYTRACER_CSG_H
#define SIMPLE_RAYTRACER_CSG_H

#include "shape.h"

typedef enum CSG_Operation { CSG_Union, CSG_Intersection, CSG_Difference } CSG_Operation;

/**
 * \extends SHAPE_Shape
 */
typedef struct CSG_Csg {
  SHAPE_Shape shape;
  SHAPE_Shape *left;
  SHAPE_Shape *right;
  CSG_Operation operation;
  BOUND_Box *bounds;
} CSG_Csg;

const SHAPE_vtable CSG_vtable;

CSG_Csg *CSG_new(CSG_Operation op, void *left, void *right);
void CSG_init(CSG_Csg *csg, CSG_Operation op, void *left, void *right);

/**
 * Destroying the CSG will delete any shape objects it contains!
 * @param csg
 */
void CSG_destroy(CSG_Csg *csg);

/**
 * Deleting the CSG will delete any shape objects it contains!
 * @param csg
 */
void CSG_delete(CSG_Csg *csg);
void CSG_delete_shape(SHAPE_Shape *shape);

void CSG_local_normal_at(TUPLES_Vector *local_normal, SHAPE_Shape *csg, const TUPLES_Point *local_point, const RAY_Xs *hit);
void CSG_local_intersect(RAY_Intersections *intersections, SHAPE_Shape *csg, const RAY_Ray *local_ray);
bool CSG_intersection_allowed(CSG_Operation op, bool lhit, bool inl, bool inr);
RAY_Intersections *CSG_filter_intersections(const CSG_Csg *csg, const RAY_Intersections *intersections);

#endif // SIMPLE_RAYTRACER_CSG_H
