#include <unity.h>
#include <matrix.h>
#include <testshape.h>
#include <sphere.h>

#include "group.h"

void setUp() {}
void tearDown() {}

void test_create_group() {
    GROUP_Group* g = GROUP_new();
    MATRIX_Matrix* identity_matrix = MATRIX_new_identity(4);
    TEST_ASSERT_TRUE(MATRIX_is_equal(identity_matrix, GROUP_get_transform(g)));
    GROUP_delete(g);
    MATRIX_delete(identity_matrix);
}

void test_adding_child_to_group() {
    GROUP_Group* g = GROUP_new();
    TESTSHAPE_TestShape* s = TESTSHAPE_new();
    GROUP_add_child(g, s);
    TEST_ASSERT_FALSE(GROUP_is_empty(g));
    TEST_ASSERT_TRUE(GROUP_contains(g, s));
    TEST_ASSERT_EQUAL_PTR(g, SHAPE_get_parent(s));
    GROUP_delete(g);
}

void test_setting_group_materials() {
    GROUP_Group* g = GROUP_new();
    TESTSHAPE_TestShape* s = TESTSHAPE_new();
    MATERIAL_Material* mat = MATERIAL_new();
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.2, 0.4, 0.6);
    TUPLES_copy(&mat->color, &expected);
    GROUP_add_child(g, s);
    GROUP_set_material(g, mat);
    MATERIAL_delete(mat);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, &SHAPE_get_material((SHAPE_Shape*)s)->color));
    GROUP_delete(g);
}

void test_intersect_ray_with_an_empty_group() {
    GROUP_Group* g = GROUP_new();
    RAY_Ray ray;
    RAY_init(&ray, 0, 0, 0, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    GROUP_local_intersect(xs, (SHAPE_Shape*)g, &ray);

    TEST_ASSERT_EQUAL(0, xs->count);

    RAY_delete_intersections(xs);
    GROUP_delete(g);
}

void test_intersect_ray_with_nonempty_group() {
    GROUP_Group* g = GROUP_new();
    SPHERE_Sphere* s1 = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    MATRIX_Matrix* s2_transform = MATRIX_new_translation(0, 0, -3);
    SPHERE_set_transform(s2, s2_transform);
    SPHERE_Sphere* s3 = SPHERE_new();
    MATRIX_Matrix* s3_transform = MATRIX_new_translation(5, 0, 0);
    SPHERE_set_transform(s3, s3_transform);
    MATRIX_delete_all(s2_transform, s3_transform);

    GROUP_add_child(g, s1);
    GROUP_add_child(g, s2);
    GROUP_add_child(g, s3);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    GROUP_local_intersect(xs, (SHAPE_Shape*)g, &ray);

    TEST_ASSERT_EQUAL(4, xs->count);
    TEST_ASSERT_EQUAL_PTR(s2, xs->xs[0].object);
    TEST_ASSERT_EQUAL_PTR(s2, xs->xs[1].object);
    TEST_ASSERT_EQUAL_PTR(s1, xs->xs[2].object);
    TEST_ASSERT_EQUAL_PTR(s1, xs->xs[3].object);

    RAY_delete_intersections(xs);
    GROUP_delete(g);
}

void test_intersecting_a_transformed_group() {
    GROUP_Group* group = GROUP_new();
    MATRIX_Matrix* group_transform = MATRIX_new_scaling(2, 2, 2);
    GROUP_set_transform(group, group_transform);
    MATRIX_delete(group_transform);

    SPHERE_Sphere* sphere = SPHERE_new();
    MATRIX_Matrix* sphere_transform = MATRIX_new_translation(5, 0, 0);
    SPHERE_set_transform(sphere, sphere_transform);
    MATRIX_delete(sphere_transform);

    GROUP_add_child(group, sphere);

    RAY_Ray ray;
    RAY_init(&ray, 10, 0, -10, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    SHAPE_intersect(xs, (SHAPE_Shape*)group, &ray);

    TEST_ASSERT_EQUAL(2, xs->count);

    RAY_delete_intersections(xs);
    GROUP_delete(group);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_group);
    RUN_TEST(test_adding_child_to_group);
    RUN_TEST(test_intersect_ray_with_an_empty_group);
    RUN_TEST(test_intersect_ray_with_nonempty_group);
    RUN_TEST(test_intersecting_a_transformed_group);
    RUN_TEST(test_setting_group_materials);
    return UNITY_END();
}
