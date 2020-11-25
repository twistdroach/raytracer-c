#include <lights.h>
#include <sequences.h>
#include <tuples.h>
#include <unity.h>

#include "test_world_utils.h"
#include "../testutils.h"

void setUp() {}
void tearDown() {}

void test_pointlight_has_position_and_intensity() {
  TUPLES_Color *c = TUPLES_new_color(1, 1, 1);
  TUPLES_Point *p = TUPLES_new_point(0, 0, 0);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(p, c);
  TEST_ASSERT_TRUE(TUPLES_is_equal(c, &pl->intensity));
  TEST_ASSERT_TRUE(TUPLES_is_equal(p, &pl->corner));
  TUPLES_delete_all(c, p);
  LIGHTS_delete(pl);
}

void test_get_color() {
  TUPLES_Color *c = TUPLES_new_color(0.1, 0.2, 0.3);
  TUPLES_Point *p = TUPLES_new_point(0.4, 0.5, 0.6);
  LIGHTS_Light *pl = LIGHTS_new_pointlight(p, c);
  const TUPLES_Color *c_returned = LIGHTS_get_color(pl);
  TEST_ASSERT_TRUE(TUPLES_is_equal(c, c_returned));
  TUPLES_delete_all(c, p);
  LIGHTS_delete(pl);
}

void point_lights_evaluate_intensity_at_single_point(double x, double y, double z, double expected_result) {
  WORLD_World *world = construct_test_world();
  const LIGHTS_Light *light = WORLD_get_light(world);
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);

  double result = LIGHTS_intensity_at(light, &p, world);
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
  LIGHTS_Light *light = LIGHTS_new_arealight(&corner, &v1, 4, &v2, 2, &color);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&corner, &light->corner));
  TUPLES_Vector expected;
  TUPLES_init_vector(&expected, 0.5, 0, 0);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&light->uvec, &expected));
  TUPLES_init_vector(&expected, 0, 0, 0.5);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&light->vvec, &expected));
  TEST_ASSERT_EQUAL_UINT(4, light->usteps);
  TEST_ASSERT_EQUAL_UINT(2, light->vsteps);
  TEST_ASSERT_EQUAL_UINT(8, light->samples);
  LIGHTS_delete(light);
}

void find_single_point_on_an_area_light(unsigned int u, unsigned int v, double x, double y, double z) {
  TUPLES_Point corner;
  TUPLES_init_point(&corner, 0, 0, 0);
  TUPLES_Vector v1, v2;
  TUPLES_init_vector(&v1, 2, 0, 0);
  TUPLES_init_vector(&v2, 0, 0, 1);
  TUPLES_Color color;
  TUPLES_init_color(&color, 1, 1, 1);
  LIGHTS_Light *light = LIGHTS_new_arealight(&corner, &v1, 4, &v2, 2, &color);
  TUPLES_Point result, expected_result;
  LIGHTS_point_on_area_light(&result, light, u, v);
  TUPLES_init_point(&expected_result, x, y, z);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_result, &result));
  LIGHTS_delete(light);
}

void test_find_single_point_on_an_area_light() {
  find_single_point_on_an_area_light(0, 0, 0.25, 0, 0.25);
  find_single_point_on_an_area_light(1, 0, 0.75, 0, 0.25);
  find_single_point_on_an_area_light(0, 1, 0.25, 0, 0.75);
  find_single_point_on_an_area_light(2, 0, 1.25, 0, 0.25);
  find_single_point_on_an_area_light(3, 1, 1.75, 0, 0.75);
}

void area_light_intensity_function(double x, double y, double z, double expected) {
  WORLD_World *world = construct_test_world();
  TUPLES_Point corner;
  TUPLES_init_point(&corner, -0.5, -0.5, -5);
  TUPLES_Vector v1, v2;
  TUPLES_init_vector(&v1, 1, 0, 0);
  TUPLES_init_vector(&v2, 0, 1, 0);
  TUPLES_Color color;
  TUPLES_init_color(&color, 1, 1, 1);
  LIGHTS_Light *light = LIGHTS_new_arealight(&corner, &v1, 2, &v2, 2, &color);

  TUPLES_Point point;
  TUPLES_init_point(&point, x, y, z);
  double result = LIGHTS_intensity_at(light, &point, world);
  TEST_ASSERT_EQUAL_DOUBLE(expected, result);
  destruct_test_world(world);
  LIGHTS_delete(light);
}

void test_area_light_intensity_function() {
  area_light_intensity_function(0, 0, 2, 0);
  area_light_intensity_function(1, -1, 2, 0.25);
  area_light_intensity_function(1.5, 0, 2, 0.5);
  area_light_intensity_function(1.25, 1.25, 3, 0.75);
  area_light_intensity_function(0, 0, -2, 1);
}

void find_single_point_on_a_jittered_area(unsigned int u, unsigned int v, double x, double y, double z) {
  TUPLES_Point corner;
  TUPLES_init_point(&corner, 0, 0, 0);
  TUPLES_Vector v1, v2;
  TUPLES_init_vector(&v1, 2, 0, 0);
  TUPLES_init_vector(&v2, 0, 0, 1);
  TUPLES_Color color;
  TUPLES_init_color(&color, 1, 1, 1);
  LIGHTS_Light *light = LIGHTS_new_arealight(&corner, &v1, 4, &v2, 2, &color);
  SEQUENCES_Sequence *seq = SEQUENCES_new(2, (double[]){0.3, 0.7});
  LIGHTS_set_jitter_on_area_light(light, seq);
  SEQUENCES_delete(seq);
  TUPLES_Point expected, result;
  TUPLES_init_point(&expected, x, y, z);
  LIGHTS_point_on_area_light(&result, light, u, v);
  test_tuples(&expected, &result);
  LIGHTS_delete(light);
}

