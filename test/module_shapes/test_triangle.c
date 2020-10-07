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
    TRIANGLE_local_normal_at(&n1, (SHAPE_Shape*)t, &p1, NULL);
    TRIANGLE_local_normal_at(&n2, (SHAPE_Shape*)t, &p2, NULL);
    TRIANGLE_local_normal_at(&n3, (SHAPE_Shape*)t, &p3, NULL);
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

TRIANGLE_SmoothTriangle* create_smoothtriangle() {
    TUPLES_Point p1, p2, p3;
    TUPLES_init_point(&p1, 0, 1, 0);
    TUPLES_init_point(&p2, -1, 0, 0);
    TUPLES_init_point(&p3, 1, 0, 0);

    TUPLES_Vector v1, v2, v3;
    TUPLES_init_vector(&v1, 0, 1, 0);
    TUPLES_init_vector(&v2, -1, 0, 0);
    TUPLES_init_vector(&v3, 1, 0, 0);
    TRIANGLE_SmoothTriangle* st = TRIANGLE_new_smooth_from_points(&p1, &p2, &p3, &v1, &v2, &v3);
    TEST_ASSERT_NOT_NULL(st);
    test_tuples(&p1, &st->p1);
    test_tuples(&p2, &st->p2);
    test_tuples(&p3, &st->p3);
    test_tuples(&v1, &st->n1);
    test_tuples(&v2, &st->n2);
    test_tuples(&v3, &st->n3);
    return st;
}

void test_create_smooth_triangle() {
    TRIANGLE_SmoothTriangle* st = create_smoothtriangle();
    TRIANGLE_delete_smooth(st);
}

void test_intersection_with_smooth_triangle_stores_u_and_v() {
    TRIANGLE_SmoothTriangle* st = create_smoothtriangle();

    RAY_Ray ray;
    RAY_init(&ray, -0.2, 0.3, -2, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    TRIANGLE_local_intersect(xs, (SHAPE_Shape*)st, &ray);

    TEST_ASSERT_EQUAL_DOUBLE(0.45, xs->xs[0].u);
    TEST_ASSERT_EQUAL_DOUBLE(0.25, xs->xs[0].v);

    RAY_delete_intersections(xs);
    TRIANGLE_delete_smooth(st);
}

void test_smooth_triangle_uses_u_v_to_interpolate_normal() {
    TRIANGLE_SmoothTriangle* st = create_smoothtriangle();

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection_tri(xs, 1, (SHAPE_Shape*)st, 0.45, 0.25);

    TUPLES_Point p;
    TUPLES_init_point(&p, 0, 0, 0);

    TUPLES_Vector expected, computed;
    TUPLES_init_vector(&expected, -0.5547, 0.83205, 0);

    SHAPE_normal_at(&computed, (SHAPE_Shape*)st, &p, &xs->xs[0]);

    test_tuples(&expected, &computed);

    RAY_delete_intersections(xs);
    TRIANGLE_delete_smooth(st);
}

void test_prepare_normal_on_smooth_triangle() {
    TRIANGLE_SmoothTriangle* st = create_smoothtriangle();

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection_tri(xs, 1, (SHAPE_Shape*)st, 0.45, 0.25);

    RAY_Ray ray;
    RAY_init(&ray, -0.2, 0.3, -2, 0, 0, 1);

    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Vector expected;
    TUPLES_init_vector(&expected, -0.5547, 0.83205, 0);

    test_tuples(&expected, &comps->normalv);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    TRIANGLE_delete_smooth(st);
}

void test_triangle_bounding_box() {
    TRIANGLE_Triangle* t = TRIANGLE_new(-3, 7, 2,
                                        6, 2, -4,
                                        2, -1, -1);
    BOUND_Box box;
    t->vtable->bounds_of((SHAPE_Shape*)t, &box);
    TUPLES_Point min_expected, max_expected;
    TUPLES_init_point(&min_expected, -3, -1, -4);
    TUPLES_init_point(&max_expected, 6, 7, 2);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&min_expected, &box.min));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&max_expected, &box.max));
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
    RUN_TEST(test_create_smooth_triangle);
    RUN_TEST(test_intersection_with_smooth_triangle_stores_u_and_v);
    RUN_TEST(test_smooth_triangle_uses_u_v_to_interpolate_normal);
    RUN_TEST(test_prepare_normal_on_smooth_triangle);
    RUN_TEST(test_triangle_bounding_box);
    return UNITY_END();
}
