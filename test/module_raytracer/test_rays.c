#include <matrix.h>
#include <plane.h>
#include <ray.h>
#include <sphere.h>
#include <triangle.h>
#include <unity.h>

void setUp() {}
void tearDown() {}

void test_create_and_query_ray() {
  RAY_Ray *ray = RAY_new(1, 2, 3, 4, 5, 6);
  TEST_ASSERT_EQUAL_DOUBLE(2, ray->origin.y);
  TEST_ASSERT_EQUAL_DOUBLE(6, ray->direction.z);
  RAY_delete(ray);
}

void test_create_and_query_from_tuples() {
  TUPLES_Point *origin = TUPLES_new_point(1, 2, 3);
  TUPLES_Vector *direction = TUPLES_new_vector(4, 5, 6);
  RAY_Ray *ray = RAY_new_from_tuples(origin, direction);
  TEST_ASSERT_EQUAL_DOUBLE(2, ray->origin.y);
  TEST_ASSERT_EQUAL_DOUBLE(6, ray->direction.z);
  RAY_delete(ray);
  TUPLES_delete_all(origin, direction);
}

void test_create_and_query_ray_on_stack() {
  RAY_Ray ray;
  RAY_init(&ray, 1, 2, 3, 4, 5, 6);
  TEST_ASSERT_EQUAL_DOUBLE(2, ray.origin.y);
  TEST_ASSERT_EQUAL_DOUBLE(6, ray.direction.z);
  RAY_destroy(&ray);
}

void test_compute_point_from_a_distance() {
  RAY_Ray *ray = RAY_new(2, 3, 4, 1, 0, 0);
  TUPLES_Point p;
  RAY_position(&p, ray, 0);
  TEST_ASSERT_EQUAL_DOUBLE(2, p.x);
  TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
  TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
  TEST_ASSERT_TRUE(TUPLES_is_point(&p));

  RAY_position(&p, ray, 1);
  TEST_ASSERT_EQUAL_DOUBLE(3, p.x);
  TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
  TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
  TEST_ASSERT_TRUE(TUPLES_is_point(&p));

  RAY_position(&p, ray, -1);
  TEST_ASSERT_EQUAL_DOUBLE(1, p.x);
  TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
  TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
  TEST_ASSERT_TRUE(TUPLES_is_point(&p));

  RAY_position(&p, ray, 2.5);
  TEST_ASSERT_EQUAL_DOUBLE(4.5, p.x);
  TEST_ASSERT_EQUAL_DOUBLE(3, p.y);
  TEST_ASSERT_EQUAL_DOUBLE(4, p.z);
  TEST_ASSERT_TRUE(TUPLES_is_point(&p));

  TUPLES_destroy(&p);
  RAY_delete(ray);
}

void test_intersection() {
  SPHERE_Sphere s;
  SPHERE_init(&s);
  RAY_Intersections *intersections = RAY_new_intersections();
  RAY_add_intersection(intersections, 4.0, &s);
  RAY_add_intersection(intersections, 6.0, &s);
  RAY_sort_intersections(intersections);
  TEST_ASSERT_EQUAL_DOUBLE(4, intersections->xs[0].t);
  TEST_ASSERT_EQUAL_DOUBLE(6, intersections->xs[1].t);
  RAY_delete_intersections(intersections);
  SPHERE_destroy(&s);
}

void test_find_hit_with_all_positive_ts() {
  SPHERE_Sphere s;
  SPHERE_init(&s);
  RAY_Intersections *intersections = RAY_new_intersections();
  RAY_add_intersection(intersections, 1, &s);
  RAY_add_intersection(intersections, 2, &s);
  RAY_Xs *hit = RAY_hit(intersections);
  TEST_ASSERT_NOT_NULL(hit);
  TEST_ASSERT_EQUAL_DOUBLE(1.0, hit->t);
  RAY_delete_intersections(intersections);
  SPHERE_destroy(&s);
}

void test_find_hit_with_a_negative_t() {
  SPHERE_Sphere s;
  SPHERE_init(&s);
  RAY_Intersections *intersections = RAY_new_intersections();
  RAY_add_intersection(intersections, -1, &s);
  RAY_add_intersection(intersections, 1, &s);
  RAY_Xs *hit = RAY_hit(intersections);
  TEST_ASSERT_NOT_NULL(hit);
  TEST_ASSERT_EQUAL_DOUBLE(1.0, hit->t);
  RAY_delete_intersections(intersections);
  SPHERE_destroy(&s);
}

