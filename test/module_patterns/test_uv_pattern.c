#include "../testutils.h"
#include "unity.h"
#include <pattern.h>
#include <uv_pattern.h>

TUPLES_Color white, black, red, yellow, brown, green, cyan, blue, purple;

void setUp() {
  TUPLES_init_color(&white, 1, 1, 1);
  TUPLES_init_color(&black, 0, 0, 0);
  TUPLES_init_color(&red, 1, 0, 0);
  TUPLES_init_color(&yellow, 1, 1, 0);
  TUPLES_init_color(&brown, 1, 0.5, 0);
  TUPLES_init_color(&green, 0, 1, 0);
  TUPLES_init_color(&cyan, 0, 1, 1);
  TUPLES_init_color(&blue, 0, 0, 1);
  TUPLES_init_color(&purple, 1, 0, 1);
  TUPLES_init_color(&white, 1, 1, 1);
}

void tearDown() {
}

void check_pattern_in_2d(double u, double v, const TUPLES_Color* expected) {
  UV_Pattern *p = UV_PATTERN_new_checkers(2, 2, &black, &white);
  TUPLES_Color result;
  UV_PATTERN_pattern_at(&result, p, u, v);
  test_tuples(expected, &result);
  UV_PATTERN_delete(p);
}

void test_check_pattern_in_2d() {
  check_pattern_in_2d(0, 0, &black);
  check_pattern_in_2d(0.5, 0, &white);
  check_pattern_in_2d(0, 0.5, &white);
  check_pattern_in_2d(0.5, 0.5, &black);
  check_pattern_in_2d(1.0, 1.0, &black);
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
  UV_Pattern *p = UV_PATTERN_new_checkers(16, 8, &black, &white);
  PATTERN_Pattern *pattern = PATTERN_new_map(p, &UV_PATTERN_spherical_map);
  UV_PATTERN_delete(p);

  TUPLES_Color result;
  TUPLES_Point point;
  TUPLES_init_point(&point, x, y, z);
  PATTERN_color_at(&result, pattern, &point);
  test_tuples(expected_color, &result);

  PATTERN_delete(pattern);
}

void test_use_texture_map_pattern_with_spherical_map() {
  use_texture_map_pattern_with_spherical_map(0.4315, 0.4670, 0.7719, &white);
  use_texture_map_pattern_with_spherical_map(-0.9654, 0.2552, -0.0534, &black);
  use_texture_map_pattern_with_spherical_map(0.1039, 0.7090, 0.6975, &white);
  use_texture_map_pattern_with_spherical_map(-0.4986, -0.7856, -0.3663, &black);
  use_texture_map_pattern_with_spherical_map(-0.0317, -0.9395, 0.3411, &black);
  use_texture_map_pattern_with_spherical_map(0.4809, -0.7721, 0.4154, &black);
  use_texture_map_pattern_with_spherical_map(0.0285, -0.9612, -0.2745, &black);
  use_texture_map_pattern_with_spherical_map(-0.5734, -0.2162, -0.7903, &white);
  use_texture_map_pattern_with_spherical_map(0.7688, -0.1470, 0.6223, &black);
  use_texture_map_pattern_with_spherical_map(-0.7652, 0.2175, 0.6060, &black);
}

void use_planar_mapping_on_a_3d_point(double x, double y, double z, double expected_u, double expected_v) {
  double u, v;
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);
  UV_PATTERN_planar_map(&u, &v, &p);
  printf("Testing (%.2f, %.2f, %.2f), got (%.2f, %.2f), expected(%.2f, %.2f)\n", x, y, z, u, v, expected_u, expected_v);
  TEST_ASSERT_EQUAL_DOUBLE(expected_u, u);
  TEST_ASSERT_EQUAL_DOUBLE(expected_v, v);
}

