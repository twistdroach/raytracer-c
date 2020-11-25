#ifndef DATA_SHAPES_TESTUTILS_H
#define DATA_SHAPES_TESTUTILS_H
#include <canvas.h>
#include <memory.h>
#include <tuples.h>
#include <unity.h>

void test_tuples(const TUPLES_Tuple *expected, const TUPLES_Tuple *got) {
  if (!TUPLES_is_equal(expected, got)) {
    printf("Expected: %s\nBut got: %s\n", TUPLES_to_string(expected), TUPLES_to_string(got));
    TEST_FAIL();
  }
}

void test_tuples_str(const TUPLES_Tuple *expected, const TUPLES_Tuple *got, const char *info) {
  if (!TUPLES_is_equal(expected, got)) {
    printf("%s - Expected: %s\nBut got: %s\n", info, TUPLES_to_string(expected), TUPLES_to_string(got));
    TEST_FAIL();
  }
}

static __attribute__((unused)) CANVAS_Canvas *from_string(char *str) {
  volatile CEXCEPTION_T exception;
  TEST_ASSERT_NOT_NULL(str);
  FILE *stream = fmemopen(str, strlen(str), "r");
  TEST_ASSERT_NOT_NULL_MESSAGE(stream, "fmemopen failed");
  CANVAS_Canvas *canvas = NULL;
  Try {
        canvas = CANVAS_parse_stream(stream);
      }
  Catch(exception) {
    fclose(stream);
    Throw(exception);
  }
  fclose(stream);
  TEST_ASSERT_NOT_NULL(canvas);
  return canvas;
}

#endif // DATA_SHAPES_TESTUTILS_H
