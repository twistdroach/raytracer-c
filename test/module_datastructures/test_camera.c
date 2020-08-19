#include <unity.h>
#include <tuples.h>
#include <camera.h>
#include <ray.h>
#include <canvas.h>
#include "test_world_utils.h"

void setUp() {}
void tearDown() {}

void test_transformation_matrix_for_default_orientation() {
    TUPLES_Point from, to;
    TUPLES_Vector up;
    TUPLES_init_vector(&from, 0, 0, 0);
    TUPLES_init_vector(&to, 0, 0, -1);
    TUPLES_init_vector(&up, 0, 1, 0);

    MATRIX_Matrix* t = CAMERA_view_transform(&from, &to, &up);
    MATRIX_Matrix* expected = MATRIX_new_identity(4);
    TEST_ASSERT_TRUE(MATRIX_is_equal(expected, t));
    MATRIX_delete_all(t, expected);
}

void test_transformation_matrix_when_looking_in_positive_z_direction() {
    TUPLES_Point from, to;
    TUPLES_Vector up;
    TUPLES_init_vector(&from, 0, 0, 0);
    TUPLES_init_vector(&to, 0, 0, 1);
    TUPLES_init_vector(&up, 0, 1, 0);

    MATRIX_Matrix* t = CAMERA_view_transform(&from, &to, &up);
    MATRIX_Matrix* expected = MATRIX_new_scaling(-1, 1, -1);
    TEST_ASSERT_TRUE(MATRIX_is_equal(expected, t));
    MATRIX_delete_all(t, expected);
}

void test_transformation_moves_the_world() {
    TUPLES_Point from, to;
    TUPLES_Vector up;
    TUPLES_init_vector(&from, 0, 0, 8);
    TUPLES_init_vector(&to, 0, 0, 0);
    TUPLES_init_vector(&up, 0, 1, 0);

    MATRIX_Matrix* t = CAMERA_view_transform(&from, &to, &up);
    MATRIX_Matrix* expected = MATRIX_new_translation(0, 0, -8);
    TEST_ASSERT_TRUE(MATRIX_is_equal(expected, t));
    MATRIX_delete_all(t, expected);
}

void test_arbitrary_view_transformation() {
    TUPLES_Point from, to;
    TUPLES_Vector up;
    TUPLES_init_vector(&from, 1, 3, 2);
    TUPLES_init_vector(&to, 4, -2, 8);
    TUPLES_init_vector(&up, 1, 1, 0);

    MATRIX_Matrix* t = CAMERA_view_transform(&from, &to, &up);
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected, -0.50709, 0.50709, 0.67612, -2.36643,
                           0.76772, 0.60609, 0.12122, -2.82842,
                          -0.35857, 0.59761, -0.71714, 0.0,
                           0.0    , 0.0,     0.0,      1.0);
    TEST_ASSERT_TRUE(MATRIX_is_equal(expected, t));
    MATRIX_delete_all(t, expected);
}

void test_construct_a_camera() {
    CAMERA_Camera* camera = CAMERA_new(160, 120, M_PI_2);
    TEST_ASSERT_NOT_NULL(camera);
    TEST_ASSERT_EQUAL_DOUBLE(160, camera->hsize);
    TEST_ASSERT_EQUAL_DOUBLE(120, camera->vsize);
    TEST_ASSERT_EQUAL_DOUBLE(M_PI_2, camera->field_of_view);
    MATRIX_Matrix* expected = MATRIX_new_identity(4);
    TEST_ASSERT_TRUE(MATRIX_is_equal(expected, camera->transform));
    CAMERA_delete(camera);
    MATRIX_delete(expected);
}

void test_pixel_size_for_horizontal_canvas() {
    CAMERA_Camera* camera = CAMERA_new(200, 125, M_PI_2);
    TEST_ASSERT_EQUAL_DOUBLE(0.01, camera->pixel_size);
    CAMERA_delete(camera);
}

void test_pixel_size_for_vertical_canvas() {
    CAMERA_Camera* camera = CAMERA_new(125, 200, M_PI_2);
    TEST_ASSERT_EQUAL_DOUBLE(0.01, camera->pixel_size);
    CAMERA_delete(camera);
}

