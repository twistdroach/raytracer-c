#include "intersections.h"

INTERSECTION_Intersection* INTERSECTION_prepare_computations(const RAY_Xs* hit, const RAY_Ray* ray) {
    INTERSECTION_Intersection* intersection = malloc(sizeof(INTERSECTION_Intersection));
    //keep a handle on the object intersected
    intersection->t = hit->t;
    intersection->object = hit->object;

    //compute the point of intersection
    RAY_position(&intersection->point, ray, intersection->t);

    //compute the eye vector
    intersection->eyev = TUPLES_negate(ray->direction);

    //compute the normal @ the intersection
    intersection->normalv = SPHERE_normal_at(intersection->object, intersection->point);

    //compute the "over_point" to deal with floating point imprecision...
    intersection->over_point = TUPLES_add(TUPLES_multiply(intersection->normalv, EPSILON), intersection->point);

    if (TUPLES_dot(intersection->normalv, intersection->eyev) < 0) {
        intersection->inside = true;
        intersection->normalv = TUPLES_negate(intersection->normalv);
    } else {
        intersection->inside = false;
    }

    return intersection;
}

void INTERSECTION_delete( INTERSECTION_Intersection* intersection) {
    free(intersection);
}
