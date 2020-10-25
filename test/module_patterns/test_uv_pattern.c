#include "../testutils.h"
#include "unity.h"
#include <pattern.h>
#include <uv_pattern.h>

TUPLES_Color *white;
TUPLES_Color *black;

void setUp() {
  white = TUPLES_new_color(1, 1, 1);
  black = TUPLES_new_color(0, 0, 0);
}

void tearDown() {
  TUPLES_delete(white);
  TUPLES_delete(black);
}

void check_pattern_in_2d(double u, double v, const TUPLES_Color* expected) {
  UV_Pattern p;
  UV_PATTERN_init(&p, 2, 2, black, white);
  TUPLES_Color result;
  UV_PATTERN_pattern_at(&result, &p, u, v);
  test_tuples(expected, &result);
}

void test_check_pattern_in_2d() {
  check_pattern_in_2d(0, 0, black);
  check_pattern_in_2d(0.5, 0, white);
  check_pattern_in_2d(0, 0.5, white);
  check_pattern_in_2d(0.5, 0.5, black);
  check_pattern_in_2d(1.0, 1.0, black);
}

void spherical_mapping_on_a_3d_point(double x, double y, double z, double expected_u, double expected_v) {
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);
  double u, v;
  UV_PATTERN_spherical_map(&u, &v, &p);
  TEST_ASSERT_EQUAL_DOUBLE(expected_u, u);
  TEST_ASSERT_EQUAL_DOUBLE(expected_v, v);
}

void test_spherical_mapping_on_a_3d_point() {
  spherical_mapping_on_a_3d_point(0, 0, -1, 0, 0.5);
  spherical_mapping_on_a_3d_point(1, 0, 0, 0.25, 0.5);
  spherical_mapping_on_a_3d_point(0, 0, 1, 0.5, 0.5);
  spherical_mapping_on_a_3d_point(-1, 0, 0, 0.75, 0.5);
  spherical_mapping_on_a_3d_point(0, 1, 0, 0.5, 1.0);
  spherical_mapping_on_a_3d_point(0, -1, 0, 0.5, 0);
  spherical_mapping_on_a_3d_point(sqrt(2)/2, sqrt(2)/2, 0, 0.25, 0.75);
}

void use_texture_map_pattern_with_spherical_map(double x, double y, double z, TUPLES_Color* expected_color) {
  UV_Pattern p;
  UV_PATTERN_init(&p, 16, 8, black, white);

  PATTERN_Pattern *pattern = PATTERN_new_map(&p, &UV_PATTERN_spherical_map);

  TUPLES_Color result;
  TUPLES_Point point;
  TUPLES_init_point(&point, x, y, z);
  PATTERN_color_at(&result, pattern, &point);
  test_tuples(expected_color, &result);

  PATTERN_delete(pattern);
}

void test_use_texture_map_pattern_with_spherical_map() {
  use_texture_map_pattern_with_spherical_map(0.4315, 0.4670, 0.7719, white);
  use_texture_map_pattern_with_spherical_map(-0.9654, 0.2552, -0.0534, black);
  use_texture_map_pattern_with_spherical_map(0.1039, 0.7090, 0.6975, white);
  use_texture_map_pattern_with_spherical_map(-0.4986, -0.7856, -0.3663, black);
  use_texture_map_pattern_with_spherical_map(-0.0317, -0.9395, 0.3411, black);
  use_texture_map_pattern_with_spherical_map(0.4809, -0.7721, 0.4154, black);
  use_texture_map_pattern_with_spherical_map(0.0285, -0.9612, -0.2745, black);
  use_texture_map_pattern_with_spherical_map(-0.5734, -0.2162, -0.7903, white);
  use_texture_map_pattern_with_spherical_map(0.7688, -0.1470, 0.6223, black);
  use_texture_map_pattern_with_spherical_map(-0.7652, 0.2175, 0.6060, black);
}



int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_check_pattern_in_2d);
  RUN_TEST(test_spherical_mapping_on_a_3d_point);
  RUN_TEST(test_use_texture_map_pattern_with_spherical_map);
  return UNITY_END();
}
