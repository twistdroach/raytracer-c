#include <math.h>
#include <assert.h>
#include "cube.h"


const SHAPE_vtable CUBE_vtable = {
        &CUBE_local_intersect,
        &SHAPE_delete,
        &CUBE_local_normal_at
};

static double max(double x, double y, double z) {
    double xory = (x > y) ? x : y;
    return (xory > z) ? xory : z;
}

static double min(double x, double y, double z) {
    double xory = (x < y) ? x : y;
    return (xory < z) ? xory : z;
}

void CUBE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cube, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(cube);
    assert(local_point);
    UNUSED(cube);
    UNUSED(hit);
    double maxc = max(fabs(local_point->x), fabs(local_point->y), fabs(local_point->z));

    if (maxc == fabs(local_point->x)) {
        TUPLES_init_vector(local_normal, local_point->x, 0, 0);
    } else if (maxc == fabs(local_point->y)) {
        TUPLES_init_vector(local_normal, 0, local_point->y, 0);
    } else {
        TUPLES_init_vector(local_normal, 0, 0, local_point->z);
    }
}

static void check_axis(double* tmin, double* tmax, double origin, double direction) {
    double tmin_numerator = -1 - origin;
    double tmax_numerator = 1 - origin;

    if (fabs(direction) >= EPSILON) {
        *tmin = tmin_numerator / direction;
        *tmax = tmax_numerator / direction;
    } else {
        *tmin = tmin_numerator * INFINITY;
        *tmax = tmax_numerator * INFINITY;
    }

    if (*tmin > *tmax) {
        double tmp = *tmax;
        *tmax = *tmin;
        *tmin = tmp;
    }
}

void CUBE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* cube, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(cube);
    assert(local_ray);
    double xtmin, xtmax, ytmin, ytmax, ztmin, ztmax;
    check_axis(&xtmin, &xtmax, local_ray->origin.x, local_ray->direction.x);
    check_axis(&ytmin, &ytmax, local_ray->origin.y, local_ray->direction.y);
    check_axis(&ztmin, &ztmax, local_ray->origin.z, local_ray->direction.z);

    double tmin = max(xtmin, ytmin, ztmin);
    double tmax = min(xtmax, ytmax, ztmax);

    if (tmin < tmax) {
        RAY_add_intersection(intersections, tmin, cube);
        RAY_add_intersection(intersections, tmax, cube);
    }
}
