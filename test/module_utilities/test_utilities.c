//
// Created by zrowitsch on 8/1/20.
//

#define _GNU_SOURCE

#include "utilities.h"
#include <unity.h>
#define uint unsigned int
void setUp(void) {}

void tearDown(void) {}

void test_double_equal_zeros() { TEST_ASSERT_TRUE(double_equal(0.0, 0.0)); }

void test_double_equal_zero_one() { TEST_ASSERT_FALSE(double_equal(0.0, 1.0)); }

void test_double_equal_smaller_than_epsilon() { TEST_ASSERT_TRUE(double_equal(0.00000001, 0.0000001)); }

void test_sasprintf() {
  char *str = NULL;
  Sasprintf(str, "This is ten ");
  Sasprintf(str, "%s%d", str, 10);
  TEST_ASSERT_EQUAL_STRING("This is ten 10", str);
  free(str);
}

void test_random_double_equal_min_max() {
  srand(0);
  double t = UTILITIES_random_double(0.0, 0.0);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, t);
  t = UTILITIES_random_double(3.0, 3.0);
  TEST_ASSERT_EQUAL_DOUBLE(3.0, t);
}

void test_random_double() {
  srand(0);
  for (uint count = 0; count < 5000; count++) {
    double t = UTILITIES_random_double(3.0, 4.0);
    TEST_ASSERT_TRUE(t >= 3.0);
    TEST_ASSERT_TRUE(t <= 4.0);
  }
  for (uint count = 0; count < 5000; count++) {
    double t = UTILITIES_random_double(0, 1);
    TEST_ASSERT_TRUE(t >= 0);
    TEST_ASSERT_TRUE(t <= 1);
  }
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_double_equal_zeros);
  RUN_TEST(test_double_equal_zero_one);
  RUN_TEST(test_double_equal_smaller_than_epsilon);
  RUN_TEST(test_sasprintf);
  RUN_TEST(test_random_double_equal_min_max);
  RUN_TEST(test_random_double);
  return UNITY_END();
}
