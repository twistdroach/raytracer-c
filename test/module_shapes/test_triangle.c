#include <unity.h>

#include "triangle.h"

#include "testutils.h"

void setUp() {}
void tearDown() {}

void test_construct_a_triangle() {
    TRIANGLE_Triangle* t = TRIANGLE_new(0, 1, 0, -1, 0, 0, 1, 0, 0);
    TUPLES_Vector expected_e1, expected_e2, expected_normal;
    TUPLES_init_vector(&expected_e1, -1, -1, 0);
    TUPLES_init_vector(&expected_e2, 1, -1, 0);
    TUPLES_init_vector(&expected_normal, 0, 0, -1);
    test_tuples(&expected_e1, &t->e1);
    test_tuples(&expected_e2, &t->e2);
    test_tuples(&expected_normal, &t->normal);
    TRIANGLE_delete(t);
}

void test_construct_a_triangle_from_points() {
    TUPLES_Point p1, p2, p3;
    TUPLES_init_point(&p1, 0, 1, 0);
    TUPLES_init_point(&p2, -1, 0, 0);
    TUPLES_init_point(&p3, 1, 0, 0);
    TRIANGLE_Triangle* t = TRIANGLE_new_from_points(&p1, &p2, &p3);
    TUPLES_Vector expected_e1, expected_e2, expected_normal;
    TUPLES_init_vector(&expected_e1, -1, -1, 0);
    TUPLES_init_vector(&expected_e2, 1, -1, 0);
    TUPLES_init_vector(&expected_normal, 0, 0, -1);
    test_tuples(&expected_e1, &t->e1);
    test_tuples(&expected_e2, &t->e2);
    test_tuples(&expected_normal, &t->normal);
    TRIANGLE_delete(t);
}

void test_find_normal_on_a_triangle() {
    TRIANGLE_Triangle* t = TRIANGLE_new(0, 1, 0,
                                        -1, 0, 0,
                                        1, 0, 0);
    TUPLES_Vector n1, n2, n3;
    TUPLES_Point p1, p2, p3;
    TUPLES_init_point(&p1, 0, 0.5, 0);
    TUPLES_init_point(&p2, -0.5, 0.75, 0);
    TUPLES_init_point(&p3, 0.5, 0.25, 0);
    TRIANGLE_local_normal_at(&n1, (SHAPE_Shape*)t, &p1);
    TRIANGLE_local_normal_at(&n2, (SHAPE_Shape*)t, &p2);
    TRIANGLE_local_normal_at(&n3, (SHAPE_Shape*)t, &p3);
    test_tuples(&t->normal, &n1);
    test_tuples(&t->normal, &n2);
    test_tuples(&t->normal, &n3);
    TRIANGLE_delete(t);
}

void test_intersecting_ray_parallel_to_triangle() {
    TRIANGLE_Triangle* t = TRIANGLE_new(0, 1, 0,
                                        -1, 0, 0,
                                        1, 0, 0);
    RAY_Ray ray;
    RAY_init(&ray, 0, -1, -2, 0, 1, 0);

    RAY_Intersections* xs = RAY_new_intersections();
    TRIANGLE_local_intersect(xs, (SHAPE_Shape*)t, &ray);
    TEST_ASSERT_EQUAL(0, xs->count);
    RAY_delete_intersections(xs);
    TRIANGLE_delete(t);
}

void helper_ray_misses_triangle(double ray_x, double ray_y, double ray_z) {
    TRIANGLE_Triangle* t = TRIANGLE_new(0, 1, 0,
                                        -1, 0, 0,
                                        1, 0, 0);
    RAY_Ray ray;
    RAY_init(&ray, ray_x, ray_y, ray_z, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    TRIANGLE_local_intersect(xs, (SHAPE_Shape*)t, &ray);
    TEST_ASSERT_EQUAL(0, xs->count);
    RAY_delete_intersections(xs);
    TRIANGLE_delete(t);
}

void test_ray_misses_p1_p3_edge() {
    helper_ray_misses_triangle(1, 1, -2);
}

void test_ray_misses_p1_p2_edge() {
    helper_ray_misses_triangle(-1, 1, -2);
}

void test_ray_misses_p2_p3_edge() {
    helper_ray_misses_triangle(0, -1, -2);
}

void test_ray_strikes_triangle() {
    TRIANGLE_Triangle* t = TRIANGLE_new(0, 1, 0,
                                        -1, 0, 0,
                                        1, 0, 0);
    RAY_Ray ray;
    RAY_init(&ray, 0, 0.5, -2, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    TRIANGLE_local_intersect(xs, (SHAPE_Shape*)t, &ray);
    TEST_ASSERT_EQUAL(1, xs->count);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, xs->xs[0].t);
    TEST_ASSERT_EQUAL_PTR(t, xs->xs[0].object);
    RAY_delete_intersections(xs);
    TRIANGLE_delete(t);
}

int main(void)

{
    UNITY_BEGIN();
    RUN_TEST(test_construct_a_triangle);
    RUN_TEST(test_construct_a_triangle_from_points);
    RUN_TEST(test_find_normal_on_a_triangle);
    RUN_TEST(test_intersecting_ray_parallel_to_triangle);
    RUN_TEST(test_ray_misses_p1_p2_edge);
    RUN_TEST(test_ray_misses_p1_p3_edge);
    RUN_TEST(test_ray_misses_p2_p3_edge);
    RUN_TEST(test_ray_strikes_triangle);
    return UNITY_END();
}
