#include "tuples.h"
#include <unity.h>
#include <stdlib.h>
#include <stdbool.h>

void setUp() {}
void tearDown() {}

void test_dummy()
{
  TEST_ASSERT_NOT_NULL(NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_dummy);

    return UNITY_END();
}

