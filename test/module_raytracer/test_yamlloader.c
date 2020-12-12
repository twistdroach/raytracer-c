#include <unity.h>
#include "../testutils.h"

#include <yamlloader.h>

#include <camera.h>
#include <configuration.h>
#include <cube.h>
#include <matrix.h>
#include <tuples.h>

void setUp() {}
void tearDown() {}


//void YAMLLOADER_parse_map_entries(char *data, void (*process_entry)(char *key, char *value, void *context), void *context);
static int parse_map_entries;
void validate_key_value(char *key, char *value, void *context) {
  UNUSED(context);
  TEST_ASSERT_EQUAL_STRING("akey", key);
  TEST_ASSERT_EQUAL_STRING("[the value]", value);
  parse_map_entries++;
}

void test_parse_map_entries() {
  parse_map_entries = 0;
  char data[] = "\nakey:[the value]\n\n";
  YAMLLOADER_parse_map_entries(data, validate_key_value, NULL);
  TEST_ASSERT_EQUAL(1, parse_map_entries);
}

void test_parse_color() {
  TUPLES_Color result, expected;
  TUPLES_init_color(&expected, 100.5, 22.75, 4.123);
  char data[] = "[100.5, 22.75,    4.123]";
  YAMLLOADER_parse_color(data, &result);
  test_tuples(&expected, &result);
}

void test_parse_vector() {
  TUPLES_Vector result, expected;
  TUPLES_init_vector(&expected, 1.5, 2.75, 4);
  char data[] = "[1.5, 2.75,4]";
  YAMLLOADER_parse_vector(data, &result);
  test_tuples(&expected, &result);
}

void test_parse_point() {
  TUPLES_Point result, expected;
  TUPLES_init_point(&expected, .5, 7.75, 0.4);
  char data[] = "[.5, 7.75, 0.4]";
  YAMLLOADER_parse_point(data, &result);
  test_tuples(&expected, &result);
}

void test_parse_bool() {
  bool result = false;
  YAMLLOADER_parse_bool("true", &result);
  TEST_ASSERT_TRUE(result);
  YAMLLOADER_parse_bool("false", &result);
  TEST_ASSERT_FALSE(result);
}

void test_parse_uint() {
  unsigned int result = 0;
  YAMLLOADER_parse_uint("5", &result);
  TEST_ASSERT_EQUAL_UINT(5, result);
}

void test_parse_double() {
  double result = 0.0;
  YAMLLOADER_parse_double("1.543", &result);
  TEST_ASSERT_EQUAL_DOUBLE(1.543, result);
}

void test_parse_single_transform_scale_entry() {
  char data[] = "[  scale     , 3, 0.1, 2]";

  MATRIX_Matrix *expected = MATRIX_new_scaling(3, 0.1, 2);
  MATRIX_Matrix *result = YAMLLOADER_parse_transform_entry(data);
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(MATRIX_is_equal(expected, result));
  MATRIX_delete_all(expected, result);
}

void test_parse_single_transform_translate_entry() {
  char data[] = "[  translate   , 31, 0.2, 2.19]";

  MATRIX_Matrix *expected = MATRIX_new_translation(31, 0.2, 2.19);
  MATRIX_Matrix *result = YAMLLOADER_parse_transform_entry(data);
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(MATRIX_is_equal(expected, result));
  MATRIX_delete_all(expected, result);
}

void test_parse_single_transform_rotate_x_entry() {
  char data[] = "[  rotate-x, 0.5]";

  MATRIX_Matrix *expected = MATRIX_new_rotation_x(0.5);
  MATRIX_Matrix *result = YAMLLOADER_parse_transform_entry(data);
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(MATRIX_is_equal(expected, result));
  MATRIX_delete_all(expected, result);
}

void test_parse_single_transform_rotate_y_entry() {
  char data[] = "[  rotate-y, 0.25]";

  MATRIX_Matrix *expected = MATRIX_new_rotation_y(0.25);
  MATRIX_Matrix *result = YAMLLOADER_parse_transform_entry(data);
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(MATRIX_is_equal(expected, result));
  MATRIX_delete_all(expected, result);
}

void test_parse_single_transform_rotate_z_entry() {
  char data[] = " [  rotate-z, 0.75]";

  MATRIX_Matrix *expected = MATRIX_new_rotation_z(0.75);
  MATRIX_Matrix *result = YAMLLOADER_parse_transform_entry(data);
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(MATRIX_is_equal(expected, result));
  MATRIX_delete_all(expected, result);
}

