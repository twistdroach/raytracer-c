#include <unity.h>
#include <cylinder.h>
#include <tuples.h>
#include <ray.h>
#include "testutils.h"

void setUp() {}
void tearDown() {}

void helper_ray_misses_cylinder(double ox, double oy, double oz,
                                double dx, double dy, double dz) {
    CYLINDER_Cylinder* c = CYLINDER_new();
    TUPLES_Vector direction;
    TUPLES_Point origin;
    TUPLES_init_vector(&direction, dx, dy, dz);
    TUPLES_normalize(&direction);
    TUPLES_init_vector(&origin, ox, oy, oz);
    RAY_Ray ray;
    RAY_init_from_tuples(&ray, &origin, &direction);
    RAY_Intersections* xs = RAY_new_intersections();
    CYLINDER_local_intersect(xs, c, &ray);
    TEST_ASSERT_EQUAL(0, xs->count);
    RAY_delete_intersections(xs);
    CYLINDER_delete(c);
}

void test_ray_misses_cylinder() {
    helper_ray_misses_cylinder(1, 0, 0, 0, 1, 0);
    helper_ray_misses_cylinder(0, 0, 0, 0, 1, 0);
    helper_ray_misses_cylinder(0, 0, -5, 1, 1, 1);
}

void helper_ray_hits_cylinder(double ox, double oy, double oz,
                              double dx, double dy, double dz,
                              double t0, double t1) {
    CYLINDER_Cylinder* c = CYLINDER_new();
    TUPLES_Vector direction;
    TUPLES_Point origin;
    TUPLES_init_vector(&direction, dx, dy, dz);
    TUPLES_normalize(&direction);
    TUPLES_init_point(&origin, ox, oy, oz);
    RAY_Ray ray;
    RAY_init_from_tuples(&ray, &origin, &direction);
    RAY_Intersections* xs = RAY_new_intersections();
    CYLINDER_local_intersect(xs, c, &ray);
    TEST_ASSERT_EQUAL(2, xs->count);
    TEST_ASSERT_EQUAL_DOUBLE(t0, xs->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(t1, xs->xs[1].t);
    RAY_delete_intersections(xs);
    CYLINDER_delete(c);
}

void test_ray_hits_cylinder() {
    helper_ray_hits_cylinder(1, 0, -5, 0, 0, 1, 5, 5);
    helper_ray_hits_cylinder(0, 0, -5, 0, 0, 1, 4, 6);
    helper_ray_hits_cylinder(0.5, 0, -5, 0.1, 1, 1, 6.80798, 7.08872);
}

void helper_normal_vector_on_cylinder(double px, double py, double pz,
                                    double nx, double ny, double nz) {
    CYLINDER_Cylinder* c = CYLINDER_new();
    TUPLES_Point p;
    TUPLES_init_point(&p, px, py, pz);
    TUPLES_Vector got, expected;
    CYLINDER_local_normal_at(&got, c, &p);
    TUPLES_init_vector(&expected, nx, ny, nz);
    test_tuples(&got, &expected);
    CYLINDER_delete(c);
}

void test_normal_vector_on_cylinder() {
    helper_normal_vector_on_cylinder(1, 0, 0, 1, 0, 0);
    helper_normal_vector_on_cylinder(0, 5, -1, 0, 0, -1);
    helper_normal_vector_on_cylinder(0, -2, 1, 0, 0, 1);
    helper_normal_vector_on_cylinder(-1, 1, 0, -1, 0, 0);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ray_misses_cylinder);
    RUN_TEST(test_ray_hits_cylinder);
    RUN_TEST(test_normal_vector_on_cylinder);
    return UNITY_END();
}
