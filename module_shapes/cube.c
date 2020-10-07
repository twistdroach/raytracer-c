#include <math.h>
#include <assert.h>
#include "cube.h"


void CUBE_bounds_of(const SHAPE_Shape* shape, BOUND_Box* box) {
    assert(shape);
    assert(box);
    UNUSED(shape);
    BOUND_init(box);
    BOUND_add_point(box, 1, 1, 1);
    BOUND_add_point(box, -1, -1, -1);
}

const SHAPE_vtable CUBE_vtable = {
        &CUBE_local_intersect,
        &SHAPE_delete,
        &CUBE_local_normal_at,
        &SHAPE_default_shape_contains,
        &CUBE_bounds_of
};


void CUBE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cube, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(cube);
    assert(local_point);
    UNUSED(cube);
    UNUSED(hit);
    double maxc = UTILITIES_max(fabs(local_point->x), fabs(local_point->y), fabs(local_point->z));

    if (maxc == fabs(local_point->x)) {
        TUPLES_init_vector(local_normal, local_point->x, 0, 0);
    } else if (maxc == fabs(local_point->y)) {
        TUPLES_init_vector(local_normal, 0, local_point->y, 0);
    } else {
        TUPLES_init_vector(local_normal, 0, 0, local_point->z);
    }
}

void CUBE_check_axis(double* tmin, double* tmax, double origin, double direction, double min, double max) {
    double tmin_numerator = min - origin;
    double tmax_numerator = max - origin;

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
    CUBE_check_axis(&xtmin, &xtmax, local_ray->origin.x, local_ray->direction.x, -1, 1);
    CUBE_check_axis(&ytmin, &ytmax, local_ray->origin.y, local_ray->direction.y, -1, 1);
    CUBE_check_axis(&ztmin, &ztmax, local_ray->origin.z, local_ray->direction.z, -1, 1);

    double tmin = UTILITIES_max(xtmin, ytmin, ztmin);
    double tmax = UTILITIES_min(xtmax, ytmax, ztmax);

    if (tmin < tmax) {
        RAY_add_intersection(intersections, tmin, cube);
        RAY_add_intersection(intersections, tmax, cube);
    }
}
