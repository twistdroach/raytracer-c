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
    SPHERE_destroy(&s);
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
    SPHERE_destroy(&s);
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
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_ray_misses_sphere() {
    SPHERE_Sphere s;
    SPHERE_init(&s, 0, 0, 0, 1);
    RAY_Ray r;
    RAY_init(&r, 0, 2, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(&s, &r);
    TEST_ASSERT_EQUAL(0, intersections->count);
    SPHERE_destroy(&s);
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
    SPHERE_destroy(&s);
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
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_sphere_default_transformation() {
    SPHERE_Sphere* s = SPHERE_new(0, 0, 0, 1);
    MATRIX_Matrix* identity_m = MATRIX_new_identity(4);
    TEST_ASSERT_TRUE(MATRIX_is_equal(identity_m, s->transform));
    SPHERE_delete(s);
    MATRIX_delete(identity_m);
}

void test_sphere_modifying_transformation() {
    SPHERE_Sphere* s = SPHERE_new(0, 0, 0, 1);
    MATRIX_Matrix* t = MATRIX_new_translation(2, 3, 4);
    SPHERE_set_transform(s, t);
    TEST_ASSERT_TRUE(MATRIX_is_equal(t, s->transform));
    SPHERE_delete(s);
    MATRIX_delete(t);
}

void test_sphere_intersect_scaled_ray() {
    SPHERE_Sphere* s = SPHERE_new(0, 0, 0, 1);
    MATRIX_Matrix* m = MATRIX_new_scaling(2, 2, 2);
    SPHERE_set_transform(s, m);
    MATRIX_delete(m);
    RAY_Ray* r = RAY_new(0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(s, r);
    TEST_ASSERT_NOT_NULL(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(3.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(7.0, intersections->xs[1].t);
    SPHERE_delete(s);
    RAY_delete_intersections(intersections);
    RAY_delete(r);
}

void test_sphere_intersect_translated_ray() {
    SPHERE_Sphere* s = SPHERE_new(0, 0, 0, 1);
    MATRIX_Matrix* m = MATRIX_new_translation(5, 0, 0);
    SPHERE_set_transform(s, m);
    MATRIX_delete(m);
    RAY_Ray* r = RAY_new(0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = SPHERE_intersect(s, r);
    TEST_ASSERT_NOT_NULL(intersections);
    TEST_ASSERT_EQUAL(0, intersections->count);
    SPHERE_delete(s);
    RAY_delete_intersections(intersections);
    RAY_delete(r);
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
    RUN_TEST(test_sphere_default_transformation);
    RUN_TEST(test_sphere_modifying_transformation);
    RUN_TEST(test_sphere_intersect_scaled_ray);
    RUN_TEST(test_sphere_intersect_translated_ray);
    return UNITY_END();
}
