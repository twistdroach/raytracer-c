#include <unity.h>
#include <group.h>
#include <math.h>
#include <sphere.h>

#include "shape.h"
#include "testshape.h"

#include "testutils.h"


void setUp() {}
void tearDown() {}

void test_build_a_test_shape() {
    TESTSHAPE_TestShape* shape = TESTSHAPE_new();
    TEST_ASSERT_EQUAL_DOUBLE(5.0, shape->size);
    TESTSHAPE_delete(shape);
}

void test_shape_default_transformation() {
    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATRIX_Matrix *identity_m = MATRIX_new_identity(4);
    TEST_ASSERT_TRUE(MATRIX_is_equal(identity_m,((SHAPE_Shape*)s)->transform));
    TESTSHAPE_delete(s);
    MATRIX_delete(identity_m);
}

void test_shape_modifying_transformation() {
    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATRIX_Matrix* t = MATRIX_new_translation(2, 3, 4);
    SHAPE_set_transform((SHAPE_Shape*)s, t);
    TEST_ASSERT_TRUE(MATRIX_is_equal(t, ((SHAPE_Shape *)s)->transform));
    TESTSHAPE_delete(s);
    MATRIX_delete(t);
}

void test_shape_has_a_default_material() {
    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATERIAL_Material* m = MATERIAL_new();
    TEST_ASSERT_TRUE(MATERIAL_is_equal(((SHAPE_Shape *)s)->material, m));
    MATERIAL_delete(m);
    TESTSHAPE_delete(s);
}

void test_shape_may_be_assigned_material() {
    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATERIAL_Material* m = MATERIAL_new();
    m->ambient = 1;
    SHAPE_set_material((SHAPE_Shape*)s, m);
    TEST_ASSERT_TRUE(MATERIAL_is_equal(m, ((SHAPE_Shape*)s)->material));
    TESTSHAPE_delete(s);
    MATERIAL_delete(m);
}

void test_shapeholder_intersect_scaled_shape_with_ray() {
    RAY_Ray* r = RAY_new(0,0,-5,0,0,1);

    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATRIX_Matrix* transform = MATRIX_new_scaling(2, 2, 2);
    SHAPE_set_transform((SHAPE_Shape*)s, transform);

    RAY_Intersections* intersections = RAY_new_intersections();
    SHAPE_intersect(intersections, (SHAPE_Shape*)s, r);
    RAY_delete_intersections(intersections);

    TUPLES_Point* expected_origin = TUPLES_new_point(0, 0, -2.5);
    TUPLES_Vector* expected_vector = TUPLES_new_vector(0, 0, 0.5);
    test_tuples(expected_origin, &s->saved_ray.origin);
    test_tuples(expected_vector, &s->saved_ray.direction);

    TESTSHAPE_delete(s);
    MATRIX_delete(transform);
    TUPLES_delete_all(expected_origin, expected_vector);
    RAY_delete(r);
}

void test_shapeholder_intersect_translated_shape_with_ray() {
    RAY_Ray* r = RAY_new(0,0,-5,0,0,1);

    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATRIX_Matrix* transform = MATRIX_new_translation(5, 0, 0);
    SHAPE_set_transform((SHAPE_Shape*)s, transform);
    MATRIX_delete(transform);

    RAY_Intersections* intersections = RAY_new_intersections();
    SHAPE_intersect(intersections,(SHAPE_Shape*) s, r);
    RAY_delete_intersections(intersections);

    TUPLES_Point* expected_origin = TUPLES_new_point(-5, 0, -5);
    TUPLES_Vector* expected_vector = TUPLES_new_vector(0, 0, 1);
    test_tuples(expected_origin, &s->saved_ray.origin);
    test_tuples(expected_vector, &s->saved_ray.direction);

    TESTSHAPE_delete(s);
    TUPLES_delete_all(expected_origin, expected_vector);
    RAY_delete(r);
}

void test_compute_normal_on_translated_shape() {
    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATRIX_Matrix* transform = MATRIX_new_translation(0, 1, 0);
    SHAPE_set_transform((SHAPE_Shape*)s, transform);
    MATRIX_delete(transform);

    TUPLES_Point hitpoint;
    TUPLES_init_point(&hitpoint, 0, 1.70711, -0.70711);

    TUPLES_Vector world_normal;
    SHAPE_normal_at(&world_normal, (SHAPE_Shape*)s, &hitpoint);

    TUPLES_Vector expected;
    TUPLES_init_vector(&expected, 0, 0.70711, -0.70711);
    test_tuples(&expected, &world_normal);

    TESTSHAPE_delete(s);
}

