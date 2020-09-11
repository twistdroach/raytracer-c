#include <assert.h>
#include <math.h>

#include "cylinder.h"

const SHAPE_vtable CYLINDER_vtable = {
        &CYLINDER_local_intersect,
        &SHAPE_delete,
        &CYLINDER_local_normal_at
};

void CYLINDER_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cylinder, const TUPLES_Point* local_point) {
    assert(local_normal);
    assert(cylinder);
    assert(local_point);
    TUPLES_init_vector(local_normal, local_point->x, 0, local_point->z);
}

void CYLINDER_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* cylinder, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(cylinder);
    assert(local_ray);

    double a = pow(local_ray->direction.x, 2) + pow(local_ray->direction.z, 2);
    //ray parallel to y axis
    if (double_equal(0, a)) {
        return;
    }

    double b = 2 * local_ray->origin.x * local_ray->direction.x +
               2 * local_ray->origin.z * local_ray->direction.z;

    double c = pow(local_ray->origin.x, 2) + pow(local_ray->origin.z, 2) - 1;

    double disc = pow(b, 2) - 4 * a * c;

    if (disc < 0) {
        return;
    }

    double t0 = (-b - sqrt(disc)) / (2 * a);
    double t1 = (-b + sqrt(disc)) / (2 * a);
    RAY_add_intersection(intersections, t0, cylinder);
    RAY_add_intersection(intersections, t1, cylinder);
}
