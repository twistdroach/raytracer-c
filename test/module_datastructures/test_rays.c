#include <unity.h>
#include <ray.h>
#include <sphere.h>
#include <matrix.h>
#include "shapeholder.h"

void setUp() {}
void tearDown() {}

void test_create_and_query_ray() {
    RAY_Ray* ray = RAY_new(1, 2, 3, 4, 5, 6);
    TEST_ASSERT_EQUAL_DOUBLE(2, ray->origin.y);
    TEST_ASSERT_EQUAL_DOUBLE(6, ray->direction.z);
    RAY_delete(ray);
}

void test_create_and_query_from_tuples() {
    TUPLES_Point* origin = TUPLES_new_point(1, 2, 3);
    TUPLES_Vector* direction = TUPLES_new_vector(4, 5, 6);
    RAY_Ray* ray = RAY_new_from_tuples(origin, direction);
    TEST_ASSERT_EQUAL_DOUBLE(2, ray->origin.y);
    TEST_ASSERT_EQUAL_DOUBLE(6, ray->direction.z);
    RAY_delete(ray);
    TUPLES_delete_all(origin, direction);
}

void test_create_and_query_ray_on_stack() {
    RAY_Ray ray;
    RAY_init(&ray, 1, 2, 3, 4, 5, 6);
    TEST_ASSERT_EQUAL_DOUBLE(2, ray.origin.y);
    TEST_ASSERT_EQUAL_DOUBLE(6, ray.direction.z);
    RAY_destroy(&ray);
}

void test_compute_point_from_a_distance() {
    RAY_Ray* ray = RAY_new(2, 3, 4, 1, 0, 0);
    TUPLES_Point p;
    RAY_position(&p, ray, 0);
    TEST_ASSERT_EQUAL_DOUBLE(2, p.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p));

    RAY_position(&p, ray, 1);
    TEST_ASSERT_EQUAL_DOUBLE(3, p.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p));

    RAY_position(&p, ray, -1);
    TEST_ASSERT_EQUAL_DOUBLE(1, p.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p));

    RAY_position(&p, ray, 2.5);
    TEST_ASSERT_EQUAL_DOUBLE(4.5, p.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p));

    TUPLES_destroy(&p);
    RAY_delete(ray);
}

void test_intersection() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_add_intersection(intersections, 4.0, &s);
    RAY_add_intersection(intersections, 6.0, &s);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL_DOUBLE(4, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(6, intersections->xs[1].t);
    RAY_delete_intersections(intersections);
    SPHERE_destroy(&s);
}

void test_find_hit_with_all_positive_ts() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_add_intersection(intersections, 1, &s);
    RAY_add_intersection(intersections, 2, &s);
    RAY_Xs* hit = RAY_hit(intersections);
    TEST_ASSERT_NOT_NULL(hit);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, hit->t);
    RAY_delete_intersections(intersections);
    SPHERE_destroy(&s);
}

void test_find_hit_with_a_negative_t() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_add_intersection(intersections, -1, &s);
    RAY_add_intersection(intersections, 1, &s);
    RAY_Xs* hit = RAY_hit(intersections);
    TEST_ASSERT_NOT_NULL(hit);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, hit->t);
    RAY_delete_intersections(intersections);
    SPHERE_destroy(&s);
}

void test_find_hit_with_negative_ts() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_add_intersection(intersections, -1, &s);
    RAY_add_intersection(intersections, -2, &s);
    RAY_Xs* hit = RAY_hit(intersections);
    TEST_ASSERT_NULL(hit);
    RAY_delete_intersections(intersections);
    SPHERE_destroy(&s);
}

void test_find_hit_with_lowest_nonnegative_t() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_add_intersection(intersections, 5, &s);
    RAY_add_intersection(intersections, 7, &s);
    RAY_add_intersection(intersections, -3, &s);
    RAY_add_intersection(intersections, 2, &s);
    RAY_sort_intersections(intersections);
    RAY_Xs* hit = RAY_hit(intersections);
    TEST_ASSERT_NOT_NULL(hit);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, hit->t);
    RAY_delete_intersections(intersections);
    SPHERE_destroy(&s);
}

void test_translate_ray() {
    RAY_Ray* orig = RAY_new(1, 2, 3, 0, 1, 0);
    RAY_Ray res;
    MATRIX_Matrix* m = MATRIX_new_translation(3, 4, 5);
    RAY_transform(&res, orig, m);
    TEST_ASSERT_EQUAL_DOUBLE(4, res.origin.x);
    TEST_ASSERT_EQUAL_DOUBLE(6, res.origin.y);
    TEST_ASSERT_EQUAL_DOUBLE(8, res.origin.z);
    TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.x);
    TEST_ASSERT_EQUAL_DOUBLE(1, res.direction.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.z);
    MATRIX_delete(m);
    RAY_delete(orig);
    RAY_destroy(&res);
}

