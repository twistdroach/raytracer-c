#include "../testutils.h"
#include <cylinder.h>
#include <ray.h>
#include <tuples.h>
#include <unity.h>

void setUp() {}
void tearDown() {}

void helper_ray_misses_cylinder(double ox, double oy, double oz, double dx, double dy, double dz) {
  CYLINDER_Cylinder *c = CYLINDER_new();
  TUPLES_Vector direction;
  TUPLES_Point origin;
  TUPLES_init_vector(&direction, dx, dy, dz);
  TUPLES_normalize(&direction);
  TUPLES_init_vector(&origin, ox, oy, oz);
  RAY_Ray ray;
  RAY_init_from_tuples(&ray, &origin, &direction);
  RAY_Intersections *xs = RAY_new_intersections();
  CYLINDER_local_intersect(xs, (SHAPE_Shape *)c, &ray);
  TEST_ASSERT_EQUAL(0, xs->count);
  RAY_delete_intersections(xs);
  CYLINDER_delete(c);
}

void test_ray_misses_cylinder() {
  helper_ray_misses_cylinder(1, 0, 0, 0, 1, 0);
  helper_ray_misses_cylinder(0, 0, 0, 0, 1, 0);
  helper_ray_misses_cylinder(0, 0, -5, 1, 1, 1);
}

void helper_ray_hits_cylinder(double ox, double oy, double oz, double dx, double dy, double dz, double t0, double t1) {
  CYLINDER_Cylinder *c = CYLINDER_new();
  TUPLES_Vector direction;
  TUPLES_Point origin;
  TUPLES_init_vector(&direction, dx, dy, dz);
  TUPLES_normalize(&direction);
  TUPLES_init_point(&origin, ox, oy, oz);
  RAY_Ray ray;
  RAY_init_from_tuples(&ray, &origin, &direction);
  RAY_Intersections *xs = RAY_new_intersections();
  CYLINDER_local_intersect(xs, (SHAPE_Shape *)c, &ray);
  TEST_ASSERT_EQUAL(2, xs->count);
  TEST_ASSERT_EQUAL_DOUBLE(t0, xs->xs[0].t);
  TEST_ASSERT_EQUAL_DOUBLE(t1, xs->xs[1].t);
  RAY_delete_intersections(xs);
  CYLINDER_delete(c);
}

void test_ray_hits_cylinder() {
  helper_ray_hits_cylinder(1, 0, -5, 0, 0, 1, 5, 5);
  helper_ray_hits_cylinder(0, 0, -5, 0, 0, 1, 4, 6);
  helper_ray_hits_cylinder(0.5, 0, -5, 0.1, 1, 1, 6.80798, 7.08872);
}

void helper_normal_vector_on_cylinder(double px, double py, double pz, double nx, double ny, double nz) {
  CYLINDER_Cylinder *c = CYLINDER_new();
  TUPLES_Point p;
  TUPLES_init_point(&p, px, py, pz);
  TUPLES_Vector got, expected;
  CYLINDER_local_normal_at(&got, (SHAPE_Shape *)c, &p, NULL);
  TUPLES_init_vector(&expected, nx, ny, nz);
  test_tuples(&got, &expected);
  CYLINDER_delete(c);
}

void test_normal_vector_on_cylinder() {
  helper_normal_vector_on_cylinder(1, 0, 0, 1, 0, 0);
  helper_normal_vector_on_cylinder(0, 5, -1, 0, 0, -1);
  helper_normal_vector_on_cylinder(0, -2, 1, 0, 0, 1);
  helper_normal_vector_on_cylinder(-1, 1, 0, -1, 0, 0);
}

void test_default_cylinder_min_max() {
  CYLINDER_Cylinder *c = CYLINDER_new();
  TEST_ASSERT_EQUAL_DOUBLE(-INFINITY, c->minimum);
  TEST_ASSERT_EQUAL_DOUBLE(INFINITY, c->maximum);
  CYLINDER_delete(c);
}

void helper_intersect_constrained_cylinder(double px, double py, double pz, double dx, double dy, double dz, unsigned int count) {
  CYLINDER_Cylinder *c = CYLINDER_new();
  c->maximum = 2;
  c->minimum = 1;

  TUPLES_Vector direction;
  TUPLES_init_vector(&direction, dx, dy, dz);
  TUPLES_normalize(&direction);

  TUPLES_Point p;
  TUPLES_init_point(&p, px, py, pz);

  RAY_Ray ray;
  RAY_init_from_tuples(&ray, &p, &direction);

  RAY_Intersections *xs = RAY_new_intersections();
  CYLINDER_local_intersect(xs, (SHAPE_Shape *)c, &ray);

  TEST_ASSERT_EQUAL(count, xs->count);

  RAY_delete_intersections(xs);
  CYLINDER_delete(c);
}

