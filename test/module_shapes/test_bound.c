#include <unity.h>
#include <bound.h>
#include <matrix.h>
#include <ray.h>

void setUp() {}
void tearDown() {}

void test_create_empty_bounding_box() {
    BOUND_Box* box = BOUND_new();
    TEST_ASSERT_EQUAL_DOUBLE(INFINITY, box->min.x);
    TEST_ASSERT_EQUAL_DOUBLE(INFINITY, box->min.y);
    TEST_ASSERT_EQUAL_DOUBLE(INFINITY, box->min.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&box->min));
    TEST_ASSERT_EQUAL_DOUBLE(-INFINITY, box->max.x);
    TEST_ASSERT_EQUAL_DOUBLE(-INFINITY, box->max.y);
    TEST_ASSERT_EQUAL_DOUBLE(-INFINITY, box->max.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&box->max));
    BOUND_delete(box);
}

void test_adding_points_to_an_empty_box() {
    BOUND_Box* box = BOUND_new();
    TUPLES_Point p1, p2;
    TUPLES_init_point(&p1, -5, 2, 0);
    TUPLES_init_point(&p2, 7, 0, -3);
    BOUND_add_point_from_tuple(box, &p1);
    BOUND_add_point_from_tuple(box, &p2);
    TEST_ASSERT_EQUAL_DOUBLE(-5, box->min.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, box->min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, box->min.z);
    TEST_ASSERT_EQUAL_DOUBLE(7, box->max.x);
    TEST_ASSERT_EQUAL_DOUBLE(2, box->max.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, box->max.z);
    BOUND_delete(box);
}