void test_get_ray_through_center_of_canvas() {
    CAMERA_Camera* camera = CAMERA_new(201, 101, M_PI_2);
    RAY_Ray r;
    CAMERA_ray_for_pixel(&r, camera, 100, 50);

    TUPLES_Point origin_expected;
    TUPLES_Vector direction_expected;
    TUPLES_init_point(&origin_expected, 0, 0, 0);
    TUPLES_init_vector(&direction_expected, 0, 0, -1);

    TEST_ASSERT_TRUE(TUPLES_is_equal(&origin_expected, &r.origin));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&direction_expected, &r.direction));
    CAMERA_delete(camera);
}

void test_get_ray_through_corner_of_canvas() {
    CAMERA_Camera* camera = CAMERA_new(201, 101, M_PI_2);
    RAY_Ray r;
    CAMERA_ray_for_pixel(&r, camera, 0, 0);

    TUPLES_Point origin_expected;
    TUPLES_Vector direction_expected;
    TUPLES_init_point(&origin_expected, 0, 0, 0);
    TUPLES_init_vector(&direction_expected, 0.66519, 0.33259, -0.66851);

    TEST_ASSERT_TRUE(TUPLES_is_equal(&origin_expected, &r.origin));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&direction_expected, &r.direction));
    CAMERA_delete(camera);
}

void test_get_ray_when_camera_is_transformed() {
    CAMERA_Camera* camera = CAMERA_new(201, 101, M_PI_2);
    MATRIX_Matrix* rotation = MATRIX_new_rotation_y(M_PI_4);
    MATRIX_Matrix* translation = MATRIX_new_translation(0, -2, 5);
    MATRIX_Matrix* transform = MATRIX_multiply(rotation, translation);
    CAMERA_set_transform(camera, transform);
    RAY_Ray r;
    CAMERA_ray_for_pixel(&r, camera, 100, 50);

    TUPLES_Point origin_expected;
    TUPLES_Vector direction_expected;
    TUPLES_init_point(&origin_expected, 0, 2, -5);
    TUPLES_init_vector(&direction_expected, sqrt(2)/2.0, 0, -sqrt(2)/2.0);

    TEST_ASSERT_TRUE(TUPLES_is_equal(&origin_expected, &r.origin));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&direction_expected, &r.direction));
    MATRIX_delete_all(rotation, translation, transform);
    CAMERA_delete(camera);
}

void test_redering_world_with_a_camera() {
    WORLD_World* world = construct_test_world();
    CAMERA_Camera* camera = CAMERA_new(11, 11, M_PI_2);
    TUPLES_Point from, to;
    TUPLES_Vector up;
    TUPLES_init_point(&from, 0, 0, -5);
    TUPLES_init_point(&to, 0, 0, 0);
    TUPLES_init_vector(&up, 0, 1, 0);

    MATRIX_Matrix* transform = CAMERA_view_transform(&from, &to, &up);
    CAMERA_set_transform(camera, transform);
    MATRIX_delete(transform);

    CANVAS_Canvas* canvas = CAMERA_render(camera, world);

    TUPLES_Color* c = CANVAS_read_pixel(canvas, 5, 5);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.38066, 0.47583, 0.2855);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, c));
    TUPLES_delete(c);

    destruct_test_world(world);
    CAMERA_delete(camera);
    CANVAS_delete(canvas);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_transformation_matrix_for_default_orientation);
    RUN_TEST(test_transformation_matrix_when_looking_in_positive_z_direction);
    RUN_TEST(test_transformation_moves_the_world);
    RUN_TEST(test_arbitrary_view_transformation);
    RUN_TEST(test_construct_a_camera);
    RUN_TEST(test_pixel_size_for_horizontal_canvas);
    RUN_TEST(test_pixel_size_for_vertical_canvas);
    RUN_TEST(test_get_ray_through_center_of_canvas);
    RUN_TEST(test_get_ray_through_corner_of_canvas);
    RUN_TEST(test_get_ray_when_camera_is_transformed);
    UNITY_END();
}
