#ifndef DATA_STRUCTURES_TRIANGLE_H
#define DATA_STRUCTURES_TRIANGLE_H

#include "shape.h"

typedef struct TRIANGLE_Triangle {
    SHAPE_Shape shape;              /* Superclass */
    TUPLES_Point p1, p2, p3;        /* Corners of the triangle */
    TUPLES_Vector e1, e2, normal;   /* Two edges and the normal vector */
} TRIANGLE_Triangle;

const SHAPE_vtable TRIANGLE_vtable;

TRIANGLE_Triangle* TRIANGLE_new_from_points(const TUPLES_Point* p1, const TUPLES_Point* p2, const TUPLES_Point* p3);
TRIANGLE_Triangle* TRIANGLE_new(double p1x, double p1y, double p1z,
                                double p2x, double p2y, double p2z,
                                double p3x, double p3y, double p3z);
void TRIANGLE_init_from_points(TRIANGLE_Triangle* triangle, const TUPLES_Point* p1, const TUPLES_Point* p2, const TUPLES_Point* p3);
void TRIANGLE_init(TRIANGLE_Triangle* triangle,
                   double p1x, double p1y, double p1z,
                   double p2x, double p2y, double p2z,
                   double p3x, double p3y, double p3z);
void TRIANGLE_destroy(TRIANGLE_Triangle* triangle);
void TRIANGLE_delete(TRIANGLE_Triangle* triangle);
void TRIANGLE_delete_shape(SHAPE_Shape* shape);

void TRIANGLE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* triangle, const TUPLES_Point* local_point);
void TRIANGLE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* triangle, const RAY_Ray* local_ray);

#define TRIANGLE_set_material(triangle, material) SHAPE_set_material((SHAPE_Shape*)triangle, material)
#define TRIANGLE_get_material(triangle) SHAPE_get_material((SHAPE_Shape*)triangle)
#define TRIANGLE_set_transform(triangle, transform) SHAPE_set_transform((SHAPE_Shape*)triangle, transform)

#endif //DATA_STRUCTURES_TRIANGLE_H
