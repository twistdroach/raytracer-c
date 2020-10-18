#include <unity.h>
#include <tuples.h>
#include <lights.h>

#include "test_world_utils.h"

void setUp() {}
void tearDown() {}

void test_pointlight_has_position_and_intensity() {
    TUPLES_Color* c = TUPLES_new_color(1, 1, 1);
    TUPLES_Point* p = TUPLES_new_point(0, 0, 0);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, &pl->intensity));
    TEST_ASSERT_TRUE(TUPLES_is_equal(p, &pl->position));
    TUPLES_delete_all(c, p);
    LIGHTS_delete_pointlight(pl);
}

void test_light_copy() {
    TUPLES_Color* c = TUPLES_new_color(0.1, 0.2, 0.3);
    TUPLES_Point* p = TUPLES_new_point(0.4, 0.5, 0.6);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    LIGHTS_PointLight pl_copy;
    LIGHTS_copy(&pl_copy, pl);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, &pl_copy.intensity));
    TEST_ASSERT_TRUE(TUPLES_is_equal(p, &pl_copy.position));
    TUPLES_delete_all(c, p);
    LIGHTS_delete_pointlight(pl);
}

void test_get_color() {
    TUPLES_Color* c = TUPLES_new_color(0.1, 0.2, 0.3);
    TUPLES_Point* p = TUPLES_new_point(0.4, 0.5, 0.6);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    const TUPLES_Color* c_returned = LIGHTS_get_color(pl);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, c_returned));
    TUPLES_delete_all(c, p);
    LIGHTS_delete_pointlight(pl);
}

void point_lights_evaluate_intensity_at_single_point(double x, double y, double z, double expected_result) {
    WORLD_World* world = construct_test_world();
    const LIGHTS_PointLight* light = WORLD_get_light(world);
    TUPLES_Point p;
    TUPLES_init_point(&p, x, y, z);

    double result = light->intensity_at(light, &p, world);
    if (!double_equal(expected_result, result)) {
        printf("Failed for %f, %f, %f expected %f but got %f\n", x, y, z, expected_result, result);
        TEST_FAIL();
    }
    destruct_test_world(world);
}

void test_point_lights_evaluate_intensity_at_single_point() {
    point_lights_evaluate_intensity_at_single_point(0, 1.0001, 0, 1.0);
    point_lights_evaluate_intensity_at_single_point(-1.0001, 0, 0, 1.0);
    point_lights_evaluate_intensity_at_single_point(0, 0, -1.0001, 1.0);
    point_lights_evaluate_intensity_at_single_point(0, 0, 1.0001, 0.0);
    point_lights_evaluate_intensity_at_single_point(1.0001, 0, 0, 0.0);
    point_lights_evaluate_intensity_at_single_point(0, -1.0001, 0, 0.0);
    point_lights_evaluate_intensity_at_single_point(0, 0, 0, 0.0);
}

void test_create_area_light() {
    TUPLES_Point corner;
    TUPLES_init_point(&corner, 0, 0, 0);
    TUPLES_Vector v1, v2;
    TUPLES_init_vector(&v1, 2, 0, 0);
    TUPLES_init_vector(&v2, 0, 0, 1);
    TUPLES_Color color;
    TUPLES_init_color(&color, 1, 1, 1);
    LIGHTS_AreaLight* light = LIGHTS_new_arealight(&corner, &v1, 4, &v2, 2, &color);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&corner, &light->corner));
    TUPLES_Vector expected;
    TUPLES_init_vector(&expected, 0.5, 0, 0);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&light->uvec, &expected));
    TUPLES_init_vector(&expected, 0, 0, 0.5);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&light->vvec, &expected));
    TEST_ASSERT_EQUAL_UINT(4, light->usteps);
    TEST_ASSERT_EQUAL_UINT(2, light->vsteps);
    TEST_ASSERT_EQUAL_UINT(8, light->samples);
    TUPLES_Point expected_p;
    TUPLES_init_point(&expected_p, 1, 0, 0.5);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&light->position, &expected_p));

    LIGHTS_delete_arealight(light);
}

void find_single_point_on_an_area_light(unsigned int u,
                                        unsigned int v,
                                        double x,
                                        double y,
                                        double z) {
    TUPLES_Point corner;
    TUPLES_init_point(&corner, 0, 0, 0);
    TUPLES_Vector v1, v2;
    TUPLES_init_vector(&v1, 2, 0, 0);
    TUPLES_init_vector(&v2, 0, 0, 1);
    TUPLES_Color color;
    TUPLES_init_color(&color, 1, 1, 1);
    LIGHTS_AreaLight* light = LIGHTS_new_arealight(&corner, &v1, 4, &v2, 2, &color);
    TUPLES_Point result, expected_result;
    LIGHTS_point_on_area_light(&result, light, u, v);
    TUPLES_init_point(&expected_result, x, y, z);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_result, &result));
    LIGHTS_delete_arealight(light);
}

void test_find_single_point_on_an_area_light() {
    find_single_point_on_an_area_light(0, 0, 0.25, 0, 0.25);
    find_single_point_on_an_area_light(1, 0, 0.75, 0, 0.25);
    find_single_point_on_an_area_light(0, 1, 0.25, 0, 0.75);
    find_single_point_on_an_area_light(2, 0, 1.25, 0, 0.25);
    find_single_point_on_an_area_light(3, 1, 1.75, 0, 0.75);
}

void area_light_intensity_function(double x, double y,
                                   double z, double expected) {
    WORLD_World* world = construct_test_world();
    TUPLES_Point corner;
    TUPLES_init_point(&corner, -0.5, -0.5, -5);
    TUPLES_Vector v1, v2;
    TUPLES_init_vector(&v1, 1, 0, 0);
    TUPLES_init_vector(&v2, 0, 1, 0);
    TUPLES_Color color;
    TUPLES_init_color(&color, 1, 1, 1);
    LIGHTS_AreaLight* light = LIGHTS_new_arealight(&corner, &v1, 2, &v2, 2, &color);

    TUPLES_Point point;
    TUPLES_init_point(&point, x, y ,z);
    double result = light->intensity_at((LIGHTS_Light*)light, &point, world);
    TEST_ASSERT_EQUAL_DOUBLE(expected, result);
    destruct_test_world(world);
    LIGHTS_delete_arealight(light);
}

void test_area_light_intensity_function() {
    area_light_intensity_function(0, 0, 2, 0);
    area_light_intensity_function(1, -1, 2, 0.25);
    area_light_intensity_function(1.5, 0, 2, 0.5);
    area_light_intensity_function(1.25, 1.25, 3, 0.75);
    area_light_intensity_function(0, 0, -2, 1);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pointlight_has_position_and_intensity);
    RUN_TEST(test_light_copy);
    RUN_TEST(test_get_color);
    RUN_TEST(test_point_lights_evaluate_intensity_at_single_point);
    RUN_TEST(test_create_area_light);
    RUN_TEST(test_find_single_point_on_an_area_light);
    RUN_TEST(test_area_light_intensity_function);
    return UNITY_END();
}
