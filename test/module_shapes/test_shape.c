#include <unity.h>

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
    return UNITY_END();
}
