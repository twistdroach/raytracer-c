#include <assert.h>
#include <math.h>

#include "exceptions.h"
#include "cone.h"

const SHAPE_vtable CONE_vtable = {
        &CONE_local_intersect,
        &CYLINDER_delete_shape,
        &CONE_local_normal_at,
        &SHAPE_default_shape_contains
};

CONE_Cone* CONE_new() {
    CONE_Cone* cone = malloc(sizeof(CONE_Cone));
    if (!cone) {
        Throw(E_MALLOC_FAILED);
    }
    CONE_init(cone);
    return cone;
}

void CONE_init(CONE_Cone* cone) {
    assert(cone);
    SHAPE_init(&cone->shape, &CONE_vtable);
    cone->minimum = -INFINITY;
    cone->maximum = INFINITY;
    cone->closed = false;
}

void CONE_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* shape, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    assert(local_normal);
    assert(shape);
    assert(local_point);
    UNUSED(hit);

    CONE_Cone* cone = (CONE_Cone*)shape;
    double dist = pow(local_point->x, 2) + pow(local_point->z, 2);
    if (dist < 1 && local_point->y >= cone->maximum - EPSILON) {
        TUPLES_init_vector(local_normal, 0, 1, 0);
    } else if (dist < 1 && local_point->y <= cone->minimum + EPSILON) {
        TUPLES_init_vector(local_normal, 0, -1, 0);
    } else {
        double y = sqrt(pow(local_point->x, 2) + pow(local_point->z, 2));
        if (local_point->y > 0) {
            y = -y;
        }
        TUPLES_init_vector(local_normal, local_point->x, y, local_point->z);
    }
}

static void intersect_caps(RAY_Intersections* intersections, CONE_Cone* cone, const RAY_Ray* local_ray) {
    // if not closed, no need to check
    if (!cone->closed || double_equal(0.0, local_ray->direction.y)) {
        return;
    }

    // check bottom cap
    double t = (cone->minimum - local_ray->origin.y) / local_ray->direction.y;
    if (CYLINDER_check_cap(local_ray, t, cone->minimum)) {
        RAY_add_intersection(intersections, t, cone);
    }

    // check top cap
    t = (cone->maximum - local_ray->origin.y) / local_ray->direction.y;
    if (CYLINDER_check_cap(local_ray, t, cone->maximum)) {
        RAY_add_intersection(intersections, t, cone);
    }
}

void CONE_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);
    CONE_Cone* cone = (CONE_Cone*)shape;

    double a = pow(local_ray->direction.x, 2) - pow(local_ray->direction.y, 2) + pow(local_ray->direction.z, 2);

    double b = 2 * local_ray->origin.x * local_ray->direction.x
             - 2 * local_ray->origin.y * local_ray->direction.y
             + 2 * local_ray->origin.z * local_ray->direction.z;

    if (double_equal(0, a) && double_equal(0, b)) {
        return;
    }

    double c = pow(local_ray->origin.x, 2) - pow(local_ray->origin.y, 2) + pow(local_ray->origin.z, 2);
    if (double_equal(0, a)) {
        double t = -c / (2 * b);
        RAY_add_intersection(intersections, t, shape);
        return;
    }

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
    if (cone->minimum < y0 && y0 < cone->maximum) {
        RAY_add_intersection(intersections, t0, cone);
    }

    double y1 = local_ray->origin.y + t1 * local_ray->direction.y;
    if (cone->minimum < y1 && y1 < cone->maximum) {
        RAY_add_intersection(intersections, t1, cone);
    }

    intersect_caps(intersections, cone, local_ray);
}
