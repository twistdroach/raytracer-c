#include "../testutils.h"
#include <canvas.h>
#include <exceptions.h>
#include <string.h>
#include <unity.h>

void setUp() {}
void tearDown() {}

static CEXCEPTION_T exception;

static CANVAS_Canvas *from_string(char *str) {
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

static void validate_colors(CANVAS_Canvas *canvas, double r, double g, double b) {
  for (uint x = 0; x < canvas->width; x++)
    for (uint y = 0; y < canvas->height; y++) {
      TUPLES_Color *c = CANVAS_read_pixel(canvas, x, y);
      TEST_ASSERT_EQUAL_DOUBLE(r, c->red);
      TEST_ASSERT_EQUAL_DOUBLE(g, c->green);
      TEST_ASSERT_EQUAL_DOUBLE(b, c->blue);
    }
}

void test_canvas_new() {
  CANVAS_Canvas *canvas = CANVAS_new(10, 20);
  validate_colors(canvas, 0, 0, 0);
  CANVAS_delete(canvas);
}

void test_canvas_init() {
  CANVAS_Canvas canvas;
  CANVAS_init(&canvas, 10, 20);
  validate_colors(&canvas, 0, 0, 0);
  CANVAS_destroy(&canvas);
}

void test_canvas_write_pixel() {
  CANVAS_Canvas canvas;
  CANVAS_init(&canvas, 10, 20);
  TUPLES_Color red;
  TUPLES_init_color(&red, 1, 0, 0);
  CANVAS_write_pixel(&canvas, 5, 5, &red);
  TUPLES_Color *pxlcolor = CANVAS_read_pixel(&canvas, 5, 5);
  TEST_ASSERT_EQUAL_DOUBLE(1, pxlcolor->red);
  TEST_ASSERT_EQUAL_DOUBLE(0, pxlcolor->green);
  TEST_ASSERT_EQUAL_DOUBLE(0, pxlcolor->blue);
  CANVAS_destroy(&canvas);
}

void test_canvas_get_ppm_header() {
  CANVAS_Canvas canvas;
  CANVAS_init(&canvas, 5, 3);
  char *header = CANVAS_get_ppm_header_string(&canvas);
  TEST_ASSERT_NOT_EMPTY(header);
  TEST_ASSERT_EQUAL_STRING("P3\n5 3\n255\n", header);
  free(header);
  CANVAS_destroy(&canvas);
}

void test_canvas_get_ppm_body() {
  CANVAS_Canvas canvas;
  CANVAS_init(&canvas, 5, 3);
  TUPLES_Color c1, c2, c3;
  TUPLES_init_color(&c1, 1.5, 0, 0);
  TUPLES_init_color(&c2, 0, 0.5, 0);
  TUPLES_init_color(&c3, -0.5, 0, 1);
  CANVAS_write_pixel(&canvas, 0, 0, &c1);
  CANVAS_write_pixel(&canvas, 2, 1, &c2);
  CANVAS_write_pixel(&canvas, 4, 2, &c3);
  char *body = CANVAS_get_ppm_body_string(&canvas);
  TEST_ASSERT_NOT_EMPTY(body);
  TEST_ASSERT_EQUAL_STRING("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 \n"
                           "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0 \n"
                           "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 \n\n",
                           body);
  free(body);
  CANVAS_destroy(&canvas);
}

void test_reading_a_file_with_wrong_magic_number() {
  char *data = "P32\n"
               "1 1\n"
               "255\n"
               "0 0 0\n"
               "\n";
  CANVAS_Canvas *canvas = NULL;
  Try {
    canvas = from_string(data);
    TEST_FAIL_MESSAGE("Exception should have been thrown");
  }
  Catch(exception) {
    TEST_ASSERT_EQUAL(E_PARSE_FAILED, exception);
  }
  if (canvas) {
    CANVAS_delete(canvas);
  }
}

void test_read_ppm_and_return_correct_size() {
  char *data = "P3\n"
               "10 2\n"
               "255\n"
               "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
               "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
               "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
               "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n";
  CANVAS_Canvas *canvas = NULL;
  Try {
    canvas = from_string(data);
    TEST_ASSERT_EQUAL(10, canvas->width);
    TEST_ASSERT_EQUAL(2, canvas->height);
  }
  Catch(exception) {
    printf("Exception: %s\n", EXCEPTIONS_strings[exception]);
    TEST_FAIL_MESSAGE("Caught exception");
  }
  CANVAS_delete(canvas);
}

void reading_pixel_data_from_a_ppm_file(unsigned int x, unsigned int y, double r, double g, double b) {
  printf("Testing (%u,%u) should be (%.2f, %.2f, %.2f)\n", x, y, r, g, b);
  char *data = "P3\n"
               "4 3\n"
               "255\n"
               "255 127 0  0 127 255  127 255 0  255 255 255\n"
               "0 0 0  255 0 0  0 255 0  0 0 255\n"
               "255 255 0  0 255 255  255 0 255  127 127 127\n";
  CANVAS_Canvas *canvas = NULL;
  Try {
    canvas = from_string(data);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, r, g, b);
    test_tuples(&expected, CANVAS_read_pixel(canvas, x, y));
  }
  Catch(exception) {
    printf("Exception: %s\n", EXCEPTIONS_strings[exception]);
    TEST_FAIL_MESSAGE("Caught exception");
  }
  CANVAS_delete(canvas);
}

