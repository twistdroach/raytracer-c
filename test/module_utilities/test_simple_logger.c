//
// Created by zrowitsch on 1/11/20.
//

#include <unity.h>
#include "logger.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_dummytest() {
    LOGGER_log(LOGGER_ERROR, "This is an error (%s)\n", "asdf");
    LOGGER_log(LOGGER_WARN, "This is a warn (%s)\n", "asdf");
    LOGGER_log(LOGGER_INFO, "This is an info (%s)\n", "asdf");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_dummytest);

    return UNITY_END();
}