void test_add_bounding_box_to_another() {
    BOUND_Box box1;
    BOUND_init(&box1);
    BOUND_add_point(&box1, -5, -2, 0);
    BOUND_add_point(&box1, 7, 4, 4);
    BOUND_Box box2;
    BOUND_init(&box2);
    BOUND_add_point(&box2, 8, -7, -2);
    BOUND_add_point(&box2, 14, 2, 8);
    BOUND_add_box(&box1, &box2);

    TEST_ASSERT_EQUAL_DOUBLE(-5, box1.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-7, box1.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-2, box1.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(14, box1.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(4, box1.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(8, box1.max.z);
}

void check_point_in_box(double x, double y, double z, bool expected_result) {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, 5, -2, 0);
    BOUND_add_point(&box, 11, 4, 7);

    TUPLES_Point p;
    TUPLES_init_point(&p, x, y, z);

    bool result = BOUND_contains_point(&box, &p);
    if (result != expected_result) {
        printf("(%f, %f, %f) expected %d got %d\n", x, y, z, expected_result, result);
        TEST_FAIL();
    }
}

void test_see_if_box_contains_point() {
    check_point_in_box(5, -2, 0, true);
    check_point_in_box(11, 4, 7, true);
    check_point_in_box(8, 1, 3, true);
    check_point_in_box(3, 0, 3, false);
    check_point_in_box(8, -4, 3, false);
    check_point_in_box(8, 1, -1, false);
    check_point_in_box(13, 1, 3, false);
    check_point_in_box(8, 5, 3, false);
    check_point_in_box(8, 1, 8, false);
}

void check_box_in_box(double minx, double miny, double minz,
                      double maxx, double maxy, double maxz,
                      bool expected_result) {
    BOUND_Box box1;
    BOUND_init(&box1);
    BOUND_add_point(&box1, -5, -2, 0);
    BOUND_add_point(&box1, 11, 4, 7);

    BOUND_Box box2;
    BOUND_init(&box2);
    BOUND_add_point(&box2, minx, miny, minz);
    BOUND_add_point(&box2, maxx, maxy, maxz);

    bool result = BOUND_contains_box(&box1, &box2);
    if (result != expected_result) {
        printf("(%f, %f, %f)(%f, %f, %f) expected %d got %d\n", minx, miny, minz, maxx, maxy, maxz, expected_result, result);
        TEST_FAIL();
    }
}

void test_check_if_box_contains_another_box() {
    check_box_in_box(5, -2, 0, 11, 4, 7, true);
    check_box_in_box(6, -1, 1, 10, 3, 6, true);
    check_box_in_box(4, -3, -1, 10, 3, 6, false);
    check_box_in_box(6, -1, 1, 12, 5, 8, false);
}

void test_transform_a_bounding_box() {
    BOUND_Box* box = BOUND_new();
    BOUND_add_point(box, -1, -1, -1);
    BOUND_add_point(box, 1, 1, 1);
    MATRIX_Matrix* x_rot = MATRIX_new_rotation_x(M_PI_4);
    MATRIX_Matrix* y_rot = MATRIX_new_rotation_y(M_PI_4);
    MATRIX_Matrix* transformation = MATRIX_multiply_many(x_rot, y_rot);
    BOUND_Box transformed_box;
    BOUND_transform(&transformed_box, box, transformation);

    TEST_ASSERT_EQUAL_DOUBLE(-1.4142, transformed_box.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-1.7071, transformed_box.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-1.7071, transformed_box.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(1.4142, transformed_box.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(1.7071, transformed_box.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(1.7071, transformed_box.max.z);

    BOUND_delete(box);
    MATRIX_delete_all(x_rot, y_rot, transformation);
}

void check_ray_bound_box_intersection_with_box(BOUND_Box* box,
                                    double ox, double oy, double oz,
                                    double dx, double dy, double dz,
                                    bool expected_result) {

    TUPLES_Vector direction;
    TUPLES_init_vector(&direction, dx, dy, dz);
    TUPLES_normalize(&direction);

    TUPLES_Point origin;
    TUPLES_init_point(&origin, ox, oy, oz);

    RAY_Ray ray;
    RAY_init_from_tuples(&ray, &origin, &direction);

    bool result = BOUND_intersect(box, &ray);
    if (result != expected_result) {
        printf("Failed for (%f, %f, %f)(%f, %f, %f).  Expected %d but got %d.\n",
               ox, oy, oz, dx, dy, dz, result, expected_result);
        TEST_FAIL();
    }
}

void check_ray_bound_box_intersection(
        double ox, double oy, double oz,
        double dx, double dy, double dz,
        bool expected_result) {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, -1, -1, -1);
    BOUND_add_point(&box, 1, 1, 1);
    check_ray_bound_box_intersection_with_box(&box, ox, oy, oz, dx, dy, dz, expected_result);
}

void test_intersecting_ray_with_bounding_box_at_the_origin() {
    check_ray_bound_box_intersection(5, 0.5, 0, -1, 0, 0, true);
    check_ray_bound_box_intersection(-5, 0.5, 0, 1, 0, 0, true);
    check_ray_bound_box_intersection(0.5, 5, 0, 0, -1, 0, true);
    check_ray_bound_box_intersection(0.5, -5, 0, 0, 1, 0, true);
    check_ray_bound_box_intersection(0.5, 0, 5, 0, 0, -1, true);
    check_ray_bound_box_intersection(0.5, 0, -5, 0, 0, 1, true);
    check_ray_bound_box_intersection(0, 0.5, 0, 0, 0, 1, true);
    check_ray_bound_box_intersection(-2, 0, 0, 2, 4, 6, false);
    check_ray_bound_box_intersection(0, -2, 0, 6, 2, 4, false);
    check_ray_bound_box_intersection(0, 0, -2, 4, 6, 2, false);
    check_ray_bound_box_intersection(2, 0, 2, 0, 0, -1, false);
    check_ray_bound_box_intersection(0, 2, 2, 0, -1, 0, false);
    check_ray_bound_box_intersection(2, 2, 0, -1, 0, 0, false);
}

void check_ray_non_cube_bound_box_intersection(
        double ox, double oy, double oz,
        double dx, double dy, double dz,
        bool expected_result) {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, 5, -2, 0);
    BOUND_add_point(&box, 11, 4, 7);
    check_ray_bound_box_intersection_with_box(&box, ox, oy, oz, dx, dy, dz, expected_result);
}

void test_intersecting_ray_with_non_cubic_bounding_box() {
    check_ray_non_cube_bound_box_intersection(15, 1, 2, -1, 0, 0, true);
    check_ray_non_cube_bound_box_intersection(-5, -1, 4, 1, 0, 0, true);
    check_ray_non_cube_bound_box_intersection(7, 6, 5, 0, -1, 0, true);
    check_ray_non_cube_bound_box_intersection(9, -5, 6, 0, 1, 0, true);
    check_ray_non_cube_bound_box_intersection(8, 2, 12, 0, 0, -1, true);
    check_ray_non_cube_bound_box_intersection(6, 0, -5, 0, 0, 1, true);
    check_ray_non_cube_bound_box_intersection(8, 1, 3.5, 0, 0, 1, true);
    check_ray_non_cube_bound_box_intersection(9, -1, -8, 2, 4, 6, false);
    check_ray_non_cube_bound_box_intersection(8, 3, -4, 6, 2, 4, false);
    check_ray_non_cube_bound_box_intersection(9, -1, -2, 4, 6, 2, false);
    check_ray_non_cube_bound_box_intersection(4, 0, 9, 0, 0, -1, false);
    check_ray_non_cube_bound_box_intersection(8, 6, -1, 0, -1, 0, false);
    check_ray_non_cube_bound_box_intersection(12, 5, 4, 01, 0, 0, false);
}

void test_splitting_a_perfect_cube() {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, -1, -4, -5);
    BOUND_add_point(&box, 9, 6, 5);

    BOUND_Box left, right;
    BOUND_split(&box, &left, &right);

    TEST_ASSERT_EQUAL_DOUBLE(-1, left.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-4, left.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-5, left.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(4, left.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(6, left.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(5, left.max.z);

    TEST_ASSERT_EQUAL_DOUBLE(4, right.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-4, right.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-5, right.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(9, right.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(6, right.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(5, right.max.z);
}

void test_splitting_an_xwide_box() {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, -1, -2, -3);
    BOUND_add_point(&box, 9, 5.5, 3);

    BOUND_Box left, right;
    BOUND_split(&box, &left, &right);

    TEST_ASSERT_EQUAL_DOUBLE(-1, left.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-2, left.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, left.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(4, left.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(5.5, left.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(3, left.max.z);

    TEST_ASSERT_EQUAL_DOUBLE(4, right.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-2, right.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, right.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(9, right.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(5.5, right.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(3, right.max.z);
}

void test_splitting_an_ywide_box() {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, -1, -2, -3);
    BOUND_add_point(&box, 5, 8, 3);

    BOUND_Box left, right;
    BOUND_split(&box, &left, &right);

    TEST_ASSERT_EQUAL_DOUBLE(-1, left.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-2, left.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, left.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(5, left.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, left.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(3, left.max.z);

    TEST_ASSERT_EQUAL_DOUBLE(-1, right.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, right.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, right.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(5, right.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(8, right.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(3, right.max.z);
}

void test_splitting_an_zwide_box() {
    BOUND_Box box;
    BOUND_init(&box);
    BOUND_add_point(&box, -1, -2, -3);
    BOUND_add_point(&box, 5, 3, 7);

    BOUND_Box left, right;
    BOUND_split(&box, &left, &right);

    TEST_ASSERT_EQUAL_DOUBLE(-1, left.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-2, left.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, left.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(5, left.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, left.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(2, left.max.z);

    TEST_ASSERT_EQUAL_DOUBLE(-1, right.min.x);
    TEST_ASSERT_EQUAL_DOUBLE(-2, right.min.y);
    TEST_ASSERT_EQUAL_DOUBLE(2, right.min.z);
    TEST_ASSERT_EQUAL_DOUBLE(5, right.max.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, right.max.y);
    TEST_ASSERT_EQUAL_DOUBLE(7, right.max.z);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_empty_bounding_box);
    RUN_TEST(test_adding_points_to_an_empty_box);
    RUN_TEST(test_add_bounding_box_to_another);
    RUN_TEST(test_see_if_box_contains_point);
    RUN_TEST(test_check_if_box_contains_another_box);
    RUN_TEST(test_transform_a_bounding_box);
    RUN_TEST(test_intersecting_ray_with_bounding_box_at_the_origin);
    RUN_TEST(test_intersecting_ray_with_non_cubic_bounding_box);
    RUN_TEST(test_splitting_a_perfect_cube);
    RUN_TEST(test_splitting_an_xwide_box);
    RUN_TEST(test_splitting_an_ywide_box);
    RUN_TEST(test_splitting_an_zwide_box);
    return UNITY_END();
}
