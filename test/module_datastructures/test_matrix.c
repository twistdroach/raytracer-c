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
    MATRIX_destroy(&m);
    TUPLES_destroy(&p);
    TUPLES_destroy(&result);
}

void test_identity_matrix()
{
    MATRIX_Matrix m;
    MATRIX_init(&m, 4, 4);
    MATRIX_fill(&m, 0.0, 1.0, 2.0, 4.0,
                    1.0, 2.0, 4.0, 8.0,
                    2.0, 4.0, 8.0, 16.0,
                    4.0, 8.0, 16.0, 32.0);
    MATRIX_Matrix ident;
    MATRIX_init_identity(&ident, 4);
    MATRIX_Matrix result;
    MATRIX_multiply(&result, &m, &ident);
    TEST_ASSERT_TRUE(MATRIX_is_equal(&m, &result));
    MATRIX_destroy(&m);
    MATRIX_destroy(&result);
    MATRIX_destroy(&ident);
}

void test_transpose()
{
    MATRIX_Matrix m;
    MATRIX_init(&m, 4, 4);
    MATRIX_fill(&m, 0.0, 9.0, 3.0, 0.0,
                    9.0, 8.0, 0.0, 8.0,
                    1.0, 8.0, 5.0, 3.0,
                    0.0, 0.0, 5.0, 8.0);
    MATRIX_transpose(&m);
    MATRIX_Matrix expected;
    MATRIX_init(&expected, 4, 4);
    MATRIX_fill(&expected, 0.0, 9.0, 1.0, 0.0,
                           9.0, 8.0, 8.0, 0.0,
                           3.0, 0.0, 5.0, 5.0,
                           0.0, 8.0, 3.0, 8.0);
    if (!MATRIX_is_equal(&expected, &m)) {
        printf("Expected:\n%s\nGot:\n%s\n", MATRIX_to_string(&expected), MATRIX_to_string(&m));
        TEST_FAIL();
    }
    MATRIX_destroy(&m);
    MATRIX_destroy(&expected);
}

void test_transpose_identity()
{
    MATRIX_Matrix ident;
    MATRIX_init_identity(&ident, 4);
    MATRIX_Matrix expected;
    MATRIX_init_identity(&expected, 4);
    if (!MATRIX_is_equal(&expected, &ident)) {
        printf("Expected:\n%s\nGot:\n%s\n", MATRIX_to_string(&expected), MATRIX_to_string(&ident));
        TEST_FAIL_MESSAGE("Identity matrix transposed should be unchanged");
    }
    MATRIX_destroy(&ident);
    MATRIX_destroy(&expected);
}

void test_2x2_determinant()
{
    MATRIX_Matrix* m = MATRIX_new(2,2);
    MATRIX_fill(m, 1.0, 5.0,
                   -3.0, 2.0);
    double determinant = MATRIX_determinant(m);
    TEST_ASSERT_EQUAL_DOUBLE(17.0, determinant);
    MATRIX_delete(m);
}

void test_3x3_submatrix()
{
    MATRIX_Matrix* m = MATRIX_new(3, 3);
    MATRIX_fill(m, 1.0, 5.0, 0.0,
                  -3.0, 2.0, 7.0,
                   0.0, 6.0, -3.0);
    MATRIX_Matrix* submatrix = MATRIX_submatrix(m, 0, 2);
    MATRIX_Matrix* expected = MATRIX_new(2, 2);
    MATRIX_fill(expected, -3.0, 2.0,
                    0.0, 6.0);
    if (!MATRIX_is_equal(expected, submatrix)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(m),
               MATRIX_to_string(expected),
               MATRIX_to_string(submatrix));
        TEST_FAIL();
    }
    MATRIX_delete(m);
    MATRIX_delete(submatrix);
    MATRIX_delete(expected);
}

