#ifndef DATA_STRUCTURES_RAY_H
#define DATA_STRUCTURES_RAY_H

#include "tuples.h"
#include "matrix.h"

typedef struct {
    TUPLES_Point  origin;
    TUPLES_Vector direction;
} RAY_Ray;

RAY_Ray* RAY_new(double origin_x, double origin_y, double origin_z, double direction_x, double direction_y, double direction_z);
RAY_Ray* RAY_new_from_tuples(const TUPLES_Point* origin, const TUPLES_Vector* direction);
void RAY_init(RAY_Ray* ray, double origin_x, double origin_y, double origin_z, double direction_x, double direction_y, double direction_z);
void RAY_init_from_tuples(RAY_Ray* ray, const TUPLES_Point* origin, const TUPLES_Vector* direction);
void RAY_destroy(RAY_Ray* ray);
#define RAY_destroy_all(...) Fn_apply(RAY_Ray, RAY_destroy, __VA_ARGS__);
void RAY_delete(RAY_Ray* ray);
#define RAY_delete_all(...) Fn_apply(RAY_Ray, RAY_delete, __VA_ARGS__);
void RAY_transform(RAY_Ray* dest, const RAY_Ray* orig, const MATRIX_Matrix* matrix);
void RAY_position(TUPLES_Point* pos, const RAY_Ray* ray, double t);



typedef struct {
    double t;
    const void* object;
} RAY_Xs;

typedef struct {
    unsigned int count;
    RAY_Xs* xs;
} RAY_Intersections;

RAY_Intersections* RAY_new_intersections();
void RAY_add_intersection(RAY_Intersections* intersections, double intersection, const void* object);
void RAY_delete_intersections(RAY_Intersections* intersections);
RAY_Xs* RAY_hit(RAY_Intersections* intersections);

#endif //DATA_STRUCTURES_RAY_H