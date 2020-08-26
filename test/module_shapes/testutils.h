#ifndef DATA_STRUCTURES_TESTUTILS_H
#define DATA_STRUCTURES_TESTUTILS_H

void test_tuples(const TUPLES_Tuple* expected, const TUPLES_Tuple* got) {
    if (!TUPLES_is_equal(expected, got)) {
        printf("Expected: %s\nBut got: %s\n", TUPLES_to_string(expected), TUPLES_to_string(got));
        TEST_FAIL();
    }
}

#endif //DATA_STRUCTURES_TESTUTILS_H