void test_find_hit_with_negative_ts() {
  SPHERE_Sphere s;
  SPHERE_init(&s);
  RAY_Intersections *intersections = RAY_new_intersections();
  RAY_add_intersection(intersections, -1, &s);
  RAY_add_intersection(intersections, -2, &s);
  RAY_Xs *hit = RAY_hit(intersections);
  TEST_ASSERT_NULL(hit);
  RAY_delete_intersections(intersections);
  SPHERE_destroy(&s);
}

void test_find_hit_with_lowest_nonnegative_t() {
  SPHERE_Sphere s;
  SPHERE_init(&s);
  RAY_Intersections *intersections = RAY_new_intersections();
  RAY_add_intersection(intersections, 5, &s);
  RAY_add_intersection(intersections, 7, &s);
  RAY_add_intersection(intersections, -3, &s);
  RAY_add_intersection(intersections, 2, &s);
  RAY_sort_intersections(intersections);
  RAY_Xs *hit = RAY_hit(intersections);
  TEST_ASSERT_NOT_NULL(hit);
  TEST_ASSERT_EQUAL_DOUBLE(2.0, hit->t);
  RAY_delete_intersections(intersections);
  SPHERE_destroy(&s);
}

void test_translate_ray() {
  RAY_Ray *orig = RAY_new(1, 2, 3, 0, 1, 0);
  RAY_Ray res;
  MATRIX_Matrix *m = MATRIX_new_translation(3, 4, 5);
  RAY_transform(&res, orig, m);
  TEST_ASSERT_EQUAL_DOUBLE(4, res.origin.x);
  TEST_ASSERT_EQUAL_DOUBLE(6, res.origin.y);
  TEST_ASSERT_EQUAL_DOUBLE(8, res.origin.z);
  TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.x);
  TEST_ASSERT_EQUAL_DOUBLE(1, res.direction.y);
  TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.z);
  MATRIX_delete(m);
  RAY_delete(orig);
  RAY_destroy(&res);
}

void test_scale_ray() {
  RAY_Ray *orig = RAY_new(1, 2, 3, 0, 1, 0);
  RAY_Ray res;
  MATRIX_Matrix *m = MATRIX_new_scaling(2, 3, 4);
  RAY_transform(&res, orig, m);
  TEST_ASSERT_EQUAL_DOUBLE(2, res.origin.x);
  TEST_ASSERT_EQUAL_DOUBLE(6, res.origin.y);
  TEST_ASSERT_EQUAL_DOUBLE(12, res.origin.z);
  TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.x);
  TEST_ASSERT_EQUAL_DOUBLE(3, res.direction.y);
  TEST_ASSERT_EQUAL_DOUBLE(0, res.direction.z);
  MATRIX_delete(m);
  RAY_delete(orig);
  RAY_destroy(&res);
}

void test_precompute_the_state_of_an_intersection() {
  TUPLES_Point expected_point;
  TUPLES_init_point(&expected_point, 0, 0, -1);
  TUPLES_Vector expected_eyev;
  TUPLES_init_vector(&expected_eyev, 0, 0, -1);
  TUPLES_Vector expected_normalv;
  TUPLES_init_vector(&expected_normalv, 0, 0, -1);

  RAY_Ray *ray = RAY_new(0, 0, -5, 0, 0, 1);
  SPHERE_Sphere *s = SPHERE_new();
  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 4.0, s);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], ray, xs);
  TEST_ASSERT_EQUAL_DOUBLE(xs->xs[0].t, comps->t);
  TEST_ASSERT_EQUAL_PTR(s, comps->object);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_point, &comps->point));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_eyev, &comps->eyev));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_normalv, &comps->normalv));
  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  SPHERE_delete(s);
  RAY_delete(ray);
}

void test_hit_when_intersection_occurs_on_outside() {
  RAY_Ray *ray = RAY_new(0, 0, -5, 0, 0, 1);
  SPHERE_Sphere *s = SPHERE_new();
  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 4.0, s);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], ray, xs);
  TEST_ASSERT_FALSE(comps->inside);
  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  SPHERE_delete(s);
  RAY_delete(ray);
}