void test_use_planar_mapping_on_a_3d_point() {
  use_planar_mapping_on_a_3d_point(0.25, 0, 0.5, 0.25, 0.5);
  use_planar_mapping_on_a_3d_point(0.25, 0, -0.25, 0.25, 0.75);
  use_planar_mapping_on_a_3d_point(0.25, 0.5, -0.25, 0.25, 0.75);
  use_planar_mapping_on_a_3d_point(1.25, 0, 0.5, 0.25, 0.5);
  use_planar_mapping_on_a_3d_point(0.25, 0, -1.75, 0.25, 0.25);
  use_planar_mapping_on_a_3d_point(1, 0, -1, 0, 0);
  use_planar_mapping_on_a_3d_point(0, 0, 0, 0, 0);
}

void use_cylindrical_mapping_on_a_3d_point(double x, double y, double z, double expected_u, double expected_v) {
  double u, v;
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);
  UV_PATTERN_cylinder_map(&u, &v, &p);
  printf("Testing (%.2f, %.2f, %.2f), got (%.2f, %.2f), expected(%.2f, %.2f)\n", x, y, z, u, v, expected_u, expected_v);
  TEST_ASSERT_EQUAL_DOUBLE(expected_u, u);
  TEST_ASSERT_EQUAL_DOUBLE(expected_v, v);
}

void test_use_cylindrical_mapping_on_a_3d_point() {
  use_cylindrical_mapping_on_a_3d_point(0, 0, -1, 0, 0);
  use_cylindrical_mapping_on_a_3d_point(0, 0.5, -1, 0, 0.5);
  use_cylindrical_mapping_on_a_3d_point(0, 1, -1, 0, 0);
  use_cylindrical_mapping_on_a_3d_point(0.70711, 0.5, -0.70711, 0.125, 0.5);
  use_cylindrical_mapping_on_a_3d_point(1, 0.5, 0, 0.25, 0.5);
  use_cylindrical_mapping_on_a_3d_point(0.70711, 0.5, 0.70711, 0.375, 0.5);
  use_cylindrical_mapping_on_a_3d_point(0, -0.25, 1, 0.5, 0.75);
  use_cylindrical_mapping_on_a_3d_point(-0.70711, 0.5, 0.70711, 0.625, 0.5);
  use_cylindrical_mapping_on_a_3d_point(-1, 1.25, 0, 0.75, 0.25);
  use_cylindrical_mapping_on_a_3d_point(-0.70711, 0.5, -0.70711, 0.875, 0.5);
}

TUPLES_Color main_color, ul, ur, bl, br;
void layout_of_align_check_pattern(double u, double v, const TUPLES_Color* expected_color) {
  UV_Pattern *ptn = UV_PATTERN_new_align_check(&main_color, &ul, &ur, &bl, &br);
  TEST_ASSERT_NOT_NULL(ptn);
  TUPLES_Color result;
  UV_PATTERN_pattern_at(&result, ptn, u, v);
  test_tuples(expected_color, &result);
  UV_PATTERN_delete(ptn);
}

void test_layout_of_align_check_pattern() {
  TUPLES_init_color(&main_color, 1, 1, 1);
  TUPLES_init_color(&ul, 1, 0, 0);
  TUPLES_init_color(&ur, 1, 1, 0);
  TUPLES_init_color(&bl, 0, 1, 0);
  TUPLES_init_color(&br, 0, 1, 1);

  layout_of_align_check_pattern(0.5, 0.5, &main_color);
  layout_of_align_check_pattern(0.1, 0.9, &ul);
  layout_of_align_check_pattern(0.9, 0.9, &ur);
  layout_of_align_check_pattern(0.1, 0.1, &bl);
  layout_of_align_check_pattern(0.9, 0.1, &br);
}

void identify_face_of_a_cube_from_a_point(double x, double y, double z, enum UV_PATTERN_Cube_Face expected_face) {
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);
  enum UV_PATTERN_Cube_Face result_face = UV_PATTERN_face_from_point(&p);
  TEST_ASSERT_EQUAL(expected_face, result_face);
}

