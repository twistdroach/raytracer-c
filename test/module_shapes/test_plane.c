#include <unity.h>
#include <tuples.h>

#include "testutils.h"
#include "plane.h"

void setUp() {}
void tearDown() {}

void test_construct_a_plane_on_heap() {
    PLANE_Plane *p = PLANE_new();
    TEST_ASSERT_NOT_NULL(p);
    PLANE_delete(p);
}

void test_initialize_plane_on_stack() {
    PLANE_Plane p2;
    PLANE_init(&p2);
    TEST_ASSERT_NOT_NULL(p2.transform);
    PLANE_destroy(&p2);
}

void test_normal_of_plane_is_constant_everywhere() {
    PLANE_Plane* p = PLANE_new();
    TUPLES_Vector n1, n2, n3, normal;
    TUPLES_Point p1, p2, p3;
    TUPLES_init_vector(&normal, 0, 1, 0);
    TUPLES_init_point(&p1, 0, 0, 0);
    TUPLES_init_point(&p2, 10, 0, -10);
    TUPLES_init_point(&p3, -5, 0, 150);
    PLANE_local_normal_at(&n1, p, &p1, NULL);
    PLANE_local_normal_at(&n2, p, &p2, NULL);
    PLANE_local_normal_at(&n3, p, &p3, NULL);
    test_tuples(&normal, &n1);
    test_tuples(&normal, &n2);
    test_tuples(&normal, &n3);
    PLANE_delete(p);
}

void test_intersect_ray_parallel_to_plane() {
    PLANE_Plane* p = PLANE_new();
    RAY_Ray r;
    RAY_init(&r, 0, 10, 0, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    PLANE_local_intersect(intersections, p, &r);
    TEST_ASSERT_EQUAL(0, intersections->count);
    RAY_delete_intersections(intersections);
    PLANE_delete(p);
}

void test_intersect_with_coplanar_ray() {
    PLANE_Plane* p = PLANE_new();
    RAY_Ray r;
    RAY_init(&r, 0, 0, 0, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    PLANE_local_intersect(intersections, p, &r);
    TEST_ASSERT_EQUAL(0, intersections->count);
    RAY_delete_intersections(intersections);
    PLANE_delete(p);
}

void test_intersect_plane_from_above() {
    PLANE_Plane* p = PLANE_new();
    RAY_Ray r;
    RAY_init(&r, 0, 1, 0, 0, -1, 0);
    RAY_Intersections* intersections = RAY_new_intersections();
    PLANE_local_intersect(intersections, p, &r);
    TEST_ASSERT_EQUAL(1, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_PTR(p, intersections->xs[0].object);
    RAY_delete_intersections(intersections);
    PLANE_delete(p);
}

void test_intersect_plane_from_below() {
    PLANE_Plane* p = PLANE_new();
    RAY_Ray r;
    RAY_init(&r, 0, -1, 0, 0, 1, 0);
    RAY_Intersections* intersections = RAY_new_intersections();
    PLANE_local_intersect(intersections, p, &r);
    TEST_ASSERT_EQUAL(1, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_PTR(p, intersections->xs[0].object);
    RAY_delete_intersections(intersections);
    PLANE_delete(p);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_construct_a_plane_on_heap);
    RUN_TEST(test_initialize_plane_on_stack);
    RUN_TEST(test_normal_of_plane_is_constant_everywhere);
    RUN_TEST(test_intersect_ray_parallel_to_plane);
    RUN_TEST(test_intersect_with_coplanar_ray);
    RUN_TEST(test_intersect_plane_from_above);
    RUN_TEST(test_intersect_plane_from_below);
    return UNITY_END();
}
