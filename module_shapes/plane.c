#include <assert.h>
#include <math.h>

#include "plane.h"

const SHAPE_vtable PLANE_vtable = {
        &PLANE_local_intersect,
        &SHAPE_delete,
        &PLANE_local_normal_at
};

void PLANE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    TUPLES_init_vector(local_normal, 0, 1, 0);
}

void PLANE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);
    PLANE_Plane* plane = (PLANE_Plane*) shape;
    if (fabs(local_ray->direction.y) < EPSILON) {
        //slope is 0 or near to it, so no intersections are possible
        return;
    }

    double t = -local_ray->origin.y / local_ray->direction.y;
    RAY_add_intersection(intersections, t, plane);
}