void test_identify_face_of_a_cube_from_a_point() {
  identify_face_of_a_cube_from_a_point(-1, 0.5, -0.25, UV_PATTERN_LEFT);
  identify_face_of_a_cube_from_a_point(1.1, -0.75, 0.8, UV_PATTERN_RIGHT);
  identify_face_of_a_cube_from_a_point(0.1, 0.6, 0.9, UV_PATTERN_FRONT);
  identify_face_of_a_cube_from_a_point(-0.7, 0, -2, UV_PATTERN_BACK);
  identify_face_of_a_cube_from_a_point(0.5, 1, 0.9, UV_PATTERN_UP);
  identify_face_of_a_cube_from_a_point(-0.2, -1.3, 1.1, UV_PATTERN_DOWN);
}

void find_color_on_a_mapped_cube(double x, double y, double z, const TUPLES_Color *expected_result) {
  UV_Pattern *left = UV_PATTERN_new_align_check(&yellow, &cyan, &red, &blue, &brown);
  UV_Pattern *front = UV_PATTERN_new_align_check(&cyan, &red, &yellow, &brown, &green);
  UV_Pattern *right = UV_PATTERN_new_align_check(&red, &yellow, &purple, &green, &white);
  UV_Pattern *back = UV_PATTERN_new_align_check(&green, &purple, &cyan, &white, &blue);
  UV_Pattern *up = UV_PATTERN_new_align_check(&brown, &cyan, &purple, &red, &yellow);
  UV_Pattern *down = UV_PATTERN_new_align_check(&purple, &brown, &green, &blue, &white);
  PATTERN_Pattern *pattern = PATTERN_new_cube_map(left, front, right, back, up, down);
  UV_PATTERN_delete_all(left, front, right, back, up, down);
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);
  TUPLES_Color result;
  PATTERN_color_at(&result, pattern, &p);
  printf("Testing point (%.2f, %.2f, %.2f)\n", x, y, z);
  test_tuples(expected_result, &result);
  PATTERN_delete(pattern);
}

void test_find_color_on_a_mapped_cube() {
  //Left
  TEST_MESSAGE("Test left surface");
  find_color_on_a_mapped_cube(-1, 0, 0, &yellow);
  find_color_on_a_mapped_cube(-1, 0.9, -0.9, &cyan);
  find_color_on_a_mapped_cube(-1, 0.9, 0.9, &red);
  find_color_on_a_mapped_cube(-1, -0.9, -0.9, &blue);
  find_color_on_a_mapped_cube(-1, -0.9, 0.9, &brown);
  //Front
  TEST_MESSAGE("Test front surface");
  find_color_on_a_mapped_cube(0, 0, 1, &cyan);
  find_color_on_a_mapped_cube(-0.9, 0.9, 1, &red);
  find_color_on_a_mapped_cube(0.9, 0.9, 1, &yellow);
  find_color_on_a_mapped_cube(-0.9, -0.9, 1, &brown);
  find_color_on_a_mapped_cube(0.9, -0.9, 1, &green);
  //Rear
  TEST_MESSAGE("Test rear surface");
  find_color_on_a_mapped_cube(1, 0, 0, &red);
  find_color_on_a_mapped_cube(1, 0.9, 0.9, &yellow);
  find_color_on_a_mapped_cube(1, 0.9, -0.9, &purple);
  find_color_on_a_mapped_cube(1, -0.9, 0.9, &green);
  find_color_on_a_mapped_cube(1, -0.9, -0.9, &white);
  //Back
  TEST_MESSAGE("Test back surface");
  find_color_on_a_mapped_cube(0, 0, -1, &green);
  find_color_on_a_mapped_cube(0.9, 0.9, -1, &purple);
  find_color_on_a_mapped_cube(-0.9, 0.9, -1, &cyan);
  find_color_on_a_mapped_cube(0.9, -0.9, -1, &white);
  find_color_on_a_mapped_cube(-0.9, -0.9, -1, &blue);
  //Up
  TEST_MESSAGE("Test up surface");
  find_color_on_a_mapped_cube(0, 1, 0, &brown);
  find_color_on_a_mapped_cube(-0.9, 1, -0.9, &cyan);
  find_color_on_a_mapped_cube(0.9, 1, -0.9, &purple);
  find_color_on_a_mapped_cube(-0.9, 1, 0.9, &red);
  find_color_on_a_mapped_cube(0.9, 1, 0.9, &yellow);
  //Down
  TEST_MESSAGE("Test down surface");
  find_color_on_a_mapped_cube(0, -1, 0, &purple);
  find_color_on_a_mapped_cube(-0.9, -1, 0.9, &brown);
  find_color_on_a_mapped_cube(0.9, -1, 0.9, &green);
  find_color_on_a_mapped_cube(-0.9, -1, -0.9, &blue);
  find_color_on_a_mapped_cube(0.9, -1, -0.9, &white);
}

