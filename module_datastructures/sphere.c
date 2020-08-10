//
// Created by zrowitsch on 8/7/20.
//

#include "sphere.h"
#include <math.h>

void SPHERE_init(SPHERE_Sphere* sphere, double x, double y, double z, double radius) {
    TUPLES_init_point(&sphere->origin, x, y , z);
    sphere->radius = radius;
}

RAY_Intersections* SPHERE_intersect(const SPHERE_Sphere* sphere, const RAY_Ray* ray) {
    RAY_Intersections* intersections = RAY_new_intersections();
    TUPLES_Vector sphere_to_ray;

    //compute discriminant
    TUPLES_subtract(&sphere_to_ray, &ray->origin, &sphere->origin);
    double a = TUPLES_dot(&ray->direction, &ray->direction);
    double b = 2 * TUPLES_dot(&ray->direction, &sphere_to_ray);
    double c = TUPLES_dot(&sphere_to_ray, &sphere_to_ray) - 1.0;
    double discriminant = pow(b, 2.0) - 4.0 * a *c;
    if (discriminant < 0) {
        //no hit
        return intersections;
    }

    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    RAY_add_intersection(intersections, t1, sphere);
    RAY_add_intersection(intersections, t2, sphere);
    return intersections;
}
