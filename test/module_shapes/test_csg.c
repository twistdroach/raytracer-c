#include <unity.h>
#include <csg.h>
#include <sphere.h>
#include <cube.h>

void setUp() {}
void tearDown() {}

void test_create_csg() {
    SPHERE_Sphere* s = SPHERE_new();
    CUBE_Cube* c = CUBE_new();
    CSG_Csg* csg = CSG_new(CSG_Union, s, c);

    TEST_ASSERT_EQUAL(CSG_Union, csg->operation);
    TEST_ASSERT_EQUAL_PTR(c, csg->right);
    TEST_ASSERT_EQUAL_PTR(s, csg->left);
    TEST_ASSERT_EQUAL_PTR(csg, SHAPE_get_parent(s));
    TEST_ASSERT_EQUAL_PTR(csg, SHAPE_get_parent(c));

    SPHERE_delete(s);
    CUBE_delete(c);
    CSG_delete(csg);
}

void test_csg_intersection_allowed(CSG_Operation op, bool lhit, bool inl, bool inr, bool expected) {
    bool result = CSG_intersection_allowed(op, lhit, inl, inr);
    if (expected != result) {
        printf("Testing intersection allowed op(%u) lhit(%d) inl(%d) inr(%d) expected(%d) got(%d)\n", op, lhit, inl, inr, expected, result);
        TEST_FAIL();
    }
}

void test_csg_intersection_allowed_union_op() {
    test_csg_intersection_allowed(CSG_Union, true, true, true, false);
    test_csg_intersection_allowed(CSG_Union, true, true, false, true);
    test_csg_intersection_allowed(CSG_Union, true, false, true, false);
    test_csg_intersection_allowed(CSG_Union, true, false, false, true);
    test_csg_intersection_allowed(CSG_Union, false, true, true, false);
    test_csg_intersection_allowed(CSG_Union, false, true, false, false);
    test_csg_intersection_allowed(CSG_Union, false, false, true, true);
    test_csg_intersection_allowed(CSG_Union, false, false, false, true);

    test_csg_intersection_allowed(CSG_Intersection, true, true, true, true);
    test_csg_intersection_allowed(CSG_Intersection, true, true, false, false);
    test_csg_intersection_allowed(CSG_Intersection, true, false, true, true);
    test_csg_intersection_allowed(CSG_Intersection, true, false, false, false);
    test_csg_intersection_allowed(CSG_Intersection, false, true, true, true);
    test_csg_intersection_allowed(CSG_Intersection, false, true, false, true);
    test_csg_intersection_allowed(CSG_Intersection, false, false, true, false);
    test_csg_intersection_allowed(CSG_Intersection, false, false, false, false);

    test_csg_intersection_allowed(CSG_Difference, true, true, true, false);
    test_csg_intersection_allowed(CSG_Difference, true, true, false, true);
    test_csg_intersection_allowed(CSG_Difference, true, false, true, false);
    test_csg_intersection_allowed(CSG_Difference, true, false, false, true);
    test_csg_intersection_allowed(CSG_Difference, false, true, true, true);
    test_csg_intersection_allowed(CSG_Difference, false, true, false, true);
    test_csg_intersection_allowed(CSG_Difference, false, false, true, false);
    test_csg_intersection_allowed(CSG_Difference, false, false, false, false);
}

void filter_list_of_intersections(CSG_Operation op, uint first, uint second) {
    SPHERE_Sphere* s1 = SPHERE_new();
    CUBE_Cube* s2 = CUBE_new();
    CSG_Csg* csg = CSG_new(op, s1, s2);
    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 1, s1);
    RAY_add_intersection(xs, 2, s2);
    RAY_add_intersection(xs, 3, s1);
    RAY_add_intersection(xs, 4, s2);
    RAY_Intersections* filtered_xs = CSG_filter_intersections(csg, xs);
    TEST_ASSERT_EQUAL(2, filtered_xs->count);
    TEST_ASSERT_EQUAL_PTR(xs->xs[first].object, filtered_xs->xs[0].object);
    TEST_ASSERT_EQUAL_DOUBLE(xs->xs[first].t, filtered_xs->xs[0].t);
    TEST_ASSERT_EQUAL_PTR(xs->xs[second].object, filtered_xs->xs[1].object);
    TEST_ASSERT_EQUAL_DOUBLE(xs->xs[second].t, filtered_xs->xs[1].t);
    RAY_delete_intersections(xs);
    RAY_delete_intersections(filtered_xs);
    CSG_delete(csg);
    SPHERE_delete(s1);
    CUBE_delete(s2);
}

void test_filter_list_of_intersections() {
    filter_list_of_intersections(CSG_Union, 0, 3);
    filter_list_of_intersections(CSG_Intersection, 1, 2);
    filter_list_of_intersections(CSG_Difference, 0, 1);
}

void test_ray_misses_a_csg_object() {
    SPHERE_Sphere* s = SPHERE_new();
    CUBE_Cube* c = CUBE_new();
    CSG_Csg* csg = CSG_new(CSG_Union, s, c);

    RAY_Ray ray;
    RAY_init(&ray, 0, 2, -5, 0, 0, 1);
    RAY_Intersections *xs = RAY_new_intersections();

    CSG_local_intersect(xs, (SHAPE_Shape*)csg, &ray);

    TEST_ASSERT_EQUAL(0, xs->count);
    RAY_delete_intersections(xs);
    CSG_delete(csg);
    CUBE_delete(c);
    SPHERE_delete(s);
}

void test_ray_hits_csg_object() {
    SPHERE_Sphere* s1 = SPHERE_new();
    SPHERE_Sphere* s2 = SPHERE_new();
    MATRIX_Matrix* s2_transform = MATRIX_new_translation(0, 0, 0.5);
    SPHERE_set_transform(s2, s2_transform);
    CSG_Csg* csg = CSG_new(CSG_Union, s1, s2);
    MATRIX_delete(s2_transform);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* xs = RAY_new_intersections();

    CSG_local_intersect(xs, (SHAPE_Shape*)csg, &ray);

    TEST_ASSERT_EQUAL(2, xs->count);
    TEST_ASSERT_EQUAL_DOUBLE(4, xs->xs[0].t);
    TEST_ASSERT_EQUAL_PTR(s1, xs->xs[0].object);
    TEST_ASSERT_EQUAL_DOUBLE(6.5, xs->xs[1].t);
    TEST_ASSERT_EQUAL_PTR(s2, xs->xs[1].object);

    RAY_delete_intersections(xs);
    CSG_delete(csg);
    SPHERE_delete_all(s1, s2);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_csg);
    RUN_TEST(test_csg_intersection_allowed_union_op);
    RUN_TEST(test_filter_list_of_intersections);
    RUN_TEST(test_ray_misses_a_csg_object);
    RUN_TEST(test_ray_hits_csg_object);
    return UNITY_END();
}
