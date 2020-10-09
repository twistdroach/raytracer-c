#include <unity.h>
#include <matrix.h>
#include <testshape.h>
#include <sphere.h>
#include <cylinder.h>

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

void test_group_has_bounding_box_that_contains_all_children() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* s_translation = MATRIX_new_translation(2, 5, -3);
    MATRIX_Matrix* s_scaling = MATRIX_new_scaling(2, 2, 2);
    MATRIX_Matrix* s_transform = MATRIX_multiply_many(s_translation, s_scaling);
    SPHERE_set_transform(s, s_transform);

    CYLINDER_Cylinder* c = CYLINDER_new();
    c->minimum = -2;
    c->maximum = 2;
    MATRIX_Matrix* c_translation = MATRIX_new_translation(-4, -1, 4);
    MATRIX_Matrix* c_scaling = MATRIX_new_scaling(0.5, 1, 0.5);
    MATRIX_Matrix* c_transform = MATRIX_multiply_many(c_translation, c_scaling);
    CYLINDER_set_transform(c, c_transform);

    GROUP_Group* g = GROUP_new();
    GROUP_add_child(g, s);
    GROUP_add_child(g, c);

    BOUND_Box box;
    SHAPE_bounds_of(g, &box);

    TEST_ASSERT_EQUAL_DOUBLE(-4.5, box.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-3, box.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-5, box.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(4, box.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(7, box.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(4.5, box.max.z);

    MATRIX_delete_all(s_translation, s_transform, s_scaling,
                      c_transform, c_translation, c_scaling);
    GROUP_delete(g);
}

void test_intersect_ray_and_group_doesnt_test_children_if_box_is_missed() {
    TESTSHAPE_TestShape* child = TESTSHAPE_new();
    GROUP_Group* g = GROUP_new();
    GROUP_add_child(g, child);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 1, 0);

    RAY_Intersections* xs = RAY_new_intersections();
    SHAPE_intersect(xs, (SHAPE_Shape*)g, &ray);
    TEST_ASSERT_FALSE(child->ray_set);
    RAY_delete_intersections(xs);
    GROUP_delete(g);
}

void test_intersect_ray_and_group_test_children_if_box_is_hit() {
    TESTSHAPE_TestShape* child = TESTSHAPE_new();
    GROUP_Group* g = GROUP_new();
    GROUP_add_child(g, child);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    SHAPE_intersect(xs, (SHAPE_Shape*)g, &ray);
    TEST_ASSERT_TRUE(child->ray_set);
    RAY_delete_intersections(xs);
    GROUP_delete(g);
}

void test_partition_groups_children() {
    SPHERE_Sphere* s1 = SPHERE_new();
    MATRIX_Matrix* s1_trans = MATRIX_new_translation(-2, 0, 0);
    SPHERE_set_transform(s1, s1_trans);
    SPHERE_Sphere* s2 = SPHERE_new();
    MATRIX_Matrix* s2_trans = MATRIX_new_translation(2, 0, 0);
    SPHERE_set_transform(s2, s2_trans);
    SPHERE_Sphere* s3 = SPHERE_new();
    GROUP_Group* g = GROUP_new();
    GROUP_add_child(g, s1);
    GROUP_add_child(g, s2);
    GROUP_add_child(g, s3);
    GROUP_Group* left_group = GROUP_new();
    GROUP_Group* right_group = GROUP_new();
    GROUP_partition_children(g, left_group, right_group);
    TEST_ASSERT_TRUE(GROUP_contains(g, s3));
    TEST_ASSERT_TRUE(GROUP_contains(left_group, s1));
    TEST_ASSERT_TRUE(GROUP_contains(right_group, s2));
    MATRIX_delete_all(s1_trans, s2_trans);
    GROUP_delete(left_group);
    GROUP_delete(right_group);
    GROUP_delete(g);
}

void test_subdividing_group_partition_its_children() {
    SPHERE_Sphere* s1 = SPHERE_new();
    MATRIX_Matrix* s1_trans = MATRIX_new_translation(-2, 0, 0);
    SPHERE_set_transform(s1, s1_trans);
    SPHERE_Sphere* s2 = SPHERE_new();
    MATRIX_Matrix* s2_trans = MATRIX_new_translation(2, 0, 0);
    SPHERE_set_transform(s2, s2_trans);
    SPHERE_Sphere* s3 = SPHERE_new();
    MATRIX_Matrix* s3_trans = MATRIX_new_scaling(4, 4, 4);
    SPHERE_set_transform(s3, s3_trans);
    GROUP_Group* g = GROUP_new();
    GROUP_add_child(g, s1);
    GROUP_add_child(g, s2);
    GROUP_add_child(g, s3);
    SHAPE_divide((SHAPE_Shape*)g, 1);
    //TODO - decide on a good way to tell if we have group or shape?
    MATRIX_delete_all(s1_trans, s2_trans, s3_trans);
    GROUP_delete(g);
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
    RUN_TEST(test_group_has_bounding_box_that_contains_all_children);
    RUN_TEST(test_intersect_ray_and_group_doesnt_test_children_if_box_is_missed);
    RUN_TEST(test_intersect_ray_and_group_test_children_if_box_is_hit);
    RUN_TEST(test_partition_groups_children);
    RUN_TEST(test_subdividing_group_partition_its_children);
    return UNITY_END();
}