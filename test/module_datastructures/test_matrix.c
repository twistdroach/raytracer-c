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
    MATRIX_Matrix m1, m2;
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
   MATRIX_Matrix* m3 = MATRIX_multiply(&m1, &m2);

   MATRIX_Matrix expected;
   MATRIX_init(&expected, 4, 4);
   MATRIX_fill(&expected, 20.0, 22.0, 50.0, 48.0,
               44.0, 54.0, 114.0, 108.0,
               40.0, 58.0, 110.0, 102.0,
               16.0, 26.0, 46.0, 42.0);
   TEST_ASSERT_TRUE(MATRIX_is_equal(&expected, m3));
   MATRIX_destroy_all(&m1, &m2, &expected);
   MATRIX_delete(m3);
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
}

void test_identity_matrix()
{
    MATRIX_Matrix m;
    MATRIX_init(&m, 4, 4);
    MATRIX_fill(&m, 0.0, 1.0, 2.0, 4.0,
                    1.0, 2.0, 4.0, 8.0,
                    2.0, 4.0, 8.0, 16.0,
                    4.0, 8.0, 16.0, 32.0);
    MATRIX_Matrix* ident  = MATRIX_new_identity(4);
    MATRIX_Matrix* result = MATRIX_multiply(&m, ident);
    TEST_ASSERT_TRUE(MATRIX_is_equal(&m, result));
    MATRIX_destroy(&m);
    MATRIX_delete(result);
    MATRIX_delete(ident);
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
    MATRIX_Matrix* ident = MATRIX_new_identity(4);
    MATRIX_Matrix* expected = MATRIX_new_identity(4);
    if (!MATRIX_is_equal(expected, ident)) {
        printf("Expected:\n%s\nGot:\n%s\n", MATRIX_to_string(expected), MATRIX_to_string(ident));
        TEST_FAIL_MESSAGE("Identity matrix transposed should be unchanged");
    }
    MATRIX_delete_all(ident, expected);
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
    MATRIX_Matrix* submatrix = MATRIX_submatrix(NULL, m, 0, 2);
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
    MATRIX_Matrix* submatrix = MATRIX_submatrix(NULL, m, 2, 1);
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
    MATRIX_Matrix* submatrix = MATRIX_submatrix(NULL, matrix, 1, 0);
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
    MATRIX_Matrix inverse;
    MATRIX_inverse(&inverse, matrix);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-160.0/532.0, MATRIX_read_cell(&inverse, 3, 2), "inverse[3,2] is incorrect");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(105.0/532.0, MATRIX_read_cell(&inverse, 2, 3), "inverse[2,3] is incorrect");
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected,
                0.21805, 0.45113, 0.24060, -0.04511,
                -0.80827, -1.45677, -0.44361, 0.52068,
                -0.07895, -0.22368, -0.05263, 0.19737,
                -0.52256, -0.81391, -0.30075, 0.30639);
    if (!MATRIX_is_equal(expected, &inverse)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(matrix),
               MATRIX_to_string(expected),
               MATRIX_to_string(&inverse));
        TEST_FAIL();
    }
    MATRIX_delete(matrix);
    MATRIX_delete(expected);
}

void test_inverse2() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, 8.0, -5.0, 9.0, 2.0,
                        7.0, 5.0, 6.0, 1.0,
                        -6.0, 0.0, 9.0, 6.0,
                        -3.0, 0.0, -9.0, -4.0);
    TEST_ASSERT_TRUE(MATRIX_is_invertible(matrix));
    MATRIX_Matrix inverse;
    MATRIX_inverse(&inverse, matrix);
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected, -0.15385, -0.15385, -0.28205, -0.53846,
                          -0.07692, 0.12308,  0.02564,  0.03077,
                           0.35897,  0.35897,  0.43590,  0.92308,
                          -0.69231, -0.69231, -0.76923, -1.92308);
    if (!MATRIX_is_equal(expected, &inverse)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(matrix),
               MATRIX_to_string(expected),
               MATRIX_to_string(&inverse));
        TEST_FAIL();
    }
    MATRIX_delete(matrix);
    MATRIX_delete(expected);
}

void test_inverse3() {
    MATRIX_Matrix* matrix = MATRIX_new(4, 4);
    MATRIX_fill(matrix, 9.0, 3.0, 0.0, 9.0,
                       -5.0, -2.0, -6.0, -3.0,
                       -4.0, 9.0, 6.0, 4.0,
                       -7.0, 6.0, 6.0, 2.0);
    TEST_ASSERT_TRUE(MATRIX_is_invertible(matrix));
    MATRIX_Matrix inverse;
    MATRIX_inverse(&inverse, matrix);
    MATRIX_Matrix* expected = MATRIX_new(4, 4);
    MATRIX_fill(expected, -0.04074, -0.07778, 0.14444, -0.22222,
                          -0.07778, 0.03333, 0.36667, -0.33333,
                          -0.02901, -0.14630, -0.10926, 0.12963,
                          0.17778, 0.06667, -0.26667, 0.33333);
    if (!MATRIX_is_equal(expected, &inverse)) {
        printf("Source:\n%s\nExpected:\n%s\nGot:\n%s\n",
               MATRIX_to_string(matrix),
               MATRIX_to_string(expected),
               MATRIX_to_string(&inverse));
        TEST_FAIL();
    }
    MATRIX_delete(matrix);
    MATRIX_delete(expected);
}