void test_parse_transform_chain() {
  char data[] = "    - [scale, 0.1, 1.5, 0.1]\n"
                "    - [translate, 2.7, 1.5, -1.7]\n"
                "    - [rotate-y, 0.5]\n";
  MATRIX_Matrix *scale = MATRIX_new_scaling(0.1, 1.5, 0.1);
  MATRIX_Matrix *translate = MATRIX_new_translation(2.7, 1.5, -1.7);
  MATRIX_Matrix *rotate = MATRIX_new_rotation_y(0.5);
  MATRIX_Matrix *expected = MATRIX_multiply_many(rotate, translate, scale);
  MATRIX_Matrix *result = YAMLLOADER_parse_transform(data);
  TEST_ASSERT_NOT_NULL(result);
  if (!MATRIX_is_equal(expected, result)) {
    printf("expected:\n%s\nresult:\n%s\n", MATRIX_to_string(expected), MATRIX_to_string(result));
    TEST_FAIL();
  }
  MATRIX_delete_all(scale, translate, rotate, expected, result);
}

void test_parse_simple_world() {
  char data[] = "- add: light\n"
                "  at: [0, 6.9, -5]\n"
                "  intensity: [1, 1, 0.9]\n"
                "- add: camera\n"
                "  width: 400\n"
                "  height: 200\n"
                "  field_of_view: 0.785\n"
                "  from: [8, 6, -8]\n"
                "  to: [0, 3, 0]\n"
                "  up: [0, 1, 0]\n\n";

  CONFIGURATION_Config *config = YAMLLOADER_parse(data);

  TUPLES_Point p;
  TUPLES_init_point(&p, 0, 6.9, -5);
  TUPLES_Color c;
  TUPLES_init_color(&c, 1, 1, 0.9);
  LIGHTS_Light *result_light = WORLD_get_light(config->world);
  test_tuples(&p, &result_light->corner);
  test_tuples(&c, &result_light->intensity);

  LIGHTS_delete(result_light);
  WORLD_delete(config->world);
  CAMERA_delete(config->camera);
  free(config);
}

void test_parse_simple_world_with_extra_newlines() {
  char data[] = "\n\n- add: light\n"
                "  at: [0, 6.9, -5]\n"
                "  intensity: [1, 1, 0.9]\n"
                "\n\n"
                "- add: camera\n"
                "  width: 400\n"
                "  height: 200\n\n\n"
                "  field_of_view: 0.785\n"
                "  from: [8, 6, -8]\n"
                "  to: [0, 3, 0]\n"
                "  up: [0, 1, 0]\n\n";

  CONFIGURATION_Config *config = YAMLLOADER_parse(data);

  TUPLES_Point p;
  TUPLES_init_point(&p, 0, 6.9, -5);
  TUPLES_Color c;
  TUPLES_init_color(&c, 1, 1, 0.9);
  LIGHTS_Light *result_light = WORLD_get_light(config->world);
  test_tuples(&p, &result_light->corner);
  test_tuples(&c, &result_light->intensity);

  LIGHTS_delete(result_light);
  WORLD_delete(config->world);
  CAMERA_delete(config->camera);
  free(config);
}

void test_world_parser_should_ignore_comments() {
  char data[] = "# ======================================================\n"
                "# light sources\n"
                "# ======================================================\n"
                "\n"
                "- add: light\n"
                "  at: [0, 6.9, -5]\n"
                "  intensity: [1, 1, 0.9]\n"
                "\n"
                "# ======================================================\n"
                "# the camera\n"
                "# ======================================================\n"
                "\n"
                "- add: camera\n"
                "  width: 400\n"
                "  height: 200\n"
                "  field_of_view: 0.785\n"
                "  from: [8, 6, -8]\n"
                "  to: [0, 3, 0]\n"
                "  up: [0, 1, 0]\n\n";

  CONFIGURATION_Config *config = YAMLLOADER_parse(data);

  TUPLES_Point p;
  TUPLES_init_point(&p, 0, 6.9, -5);
  TUPLES_Color c;
  TUPLES_init_color(&c, 1, 1, 0.9);
  LIGHTS_Light *result_light = WORLD_get_light(config->world);
  test_tuples(&p, &result_light->corner);
  test_tuples(&c, &result_light->intensity);

  LIGHTS_delete(result_light);
  WORLD_delete(config->world);
  CAMERA_delete(config->camera);
  free(config);
}