void test_4x4_submatrix()
{
    MATRIX_Matrix* m = MATRIX_new(4, 4);
    MATRIX_fill(m, -6.0, 1.0, 1.0, 6.0,
                   -8.0, 5.0, 8.0, 6.0,
                   -1.0, 0.0, 8.0, 2.0,
                   -7.0, 1.0, -1.0, 1.0);
    MATRIX_Matrix* submatrix = MATRIX_submatrix(m, 2, 1);
    MATRIX_Matrix* expected = MATRIX_new(3, 3);
    MATRIX_fill(expected, -6.0, 1.0, 6.0,
                   -8.0, 8.0, 6.0,
                   -7.0, -1.0, 1.0);
    if (!MATRIX_is_equal(expected, submatrix)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(m),
               MATRIX_to_string(expected),
               MATRIX_to_string(submatrix));
        TEST_FAIL();
    }
    MATRIX_delete(m);
    MATRIX_delete(submatrix);
    MATRIX_delete(expected);
}

void test_minor_of_3x3_matrix()
{
    MATRIX_Matrix* matrix = MATRIX_new(3, 3);
    MATRIX_fill(matrix, 3.0, 5.0, 0.0,
                   2.0, -1.0, -7.0,
                   6.0, -1.0, 5.0);
    MATRIX_Matrix* submatrix = MATRIX_submatrix(matrix, 1, 0);
    double determinant = MATRIX_determinant(submatrix);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(25.0, determinant, "Determinant was calculated incorrectly");

    double minor = MATRIX_minor(matrix, 1, 0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(determinant, minor, "Minor was calculated incorrectly");

    MATRIX_delete(matrix);
    MATRIX_delete(submatrix);
}

void test_cofactor() {
    MATRIX_Matrix* matrix = MATRIX_new(3, 3);
    MATRIX_fill(matrix, 3.0, 5.0, 0.0,
                        2.0, -1.0, -7.0,
                        6.0, -1.0, 5.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-12.0, MATRIX_minor(matrix, 0, 0), "Minor of 0,0 was calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-12.0, MATRIX_cofactor(matrix, 0, 0), "Cofactor of 0,0 was calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(25.0, MATRIX_minor(matrix, 1, 0), "Minor of 1,0 was calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-25.0, MATRIX_cofactor(matrix, 1, 0), "Cofactor of 1,0 was calculated incorrectly");
    MATRIX_delete(matrix);
}

void test_3x3_determinant() {
    MATRIX_Matrix* matrix = MATRIX_new(3, 3);
    MATRIX_fill(matrix, 1.0, 2.0, 6.0,
                        -5.0, 8.0, -4.0,
                        2.0, 6.0, 4.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(56.0, MATRIX_cofactor(matrix, 0, 0), "Cofactor of 0,0 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(12.0, MATRIX_cofactor(matrix, 0, 1), "Cofactor of 0,1 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-46.0, MATRIX_cofactor(matrix, 0, 2), "Cofactor of 0,2 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-196.0, MATRIX_determinant(matrix), "Determinant was calculated incorrectly");
    MATRIX_delete(matrix);
}

void test_4x4_determinant() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, -2.0, -8.0, 3.0, 5.0,
                        -3.0, 1.0, 7.0, 3.0,
                        1.0, 2.0, -9.0, 6.0,
                        -6.0, 7.0, 7.0, -9.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(690.0, MATRIX_cofactor(matrix, 0, 0), "Cofactor of 0,0 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(447.0, MATRIX_cofactor(matrix, 0, 1), "Cofactor of 0,1 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(210.0, MATRIX_cofactor(matrix, 0, 2), "Cofactor of 0,2 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(51.0, MATRIX_cofactor(matrix, 0, 3), "Cofactor of 0,3 calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-4071.0, MATRIX_determinant(matrix), "Determinant was calculated incorrectly");
    MATRIX_delete(matrix);
}

void test_invertible_true() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, 6.0, 4.0, 4.0, 4.0,
                        5.0, 5.0, 7.0, 6.0,
                        4.0, -9.0, 3.0, -7.0,
                        9.0, 1.0, 7.0, -6.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2120.0, MATRIX_determinant(matrix), "Determinant was calculated incorrectly");
    TEST_ASSERT_TRUE(MATRIX_is_invertible(matrix));
    MATRIX_delete(matrix);
}

void test_invertible_false() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, -4.0, 2.0, -2.0, -3.0,
                         9.0, 6.0, 2.0, 6.0,
                         0.0, -5.0, 1.0, -5.0,
                         0.0, 0.0, 0.0, 0.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, MATRIX_determinant(matrix), "Determinant was calculated incorrectly");
    TEST_ASSERT_FALSE(MATRIX_is_invertible(matrix));
    MATRIX_delete(matrix);
}

void test_inverse() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, -5.0, 2.0, 6.0, -8.0,
                         1.0, -5.0, 1.0, 8.0,
                         7.0, 7.0, -6.0, -7.0,
                         1.0, -3.0, 7.0, 4.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(532.0, MATRIX_determinant(matrix), "Determinant was calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-160.0, MATRIX_cofactor(matrix, 2, 3), "Cofactor of 2,3 was calculated incorrectly");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(105.0, MATRIX_cofactor(matrix, 3, 2), "Cofactor of 3,2 was calculated incorrectly");
    MATRIX_Matrix* inverse = MATRIX_inverse(matrix);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-160.0/532.0, MATRIX_read_cell(inverse, 3, 2), "inverse[3,2] is incorrect");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(105.0/532.0, MATRIX_read_cell(inverse, 2, 3), "inverse[2,3] is incorrect");
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected,
                0.21805, 0.45113, 0.24060, -0.04511,
                -0.80827, -1.45677, -0.44361, 0.52068,
                -0.07895, -0.22368, -0.05263, 0.19737,
                -0.52256, -0.81391, -0.30075, 0.30639);
    if (!MATRIX_is_equal(expected, inverse)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(matrix),
               MATRIX_to_string(expected),
               MATRIX_to_string(inverse));
        TEST_FAIL();
    }
    MATRIX_delete(matrix);
    MATRIX_delete(expected);
    MATRIX_delete(inverse);
}

