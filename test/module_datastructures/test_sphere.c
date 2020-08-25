#include <unity.h>
#include "sphere.h"
#include "ray.h"

void setUp() {}
void tearDown() {}

void test_ray_intersects_sphere_at_two_points() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Ray r;
    RAY_init(&r, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    SPHERE_local_intersect(intersections, &s, &r);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(4.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(6.0, intersections->xs[1].t);
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_ray_sets_object_on_intersection() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Ray r;
    RAY_init(&r, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    SPHERE_local_intersect(intersections, &s, &r);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_PTR(&s, intersections->xs[0].object);
    TEST_ASSERT_EQUAL_PTR(&s, intersections->xs[1].object);
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_ray_intersects_sphere_at_tangent() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Ray r;
    RAY_init(&r, 0, 1, -5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    SPHERE_local_intersect(intersections, &s, &r);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(5.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(5.0, intersections->xs[1].t);
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_ray_misses_sphere() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Ray r;
    RAY_init(&r, 0, 2, -5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    SPHERE_local_intersect(intersections, &s, &r);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL(0, intersections->count);
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_ray_originates_inside_sphere() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Ray r;
    RAY_init(&r, 0, 0, 0, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    SPHERE_local_intersect(intersections, &s, &r);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(-1.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, intersections->xs[1].t);
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_sphere_behind_ray() {
    SPHERE_Sphere s;
    SPHERE_init(&s);
    RAY_Ray r;
    RAY_init(&r, 0, 0, 5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    SPHERE_local_intersect(intersections, &s, &r);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(-6.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(-4.0, intersections->xs[1].t);
    SPHERE_destroy(&s);
    RAY_delete_intersections(intersections);
}

void test_sphere_default_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* identity_m = MATRIX_new_identity(4);
    TEST_ASSERT_TRUE(MATRIX_is_equal(identity_m, s->parent.transform));
    SPHERE_delete(s);
    MATRIX_delete(identity_m);
}

void test_sphere_modifying_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* t = MATRIX_new_translation(2, 3, 4);
    SHAPE_set_transform(&s->parent, t);
    TEST_ASSERT_TRUE(MATRIX_is_equal(t, s->parent.transform));
    SPHERE_delete(s);
    MATRIX_delete(t);
}

void test_sphere_intersect_scaled_ray() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* m = MATRIX_new_scaling(2, 2, 2);
    SHAPE_set_transform(&s->parent, m);
    MATRIX_delete(m);
    RAY_Ray* r = RAY_new(0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_Ray local_ray;
    SHAPE_calc_local_ray(&local_ray, r, (SHAPE_Shape*) s);
    SPHERE_local_intersect(intersections, s, &local_ray);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_NOT_NULL(intersections);
    TEST_ASSERT_EQUAL(2, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(3.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(7.0, intersections->xs[1].t);
    SPHERE_delete(s);
    RAY_delete_intersections(intersections);
    RAY_delete(r);
}

void test_sphere_intersect_translated_ray() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* m = MATRIX_new_translation(5, 0, 0);
    SHAPE_set_transform(&s->parent, m);
    MATRIX_delete(m);
    RAY_Ray* r = RAY_new(0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = RAY_new_intersections();
    RAY_Ray local_ray;
    SHAPE_calc_local_ray(&local_ray, r, (SHAPE_Shape*) s);
    SPHERE_local_intersect(intersections, s, &local_ray);
    RAY_sort_intersections(intersections);
    TEST_ASSERT_NOT_NULL(intersections);
    TEST_ASSERT_EQUAL(0, intersections->count);
    SPHERE_delete(s);
    RAY_delete_intersections(intersections);
    RAY_delete(r);
}

void test_normal_on_sphere_at_point_on_x_axis() {
    SPHERE_Sphere* s = SPHERE_new();
    TUPLES_Point* p = TUPLES_new_point(1, 0, 0);
    TUPLES_Vector v;
    SPHERE_local_normal_at(&v, s, p);
    TEST_ASSERT_EQUAL_DOUBLE(1, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&v));
    SPHERE_delete(s);
    TUPLES_delete_all(p);
}

void test_normal_on_sphere_at_point_on_y_axis() {
    SPHERE_Sphere* s = SPHERE_new();
    TUPLES_Point* p = TUPLES_new_point(0, 1, 0);
    TUPLES_Vector v;
    SPHERE_local_normal_at(&v, s, p);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(1, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&v));
    SPHERE_delete(s);
    TUPLES_delete_all(p);
}

void test_normal_on_sphere_at_point_on_z_axis() {
    SPHERE_Sphere* s = SPHERE_new();
    TUPLES_Point* p = TUPLES_new_point(0, 0, 1);
    TUPLES_Vector v;
    SPHERE_local_normal_at(&v, s, p);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(1, v.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&v));
    SPHERE_delete(s);
    TUPLES_delete_all(p);
}

void test_normal_on_sphere_at_point_on_nonaxial() {
    SPHERE_Sphere* s = SPHERE_new();
    double sqrt3over3 = sqrt(3.0) / 3.0;
    TUPLES_Point* p = TUPLES_new_point(sqrt3over3, sqrt3over3, sqrt3over3);
    TUPLES_Vector v;
    SPHERE_local_normal_at(&v, s, p);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt3over3, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt3over3, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt3over3, v.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&v));
    SPHERE_delete(s);
    TUPLES_delete_all(p);
}

void test_normal_should_be_normalized_vector() {
    SPHERE_Sphere* s = SPHERE_new();
    double sqrt3over3 = sqrt(3.0) / 3.0;
    TUPLES_Point* p = TUPLES_new_point(sqrt3over3, sqrt3over3, sqrt3over3);
    TUPLES_Vector v;
    SPHERE_local_normal_at(&v, s, p);
    TUPLES_Vector nv;
    TUPLES_copy(&nv, &v);
    TUPLES_normalize(&nv);
    TEST_ASSERT_EQUAL_DOUBLE(nv.x, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(nv.y, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(nv.z, v.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&v));
    SPHERE_delete(s);
    TUPLES_destroy(&nv);
    TUPLES_delete_all(p);
}

void test_compute_normal_on_translated_sphere() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* t = MATRIX_new_translation(0, 1, 0);
    SHAPE_set_transform(&s->parent, t);
    TUPLES_Point* p = TUPLES_new_point(0, 1.70711, -0.70711);
    TUPLES_Point local_point;
    SHAPE_calc_local_point(&local_point, (SHAPE_Shape*) s, p);
    TUPLES_Vector normal;
    SPHERE_local_normal_at(&normal, s, &local_point);
    TUPLES_Vector world_normal;
    SHAPE_calc_world_normal(&world_normal, (SHAPE_Shape*) s, &normal);
    TEST_ASSERT_EQUAL_DOUBLE(0, world_normal.x);
    TEST_ASSERT_EQUAL_DOUBLE(0.70711, world_normal.y);
    TEST_ASSERT_EQUAL_DOUBLE(-0.70711, world_normal.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&world_normal));
    SPHERE_delete(s);
    MATRIX_delete(t);
    TUPLES_delete_all(p);
}

void test_compute_normal_on_transformed_sphere() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* scaling = MATRIX_new_scaling(1, 0.5, 1);
    MATRIX_Matrix* rotation = MATRIX_new_rotation_z(M_PI / 5.0);
    MATRIX_Matrix* transform_m = MATRIX_multiply(scaling, rotation);
    SHAPE_set_transform(&s->parent, transform_m);
    TUPLES_Point* p = TUPLES_new_point(0, sqrt(2.0)/2.0, -sqrt(2.0)/2.0);
    TUPLES_Point local_point;
    SHAPE_calc_local_point(&local_point, (SHAPE_Shape*) s, p);
    TUPLES_Vector normal;
    SPHERE_local_normal_at(&normal, s, &local_point);
    TUPLES_Vector world_normal;
    SHAPE_calc_world_normal(&world_normal, (SHAPE_Shape*) s, &normal);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, world_normal.x);
    TEST_ASSERT_EQUAL_DOUBLE(0.97014, world_normal.y);
    TEST_ASSERT_EQUAL_DOUBLE(-0.242535625, world_normal.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&world_normal));
    SPHERE_delete(s);
    MATRIX_delete_all(scaling, rotation, transform_m);
    TUPLES_delete_all(p);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ray_intersects_sphere_at_two_points);
    RUN_TEST(test_ray_intersects_sphere_at_tangent);
    RUN_TEST(test_ray_misses_sphere);
    RUN_TEST(test_ray_originates_inside_sphere);
    RUN_TEST(test_sphere_behind_ray);
    RUN_TEST(test_ray_sets_object_on_intersection);
    RUN_TEST(test_sphere_default_transformation);
    RUN_TEST(test_sphere_modifying_transformation);
    RUN_TEST(test_sphere_intersect_scaled_ray);
    RUN_TEST(test_sphere_intersect_translated_ray);
    RUN_TEST(test_normal_on_sphere_at_point_on_x_axis);
    RUN_TEST(test_normal_on_sphere_at_point_on_y_axis);
    RUN_TEST(test_normal_on_sphere_at_point_on_z_axis);
    RUN_TEST(test_normal_on_sphere_at_point_on_nonaxial);
    RUN_TEST(test_normal_should_be_normalized_vector);
    RUN_TEST(test_compute_normal_on_translated_sphere);
    RUN_TEST(test_compute_normal_on_transformed_sphere);
    return UNITY_END();
}
