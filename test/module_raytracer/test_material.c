#include "lights.h"
#include "material.h"
#include "pattern.h"
#include "test_world_utils.h"
#include "../testutils.h"
#include <sphere.h>
#include <unity.h>

SPHERE_Sphere *sphere;
void setUp() { sphere = SPHERE_new(); }
void tearDown() { SPHERE_delete(sphere); }

void test_default_material() {
  MATERIAL_Material *m = MATERIAL_new();
  TUPLES_Color *c = TUPLES_new_color(1, 1, 1);
  TEST_ASSERT_TRUE(TUPLES_is_equal(c, &m->color));
  TEST_ASSERT_EQUAL_DOUBLE(0.1, m->ambient);
  TEST_ASSERT_EQUAL_DOUBLE(0.9, m->diffuse);
  TEST_ASSERT_EQUAL_DOUBLE(0.9, m->specular);
  TEST_ASSERT_EQUAL_DOUBLE(200.0, m->shininess);
  TUPLES_delete(c);
  MATERIAL_delete(m);
}

void test_lighting_with_eye_between_light_and_surface_in_shadow() {
  MATERIAL_Material *material = MATERIAL_new();
  TUPLES_Point *position = TUPLES_new_point(0, 0, 0);
  TUPLES_Vector *eyev = TUPLES_new_vector(0, 0, -1);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 0, -10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  SPHERE_set_material(sphere, material);
  TUPLES_Color result;
  MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, 0.0);

  TUPLES_Color *expected_result = TUPLES_new_color(0.1, 0.1, 0.1);
  test_tuples(expected_result, &result);
  LIGHTS_delete(pl);
  MATERIAL_delete(material);
  TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_between_light_and_surface() {
  MATERIAL_Material *material = MATERIAL_new();
  SPHERE_set_material(sphere, material);
  TUPLES_Point *position = TUPLES_new_point(0, 0, 0);
  TUPLES_Vector *eyev = TUPLES_new_vector(0, 0, -1);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 0, -10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  TUPLES_Color result;
  MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, 1.0);

  TUPLES_Color *expected_result = TUPLES_new_color(1.9, 1.9, 1.9);
  test_tuples(expected_result, &result);
  LIGHTS_delete(pl);
  MATERIAL_delete(material);
  TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_between_light_and_surface_eye_offset_45_deg() {
  MATERIAL_Material *material = MATERIAL_new();
  TUPLES_Point *position = TUPLES_new_point(0, 0, 0);
  TUPLES_Vector *eyev = TUPLES_new_vector(0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 0, -10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  SPHERE_set_material(sphere, material);
  TUPLES_Color result;
  MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, 1.0);

  TUPLES_Color *expected_result = TUPLES_new_color(1.0, 1.0, 1.0);
  test_tuples(expected_result, &result);
  LIGHTS_delete(pl);
  MATERIAL_delete(material);
  TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_opposite_surface_light_offset_45_deg() {
  MATERIAL_Material *material = MATERIAL_new();
  TUPLES_Point *position = TUPLES_new_point(0, 0, 0);
  TUPLES_Vector *eyev = TUPLES_new_vector(0, 0, -1);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 10, -10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  SPHERE_set_material(sphere, material);
  TUPLES_Color result;
  MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, 1.0);

  TUPLES_Color *expected_result = TUPLES_new_color(0.7364, 0.7364, 0.7364);
  test_tuples(expected_result, &result);
  LIGHTS_delete(pl);
  MATERIAL_delete(material);
  TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_eye_in_path_of_reflection_vector() {
  MATERIAL_Material *material = MATERIAL_new();
  TUPLES_Point *position = TUPLES_new_point(0, 0, 0);
  TUPLES_Vector *eyev = TUPLES_new_vector(0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 10, -10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  SPHERE_set_material(sphere, material);
  TUPLES_Color result;
  MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, 1.0);

  TUPLES_Color *expected_result = TUPLES_new_color(1.6364, 1.6364, 1.6364);
  test_tuples(expected_result, &result);
  LIGHTS_delete(pl);
  MATERIAL_delete(material);
  TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_light_behind_surface() {
  MATERIAL_Material *material = MATERIAL_new();
  TUPLES_Point *position = TUPLES_new_point(0, 0, 0);
  TUPLES_Vector *eyev = TUPLES_new_vector(0, 0, -1);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 0, 10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  SPHERE_set_material(sphere, material);
  TUPLES_Color result;
  MATERIAL_lighting(&result, sphere, pl, position, eyev, normalv, 1.0);

  TUPLES_Color *expected_result = TUPLES_new_color(0.1, 0.1, 0.1);
  TEST_ASSERT_TRUE(TUPLES_is_equal(expected_result, &result));
  LIGHTS_delete(pl);
  MATERIAL_delete(material);
  TUPLES_delete_all(position, eyev, normalv, light_position, light_color, expected_result);
}

void test_lighting_with_a_pattern() {
  MATERIAL_Material *material = MATERIAL_new();
  TUPLES_Color *white = TUPLES_new_color(1, 1, 1);
  TUPLES_Color *black = TUPLES_new_color(0, 0, 0);
  PATTERN_Pattern *pattern = PATTERN_new_stripe(white, black);
  MATERIAL_set_pattern(material, pattern);
  material->ambient = 1;
  material->diffuse = 0;
  material->specular = 0;
  TUPLES_Vector *eyev = TUPLES_new_vector(0, 0, -1);
  TUPLES_Vector *normalv = TUPLES_new_vector(0, 0, -1);

  TUPLES_Point *light_position = TUPLES_new_point(0, 0, 10);
  TUPLES_Color *light_color = TUPLES_new_color(1, 1, 1);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(light_position, light_color);

  TUPLES_Point *p1 = TUPLES_new_point(0.9, 0, 0);
  TUPLES_Color c1;
  SPHERE_set_material(sphere, material);
  MATERIAL_lighting(&c1, sphere, pl, p1, eyev, normalv, 1.0);
  TEST_ASSERT_TRUE(TUPLES_is_equal(white, &c1));

  TUPLES_Point *p2 = TUPLES_new_point(1.1, 0, 0);
  TUPLES_Color c2;
  MATERIAL_lighting(&c2, sphere, pl, p2, eyev, normalv, 1.0);
  TEST_ASSERT_TRUE(TUPLES_is_equal(black, &c2));

  TUPLES_delete_all(white, black, eyev, normalv, light_color, light_position, p1, p2);
  PATTERN_delete(pattern);
  MATERIAL_delete(material);
  LIGHTS_delete(pl);
}

void test_material_default_reflective_property() {
  MATERIAL_Material *mat = MATERIAL_new();
  TEST_ASSERT_EQUAL_DOUBLE(0.0, mat->reflective);
  MATERIAL_delete(mat);
}

void test_material_default_transparency_and_refractive_index() {
  MATERIAL_Material *mat = MATERIAL_new();
  TEST_ASSERT_EQUAL_DOUBLE(0.0, mat->transparency);
  TEST_ASSERT_EQUAL_DOUBLE(1.0, mat->refractive_index);
  MATERIAL_delete(mat);
}

void use_light_intensity_to_attenuate_color(double intensity, double r, double g, double b) {
  WORLD_World *world = construct_test_world();
  TUPLES_Point lp;
  TUPLES_init_point(&lp, 0, 0, -10);
  TUPLES_Color lc;
  TUPLES_init_color(&lc, 1, 1, 1);
  LIGHTS_Light *light = LIGHTS_new_pointlight(&lp, &lc);
  WORLD_set_light(world, light);

  SHAPE_Shape *shape = WORLD_get_object(world, 0);
  MATERIAL_Material *m = SHAPE_get_material(shape);
  m->ambient = 0.1;
  m->diffuse = 0.9;
  m->specular = 0.0;
  TUPLES_init_color(&m->color, 1, 1, 1);

  TUPLES_Point pt;
  TUPLES_init_point(&pt, 0, 0, -1);

  TUPLES_Vector eyev, normalv;
  TUPLES_init_vector(&eyev, 0, 0, -1);
  TUPLES_init_vector(&normalv, 0, 0, -1);

  TUPLES_Color result, expected_result;
  TUPLES_init_color(&expected_result, r, g, b);
  MATERIAL_lighting(&result, shape, light, &pt, &eyev, &normalv, intensity);

  test_tuples(&expected_result, &result);
  LIGHTS_delete(light);
  destruct_test_world(world);
}

void test_use_light_intensity_to_attenuate_color() {
  use_light_intensity_to_attenuate_color(1.0, 1, 1, 1);
  use_light_intensity_to_attenuate_color(0.5, 0.55, 0.55, 0.55);
  use_light_intensity_to_attenuate_color(0.0, 0.1, 0.1, 0.1);
}

void lighting_function_samples_area_light(double x, double y, double z, double r, double g, double b) {
  TUPLES_Point corner;
  TUPLES_init_point(&corner, -0.5, -0.5, -5);
  TUPLES_Vector v1, v2;
  TUPLES_init_vector(&v1, 1, 0, 0);
  TUPLES_init_vector(&v2, 0, 1, 0);
  TUPLES_Color light_color;
  TUPLES_init_color(&light_color, 1, 1, 1);
  LIGHTS_Light *light = LIGHTS_new_arealight(&corner, &v1, 2, &v2, 2, &light_color);
  SPHERE_Sphere *sphere = SPHERE_new();
  MATERIAL_Material *sphere_mat = SPHERE_get_material(sphere);
  sphere_mat->ambient = 0.1;
  sphere_mat->diffuse = 0.9;
  sphere_mat->specular = 0;
  TUPLES_copy(&sphere_mat->color, &light_color);
  TUPLES_Point eye;
  TUPLES_init_point(&eye, 0, 0, -5);
  TUPLES_Point pt;
  TUPLES_init_point(&pt, x, y, z);
  TUPLES_Vector eyev;
  TUPLES_subtract(&eyev, &eye, &pt);
  TUPLES_normalize(&eyev);
  TUPLES_Vector normal_v;
  TUPLES_init_vector(&normal_v, x, y, z);
  TUPLES_Color result, expected_result;
  TUPLES_init_color(&expected_result, r, g, b);
  MATERIAL_lighting(&result, (SHAPE_Shape *)sphere, light, &pt, &eyev, &normal_v, 1.0);
  test_tuples(&expected_result, &result);
  SPHERE_delete(sphere);
  LIGHTS_delete(light);
}

void test_lighting_function_samples_area_light() {
  lighting_function_samples_area_light(0, 0, -1, 0.9965, 0.9965, 0.9965);
  lighting_function_samples_area_light(0, 0.7071, -0.7071, 0.6232, 0.6232, 0.6232);
}

int main(void) {
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
  RUN_TEST(test_material_default_transparency_and_refractive_index);
  RUN_TEST(test_use_light_intensity_to_attenuate_color);
  RUN_TEST(test_lighting_function_samples_area_light);
  return UNITY_END();
}
