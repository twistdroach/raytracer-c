#ifndef DATA_STRUCTURES_RAY_H
#define DATA_STRUCTURES_RAY_H

#include "tuples.h"
#include "matrix.h"

typedef struct RAY_Ray {
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


//forward declare temporarily until we generalize
typedef struct SPHERE_Sphere SPHERE_Sphere;
typedef struct RAY_Xs {
    double t;
    const SPHERE_Sphere* object;
} RAY_Xs;

typedef struct RAY_Intersections {
    unsigned int count;
    RAY_Xs* xs;
} RAY_Intersections;

RAY_Intersections* RAY_new_intersections();
void RAY_add_intersection(RAY_Intersections* intersections, double intersection, const void* object);
void RAY_delete_intersections(RAY_Intersections* intersections);

/**
 * Returns the intersection with the lowest positive t value.
 * Returned ptr should not be freed.  To free - free the full RAY_Intersections object.
 * @param intersections
 * @return
 */
RAY_Xs* RAY_hit(RAY_Intersections* intersections);
void RAY_sort_intersections(RAY_Intersections* intersections);

typedef struct RAY_Computations {
    double               t;
    const SPHERE_Sphere* object;
    TUPLES_Point         point;
    TUPLES_Point         over_point;
    TUPLES_Vector        eyev;
    TUPLES_Vector        normalv;
    bool                 inside;
} RAY_Computations;

/**
 * Allocates and computes some info to be used when rendering
 * RAY_delete_computations must be called on the returned pointer
 * @param hit
 * @param ray
 * @return
 */
RAY_Computations* RAY_prepare_computations(const RAY_Xs* hit, const RAY_Ray* ray);
void RAY_delete_computations(RAY_Computations* comps);

#endif //DATA_STRUCTURES_RAY_H
