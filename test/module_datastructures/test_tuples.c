#include "tuples.h"
#include <unity.h>
#include <stdlib.h>
#include <stdbool.h>

void setUp() {}
void tearDown() {}

void test_point_is_point()
{
    TUPLES_Point p = TUPLES_new_point(4.3, -4.2, 1.0);
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(p), "is_point should return true when given a point");
    TEST_ASSERT_FALSE_MESSAGE(TUPLES_is_vector(p), "is_vector should return false when given a point");
    TUPLES_destroy(p);
}

void test_point_is_vector()
{
    TUPLES_Vector v = TUPLES_new_vector(4.3, -4.2, 1.0);
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(v), "is_vector should return true when given a vector");
    TEST_ASSERT_FALSE_MESSAGE(TUPLES_is_point(v), "is_point should return false when given a vector");
    TUPLES_destroy(v);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_point_is_point);
    RUN_TEST(test_point_is_vector);

    return UNITY_END();
}

