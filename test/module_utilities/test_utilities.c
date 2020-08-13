//
// Created by zrowitsch on 8/1/20.
//

#define _GNU_SOURCE

#include <unity.h>
#include "utilities.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_double_equal_zeros() {
    TEST_ASSERT_TRUE(double_equal(0.0, 0.0));
}

void test_double_equal_zero_one() {
    TEST_ASSERT_FALSE(double_equal(0.0, 1.0));
}

void test_double_equal_smaller_than_epsilon() {
    TEST_ASSERT_TRUE(double_equal(0.00000001, 0.0000001));
}

void test_sasprintf()
{
    char * str = NULL;
    Sasprintf(str, "This is ten ");
    Sasprintf(str, "%s%d", str, 10);
    TEST_ASSERT_EQUAL_STRING("This is ten 10", str);
    free(str);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_double_equal_zeros);
    RUN_TEST(test_double_equal_zero_one);
    RUN_TEST(test_double_equal_smaller_than_epsilon);
    RUN_TEST(test_sasprintf);

    return UNITY_END();
}