void map_face_of_cube(double x, double y, double z, enum UV_PATTERN_Cube_Face face, double u_expected, double v_expected) {
  double u_result, v_result;
  TUPLES_Point p;
  TUPLES_init_point(&p, x, y, z);
  UV_PATTERN_cube_map(&u_result, &v_result, face, &p);
  printf("Testing point (%.2f, %.2f, %.2f)\n", x, y, z);
  TEST_ASSERT_EQUAL_DOUBLE(u_expected, u_result);
  TEST_ASSERT_EQUAL_DOUBLE(v_expected, v_result);
}

void test_map_face_of_cube() {
  TEST_MESSAGE("Test front surface");
  map_face_of_cube(-0.5, 0.5, 1, UV_PATTERN_FRONT, 0.25, 0.75);
  map_face_of_cube(0.5, -0.5, 1, UV_PATTERN_FRONT, 0.75, 0.25);

  TEST_MESSAGE("Test back surface");
  map_face_of_cube(0.5, 0.5, -1, UV_PATTERN_BACK, 0.25, 0.75);
  map_face_of_cube(-0.5, -0.5, -1, UV_PATTERN_BACK, 0.75, 0.25);

  TEST_MESSAGE("Test left surface");
  map_face_of_cube(-1, 0.5, -0.5, UV_PATTERN_LEFT, 0.25, 0.75);
  map_face_of_cube(-1, -0.5, 0.5, UV_PATTERN_LEFT, 0.75, 0.25);

  TEST_MESSAGE("Test right surface");
  map_face_of_cube(1, 0.5, 0.5, UV_PATTERN_RIGHT, 0.25, 0.75);
  map_face_of_cube(1, -0.5, -0.5, UV_PATTERN_RIGHT, 0.75, 0.25);

  TEST_MESSAGE("Test up surface");
  map_face_of_cube(-0.5, 1, -0.5, UV_PATTERN_UP, 0.25, 0.75);
  map_face_of_cube(0.5, 1, 0.5, UV_PATTERN_UP, 0.75, 0.25);

  TEST_MESSAGE("Test down surface");
  map_face_of_cube(-0.5, -1, 0.5, UV_PATTERN_DOWN, 0.25, 0.75);
  map_face_of_cube(0.5, -1, -0.5, UV_PATTERN_DOWN, 0.75, 0.25);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_check_pattern_in_2d);
  RUN_TEST(test_spherical_mapping_on_a_3d_point);
  RUN_TEST(test_use_texture_map_pattern_with_spherical_map);
  RUN_TEST(test_use_planar_mapping_on_a_3d_point);
  RUN_TEST(test_use_cylindrical_mapping_on_a_3d_point);
  RUN_TEST(test_layout_of_align_check_pattern);
  RUN_TEST(test_identify_face_of_a_cube_from_a_point);
  RUN_TEST(test_map_face_of_cube);
  RUN_TEST(test_find_color_on_a_mapped_cube);
  return UNITY_END();
}
