#include <assert.h>
#include <math.h>
#include <exceptions.h>

#include "cylinder.h"

void CYLINDER_delete_shape(SHAPE_Shape* shape) {
    assert(shape);
    CYLINDER_Cylinder* cylinder = (CYLINDER_Cylinder*)shape;
    CYLINDER_delete(cylinder);
}

const SHAPE_vtable CYLINDER_vtable = {
        &CYLINDER_local_intersect,
        &CYLINDER_delete_shape,
        &CYLINDER_local_normal_at
};

CYLINDER_Cylinder* CYLINDER_new() {
    CYLINDER_Cylinder* cylinder = malloc(sizeof(CYLINDER_Cylinder));
    if (!cylinder) {
        Throw(E_MALLOC_FAILED);
    }
    CYLINDER_init(cylinder);
    return cylinder;
}

void CYLINDER_init(CYLINDER_Cylinder* cylinder) {
    assert(cylinder);
    SHAPE_init(&cylinder->shape, &CYLINDER_vtable);
    cylinder->minimum = -INFINITY;
    cylinder->maximum = INFINITY;
    cylinder->closed = false;
}

void CYLINDER_destroy(CYLINDER_Cylinder* cylinder) {
    assert(cylinder);
    SHAPE_destroy(&cylinder->shape);
}

void CYLINDER_delete(CYLINDER_Cylinder* cylinder) {
    assert(cylinder);
    CYLINDER_destroy(cylinder);
    free(cylinder);
}

void CYLINDER_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(hit);

    CYLINDER_Cylinder* cylinder = (CYLINDER_Cylinder*)shape;
    double dist = pow(local_point->x, 2) + pow(local_point->z, 2);
    if (dist < 1 && local_point->y >= cylinder->maximum - EPSILON) {
        TUPLES_init_vector(local_normal, 0, 1, 0);
    } else if (dist < 1 && local_point->y <= cylinder->minimum + EPSILON) {
        TUPLES_init_vector(local_normal, 0, -1, 0);
    } else {
        TUPLES_init_vector(local_normal, local_point->x, 0, local_point->z);
    }
}

bool CYLINDER_check_cap(const RAY_Ray* local_ray, double t, double radius) {
    double x = local_ray->origin.x + t * local_ray->direction.x;
    double z = local_ray->origin.z + t * local_ray->direction.z;

    return (pow(x, 2) + pow(z,2)) <= radius;
}

static void intersect_caps(RAY_Intersections* intersections, CYLINDER_Cylinder* cylinder, const RAY_Ray* local_ray) {
    // if not closed, no need to check
    if (!cylinder->closed || double_equal(0.0, local_ray->direction.y)) {
        return;
    }

    // check bottom cap
    double t = (cylinder->minimum - local_ray->origin.y) / local_ray->direction.y;
    if (CYLINDER_check_cap(local_ray, t, 1.0)) {
        RAY_add_intersection(intersections, t, cylinder);
    }

    // check top cap
    t = (cylinder->maximum - local_ray->origin.y) / local_ray->direction.y;
    if (CYLINDER_check_cap(local_ray, t, 1.0)) {
        RAY_add_intersection(intersections, t, cylinder);
    }
}

void CYLINDER_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);

    CYLINDER_Cylinder* cylinder = (CYLINDER_Cylinder*) shape;
    double a = pow(local_ray->direction.x, 2) + pow(local_ray->direction.z, 2);

    intersect_caps(intersections, cylinder, local_ray);
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

    if (t0 > t1) {
        double tmp = t1;
        t1 = t0;
        t0 = tmp;
    }

    double y0 = local_ray->origin.y + t0 * local_ray->direction.y;
    if (cylinder->minimum < y0 && y0 < cylinder->maximum) {
        RAY_add_intersection(intersections, t0, cylinder);
    }

    double y1 = local_ray->origin.y + t1 * local_ray->direction.y;
    if (cylinder->minimum < y1 && y1 < cylinder->maximum) {
        RAY_add_intersection(intersections, t1, cylinder);
    }
}
