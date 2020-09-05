
#include <unity.h>
#include <sphere.h>
#include "material.h"
#include "lights.h"
#include "pattern.h"

SPHERE_Sphere* sphere;
void setUp() {
    sphere = SPHERE_new();
}
void tearDown() {
    SPHERE_delete(sphere);
}

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

void test_lighting_with_eye_between_light_and_surface_in_shadow() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    SPHERE_set_material(sphere, material);
    TUPLES_Color result;
    MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, true);

    TUPLES_Color* expected_result = TUPLES_new_color(0.1, 0.1, 0.1);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_between_light_and_surface() {
    MATERIAL_Material* material = MATERIAL_new();
    SPHERE_set_material(sphere, material);
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Color result;
    MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, false);

    TUPLES_Color* expected_result = TUPLES_new_color(1.9, 1.9, 1.9);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_between_light_and_surface_eye_offset_45_deg() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, sqrt(2.0)/2.0, sqrt(2.0)/2.0);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    SPHERE_set_material(sphere, material);
    TUPLES_Color result;
    MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, false);

    TUPLES_Color* expected_result = TUPLES_new_color(1.0, 1.0, 1.0);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_opposite_surface_light_offset_45_deg() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 10, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    SPHERE_set_material(sphere, material);
    TUPLES_Color result;
    MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, false);

    TUPLES_Color* expected_result = TUPLES_new_color(0.7364, 0.7364, 0.7364);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_in_path_of_reflection_vector() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, -sqrt(2.0)/2.0, -sqrt(2.0)/2.0);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 10, -10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    SPHERE_set_material(sphere, material);
    TUPLES_Color result;
    MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, false);

    TUPLES_Color* expected_result = TUPLES_new_color(1.6364, 1.6364, 1.6364);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_light_behind_surface() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Point* position = TUPLES_new_point(0, 0, 0);
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, 10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    SPHERE_set_material(sphere, material);
    TUPLES_Color result;
    MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, false);

    TUPLES_Color* expected_result = TUPLES_new_color(0.1, 0.1, 0.1);
    TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
    LIGHTS_delete_pointlight(pl);
    MATERIAL_delete(material);
    TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_a_pattern() {
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_Color* white = TUPLES_new_color(1, 1, 1);
    TUPLES_Color* black = TUPLES_new_color(0, 0, 0);
    PATTERN_Pattern* pattern = PATTERN_new_stripe(white, black);
    MATERIAL_set_pattern(material, pattern);
    material->ambient = 1;
    material->diffuse = 0;
    material->specular = 0;
    TUPLES_Vector* eyev = TUPLES_new_vector(0, 0, -1);
    TUPLES_Vector* normalv = TUPLES_new_vector(0, 0, -1);

    TUPLES_Point* light_position = TUPLES_new_point(0, 0, 10);
    TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(light_position, light_color);

    TUPLES_Point* p1 = TUPLES_new_point(0.9, 0, 0);
    TUPLES_Color c1;
    SPHERE_set_material(sphere, material);
    MATERIAL_lighting(&c1, sphere, pl, p1, eyev, normalv, false);
    TEST_ASSERT_TRUE(TUPLES_is_equal(white, &c1));

    TUPLES_Point* p2 = TUPLES_new_point(1.1, 0, 0);
    TUPLES_Color c2;
    MATERIAL_lighting(&c2, sphere, pl, p2, eyev, normalv, false);
    TEST_ASSERT_TRUE(TUPLES_is_equal(black, &c2));

    TUPLES_delete_all(white, black, eyev, normalv, light_color, light_position, p1, p2);
    PATTERN_delete(pattern);
    MATERIAL_delete(material);
    LIGHTS_delete_pointlight(pl);
}

void test_material_default_reflective_property() {
    MATERIAL_Material* mat = MATERIAL_new();
    TEST_ASSERT_EQUAL_DOUBLE(0.0, mat->reflective);
    MATERIAL_delete(mat);
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
    RUN_TEST(test_lighting_with_eye_between_light_and_surface_in_shadow);
    RUN_TEST(test_lighting_with_a_pattern);
    RUN_TEST(test_material_default_reflective_property);
    return UNITY_END();
}
