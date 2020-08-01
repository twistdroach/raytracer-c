#include <unity.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tuples.h"

void setUp() {}
void tearDown() {}

void test_point_is_point()
{
    TUPLES_Point* p = TUPLES_new_point(4.3, -4.2, 1.0);
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(p), "is_point should return true when given a point");
    TEST_ASSERT_FALSE_MESSAGE(TUPLES_is_vector(p), "is_vector should return false when given a point");
    TUPLES_destroy(p);
}

void test_point_is_vector()
{
    TUPLES_Vector* v = TUPLES_new_vector(4.3, -4.2, 1.0);
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(v), "is_vector should return true when given a vector");
    TEST_ASSERT_FALSE_MESSAGE(TUPLES_is_point(v), "is_point should return false when given a vector");
    TUPLES_destroy(v);
}

void test_add_vectors()
{
    TUPLES_Vector a1;
    TUPLES_init_vector(&a1, 1, 2, 3);
    TUPLES_Vector a2;
    TUPLES_init_vector(&a2, 4, 5, 6);
    TUPLES_Vector a3;
    TUPLES_add(&a3, &a1, &a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5, a3.x, "x component should be 5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7, a3.y, "y component should be 7");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(9, a3.z, "z component should be 9");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(&a3), "Adding 2 vectors should create a vector");
}

void test_add_vector_point()
{
    TUPLES_Vector a1;
    TUPLES_init_vector(&a1, 1, 2, 3);
    TUPLES_Point a2;
    TUPLES_init_point(&a2, 4, 5, 6);
    TUPLES_Point a3;
    TUPLES_add(&a3, &a1, &a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5, a3.x, "x component should be 5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7, a3.y, "y component should be 7");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(9, a3.z, "z component should be 9");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(&a3), "Adding vector and a point should create a point");
}

void test_subtract_points()
{
    TUPLES_Point a1;
    TUPLES_init_point(&a1, 1, 2, 3);
    TUPLES_Point a2;
    TUPLES_init_point(&a2, 4, 5, 6);
    TUPLES_Vector a3;
    TUPLES_subtract(&a3, &a1, &a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3, a3.x, "x component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3, a3.y, "y component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3, a3.z, "z component should be -3");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(&a3), "Subtracting points should create a vector");
}

void test_subtract_vectors()
{
    TUPLES_Vector a1;
    TUPLES_init_vector(&a1, 3, 2, 1);
    TUPLES_Vector a2;
    TUPLES_init_vector(&a2, 5, 6, 7);
    TUPLES_Vector a3;
    TUPLES_subtract(&a3, &a1, &a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2, a3.x, "x component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-4, a3.y, "y component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-6, a3.z, "z component should be -3");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(&a3), "Subtracting vectors should create a vector");
}

void test_subtract_vector_from_point()
{
    TUPLES_Point a1;
    TUPLES_init_point(&a1, 3, 2, 1);
    TUPLES_Vector a2;
    TUPLES_init_vector(&a2, 5, 6, 7);
    TUPLES_Point a3;
    TUPLES_subtract(&a3, &a1, &a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2, a3.x, "x component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-4, a3.y, "y component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-6, a3.z, "z component should be -3");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(&a3), "Subtracting vectors should create a vector");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_point_is_point);
    RUN_TEST(test_point_is_vector);
    RUN_TEST(test_add_vectors);
    RUN_TEST(test_add_vector_point);
    RUN_TEST(test_subtract_points);
    RUN_TEST(test_subtract_vectors);
    RUN_TEST(test_subtract_vector_from_point);

    return UNITY_END();
}

