#include <unity.h>
#include <sphere.h>
#include <intersections.h>
#include "tuples.h"
#include "ray.h"

void setUp() {}
void tearDown() {}

void test_precompute_the_state_of_an_intersection() {
    TUPLES_Point expected_point;
    TUPLES_init_point(&expected_point, 0, 0, -1);
    TUPLES_Vector expected_eyev;
    TUPLES_init_vector(&expected_eyev, 0, 0, -1);
    TUPLES_Vector expected_normalv;
    TUPLES_init_vector(&expected_normalv, 0, 0, -1);

    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    SPHERE_Sphere* s = SPHERE_new();
    RAY_Xs i;
    i.t = 4.0;
    i.object = s;
    INTERSECTION_Intersection* comps = INTERSECTION_prepare_computations(&i, ray);
    TEST_ASSERT_EQUAL_DOUBLE(i.t, comps->t);
    TEST_ASSERT_EQUAL_PTR(s, comps->object);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_point, &comps->point));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_eyev, &comps->eyev));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_normalv, &comps->normalv));
    INTERSECTION_delete(comps);
    SPHERE_delete(s);
    RAY_delete(ray);
}

void test_hit_when_intersection_occurs_on_outside() {
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    SPHERE_Sphere* s = SPHERE_new();
    RAY_Xs i;
    i.t = 4.0;
    i.object = s;
    INTERSECTION_Intersection* comps = INTERSECTION_prepare_computations(&i, ray);
    TEST_ASSERT_FALSE(comps->inside);
    INTERSECTION_delete(comps);
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
    RAY_Xs i;
    i.t = 1.0;
    i.object = s;
    INTERSECTION_Intersection* comps = INTERSECTION_prepare_computations(&i, ray);
    TEST_ASSERT_TRUE(comps->inside);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_point, &comps->point));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_eyev, &comps->eyev));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_normalv, &comps->normalv));
    INTERSECTION_delete(comps);
    SPHERE_delete(s);
    RAY_delete(ray);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_precompute_the_state_of_an_intersection);
    RUN_TEST(test_hit_when_intersection_occurs_on_outside);
    RUN_TEST(test_hit_when_intersection_occurs_on_inside);
    return UNITY_END();
}
