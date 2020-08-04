#define UNITY_DOUBLE_PRECISION 0.00001f
#include <unity.h>
#include <canvas.h>

void setUp() {}
void tearDown() {}

void validate_colors(CANVAS_Canvas* canvas, double r, double g, double b) {
    for (uint x=0; x < canvas->width; x++)
        for (uint y=0; y < canvas->height; y++) {
            TUPLES_Color* c = CANVAS_read_pixel(canvas, x, y);
            TEST_ASSERT_EQUAL_DOUBLE(r, c->red);
            TEST_ASSERT_EQUAL_DOUBLE(g, c->green);
            TEST_ASSERT_EQUAL_DOUBLE(b, c->blue);
        }
}

void test_canvas_new()
{
    CANVAS_Canvas* canvas = CANVAS_new(10, 20);
    validate_colors(canvas, 0, 0, 0);
    CANVAS_delete(canvas);
}

void test_canvas_init()
{
   CANVAS_Canvas canvas;
   CANVAS_init(&canvas, 10, 20);
   validate_colors(&canvas, 0, 0, 0);
   CANVAS_destroy(&canvas);
}

void test_canvas_write_pixel()
{
    CANVAS_Canvas canvas;
    CANVAS_init(&canvas, 10, 20);
    TUPLES_Color red;
    TUPLES_init_color(&red, 1, 0, 0);
    CANVAS_write_pixel(&canvas, 5, 5, &red);
    TUPLES_Color* pxlcolor = CANVAS_read_pixel(&canvas, 5, 5);
    TEST_ASSERT_EQUAL_DOUBLE(1, pxlcolor->red);
    TEST_ASSERT_EQUAL_DOUBLE(0, pxlcolor->green);
    TEST_ASSERT_EQUAL_DOUBLE(0, pxlcolor->blue);
    CANVAS_destroy(&canvas);
}

void test_canvas_get_ppm_header()
{
    CANVAS_Canvas canvas;
    CANVAS_init(&canvas, 5, 3);
    char* header = CANVAS_get_ppm_header_string(&canvas);
    TEST_ASSERT_NOT_EMPTY(header);
    TEST_ASSERT_EQUAL_STRING("P3\n5 3\n255\n", header);
    free(header);
    CANVAS_destroy(&canvas);
}

void test_canvas_get_ppm_body()
{
    CANVAS_Canvas canvas;
    CANVAS_init(&canvas, 5, 3);
    TUPLES_Color c1, c2, c3;
    TUPLES_init_color(&c1, 1.5, 0, 0);
    TUPLES_init_color(&c2, 0, 0.5, 0);
    TUPLES_init_color(&c3, -0.5, 0, 1);
    CANVAS_write_pixel(&canvas, 0, 0, &c1);
    CANVAS_write_pixel(&canvas, 2, 1, &c2);
    CANVAS_write_pixel(&canvas, 4, 2, &c3);
    char* body = CANVAS_get_ppm_body_string(&canvas);
    TEST_ASSERT_NOT_EMPTY(body);
    TEST_ASSERT_EQUAL_STRING("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 \n"
                                      "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0 \n"
                                      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 \n", body);
    free(body);
    CANVAS_destroy(&canvas);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_canvas_new);
    RUN_TEST(test_canvas_init);
    RUN_TEST(test_canvas_write_pixel);
    RUN_TEST(test_canvas_get_ppm_header);
    RUN_TEST(test_canvas_get_ppm_body);
    return UNITY_END();
}