/*
void test_separate_yaml_array_entities() {
  char data[] = "\n"
                "- add: array1\n"
                "  test: value\n"
                "  test2: value2\n"
                "\n\n"
                "- add: array2\n"
                "  2test: 2value\n"
                "  2test2: 2value2\n"
                "\n";
  char **result = YAMLLOADER_parse_yaml_array(data);
  char *expected1 = "- add: array1\n"
                    "  test: value\n"
                    "  test2: value2\n";
  char *expected2 = "- add: array2\n"
                    "  2test: 2value\n"
                    "  2test2: 2value2\n";
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_NOT_NULL(result[0]);
  TEST_ASSERT_EQUAL_STRING(expected1, result[0]);
  TEST_ASSERT_NOT_NULL(result[1]);
  TEST_ASSERT_EQUAL_STRING(expected2, result[1]);
  TEST_ASSERT_NULL(result[2]);

}
 */

void test_get_a_cube_from_yaml() {
  char data[] = "- add: light\n"
                "  at: [0, 6.9, -5]\n"
                "  intensity: [1, 1, 0.9]\n"
                "- add: camera\n"
                "  width: 400\n"
                "  height: 200\n"
                "  field_of_view: 0.785\n"
                "  from: [8, 6, -8]\n"
                "  to: [0, 3, 0]\n"
                "  up: [0, 1, 0]\n\n"
                "- add: cube\n";

  CONFIGURATION_Config *config = YAMLLOADER_parse(data);
  TEST_ASSERT_NOT_NULL(config);
  TEST_ASSERT_NOT_NULL(config->world);
  TEST_ASSERT_EQUAL(1, WORLD_get_object_count(config->world));
  TEST_ASSERT_NOT_NULL(WORLD_get_object(config->world, 0));
  WORLD_delete(config->world);
}

void test_get_a_transformed_cube_from_yaml() {
  char data[] = "- add: light\n"
                "  at: [0, 6.9, -5]\n"
                "  intensity: [1, 1, 0.9]\n"
                "- add: camera\n"
                "  width: 400\n"
                "  height: 200\n"
                "  field_of_view: 0.785\n"
                "  from: [8, 6, -8]\n"
                "  to: [0, 3, 0]\n"
                "  up: [0, 1, 0]\n\n"
                "- add: cube\n"
                "  transform:\n"
                "    - [translate, 0, 1, 0]\n"
                "    - [scale, 20, 7, 20]\n\n";

  MATRIX_Matrix *translate = MATRIX_new_translation(0, 1, 0);
  MATRIX_Matrix *scale = MATRIX_new_scaling(20, 7, 20);
  MATRIX_Matrix *expected_transform = MATRIX_multiply(scale, translate);

  CONFIGURATION_Config *config = YAMLLOADER_parse(data);
  TEST_ASSERT_NOT_NULL(config);
  TEST_ASSERT_NOT_NULL(config->world);
  TEST_ASSERT_EQUAL(1, WORLD_get_object_count(config->world));
  TEST_ASSERT_NOT_NULL(WORLD_get_object(config->world, 0));
  MATRIX_Matrix *result_transform = SHAPE_get_transform(WORLD_get_object(config->world, 0));

  TEST_ASSERT_TRUE(MATRIX_is_equal(expected_transform, result_transform));
  WORLD_delete(config->world);
  MATRIX_delete_all(translate, scale, expected_transform);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_parse_bool);
  RUN_TEST(test_parse_uint);
  RUN_TEST(test_parse_point);
  RUN_TEST(test_parse_vector);
  RUN_TEST(test_parse_color);
  RUN_TEST(test_parse_map_entries);
  RUN_TEST(test_parse_double);
  RUN_TEST(test_parse_single_transform_scale_entry);
  RUN_TEST(test_parse_single_transform_translate_entry);
  RUN_TEST(test_parse_single_transform_rotate_x_entry);
  RUN_TEST(test_parse_single_transform_rotate_y_entry);
  RUN_TEST(test_parse_single_transform_rotate_z_entry);
  RUN_TEST(test_parse_transform_chain);
  RUN_TEST(test_parse_simple_world);
  RUN_TEST(test_parse_simple_world_with_extra_newlines);
  RUN_TEST(test_world_parser_should_ignore_comments);
//  RUN_TEST(test_separate_yaml_array_entities);
  RUN_TEST(test_get_a_cube_from_yaml);
  RUN_TEST(test_get_a_transformed_cube_from_yaml);
  return UNITY_END();
}