void test_multiply_by_inverse() {
    MATRIX_Matrix* a = MATRIX_new(4, 4);
    MATRIX_fill(a, 3.0, -9.0, 7.0, 3.0,
                    3.0, -8.0, 2.0, -9.0,
                    -4.0, 4.0, 4.0, 1.0,
                    -6.0, 5.0, -1.0, 1.0);
    MATRIX_Matrix* b = MATRIX_new(4, 4);
    MATRIX_fill(b, 8.0, 2.0, 2.0, 2.0,
                   3.0, -1.0, 7.0, 0.0,
                   7.0, 0.0, 5.0, 4.0,
                   6.0, -2.0, 0.0, 5.0);
    MATRIX_Matrix* c = MATRIX_multiply(a, b);
    TEST_ASSERT_TRUE_MESSAGE(MATRIX_is_invertible(b), "Matrix b does not appear to be invertible");
    MATRIX_Matrix inverseb;
    MATRIX_inverse(&inverseb, b);
    MATRIX_Matrix* c_times_inverse_b = MATRIX_multiply(c, &inverseb);
    TEST_ASSERT_TRUE(MATRIX_is_equal(a, c_times_inverse_b));
    MATRIX_delete_all(c_times_inverse_b, a, b, c);
}

void test_translation_matrix() {
    MATRIX_Matrix* trans = MATRIX_new_translation(5, -3, 2);
    TUPLES_Point* p = TUPLES_new_point(-3, 4, 5);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, trans, p);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(1, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(7, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    TUPLES_delete(p);
    MATRIX_delete(trans);
}

void test_translation_inverse_matrix() {
    MATRIX_Matrix* trans = MATRIX_new_translation(5, -3, 2);
    MATRIX_Matrix trans_inv;
    MATRIX_inverse(&trans_inv, trans);
    TUPLES_Point* p = TUPLES_new_point(-3, 4, 5);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, &trans_inv, p);
    TEST_ASSERT_EQUAL_DOUBLE(-8, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(7, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    TUPLES_delete(p);
    MATRIX_delete_all(trans);
}

void test_translation_not_affect_vector() {
    MATRIX_Matrix* trans = MATRIX_new_translation(5, -3, 2);
    TUPLES_Vector* v = TUPLES_new_vector(-3, 4, 5);
    TUPLES_Vector res;
    MATRIX_multiply_tuple(&res, trans, v);
    TEST_ASSERT_EQUAL_DOUBLE(v->x, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(v->y, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(v->z, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&res));
    TUPLES_delete_all(v);
    MATRIX_delete(trans);
}

void test_scaling_matrix_applied_to_point() {
    MATRIX_Matrix* m = MATRIX_new_scaling(2, 3, 4);
    TUPLES_Point* p = TUPLES_new_point(-4, 6, 8);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, m, p);
    TEST_ASSERT_EQUAL_DOUBLE(-8, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(18, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(32, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(m);
    TUPLES_delete_all(p);
}

void test_scaling_matrix_applied_to_vector() {
    MATRIX_Matrix* m = MATRIX_new_scaling(2, 3, 4);
    TUPLES_Vector* p = TUPLES_new_vector(-4, 6, 8);
    TUPLES_Vector res;
    MATRIX_multiply_tuple(&res, m, p);
    TEST_ASSERT_EQUAL_DOUBLE(-8, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(18, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(32, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&res));
    MATRIX_delete(m);
    TUPLES_delete_all(p);
}

void test_inverse_scaling_matrix_applied_to_vector() {
    MATRIX_Matrix* m = MATRIX_new_scaling(2, 3, 4);
    TUPLES_Vector* p = TUPLES_new_vector(-4, 6, 8);
    MATRIX_Matrix m_inv;
    MATRIX_inverse(&m_inv, m);
    TUPLES_Vector res;
    MATRIX_multiply_tuple(&res, &m_inv, p);
    TEST_ASSERT_EQUAL_DOUBLE(-2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(&res));
    MATRIX_delete_all(m);
    TUPLES_delete_all(p);
}

void test_reflection_applied_to_point() {
    MATRIX_Matrix* m = MATRIX_new_scaling(-1, 1, 1);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, m, p);
    TEST_ASSERT_EQUAL_DOUBLE(-2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(m);
    TUPLES_delete_all(p);
}

void test_rotation_x() {
    TUPLES_Point* p = TUPLES_new_point(0, 1, 0);

    MATRIX_Matrix* half_quarter = MATRIX_new_rotation_x(M_PI_4);
    TUPLES_Point half_quarter_p;
    MATRIX_multiply_tuple(&half_quarter_p, half_quarter, p);
    TEST_ASSERT_EQUAL_DOUBLE(0, half_quarter_p.x);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt(2.0)/2.0, half_quarter_p.y);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt(2.0)/2.0, half_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&half_quarter_p));

    MATRIX_Matrix* full_quarter = MATRIX_new_rotation_x(M_PI_2);
    TUPLES_Point full_quarter_p;
    MATRIX_multiply_tuple(&full_quarter_p, full_quarter, p);
    TEST_ASSERT_EQUAL_DOUBLE(0, full_quarter_p.x);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, full_quarter_p.y);
    TEST_ASSERT_EQUAL_DOUBLE(1, full_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&full_quarter_p));

    MATRIX_delete_all(half_quarter, full_quarter);
    TUPLES_delete_all(p);
}

void test_inverse_rotation_x() {
    TUPLES_Point* p = TUPLES_new_point(0, 1, 0);
    MATRIX_Matrix* half_quarter = MATRIX_new_rotation_x(M_PI / 4.0);
    MATRIX_Matrix half_quarter_inv;
    MATRIX_inverse(&half_quarter_inv, half_quarter);
    TUPLES_Point half_quarter_p;
    MATRIX_multiply_tuple(&half_quarter_p, &half_quarter_inv, p);
    TEST_ASSERT_EQUAL_DOUBLE(0, half_quarter_p.x);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt(2.0)/2.0, half_quarter_p.y);
    TEST_ASSERT_EQUAL_DOUBLE(-sqrt(2.0)/2.0, half_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&half_quarter_p));
    MATRIX_delete_all(half_quarter);
    TUPLES_delete_all(p);
}

