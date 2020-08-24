#include <exceptions.h>
#include <assert.h>
#include "shapeholder.h"

void SHAPEHOLDER_init(SHAPEHOLDER_Shapeholder* holder, void *shape, SHAPEHOLDER_Type type) {
   holder->type = type;
   holder->shape = shape;
}

void SHAPEHOLDER_intersect(RAY_Intersections* intersections, const SHAPEHOLDER_Shapeholder* shape, const RAY_Ray* ray) {
    assert(intersections);
    assert(shape);
    assert(ray);
    RAY_Ray local_ray;
    SHAPE_calc_local_ray(&local_ray, ray, (SHAPE_Shape*) shape->shape);
    switch (shape->type) {
        case SHAPEHOLDER_TESTSHAPE: TESTSHAPE_local_intercept(intersections, (TESTSHAPE_TestShape*) shape->shape, &local_ray); break;
//        case SHAPEHOLDER_SPHERE:    SPHERE_local_intercept(intersections, (SPHERE_Sphere*) shape->shape, &local_ray); break;
        default: assert(0);
    }
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
//        case SHAPEHOLDER_SPHERE:    SPHERE_local_normal_at(&local_normal, (SPHERE_Sphere*) shape->shape, &local_point); break;
        default: assert(0);
    }
    SHAPE_calc_world_normal(world_normal, (SHAPE_Shape*) shape->shape, &local_normal);
}