void test_inverse2() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, 8.0, -5.0, 9.0, 2.0,
                        7.0, 5.0, 6.0, 1.0,
                        -6.0, 0.0, 9.0, 6.0,
                        -3.0, 0.0, -9.0, -4.0);
    TEST_ASSERT_TRUE(MATRIX_is_invertible(matrix));
    MATRIX_Matrix* inverse = MATRIX_inverse(matrix);
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected, -0.15385, -0.15385, -0.28205, -0.53846,
                          -0.07692, 0.12308,  0.02564,  0.03077,
                           0.35897,  0.35897,  0.43590,  0.92308,
                          -0.69231, -0.69231, -0.76923, -1.92308);
    if (!MATRIX_is_equal(expected, inverse)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(matrix),
               MATRIX_to_string(expected),
               MATRIX_to_string(inverse));
        TEST_FAIL();
    }
    MATRIX_delete(matrix);
    MATRIX_delete(expected);
    MATRIX_delete(inverse);
}

void test_inverse3() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, 9.0, 3.0, 0.0, 9.0,
                       -5.0, -2.0, -6.0, -3.0,
                       -4.0, 9.0, 6.0, 4.0,
                       -7.0, 6.0, 6.0, 2.0);
    TEST_ASSERT_TRUE(MATRIX_is_invertible(matrix));
    MATRIX_Matrix* inverse = MATRIX_inverse(matrix);
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected, -0.04074, -0.07778, 0.14444, -0.22222,
                          -0.07778, 0.03333, 0.36667, -0.33333,
                          -0.02901, -0.14630, -0.10926, 0.12963,
                          0.17778, 0.06667, -0.26667, 0.33333);
    if (!MATRIX_is_equal(expected, inverse)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(matrix),
               MATRIX_to_string(expected),
               MATRIX_to_string(inverse));
        TEST_FAIL();
    }
    MATRIX_delete(matrix);
    MATRIX_delete(expected);
    MATRIX_delete(inverse);
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
    RUN_TEST(test_identity_matrix);
    RUN_TEST(test_transpose);
    RUN_TEST(test_transpose_identity);
    RUN_TEST(test_2x2_determinant);
    RUN_TEST(test_3x3_submatrix);
    RUN_TEST(test_4x4_submatrix);
    RUN_TEST(test_minor_of_3x3_matrix);
    RUN_TEST(test_cofactor);
    RUN_TEST(test_3x3_determinant);
    RUN_TEST(test_4x4_determinant);
    RUN_TEST(test_invertible_false);
    RUN_TEST(test_invertible_true);
    RUN_TEST(test_inverse);
    RUN_TEST(test_inverse2);
    RUN_TEST(test_inverse3);
    return UNITY_END();
}
