#ifndef DATASTRUCTURES_TESTUTILS_H
#define DATASTRUCTURES_TESTUTILS_H

#include <tuples.h>

void test_tuples(const TUPLES_Tuple* expected, const TUPLES_Tuple* got) {
    if (!TUPLES_is_equal(expected, got)) {
        printf("Expected: %s\nBut got: %s\n", TUPLES_to_string(expected), TUPLES_to_string(got));
        printf("Diff: %.6f %.6f %.6f %.6f\n", expected->x - got->x,
                                              expected->y - got->y,
                                              expected->z - got->z,
                                              expected->w - got->w);
        TEST_FAIL();
    }
}

#endif //DATASTRUCTURES_TESTUTILS_H