void test_find_single_point_on_a_jittered_area() {
  find_single_point_on_a_jittered_area(0, 0, 0.15, 0, 0.35);
  find_single_point_on_a_jittered_area(1, 0, 0.65, 0, 0.35);
  find_single_point_on_a_jittered_area(0, 1, 0.15, 0, 0.85);
  find_single_point_on_a_jittered_area(2, 0, 1.15, 0, 0.35);
  find_single_point_on_a_jittered_area(3, 1, 1.65, 0, 0.85);
}

void area_light_with_jittered_samples(double x, double y, double z, double expected_intensity) {
  WORLD_World *world = construct_test_world();
  TUPLES_Point corner;
  TUPLES_init_point(&corner, -0.5, -0.5, -5);
  TUPLES_Vector v1, v2;
  TUPLES_init_vector(&v1, 1, 0, 0);
  TUPLES_init_vector(&v2, 0, 1, 0);
  TUPLES_Color color;
  TUPLES_init_color(&color, 1, 1, 1);
  LIGHTS_Light *light = LIGHTS_new_arealight(&corner, &v1, 2, &v2, 2, &color);
  SEQUENCES_Sequence *seq = SEQUENCES_new(5, (double[]){0.7, 0.3, 0.9, 0.1, 0.5});
  LIGHTS_set_jitter_on_area_light(light, seq);
  SEQUENCES_delete(seq);
  TUPLES_Point test_point;
  TUPLES_init_point(&test_point, x, y, z);
  double result = LIGHTS_intensity_at((LIGHTS_Light *)light, &test_point, world);
  TEST_ASSERT_EQUAL_DOUBLE(expected_intensity, result);
  destruct_test_world(world);
  LIGHTS_delete(light);
}

void test_area_light_with_jittered_samples() {
  area_light_with_jittered_samples(0, 0, 2, 0.0);
  area_light_with_jittered_samples(1, -1, 2, 0.5);
  area_light_with_jittered_samples(1.5, 0, 2, 0.75);
  area_light_with_jittered_samples(1.25, 1.25, 3, 0.75);
  area_light_with_jittered_samples(0, 0, -2, 1.0);
}

void test_get_point_light_from_yaml() {
  char data[] = "\n"
                "  at: [ -10, 10, -10 ] \n"
                "  intensity: [0.5, 0.25, 0.75]\n"
                "\n";

  LIGHTS_Light *result = LIGHTS_parse_light(data);
  TEST_ASSERT_NOT_NULL(result);

  TUPLES_Point p;
  TUPLES_Color i;
  TUPLES_Vector uvec, vvec;
  TUPLES_init_point(&p, -10, 10, -10);
  TUPLES_init_color(&i, 0.5, 0.25, 0.75);
  TUPLES_init_vector(&uvec, 0, 0, 0);
  TUPLES_init_vector(&vvec, 0, 0, 0);

  test_tuples_str(&p, &result->corner, "Corner");
  test_tuples_str(&i, &result->intensity, "Intensity");
  test_tuples_str(&uvec, &result->uvec, "uvec");
  test_tuples_str(&vvec, &result->vvec, "vvec");
  TEST_ASSERT_EQUAL(1, result->usteps);
  TEST_ASSERT_EQUAL(1, result->vsteps);

  LIGHTS_delete(result);
}

void test_get_area_light_from_yaml() {
  char data[] = "\n"
                "  corner: [-1, 2, 4]\n"
                "  uvec: [2, 0, 0]\n"
                "  vvec: [0, 2, 0]\n"
                "  usteps: 10\n"
                "  vsteps: 20\n"
                "  jitter: true\n"
                "  intensity: [1.5, 1.5, 1.5]\n"
                "\n";

  LIGHTS_Light *result = LIGHTS_parse_light(data);
  TEST_ASSERT_NOT_NULL(result);

  TUPLES_Point p;
  TUPLES_Color i;
  TUPLES_Vector uvec, vvec;
  TUPLES_init_point(&p, -1, 2, 4);
  TUPLES_init_color(&i, 1.5, 1.5, 1.5);
  TUPLES_init_vector(&uvec, 0.2, 0, 0);
  TUPLES_init_vector(&vvec, 0, 0.1, 0);

  test_tuples_str(&p, &result->corner, "Corner");
  test_tuples_str(&i, &result->intensity, "Intensity");
  test_tuples_str(&uvec, &result->uvec, "uvec");
  test_tuples_str(&vvec, &result->vvec, "vvec");
  TEST_ASSERT_EQUAL(10, result->usteps);
  TEST_ASSERT_EQUAL(20, result->vsteps);
  TEST_ASSERT_NOT_NULL(result->sequence);

  LIGHTS_delete(result);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_pointlight_has_position_and_intensity);
  RUN_TEST(test_get_color);
  RUN_TEST(test_point_lights_evaluate_intensity_at_single_point);
  RUN_TEST(test_create_area_light);
  RUN_TEST(test_find_single_point_on_an_area_light);
  RUN_TEST(test_area_light_intensity_function);
  RUN_TEST(test_find_single_point_on_a_jittered_area);
  RUN_TEST(test_area_light_with_jittered_samples);
  RUN_TEST(test_get_point_light_from_yaml);
  RUN_TEST(test_get_area_light_from_yaml);
  return UNITY_END();
}