void test_compute_normal_on_transformed_shape() {
    TESTSHAPE_TestShape *s = TESTSHAPE_new();
    MATRIX_Matrix* scaling = MATRIX_new_scaling(1, 0.5, 1);
    MATRIX_Matrix* rotationz = MATRIX_new_rotation_z(M_PI / 5.0);
    MATRIX_Matrix* transform = MATRIX_multiply(scaling, rotationz);
    SHAPE_set_transform((SHAPE_Shape*)s, transform);
    MATRIX_delete_all(transform, rotationz, scaling);

    TUPLES_Point hitpoint;
    TUPLES_init_point(&hitpoint, 0, sqrt(2)/2.0, -sqrt(2)/2.0);

    TUPLES_Vector world_normal;
    SHAPE_normal_at(&world_normal, (SHAPE_Shape*)s, &hitpoint);

    TUPLES_Vector expected;
    TUPLES_init_vector(&expected, 0, 0.97014, -0.24254);
    test_tuples(&expected, &world_normal);

    TESTSHAPE_delete(s);
}

void test_shape_has_parent_attribute() {
    TESTSHAPE_TestShape* ts = TESTSHAPE_new();
    TEST_ASSERT_NULL(SHAPE_get_parent(ts));
    TESTSHAPE_delete(ts);
}

void test_convert_point_from_world_to_object_space() {
    GROUP_Group* g1 = GROUP_new();
    MATRIX_Matrix* g1_transform = MATRIX_new_rotation_y(M_PI_2);
    GROUP_set_transform(g1, g1_transform);

    GROUP_Group* g2 = GROUP_new();
    MATRIX_Matrix* g2_transform = MATRIX_new_scaling(2, 2, 2);
    GROUP_set_transform(g2, g2_transform);

    GROUP_add_child(g1, g2);

    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* s_transform = MATRIX_new_translation(5, 0, 0);
    SPHERE_set_transform(s, s_transform);

    GROUP_add_child(g2, s);

    TUPLES_Point world_point, expected, result;
    TUPLES_init_point(&world_point, -2, 0, -10);
    SHAPE_world_to_object(&result, s, &world_point);

    TUPLES_init_point(&expected, 0, 0, -1);
    test_tuples(&expected, &result);

    MATRIX_delete_all(g1_transform, g2_transform, s_transform);
    GROUP_delete(g1);
    GROUP_delete(g2);
    SPHERE_delete(s);
}

void test_convert_normal_from_object_to_world_space() {
    GROUP_Group* g1 = GROUP_new();
    MATRIX_Matrix* g1_transform = MATRIX_new_rotation_y(M_PI_2);
    GROUP_set_transform(g1, g1_transform);

    GROUP_Group* g2 = GROUP_new();
    MATRIX_Matrix* g2_transform = MATRIX_new_scaling(1, 2, 3);
    GROUP_set_transform(g2, g2_transform);

    GROUP_add_child(g1, g2);

    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* s_transform = MATRIX_new_translation(5, 0, 0);
    SPHERE_set_transform(s, s_transform);

    GROUP_add_child(g2, s);

    TUPLES_Vector world_vector, expected, result;
    TUPLES_init_vector(&world_vector, sqrt(3)/3, sqrt(3)/3, sqrt(3)/3);
    TUPLES_init_vector(&expected, 0.2857, 0.4286, -0.8571);

    SHAPE_normal_to_world(&result, s, &world_vector);
    test_tuples(&expected, &result);

    MATRIX_delete_all(g1_transform, g2_transform, s_transform);
    GROUP_delete(g1);
    GROUP_delete(g2);
    SPHERE_delete(s);
}

void test_finding_normal_on_child_object() {
    GROUP_Group* g1 = GROUP_new();
    MATRIX_Matrix* g1_transform = MATRIX_new_rotation_y(M_PI_2);
    GROUP_set_transform(g1, g1_transform);

    GROUP_Group* g2 = GROUP_new();
    MATRIX_Matrix* g2_transform = MATRIX_new_scaling(1, 2, 3);
    GROUP_set_transform(g2, g2_transform);

    GROUP_add_child(g1, g2);

    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* s_transform = MATRIX_new_translation(5, 0, 0);
    SPHERE_set_transform(s, s_transform);

    GROUP_add_child(g2, s);

    TUPLES_Point point;
    TUPLES_init_point(&point, 1.7321, 1.1547, -5.5774);
    TUPLES_Vector expected, result;
    TUPLES_init_vector(&expected, 0.2857, 0.4286, -0.8571);

    SHAPE_normal_at(&result, s, &point);
    test_tuples(&expected, &result);

    MATRIX_delete_all(g1_transform, g2_transform, s_transform);
    GROUP_delete(g1);
    GROUP_delete(g2);
    SPHERE_delete(s);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_build_a_test_shape);
    RUN_TEST(test_shape_default_transformation);
    RUN_TEST(test_shape_modifying_transformation);
    RUN_TEST(test_shape_has_a_default_material);
    RUN_TEST(test_shape_may_be_assigned_material);
    RUN_TEST(test_shapeholder_intersect_scaled_shape_with_ray);
    RUN_TEST(test_shapeholder_intersect_translated_shape_with_ray);
    RUN_TEST(test_compute_normal_on_translated_shape);
    RUN_TEST(test_compute_normal_on_transformed_shape);
    RUN_TEST(test_shape_has_parent_attribute);
    RUN_TEST(test_convert_point_from_world_to_object_space);
    RUN_TEST(test_convert_normal_from_object_to_world_space);
    RUN_TEST(test_finding_normal_on_child_object);
    return UNITY_END();
}