void test_reading_pixel_data_from_a_ppm_file() {
  reading_pixel_data_from_a_ppm_file(0, 0, 1, 0.498, 0);
  reading_pixel_data_from_a_ppm_file(1, 0, 0, 0.498, 1);
  reading_pixel_data_from_a_ppm_file(2, 0, 0.498, 1, 0);
  reading_pixel_data_from_a_ppm_file(3, 0, 1, 1, 1);

  reading_pixel_data_from_a_ppm_file(0, 1, 0, 0, 0);
  reading_pixel_data_from_a_ppm_file(1, 1, 1, 0, 0);
  reading_pixel_data_from_a_ppm_file(2, 1, 0, 1, 0);
  reading_pixel_data_from_a_ppm_file(3, 1, 0, 0, 1);

  reading_pixel_data_from_a_ppm_file(0, 2, 1, 1, 0);
  reading_pixel_data_from_a_ppm_file(1, 2, 0, 1, 1);
  reading_pixel_data_from_a_ppm_file(2, 2, 1, 0, 1);
  reading_pixel_data_from_a_ppm_file(3, 2, 0.498, 0.498, 0.498);
}

void test_ppm_parsing_ignores_comment_lines() {
  char *data = "P3\n"
               "#this is a comment\n"
               "2 1\n"
               "# this is also a comment\n"
               "255\n"
               "# one more\n"
               "255 255 255\n"
               "# oh no, comments in the pixel data\n"
               "255 0 255\n";
  CANVAS_Canvas *canvas = NULL;
  Try {
    canvas = from_string(data);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 1, 1, 1);
    test_tuples(&expected, CANVAS_read_pixel(canvas, 0, 0));
    TUPLES_init_color(&expected, 1, 0, 1);
    test_tuples(&expected, CANVAS_read_pixel(canvas, 1, 0));
  }
  Catch(exception) {
    printf("Exception: %s\n", EXCEPTIONS_strings[exception]);
    TEST_FAIL_MESSAGE("Caught exception");
  }
  CANVAS_delete(canvas);
}

void test_ppm_parsing_allows_rgb_triple_to_span_lines() {
  char *data = "P3\n"
               "1 1\n"
               "255\n"
               "51\n"
               "153\n\n"
               "204\n";
  CANVAS_Canvas *canvas = NULL;
  Try {
    canvas = from_string(data);
    TUPLES_Color color;
    TUPLES_init_color(&color, 0.2, 0.6, 0.8);
    test_tuples(&color, CANVAS_read_pixel(canvas, 0, 0));
  }
  Catch(exception) {
    printf("Exception: %s\n", EXCEPTIONS_strings[exception]);
    TEST_FAIL_MESSAGE("Caught exception");
  }
  CANVAS_delete(canvas);
}

void test_ppm_parsing_respects_scale_setting() {
  char *data = "P3\n"
               "2 2\n"
               "100\n"
               "100 100 100  50 50 50\n"
               "75 50 25  0 0 0\n";
  CANVAS_Canvas *canvas = NULL;
  Try {
    canvas = from_string(data);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.75, 0.5, 0.25);

    test_tuples(&expected, CANVAS_read_pixel(canvas, 0, 1));
  }
  Catch(exception) {
    printf("Exception: %s\n", EXCEPTIONS_strings[exception]);
    TEST_FAIL_MESSAGE("Caught exception");
  }
  CANVAS_delete(canvas);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_canvas_new);
  RUN_TEST(test_canvas_init);
  RUN_TEST(test_canvas_write_pixel);
  RUN_TEST(test_canvas_get_ppm_header);
  RUN_TEST(test_canvas_get_ppm_body);
  RUN_TEST(test_reading_a_file_with_wrong_magic_number);
  RUN_TEST(test_read_ppm_and_return_correct_size);
  RUN_TEST(test_reading_pixel_data_from_a_ppm_file);
  RUN_TEST(test_ppm_parsing_ignores_comment_lines);
  RUN_TEST(test_ppm_parsing_allows_rgb_triple_to_span_lines);
  RUN_TEST(test_ppm_parsing_respects_scale_setting);
  return UNITY_END();
}