void test_rotation_y() {
    TUPLES_Point* p = TUPLES_new_point(0, 0, 1);

    MATRIX_Matrix* half_quarter = MATRIX_new_rotation_y(M_PI_4);
    TUPLES_Point half_quarter_p;
    MATRIX_multiply_tuple(&half_quarter_p, half_quarter, p);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt(2.0)/2.0, half_quarter_p.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, half_quarter_p.y);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt(2.0)/2.0, half_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&half_quarter_p));

    MATRIX_Matrix* full_quarter = MATRIX_new_rotation_y(M_PI_2);
    TUPLES_Point full_quarter_p;
    MATRIX_multiply_tuple(&full_quarter_p, full_quarter, p);
    TEST_ASSERT_EQUAL_DOUBLE(1, full_quarter_p.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, full_quarter_p.y);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, full_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&full_quarter_p));

    MATRIX_delete_all(half_quarter, full_quarter);
    TUPLES_delete_all(p);
}

void test_rotation_z() {
    TUPLES_Point* p = TUPLES_new_point(0, 1, 0);

    MATRIX_Matrix* half_quarter = MATRIX_new_rotation_z(M_PI_4);
    TUPLES_Point half_quarter_p;
    MATRIX_multiply_tuple(&half_quarter_p, half_quarter, p);
    TEST_ASSERT_EQUAL_DOUBLE(-sqrt(2.0)/2.0, half_quarter_p.x);
    TEST_ASSERT_EQUAL_DOUBLE(sqrt(2.0)/2.0, half_quarter_p.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, half_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&half_quarter_p));

    MATRIX_Matrix* full_quarter = MATRIX_new_rotation_z(M_PI_2);
    TUPLES_Point full_quarter_p;
    MATRIX_multiply_tuple(&full_quarter_p, full_quarter, p);
    TEST_ASSERT_EQUAL_DOUBLE(-1, full_quarter_p.x);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, full_quarter_p.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, full_quarter_p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&full_quarter_p));

    MATRIX_delete_all(half_quarter, full_quarter);
    TUPLES_delete_all(p);
}

void test_shearing_x_to_y() {
    MATRIX_Matrix* shear = MATRIX_new_shearing(1, 0, 0, 0, 0, 0);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, shear, p);
    TEST_ASSERT_EQUAL_DOUBLE(5, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(shear);
    TUPLES_delete_all(p);
}

void test_shearing_x_to_z() {
    MATRIX_Matrix* shear = MATRIX_new_shearing(0, 1, 0, 0, 0, 0);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, shear, p);
    TEST_ASSERT_EQUAL_DOUBLE(6, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(shear);
    TUPLES_delete_all(p);
}

