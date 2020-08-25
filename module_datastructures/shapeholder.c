#include <assert.h>
#include "shapeholder.h"
#include "shape.h"
#include "sphere.h"
#include "testshape.h"


void SHAPEHOLDER_init(SHAPEHOLDER_Shapeholder* holder, void *shape, SHAPEHOLDER_Type type) {
   holder->type = type;
   holder->shape = shape;
}

static void augment_intersection(RAY_Xs* xs, void* holder) {
    assert(xs);
    assert(holder);
    xs->object = (SHAPEHOLDER_Shapeholder*) holder;
}

static void augment_intersections_with_holder(RAY_Intersections* intersections, RAY_Intersections* local_intersections, const SHAPEHOLDER_Shapeholder* shape) {
    assert(intersections);
    assert(local_intersections);
    assert(shape);
    RAY_iterate_intersections(local_intersections, augment_intersection, (void*)shape);
    RAY_add_intersections(intersections, local_intersections);
}

void SHAPEHOLDER_intersect(RAY_Intersections* intersections, const SHAPEHOLDER_Shapeholder* shape, const RAY_Ray* ray) {
    assert(shape);
    assert(ray);

    // intersections can be appended for multiple shapes, so on the first call, we might get NULL here.
    if (!intersections) {
        intersections = RAY_new_intersections();
    }

    RAY_Intersections* local_intersections = RAY_new_intersections();
    RAY_Ray local_ray;
    SHAPE_calc_local_ray(&local_ray, ray, (SHAPE_Shape*) shape->shape);
    switch (shape->type) {
        case SHAPEHOLDER_TESTSHAPE: TESTSHAPE_local_intersect(local_intersections, (TESTSHAPE_TestShape*) shape->shape, &local_ray); break;
        case SHAPEHOLDER_SPHERE:    SPHERE_local_intersect(local_intersections, (SPHERE_Sphere*) shape->shape, &local_ray); break;
        default: assert(0);
    }
    //TODO - do this without copying the intersections
    augment_intersections_with_holder(intersections, local_intersections, shape);
    RAY_delete_intersections(local_intersections);
}

void SHAPEHOLDER_normal_at(TUPLES_Vector* world_normal, SHAPEHOLDER_Shapeholder* shape, const TUPLES_Point* point) {
    assert(world_normal);
    assert(shape);
    assert(point);
    TUPLES_Point local_point;
    SHAPE_calc_local_point(&local_point, (SHAPE_Shape*) shape->shape, point);
    TUPLES_Vector local_normal;
    switch (shape->type) {
        case SHAPEHOLDER_TESTSHAPE: TESTSHAPE_local_normal_at(&local_normal, (TESTSHAPE_TestShape*) shape->shape, &local_point); break;
        case SHAPEHOLDER_SPHERE:    SPHERE_local_normal_at(&local_normal, (SPHERE_Sphere*) shape->shape, &local_point); break;
        default: assert(0);
    }
    SHAPE_calc_world_normal(world_normal, (SHAPE_Shape*) shape->shape, &local_normal);
}
