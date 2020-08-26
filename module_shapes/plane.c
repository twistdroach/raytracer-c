#include <assert.h>
#include <math.h>

#include "plane.h"

void PLANE_local_normal_at(TUPLES_Vector* local_normal, const PLANE_Plane* plane, const TUPLES_Point* local_point) {
    assert(local_normal);
    assert(plane);
    assert(local_point);
    TUPLES_init_vector(local_normal, 0, 1, 0);
}

void PLANE_local_intersect(RAY_Intersections* intersections, PLANE_Plane* plane, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(plane);
    assert(local_ray);
    if (fabs(local_ray->direction.y) < EPSILON) {
        //slope is 0 or near to it, so no intersections are possible
        return;
    }

    double t = -local_ray->origin.y / local_ray->direction.y;
    RAY_add_intersection(intersections, t, plane);
}
