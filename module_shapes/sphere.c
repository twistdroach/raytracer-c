#include "sphere.h"
#include <math.h>
#include <assert.h>

void SPHERE_local_intersect(RAY_Intersections* intersections, SPHERE_Sphere* sphere, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(sphere);
    assert(local_ray);

    TUPLES_Point sphere_origin;
    TUPLES_init_point(&sphere_origin, 0, 0, 0);
    TUPLES_Vector sphere_to_ray;
    //compute discriminant
    TUPLES_subtract(&sphere_to_ray, &local_ray->origin, &sphere_origin);
    double a = TUPLES_dot(&local_ray->direction, &local_ray->direction);
    double b = 2 * TUPLES_dot(&local_ray->direction, &sphere_to_ray);
    double c = TUPLES_dot(&sphere_to_ray, &sphere_to_ray) - 1.0;
    double discriminant = pow(b, 2.0) - 4.0 * a *c;
    if (discriminant < 0) {
        //no hit
        return;
    }

    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    RAY_add_intersection(intersections, t1, sphere);
    RAY_add_intersection(intersections, t2, sphere);
}

void SPHERE_local_normal_at(TUPLES_Vector* local_normal, const SPHERE_Sphere* sphere, const TUPLES_Point* local_point) {
    assert(local_normal);
    assert(sphere);
    assert(local_point);

    TUPLES_Point sphere_origin;
    TUPLES_init_point(&sphere_origin, 0, 0, 0);

    TUPLES_subtract(local_normal, local_point, &sphere_origin);
}