void test_scale_ray() {
    RAY_Ray* orig = RAY_new(1, 2, 3, 0, 1, 0);
    RAY_Ray res;
    MATRIX_Matrix* m = MATRIX_new_scaling(2, 3, 4);
    RAY_transform(&res, orig, m);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.origin.x);
    TEST_ASSERT_EQUAL_DOUBLE(6, res.origin.y);
    TEST_ASSERT_EQUAL_DOUBLE(12, res.origin.z);
    TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.direction.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.z);
    MATRIX_delete(m);
    RAY_delete(orig);
    RAY_destroy(&res);
}

void test_precompute_the_state_of_an_intersection() {
    TUPLES_Point expected_point;
    TUPLES_init_point(&expected_point, 0, 0, -1);
    TUPLES_Vector expected_eyev;
    TUPLES_init_vector(&expected_eyev, 0, 0, -1);
    TUPLES_Vector expected_normalv;
    TUPLES_init_vector(&expected_normalv, 0, 0, -1);

    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    SPHERE_Sphere* s = SPHERE_new();
    SHAPEHOLDER_Shapeholder h;
    SHAPEHOLDER_init(&h, s, SHAPEHOLDER_SPHERE);
    RAY_Xs i;
    i.t = 4.0;
    i.object = &h;
    RAY_Computations* comps = RAY_prepare_computations(&i, ray);
    TEST_ASSERT_EQUAL_DOUBLE(i.t, comps->t);
    TEST_ASSERT_EQUAL_PTR(s, comps->object->shape);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_point, &comps->point));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_eyev, &comps->eyev));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_normalv, &comps->normalv));
    RAY_delete_computations(comps);
    SPHERE_delete(s);
    RAY_delete(ray);
}

void test_hit_when_intersection_occurs_on_outside() {
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    SPHERE_Sphere* s = SPHERE_new();
    SHAPEHOLDER_Shapeholder h;
    SHAPEHOLDER_init(&h, s, SHAPEHOLDER_SPHERE);
    RAY_Xs i;
    i.t = 4.0;
    i.object = &h;
    RAY_Computations* comps = RAY_prepare_computations(&i, ray);
    TEST_ASSERT_FALSE(comps->inside);
    RAY_delete_computations(comps);
    SPHERE_delete(s);
    RAY_delete(ray);
}

void test_hit_when_intersection_occurs_on_inside() {
    TUPLES_Point expected_point;
    TUPLES_init_point(&expected_point, 0, 0, 1);
    TUPLES_Vector expected_eyev;
    TUPLES_init_vector(&expected_eyev, 0, 0, -1);
    TUPLES_Vector expected_normalv;
    TUPLES_init_vector(&expected_normalv, 0, 0, -1);

    RAY_Ray* ray = RAY_new(0, 0, 0, 0, 0, 1);
    SPHERE_Sphere* s = SPHERE_new();
    SHAPEHOLDER_Shapeholder h;
    SHAPEHOLDER_init(&h, s, SHAPEHOLDER_SPHERE);
    RAY_Xs i;
    i.t = 1.0;
    i.object = &h;
    RAY_Computations* comps = RAY_prepare_computations(&i, ray);
    TEST_ASSERT_TRUE(comps->inside);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_point, &comps->point));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_eyev, &comps->eyev));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_normalv, &comps->normalv));
    RAY_delete_computations(comps);
    SPHERE_delete(s);
    RAY_delete(ray);
}

void test_hit_should_offset_the_point() {
    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 0, 1);

    SPHERE_Sphere* sphere = SPHERE_new();
    MATRIX_Matrix* transform = MATRIX_new_translation(0, 0, 1);
    SHAPE_set_transform((SHAPE_Shape*)sphere, transform);
    MATRIX_delete(transform);

    SHAPEHOLDER_Shapeholder h;
    SHAPEHOLDER_init(&h, sphere, SHAPEHOLDER_SPHERE);
    RAY_Xs i;
    i.t = 5.0;
    i.object = &h;

    RAY_Computations* comps = RAY_prepare_computations(&i, &ray);
    TEST_ASSERT_TRUE(comps->over_point.z < -EPSILON/2);
    TEST_ASSERT_TRUE(comps->point.z > comps->over_point.z);
    RAY_delete_computations(comps);
    SPHERE_delete(sphere);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_and_query_ray);
    RUN_TEST(test_create_and_query_ray_on_stack);
    RUN_TEST(test_compute_point_from_a_distance);
    RUN_TEST(test_intersection);
    RUN_TEST(test_find_hit_with_all_positive_ts);
    RUN_TEST(test_find_hit_with_a_negative_t);
    RUN_TEST(test_find_hit_with_negative_ts);
    RUN_TEST(test_find_hit_with_lowest_nonnegative_t);
    RUN_TEST(test_translate_ray);
    RUN_TEST(test_scale_ray);
    RUN_TEST(test_create_and_query_from_tuples);
    RUN_TEST(test_precompute_the_state_of_an_intersection);
    RUN_TEST(test_hit_when_intersection_occurs_on_outside);
    RUN_TEST(test_hit_when_intersection_occurs_on_inside);
    RUN_TEST(test_hit_should_offset_the_point);
    return UNITY_END();
}