void test_hit_when_intersection_occurs_on_inside() {
  TUPLES_Point expected_point;
  TUPLES_init_point(&expected_point, 0, 0, 1);
  TUPLES_Vector expected_eyev;
  TUPLES_init_vector(&expected_eyev, 0, 0, -1);
  TUPLES_Vector expected_normalv;
  TUPLES_init_vector(&expected_normalv, 0, 0, -1);

  RAY_Ray *ray = RAY_new(0, 0, 0, 0, 0, 1);
  SPHERE_Sphere *s = SPHERE_new();
  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 1.0, s);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], ray, xs);
  TEST_ASSERT_TRUE(comps->inside);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_point, &comps->point));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_eyev, &comps->eyev));
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected_normalv, &comps->normalv));
  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  SPHERE_delete(s);
  RAY_delete(ray);
}

void test_hit_should_offset_the_point() {
  RAY_Ray ray;
  RAY_init(&ray, 0, 0, -5, 0, 0, 1);

  SPHERE_Sphere *sphere = SPHERE_new();
  MATRIX_Matrix *transform = MATRIX_new_translation(0, 0, 1);
  SPHERE_set_transform(sphere, transform);
  MATRIX_delete(transform);

  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 5.0, sphere);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);
  TEST_ASSERT_TRUE(comps->over_point.z < -EPSILON / 2);
  TEST_ASSERT_TRUE(comps->point.z > comps->over_point.z);
  RAY_delete_intersections(xs);
  RAY_delete_computations(comps);
  SPHERE_delete(sphere);
}

void test_precompute_the_reflection_vector() {
  PLANE_Plane *plane = PLANE_new();
  RAY_Ray ray;
  RAY_init(&ray, 0, 1, -1, 0, -sqrt(2) / 2.0, sqrt(2) / 2.0);

  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, sqrt(2), plane);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

  TUPLES_Vector expected;
  TUPLES_init_vector(&expected, 0, sqrt(2) / 2.0, sqrt(2) / 2.0);
  TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, &comps->reflectv));

  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  PLANE_delete(plane);
}

void verify_computations_helper(unsigned int ndx, double n1, double n2, const RAY_Ray *ray, const RAY_Intersections *xs) {
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[ndx], ray, xs);
  printf("Testing computations for ndx(%u)\n", ndx);
  TEST_ASSERT_EQUAL_DOUBLE(n1, comps->n1);
  TEST_ASSERT_EQUAL_DOUBLE(n2, comps->n2);
  RAY_delete_computations(comps);
}

void test_finding_n1_and_n2_at_various_intersections() {
  SPHERE_Sphere *A = SPHERE_make_glassy(SPHERE_new());
  MATRIX_Matrix *A_transform = MATRIX_new_scaling(2, 2, 2);
  SPHERE_set_transform(A, A_transform);
  MATRIX_delete(A_transform);
  SPHERE_get_material(A)->refractive_index = 1.5;

  SPHERE_Sphere *B = SPHERE_make_glassy(SPHERE_new());
  MATRIX_Matrix *B_transform = MATRIX_new_translation(0, 0, -0.25);
  SPHERE_set_transform(B, B_transform);
  MATRIX_delete(B_transform);
  SPHERE_get_material(B)->refractive_index = 2.0;

  SPHERE_Sphere *C = SPHERE_make_glassy(SPHERE_new());
  MATRIX_Matrix *C_transform = MATRIX_new_translation(0, 0, 0.25);
  SPHERE_set_transform(C, C_transform);
  MATRIX_delete(C_transform);
  SPHERE_get_material(C)->refractive_index = 2.5;

  RAY_Ray ray;
  RAY_init(&ray, 0, 0, -4, 0, 0, 1);

  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 2.0, A);
  RAY_add_intersection(xs, 2.75, B);
  RAY_add_intersection(xs, 3.25, C);
  RAY_add_intersection(xs, 4.75, B);
  RAY_add_intersection(xs, 5.25, C);
  RAY_add_intersection(xs, 6, A);

  verify_computations_helper(0, 1.0, 1.5, &ray, xs);
  verify_computations_helper(1, 1.5, 2.0, &ray, xs);
  verify_computations_helper(2, 2.0, 2.5, &ray, xs);
  verify_computations_helper(3, 2.5, 2.5, &ray, xs);
  verify_computations_helper(4, 2.5, 1.5, &ray, xs);
  verify_computations_helper(5, 1.5, 1.0, &ray, xs);

  RAY_delete_intersections(xs);
  SPHERE_delete(A);
  SPHERE_delete(B);
  SPHERE_delete(C);
}