void test_shearing_y_to_x() {
    MATRIX_Matrix* shear = MATRIX_new_shearing(0, 0, 1, 0, 0, 0);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, shear, p);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(5, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(shear);
    TUPLES_delete_all(p);
}

void test_shearing_y_to_z() {
    MATRIX_Matrix* shear = MATRIX_new_shearing(0, 0, 0, 1, 0, 0);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, shear, p);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(7, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(4, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(shear);
    TUPLES_delete_all(p);
}

void test_shearing_z_to_x() {
    MATRIX_Matrix* shear = MATRIX_new_shearing(0, 0, 0, 0, 1, 0);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, shear, p);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(6, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(shear);
    TUPLES_delete_all(p);
}

void test_shearing_z_to_y() {
    MATRIX_Matrix* shear = MATRIX_new_shearing(0, 0, 0, 0, 0, 1);
    TUPLES_Point* p = TUPLES_new_point(2, 3, 4);
    TUPLES_Point res;
    MATRIX_multiply_tuple(&res, shear, p);
    TEST_ASSERT_EQUAL_DOUBLE(2, res.x);
    TEST_ASSERT_EQUAL_DOUBLE(3, res.y);
    TEST_ASSERT_EQUAL_DOUBLE(7, res.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&res));
    MATRIX_delete(shear);
    TUPLES_delete_all(p);
}

void test_apply_transformations_in_sequence() {
    TUPLES_Point p;
    TUPLES_init_point(&p, 1, 0, 1);
    MATRIX_Matrix* A = MATRIX_new_rotation_x(M_PI_2);
    MATRIX_Matrix* B = MATRIX_new_scaling(5, 5, 5);
    MATRIX_Matrix* C = MATRIX_new_translation(10, 5, 7);

    TUPLES_Point p2;
    MATRIX_multiply_tuple(&p2, A, &p);
    TEST_ASSERT_EQUAL_DOUBLE(1, p2.x);
    TEST_ASSERT_EQUAL_DOUBLE(-1, p2.y);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, p2.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p2));

    TUPLES_Point p3;
    MATRIX_multiply_tuple(&p3, B, &p2);
    TEST_ASSERT_EQUAL_DOUBLE(5, p3.x);
    TEST_ASSERT_EQUAL_DOUBLE(-5, p3.y);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, p2.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p3));

    TUPLES_Point p4;
    MATRIX_multiply_tuple(&p4, C, &p3);
    TEST_ASSERT_EQUAL_DOUBLE(15, p4.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, p4.y);
    TEST_ASSERT_EQUAL_DOUBLE(7, p4.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p4));

    MATRIX_delete_all(A, B, C);
}

void test_apply_chained_transformations_in_reverse_order() {
    TUPLES_Point* p = TUPLES_new_point(1, 0, 1);
    MATRIX_Matrix* A = MATRIX_new_rotation_x(M_PI_2);
    MATRIX_Matrix* B = MATRIX_new_scaling(5, 5, 5);
    MATRIX_Matrix* C = MATRIX_new_translation(10, 5, 7);

    MATRIX_Matrix* C_B = MATRIX_multiply(C, B);
    MATRIX_Matrix* C_B_A = MATRIX_multiply(C_B, A);

    TUPLES_Point p2;
    MATRIX_multiply_tuple(&p2, C_B_A, p);
    TEST_ASSERT_EQUAL_DOUBLE(15, p2.x);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, p2.y);
    TEST_ASSERT_EQUAL_DOUBLE(7, p2.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(&p2));

    TUPLES_delete_all(p);
    MATRIX_delete_all(A, B, C, C_B, C_B_A);
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
    RUN_TEST(test_multiply_by_inverse);
    RUN_TEST(test_translation_matrix);
    RUN_TEST(test_translation_inverse_matrix);
    RUN_TEST(test_translation_not_affect_vector);
    RUN_TEST(test_scaling_matrix_applied_to_point);
    RUN_TEST(test_scaling_matrix_applied_to_vector);
    RUN_TEST(test_inverse_scaling_matrix_applied_to_vector);
    RUN_TEST(test_reflection_applied_to_point);
    RUN_TEST(test_rotation_x);
    RUN_TEST(test_inverse_rotation_x);
    RUN_TEST(test_rotation_y);
    RUN_TEST(test_rotation_z);
    RUN_TEST(test_shearing_x_to_y);
    RUN_TEST(test_shearing_x_to_z);
    RUN_TEST(test_shearing_y_to_x);
    RUN_TEST(test_shearing_y_to_z);
    RUN_TEST(test_shearing_z_to_x);
    RUN_TEST(test_shearing_z_to_y);
    RUN_TEST(test_apply_transformations_in_sequence);
    RUN_TEST(test_apply_chained_transformations_in_reverse_order);
    return UNITY_END();
}
