#include "sphere.h"
#include <math.h>
#include <assert.h>

void SPHERE_bounds_of(const SHAPE_Shape* shape, BOUND_Box* box) {
    assert(shape);
    assert(box);
    UNUSED(shape);
    BOUND_init(box);
    BOUND_add_point(box, 1, 1, 1);
    BOUND_add_point(box, -1, -1, -1);
}

const SHAPE_vtable SPHERE_vtable = {
        &SPHERE_local_intersect,
        &SHAPE_delete,
        &SPHERE_local_normal_at,
        &SHAPE_default_shape_contains,
        &SPHERE_bounds_of,
        NULL
};

void SPHERE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);
    SPHERE_Sphere* sphere = (SPHERE_Sphere*) shape;
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

void SPHERE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(hit);
    UNUSED(shape);

    TUPLES_Point sphere_origin;
    TUPLES_init_point(&sphere_origin, 0, 0, 0);

    TUPLES_subtract(local_normal, local_point, &sphere_origin);
}

SPHERE_Sphere* SPHERE_make_glassy(SPHERE_Sphere* sphere) {
    assert(sphere);
    MATERIAL_Material* material = SPHERE_get_material(sphere);
    material->transparency = 1.0;
    material->refractive_index = 1.5;
    return sphere;
}

