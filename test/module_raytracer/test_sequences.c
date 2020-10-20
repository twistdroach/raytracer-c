#include <unity.h>
#include "sequences.h"
void setUp() {}
void tearDown() {}

void test_test_sequence_returns_a_cyclic_sequence_of_numbers() {
    SEQUENCES_Sequence* s = SEQUENCES_new(3, (double[]){0.1, 0.5, 1.0});
    TEST_ASSERT_EQUAL_DOUBLE(0.1, SEQUENCES_next(s));
    TEST_ASSERT_EQUAL_DOUBLE(0.5, SEQUENCES_next(s));
    TEST_ASSERT_EQUAL_DOUBLE(1.0, SEQUENCES_next(s));
    TEST_ASSERT_EQUAL_DOUBLE(0.1, SEQUENCES_next(s));
    SEQUENCES_delete(s);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_test_sequence_returns_a_cyclic_sequence_of_numbers);
    UNITY_END();
}
