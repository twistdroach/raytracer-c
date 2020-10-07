#include <assert.h>
#include <math.h>
#include <ray.h>
#include "bound.h"
#include "cube.h"

BOUND_Box* BOUND_new() {
    BOUND_Box* box = malloc(sizeof(BOUND_Box));
    if (!box) {
        Throw(E_MALLOC_FAILED);
    }
    BOUND_init(box);
    return box;
}
void BOUND_init(BOUND_Box* box) {
    assert(box);
    TUPLES_init_point(&box->min, INFINITY, INFINITY, INFINITY);
    TUPLES_init_point(&box->max, -INFINITY, -INFINITY, -INFINITY);
}

void BOUND_delete(BOUND_Box* box) {
    assert(box);
    free(box);
}

void BOUND_add_point_from_tuple(BOUND_Box* box, const TUPLES_Point* point) {
    assert(box);
    assert(point);
    BOUND_add_point(box, point->x, point->y, point->z);
}

void BOUND_add_point(BOUND_Box* box, double x, double y, double z) {
    assert(box);
    if (x < box->min.x)
        box->min.x = x;

    if (y < box->min.y)
        box->min.y = y;

    if (z < box->min.z)
        box->min.z = z;

    if (x > box->max.x)
        box->max.x = x;

    if (y > box->max.y)
        box->max.y = y;

    if (z > box->max.z)
        box->max.z = z;
}

void BOUND_add_box(BOUND_Box* dest, const BOUND_Box* src) {
    assert(dest);
    assert(src);
    BOUND_add_point_from_tuple(dest, &src->min);
    BOUND_add_point_from_tuple(dest, &src->max);
}

static bool between(double min, double x, double max) {
    return double_equal(min, x) ||
           double_equal(max, x) ||
           (x > min && x < max);
}

bool BOUND_contains_point(const BOUND_Box* box, const TUPLES_Point* point) {
    assert(box);
    assert(point);
    return between(box->min.x, point->x, box->max.x) &&
           between(box->min.y, point->y, box->max.y) &&
           between(box->min.z, point->z, box->max.z);
}

bool BOUND_contains_box(const BOUND_Box* outer, const BOUND_Box* inner) {
    assert(outer);
    assert(inner);
    return BOUND_contains_point(outer, &inner->min) &&
           BOUND_contains_point(outer, &inner->max);
}

void BOUND_transform(BOUND_Box* dest, const BOUND_Box* box, const MATRIX_Matrix* transformation) {
    assert(dest);
    assert(box);
    assert(transformation);
    TUPLES_Point p[8];
    const TUPLES_Point *min = &box->min;
    const TUPLES_Point *max = &box->max;

    TUPLES_copy(&p[0], min);
    TUPLES_init_point(&p[1], min->x, min->y, max->z);
    TUPLES_init_point(&p[2], min->x, max->y, min->z);
    TUPLES_init_point(&p[3], min->x, max->y, max->z);
    TUPLES_init_point(&p[4], max->x, min->y, min->z);
    TUPLES_init_point(&p[5], max->x, min->y, max->z);
    TUPLES_init_point(&p[6], max->x, max->y, min->z);
    TUPLES_copy(&p[7], max);

    BOUND_init(dest);
    for (uint ndx = 0; ndx < 8; ndx++) {
        TUPLES_Point transformed_point;
        MATRIX_multiply_tuple(&transformed_point, transformation, &p[ndx]);
        BOUND_add_point_from_tuple(dest, &transformed_point);
    }
}

bool BOUND_intersect(const BOUND_Box* box, const RAY_Ray* ray) {
    assert(box);
    assert(ray);

    double xtmin, xtmax, ytmin, ytmax, ztmin, ztmax;
    CUBE_check_axis(&xtmin, &xtmax, ray->origin.x, ray->direction.x, box->min.x, box->max.x);
    CUBE_check_axis(&ytmin, &ytmax, ray->origin.y, ray->direction.y, box->min.y, box->max.y);
    CUBE_check_axis(&ztmin, &ztmax, ray->origin.z, ray->direction.z, box->min.z, box->max.z);

    double tmin = UTILITIES_max(xtmin, ytmin, ztmin);
    double tmax = UTILITIES_min(xtmax, ytmax, ztmax);

    if (tmin < tmax) {
        return true;
    }

    return false;
}
