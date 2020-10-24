#include <ray.h>
#include <shape.h>
#include <tuples.h>
#include <unity.h>

#include "../testutils.h"
#include "cone.h"

void setUp() {}
void tearDown() {}

void helper_intersecting_cone_with_ray(double ox, double oy, double oz, double dx, double dy, double dz, double t0, double t1) {
  printf("Testing origin: %.2f,%.2f,%.2f, direction: %.2f,%.2f,%.2f, t0: %.2f, t1: %.2f\n", ox, oy, oz, dx, dy, dz, t0, t1);
  CONE_Cone *c = CONE_new();
  TUPLES_Vector direction;
  TUPLES_init_vector(&direction, dx, dy, dz);
  TUPLES_normalize(&direction);

  TUPLES_Point origin;
  TUPLES_init_point(&origin, ox, oy, oz);

  RAY_Ray ray;
  RAY_init_from_tuples(&ray, &origin, &direction);

  RAY_Intersections *xs = RAY_new_intersections();
  CONE_local_intersect(xs, (SHAPE_Shape *)c, &ray);

  TEST_ASSERT_EQUAL(2, xs->count);
  TEST_ASSERT_EQUAL_DOUBLE(t0, xs->xs[0].t);
  TEST_ASSERT_EQUAL_DOUBLE(t1, xs->xs[1].t);

  RAY_delete_intersections(xs);
  CONE_delete(c);
}

void test_intersecting_cone_with_ray() {
  helper_intersecting_cone_with_ray(0, 0, -5, 0, 0, 1, 5, 5);
  helper_intersecting_cone_with_ray(0, 0, -5, 1, 1, 1, 8.66025, 8.66025);
  helper_intersecting_cone_with_ray(1, 1, -5, -0.5, -1, 1, 4.55006, 49.44994);
}

void test_intersecting_cone_with_ray_parallel_to_one_of_its_halves() {
  TEST_IGNORE();
  CONE_Cone *c = CONE_new();

  RAY_Ray ray;
  RAY_init(&ray, 0, 0, -1, 0, 1, 1);

  RAY_Intersections *xs = RAY_new_intersections();
  CONE_local_intersect(xs, (SHAPE_Shape *)c, &ray);

  TEST_ASSERT_EQUAL(1, xs->count);
  TEST_ASSERT_EQUAL_DOUBLE(0.35355, xs->xs[0].t);

  RAY_delete_intersections(xs);
  CONE_delete(c);
}

void helper_intersect_cone_end_cap(double px, double py, double pz, double dx, double dy, double dz, unsigned int count) {
  printf("Testing point: %.2f,%.2f,%.2f, direction: %.2f,%.2f,%.2f, count: %u\n", px, py, pz, dx, dy, dz, count);
  CONE_Cone *c = CONE_new();
  c->minimum = -0.5;
  c->maximum = 0.5;
  c->closed = true;

  RAY_Ray ray;
  RAY_init(&ray, px, py, pz, dx, dy, dz);

  RAY_Intersections *xs = RAY_new_intersections();
  CONE_local_intersect(xs, (SHAPE_Shape *)c, &ray);

  TEST_ASSERT_EQUAL(count, xs->count);

  RAY_delete_intersections(xs);
  CONE_delete(c);
}

void test_intersect_cone_end_cap() {
  TEST_IGNORE();
  helper_intersect_cone_end_cap(0, 0, -5, 0, 1, 0, 0);
  helper_intersect_cone_end_cap(0, 0, -0.25, 0, 1, 1, 2);
  helper_intersect_cone_end_cap(0, 0, -0.25, 0, 1, 0, 4);
}

void helper_compute_normal_on_cone(double px, double py, double pz, double nx, double ny, double nz) {
  printf("Testing point: %.2f,%.2f,%.2f, normal: %.2f,%.2f,%.2f\n", px, py, pz, nx, ny, nz);
  CONE_Cone *c = CONE_new();
  TUPLES_Point p;
  TUPLES_init_point(&p, px, py, pz);

  TUPLES_Vector expected, got;
  TUPLES_init_vector(&expected, nx, ny, nz);

  CONE_local_normal_at(&got, (SHAPE_Shape *)c, &p, NULL);

  test_tuples(&expected, &got);

  CONE_delete(c);
}

void compute_normal_on_cone() {
  helper_compute_normal_on_cone(0, 0, 0, 0, 0, 0);
  helper_compute_normal_on_cone(1, 1, 1, 1, -sqrt(2), 1);
  helper_compute_normal_on_cone(-1, -1, 0, -1, 1, 0);
}

void test_unbounded_cone_bounding_box() {
  CONE_Cone *cone = CONE_new();
  BOUND_Box box;
  cone->vtable->bounds_of((SHAPE_Shape *)cone, &box);
  TUPLES_Point min_expected, max_expected;
  TUPLES_init_point(&min_expected, -INFINITY, -INFINITY, -INFINITY);
  TUPLES_init_point(&max_expected, INFINITY, INFINITY, INFINITY);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&min_expected, &box.min));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&max_expected, &box.max));
  CONE_delete(cone);
}

void test_bounded_cone_bounding_box() {
  CONE_Cone *cone = CONE_new();
  cone->minimum = -5;
  cone->maximum = 3;
  BOUND_Box box;
  cone->vtable->bounds_of((SHAPE_Shape *)cone, &box);
  TUPLES_Point min_expected, max_expected;
  TUPLES_init_point(&min_expected, -5, -5, -5);
  TUPLES_init_point(&max_expected, 5, 3, 5);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&min_expected, &box.min));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&max_expected, &box.max));
  CONE_delete(cone);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_intersecting_cone_with_ray);
  RUN_TEST(test_intersecting_cone_with_ray_parallel_to_one_of_its_halves);
  RUN_TEST(test_intersect_cone_end_cap);
  RUN_TEST(compute_normal_on_cone);
  RUN_TEST(test_bounded_cone_bounding_box);
  RUN_TEST(test_unbounded_cone_bounding_box);
  return UNITY_END();
}
