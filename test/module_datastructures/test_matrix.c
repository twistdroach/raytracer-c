#define UNITY_DOUBLE_PRECISION 0.00001f
#include <unity.h>
#include <matrix.h>
#include <tuples.h>

void setUp() {}
void tearDown() {}

void test_construct_4x4_matrix()
{
    MATRIX_Matrix* m = MATRIX_new(4, 4);
    MATRIX_fill(m, 1.0, 2.0, 3.0, 4.0,
                5.5, 6.5, 7.5, 8.5,
                9.0, 10.0, 11.0, 12.0,
                13.5, 14.5, 15.5, 16.5);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, MATRIX_read_cell(m, 0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(4, MATRIX_read_cell(m, 0, 3));
    TEST_ASSERT_EQUAL_DOUBLE(5.5, MATRIX_read_cell(m, 1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(7.5, MATRIX_read_cell(m, 1, 2));
    TEST_ASSERT_EQUAL_DOUBLE(11, MATRIX_read_cell(m, 2, 2));
    TEST_ASSERT_EQUAL_DOUBLE(13.5, MATRIX_read_cell(m, 3, 0));
    TEST_ASSERT_EQUAL_DOUBLE(15.5, MATRIX_read_cell(m, 3, 2));
    MATRIX_delete(m);
}

void test_construct_2x2_matrix()
{
    MATRIX_Matrix m;
    MATRIX_init(&m, 2, 2);
    MATRIX_fill(&m, -3.0, 5.0, 1.0, -2.0);
    TEST_ASSERT_EQUAL_DOUBLE(-3, MATRIX_read_cell(&m, 0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(5, MATRIX_read_cell(&m, 0, 1));
    TEST_ASSERT_EQUAL_DOUBLE(1, MATRIX_read_cell(&m, 1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-2, MATRIX_read_cell(&m, 1, 1));
    MATRIX_destroy(&m);
}

void test_construct_3x3_matrix()
{
    MATRIX_Matrix m;
    MATRIX_init(&m, 3, 3);
    MATRIX_fill(&m, -3.0, 5.0, 0.0,
                1.0, -2.0, -7.0,
                0.0, 1.0, 1.0);
    TEST_ASSERT_EQUAL_DOUBLE(-3, MATRIX_read_cell(&m, 0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-2, MATRIX_read_cell(&m, 1, 1));
    TEST_ASSERT_EQUAL_DOUBLE(1, MATRIX_read_cell(&m, 2, 2));
    MATRIX_destroy(&m);
}

void test_is_equal_same_matrix()
{
    MATRIX_Matrix m1, m2;
    MATRIX_init(&m1, 4, 4);
    MATRIX_init(&m2, 4, 4);
    MATRIX_fill(&m1, 1.0, 2.0, 3.0, 4.0,
                5.0, 6.0, 7.0, 8.0,
                9.0, 8.0, 7.0, 6.0,
                5.0, 4.0, 3.0, 2.0);
    MATRIX_fill(&m2, 1.0, 2.0, 3.0, 4.0,
                5.0, 6.0, 7.0, 8.0,
                9.0, 8.0, 7.0, 6.0,
                5.0, 4.0, 3.0, 2.0);
    TEST_ASSERT_TRUE(MATRIX_is_equal(&m1, &m2));
    MATRIX_destroy(&m1);
    MATRIX_destroy(&m2);
}

void test_is_equal_not_same_matrix() {
    MATRIX_Matrix m1, m2;
    MATRIX_init(&m1, 4, 4);
    MATRIX_init(&m2, 4, 4);
    MATRIX_fill(&m1, 1.0, 2.0, 3.0, 4.0,
                5.0, 6.0, 7.0, 8.0,
                9.0, 8.0, 7.0, 6.0,
                5.0, 4.0, 3.0, 2.0);
    MATRIX_fill(&m2, 1.0, 2.0, 3.0, 4.0,
                5.0, 6.0, 7.0, 8.0,
                9.0, 8.0, 7.0, 6.0,
                5.0, 4.0, 3.0, 2.0);
    TEST_ASSERT_TRUE(MATRIX_is_equal(&m1, &m2));
    MATRIX_destroy(&m1);
    MATRIX_destroy(&m2);
}

void test_copy() {
    MATRIX_Matrix m1, m2;
    MATRIX_init(&m1, 4, 4);
    MATRIX_fill(&m1, 1.0, 2.0, 3.0, 4.0,
                5.0, 6.0, 7.0, 8.0,
                9.0, 8.0, 7.0, 6.0,
                5.0, 4.0, 3.0, 2.0);
    MATRIX_copy(&m2, &m1);
    TEST_ASSERT_TRUE(MATRIX_is_equal(&m1, &m2));
    MATRIX_destroy(&m1);
    MATRIX_destroy(&m2);
}

void test_multiply() {
    MATRIX_Matrix m1, m2, m3;
    MATRIX_init(&m1, 4, 4);
    MATRIX_init(&m2, 4, 4);
    MATRIX_fill(&m1, 1.0, 2.0, 3.0, 4.0,
                5.0, 6.0, 7.0, 8.0,
                9.0, 8.0, 7.0, 6.0,
                5.0, 4.0, 3.0, 2.0);
    MATRIX_fill(&m2, -2.0, 1.0, 2.0, 3.0,
                3.0, 2.0, 1.0, -1.0,
                4.0, 3.0, 6.0, 5.0,
                1.0, 2.0, 7.0, 8.0);
   MATRIX_multiply(&m3, &m1, &m2);

   MATRIX_Matrix expected;
   MATRIX_init(&expected, 4, 4);
   MATRIX_fill(&expected, 20.0, 22.0, 50.0, 48.0,
               44.0, 54.0, 114.0, 108.0,
               40.0, 58.0, 110.0, 102.0,
               16.0, 26.0, 46.0, 42.0);
   TEST_ASSERT_TRUE(MATRIX_is_equal(&expected, &m3));
   MATRIX_destroy(&m1);
   MATRIX_destroy(&m2);
   MATRIX_destroy(&m3);
   MATRIX_destroy(&expected);
}

void test_multiply_matrix_by_tuple()
{
    MATRIX_Matrix m;
    MATRIX_init(&m, 4, 4);
    MATRIX_fill(&m, 1.0, 2.0, 3.0, 4.0,
                    2.0, 4.0, 4.0, 2.0,
                    8.0, 6.0, 4.0, 1.0,
                    0.0, 0.0, 0.0, 1.0);
    TUPLES_Point p;
    TUPLES_init_point(&p, 1.0, 2.0, 3.0);
    TUPLES_Tuple result;
    MATRIX_multiply_tuple(&result, &m, &p);
    TEST_ASSERT_EQUAL_DOUBLE(18, result.x);
    TEST_ASSERT_EQUAL_DOUBLE(24, result.y);
    TEST_ASSERT_EQUAL_DOUBLE(33, result.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&result));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_construct_4x4_matrix);
    RUN_TEST(test_construct_2x2_matrix);
    RUN_TEST(test_construct_3x3_matrix);
    RUN_TEST(test_is_equal_not_same_matrix);
    RUN_TEST(test_is_equal_same_matrix);
    RUN_TEST(test_copy);
    RUN_TEST(test_multiply);
    RUN_TEST(test_multiply_matrix_by_tuple);
    return UNITY_END();
}
