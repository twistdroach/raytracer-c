//
// Created by zrowitsch on 1/11/20.
//

#include <unity.h>
#include "simple_logger.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_dummytest() {
    SIMPLE_LOGGER_log(ERROR, "This is an error (%s)\n", "asdf");
    SIMPLE_LOGGER_log(WARN, "This is a warn (%s)\n", "asdf");
    SIMPLE_LOGGER_log(INFO, "This is an info (%s)\n", "asdf");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_dummytest);

    return UNITY_END();
}
