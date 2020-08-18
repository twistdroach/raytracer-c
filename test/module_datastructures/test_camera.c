#include <unity.h>
#include <tuples.h>
#include <camera.h>

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
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_transformation_matrix_for_default_orientation);
    RUN_TEST(test_transformation_matrix_when_looking_in_positive_z_direction);
    RUN_TEST(test_transformation_moves_the_world);
    RUN_TEST(test_arbitrary_view_transformation);
    RUN_TEST(test_construct_a_camera);
    UNITY_END();
}