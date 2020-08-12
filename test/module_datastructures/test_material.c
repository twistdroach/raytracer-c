
#include "material.h"
#include "lights.h"
#include <unity.h>

void setUp() {}
void tearDown() {}

void test_default_material() {
    MATERIAL_Material* m = MATERIAL_new();
    TUPLES_Color* c = TUPLES_new_color(1, 1, 1);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, &m->color));
    TEST_ASSERT_EQUAL_DOUBLE(0.1, m->ambient);
    TEST_ASSERT_EQUAL_DOUBLE(0.9, m->diffuse);
    TEST_ASSERT_EQUAL_DOUBLE(0.9, m->specular);
    TEST_ASSERT_EQUAL_DOUBLE(200.0, m->shininess);
    TUPLES_delete(c);
    MATERIAL_delete(m);
}

void test_lighting_with_eye_between_light_and_surface() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Color* result = MATERIAL_lighting(material, pl, position, eyev, normalv);

    TUPLES_Color* expected_result = TUPLES_new_color(1.9, 1.9, 1.9);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, result, expected_result);
}

void test_lighting_with_eye_between_light_and_surface_eye_offset_45_deg() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, sqrt(2.0)/2.0, sqrt(2.0)/2.0);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Color* result = MATERIAL_lighting(material, pl, position, eyev, normalv);

    TUPLES_Color* expected_result = TUPLES_new_color(1.0, 1.0, 1.0);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, result, expected_result);
}

void test_lighting_with_eye_opposite_surface_light_offset_45_deg() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 10, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Color* result = MATERIAL_lighting(material, pl, position, eyev, normalv);

    TUPLES_Color* expected_result = TUPLES_new_color(0.7364, 0.7364, 0.7364);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, result, expected_result);
}

void test_lighting_with_eye_in_path_of_reflection_vector() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, -sqrt(2.0)/2.0, -sqrt(2.0)/2.0);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 10, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Color* result = MATERIAL_lighting(material, pl, position, eyev, normalv);

    TUPLES_Color* expected_result = TUPLES_new_color(1.6364, 1.6364, 1.6364);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, result, expected_result);
}

void test_lighting_with_light_behind_surface() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, 10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Color* result = MATERIAL_lighting(material, pl, position, eyev, normalv);

    TUPLES_Color* expected_result = TUPLES_new_color(0.1, 0.1, 0.1);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, result, expected_result);
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_material);
    RUN_TEST(test_lighting_with_eye_between_light_and_surface);
    RUN_TEST(test_lighting_with_eye_between_light_and_surface_eye_offset_45_deg);
    RUN_TEST(test_lighting_with_eye_opposite_surface_light_offset_45_deg);
    RUN_TEST(test_lighting_with_eye_in_path_of_reflection_vector);
    RUN_TEST(test_lighting_with_light_behind_surface);
    return UNITY_END();
}