void test_intersect_constrained_cylinder() {
  helper_intersect_constrained_cylinder(0, 1.5, 0, 0.1, 1, 0, 0);
  helper_intersect_constrained_cylinder(0, 3, -5, 0, 0, 1, 0);
  helper_intersect_constrained_cylinder(0, 0, -5, 0, 0, 1, 0);
  helper_intersect_constrained_cylinder(0, 2, -5, 0, 0, 1, 0);
  helper_intersect_constrained_cylinder(0, 1, -5, 0, 0, 1, 0);
  helper_intersect_constrained_cylinder(0, 1.5, -2, 0, 0, 1, 2);
}

void test_default_closed_value_for_cylinder() {
  CYLINDER_Cylinder *c = CYLINDER_new();
  TEST_ASSERT_FALSE(c->closed);
  CYLINDER_delete(c);
}

void helper_intersect_caps_of_closed_cylinder(double px, double py, double pz, double dx, double dy, double dz, unsigned int count) {
  CYLINDER_Cylinder *c = CYLINDER_new();
  c->minimum = 1;
  c->maximum = 2;
  c->closed = true;

  TUPLES_Vector direction;
  TUPLES_init_vector(&direction, dx, dy, dz);
  TUPLES_normalize(&direction);

  TUPLES_Point p;
  TUPLES_init_point(&p, px, py, pz);

  RAY_Ray ray;
  RAY_init_from_tuples(&ray, &p, &direction);

  RAY_Intersections *xs = RAY_new_intersections();
  CYLINDER_local_intersect(xs, (SHAPE_Shape *)c, &ray);

  TEST_ASSERT_EQUAL(count, xs->count);

  RAY_delete_intersections(xs);
  CYLINDER_delete(c);
}

void test_intersect_caps_of_closed_cylinder() {
  helper_intersect_caps_of_closed_cylinder(0, 3, 0, 0, -1, 0, 2);
  helper_intersect_caps_of_closed_cylinder(0, 3, -2, 0, -1, 2, 2);
  helper_intersect_caps_of_closed_cylinder(0, 4, -2, 0, -1, 1, 2);
  helper_intersect_caps_of_closed_cylinder(0, 0, -2, 0, 1, 2, 2);
  helper_intersect_caps_of_closed_cylinder(0, -1, -2, 0, 1, 1, 2);
}

void helper_test_normal_vector_on_cylinder_end_caps(double px, double py, double pz, double nx, double ny, double nz) {
  CYLINDER_Cylinder *c = CYLINDER_new();
  c->minimum = 1;
  c->maximum = 2;
  c->closed = true;

  TUPLES_Point p;
  TUPLES_init_point(&p, px, py, pz);

  TUPLES_Vector expected, got;
  TUPLES_init_vector(&expected, nx, ny, nz);

  CYLINDER_local_normal_at(&got, (SHAPE_Shape *)c, &p, NULL);
  test_tuples(&expected, &got);

  CYLINDER_delete(c);
}

void test_normal_vector_on_cylinder_end_caps() {
  helper_test_normal_vector_on_cylinder_end_caps(0, 1, 0, 0, -1, 0);
  helper_test_normal_vector_on_cylinder_end_caps(0.5, 1, 0, 0, -1, 0);
  helper_test_normal_vector_on_cylinder_end_caps(0, 1, 0.5, 0, -1, 0);
  helper_test_normal_vector_on_cylinder_end_caps(0, 2, 0, 0, 1, 0);
  helper_test_normal_vector_on_cylinder_end_caps(0.5, 2, 0, 0, 1, 0);
  helper_test_normal_vector_on_cylinder_end_caps(0, 2, 0.5, 0, 1, 0);
}

void test_unbounded_cylinder_bounding_box() {
  CYLINDER_Cylinder *c = CYLINDER_new();
  BOUND_Box box;
  c->vtable->bounds_of((SHAPE_Shape *)c, &box);
  TUPLES_Point min_expected, max_expected;
  TUPLES_init_point(&min_expected, -1, -INFINITY, -1);
  TUPLES_init_point(&max_expected, 1, INFINITY, 1);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&min_expected, &box.min));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&max_expected, &box.max));
  CYLINDER_delete(c);
}

void test_bounded_cylinder_bounding_box() {
  CYLINDER_Cylinder *c = CYLINDER_new();
  c->minimum = -5;
  c->maximum = 3;
  BOUND_Box box;
  c->vtable->bounds_of((SHAPE_Shape *)c, &box);
  TUPLES_Point min_expected, max_expected;
  TUPLES_init_point(&min_expected, -1, -5, -1);
  TUPLES_init_point(&max_expected, 1, 3, 1);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&min_expected, &box.min));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&max_expected, &box.max));
  CYLINDER_delete(c);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_ray_misses_cylinder);
  RUN_TEST(test_ray_hits_cylinder);
  RUN_TEST(test_normal_vector_on_cylinder);
  RUN_TEST(test_default_cylinder_min_max);
  RUN_TEST(test_intersect_constrained_cylinder);
  RUN_TEST(test_default_closed_value_for_cylinder);
  RUN_TEST(test_intersect_caps_of_closed_cylinder);
  RUN_TEST(test_normal_vector_on_cylinder_end_caps);
  RUN_TEST(test_bounded_cylinder_bounding_box);
  RUN_TEST(test_unbounded_cylinder_bounding_box);
  return UNITY_END();
}