void test_under_point_is_offset_below_surface() {
  RAY_Ray ray;
  RAY_init(&ray, 0, 0, -5, 0, 0, 1);
  SPHERE_Sphere *s = SPHERE_make_glassy(SPHERE_new());
  MATRIX_Matrix *transform = MATRIX_new_translation(0, 0, 1);
  SPHERE_set_transform(s, transform);
  MATRIX_delete(transform);

  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 5, s);

  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

  TEST_ASSERT_TRUE(comps->under_point.z > EPSILON / 2);
  TEST_ASSERT_TRUE(comps->point.z < comps->under_point.z);

  RAY_delete_intersections(xs);
  RAY_delete_computations(comps);
  SPHERE_delete(s);
}

void test_schlick_approximation_under_total_internal_reflection() {
  SPHERE_Sphere *shape = SPHERE_make_glassy(SPHERE_new());
  RAY_Ray ray;
  RAY_init(&ray, 0, 0, sqrt(2) / 2.0, 0, 1, 0);
  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, -sqrt(2) / 2.0, shape);
  RAY_add_intersection(xs, sqrt(2) / 2.0, shape);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[1], &ray, xs);
  double reflectance = RAY_schlick(comps);
  TEST_ASSERT_EQUAL_DOUBLE(1.0, reflectance);
  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  SPHERE_delete(shape);
}

void test_schlick_approximation_with_perpendicular_angle() {
  SPHERE_Sphere *shape = SPHERE_make_glassy(SPHERE_new());
  RAY_Ray ray;
  RAY_init(&ray, 0, 0, 0, 0, 1, 0);
  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, -1, shape);
  RAY_add_intersection(xs, 1, shape);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[1], &ray, xs);
  double reflectance = RAY_schlick(comps);
  TEST_ASSERT_EQUAL_DOUBLE(0.04, reflectance);
  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  SPHERE_delete(shape);
}

void test_schlick_approximation_with_small_angle_and_n2_greater_than_n1() {
  SPHERE_Sphere *shape = SPHERE_make_glassy(SPHERE_new());
  RAY_Ray ray;
  RAY_init(&ray, 0, 0.99, -2.0, 0, 0, 1);
  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection(xs, 1.8589, shape);
  RAY_Computations *comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);
  double reflectance = RAY_schlick(comps);
  TEST_ASSERT_EQUAL_DOUBLE(0.48873, reflectance);
  RAY_delete_computations(comps);
  RAY_delete_intersections(xs);
  SPHERE_delete(shape);
}

void test_intersection_encapsulating_u_and_v() {
  TUPLES_Point p1, p2, p3;
  TUPLES_init_point(&p1, 0, 1, 0);
  TUPLES_init_point(&p2, -1, 0, 0);
  TUPLES_init_point(&p3, 1, 0, 0);
  TRIANGLE_Triangle *s = TRIANGLE_new_from_points(&p1, &p2, &p3);

  RAY_Intersections *xs = RAY_new_intersections();
  RAY_add_intersection_tri(xs, 3.5, s, 0.2, 0.4);

  TEST_ASSERT_EQUAL_DOUBLE(0.2, xs->xs[0].u);
  TEST_ASSERT_EQUAL_DOUBLE(0.4, xs->xs[0].v);

  RAY_delete_intersections(xs);
  TRIANGLE_delete(s);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_create_and_query_ray);
  RUN_TEST(test_create_and_query_ray_on_stack);
  RUN_TEST(test_compute_point_from_a_distance);
  RUN_TEST(test_intersection);
  RUN_TEST(test_find_hit_with_all_positive_ts);
  RUN_TEST(test_find_hit_with_a_negative_t);
  RUN_TEST(test_find_hit_with_negative_ts);
  RUN_TEST(test_find_hit_with_lowest_nonnegative_t);
  RUN_TEST(test_translate_ray);
  RUN_TEST(test_scale_ray);
  RUN_TEST(test_create_and_query_from_tuples);
  RUN_TEST(test_precompute_the_state_of_an_intersection);
  RUN_TEST(test_hit_when_intersection_occurs_on_outside);
  RUN_TEST(test_hit_when_intersection_occurs_on_inside);
  RUN_TEST(test_hit_should_offset_the_point);
  RUN_TEST(test_precompute_the_reflection_vector);
  RUN_TEST(test_finding_n1_and_n2_at_various_intersections);
  RUN_TEST(test_under_point_is_offset_below_surface);
  RUN_TEST(test_schlick_approximation_under_total_internal_reflection);
  RUN_TEST(test_schlick_approximation_with_perpendicular_angle);
  RUN_TEST(test_schlick_approximation_with_small_angle_and_n2_greater_than_n1);
  RUN_TEST(test_intersection_encapsulating_u_and_v);
  return UNITY_END();
}
