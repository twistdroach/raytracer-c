#include <unity.h>
#include "sphere.h"
#include "ray.h"

void setUp() {}
void tearDown() {}

void test_ray_intersects_sphere_at_two_points() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(4.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(6.0, intersections->xs[1].t);
    RAY_delete_intersections(intersections);
}

void test_ray_sets_object_on_intersection() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_PTR(&s, intersections->xs[0].object);
    TEST_ASSERT_EQUAL_PTR(&s, intersections->xs[1].object);
    RAY_delete_intersections(intersections);
}

void test_ray_intersects_sphere_at_tangent() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 1, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(5.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(5.0, intersections->xs[1].t);
    RAY_delete_intersections(intersections);
}

void test_ray_misses_sphere() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 2, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(0, intersections->count);
    RAY_delete_intersections(intersections);
}

void test_ray_originates_inside_sphere() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 0, 0, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, intersections->xs[1].t);
    RAY_delete_intersections(intersections);
}

void test_sphere_behind_ray() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 0, 5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(-6.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(-4.0, intersections->xs[1].t);
    RAY_delete_intersections(intersections);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ray_intersects_sphere_at_two_points);
    RUN_TEST(test_ray_intersects_sphere_at_tangent);
    RUN_TEST(test_ray_misses_sphere);
    RUN_TEST(test_ray_originates_inside_sphere);
    RUN_TEST(test_sphere_behind_ray);
    RUN_TEST(test_ray_sets_object_on_intersection);
    return UNITY_END();
}
