#include <unity.h>
#include <stdlib.h>
#include <math.h>
#include "tuples.h"

void setUp() {}
void tearDown() {}

void test_point_is_point()
{
    TUPLES_Point p = TUPLES_point(4.3, -4.2, 1.0);
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(p), "is_point should return true when given a point");
    TEST_ASSERT_FALSE_MESSAGE(TUPLES_is_vector(p), "is_vector should return false when given a point");
}

void test_point_is_vector()
{
    TUPLES_Vector v;
    v = TUPLES_vector(4.3, -4.2, 1.0);
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(v), "is_vector should return true when given a vector");
    TEST_ASSERT_FALSE_MESSAGE(TUPLES_is_point(v), "is_point should return false when given a vector");
}

void test_add_vectors()
{
    TUPLES_Vector a1 = TUPLES_vector(1, 2, 3);
    TUPLES_Vector a2 = TUPLES_vector(4, 5, 6);
    TUPLES_Vector a3 = TUPLES_add(a1, a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5, a3.x, "x component should be 5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7, a3.y, "y component should be 7");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(9, a3.z, "z component should be 9");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(a3), "Adding 2 vectors should create a vector");
}

void test_add_vector_point()
{
    TUPLES_Vector a1 = TUPLES_vector(1, 2, 3);
    TUPLES_Point a2 = TUPLES_point(4, 5, 6);
    TUPLES_Point a3 = TUPLES_add(a1, a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5, a3.x, "x component should be 5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7, a3.y, "y component should be 7");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(9, a3.z, "z component should be 9");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(a3), "Adding vector and a point should create a point");
}

void test_subtract_points()
{
    TUPLES_Point a1 = TUPLES_point(1, 2, 3);
    TUPLES_Point a2 = TUPLES_point(4, 5, 6);
    TUPLES_Vector a3 = TUPLES_subtract(a1, a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3, a3.x, "x component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3, a3.y, "y component should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3, a3.z, "z component should be -3");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(a3), "Subtracting points should create a vector");
}

void test_subtract_vectors()
{
    TUPLES_Vector a1 = TUPLES_vector(3, 2, 1);
    TUPLES_Vector a2 = TUPLES_vector(5, 6, 7);
    TUPLES_Vector a3 = TUPLES_subtract(a1, a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2, a3.x, "x component should be -2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-4, a3.y, "y component should be -4");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-6, a3.z, "z component should be -6");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_vector(a3), "Subtracting vectors should create a vector");
}

void test_subtract_vector_from_point()
{
    TUPLES_Point a1 = TUPLES_point(3, 2, 1);
    TUPLES_Vector a2 = TUPLES_vector(5, 6, 7);
    TUPLES_Point a3 = TUPLES_subtract(a1, a2);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2, a3.x, "x component should be -2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-4, a3.y, "y component should be -4");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-6, a3.z, "z component should be -6");
    TEST_ASSERT_TRUE_MESSAGE(TUPLES_is_point(a3), "Subtracting vectors should create a vector");
}

void test_negate_vector()
{
    TUPLES_Vector v = TUPLES_vector(1, -2, 3);
    v = TUPLES_negate(v);
    TEST_ASSERT_EQUAL_DOUBLE(-1, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(2, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(-3, v.z);
}

void test_multiply_tuple_by_scalar()
{
    TUPLES_Vector v = TUPLES_vector(1, -2, 3);
    v = TUPLES_multiply(v, 3.5);
    TEST_ASSERT_EQUAL_DOUBLE(3.5, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(-7, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(10.5, v.z);
}

void test_multiply_tuple_by_fraction()
{
    TUPLES_Vector v = TUPLES_vector(1, -2, 3);
    v = TUPLES_multiply(v, 0.5);
    TEST_ASSERT_EQUAL_DOUBLE(0.5, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(-1, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(1.5, v.z);
}

void test_divide_tuple_by_scalar()
{
    TUPLES_Vector v = TUPLES_vector(1, -2, 3);
    v = TUPLES_divide(v, 2);
    TEST_ASSERT_EQUAL_DOUBLE(0.5, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(-1, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(1.5, v.z);
}

void test_a_magnitude(double x, double y, double z, double expected_magnitude)
{
    TUPLES_Vector v = TUPLES_vector(x, y, z);
    char str[80];
    double magnitude;

    magnitude = TUPLES_magnitude(v);
    sprintf(str, "Vector(%f, %f, %f)", v.x, v.y, v.z);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(expected_magnitude, magnitude, str);
}

void test_magnitude()
{
    test_a_magnitude(1, 0, 0, 1);
    test_a_magnitude(0, 1, 0, 1);
    test_a_magnitude(0, 0, 1, 1);

    test_a_magnitude(1, 2, 3, sqrt(14));
    test_a_magnitude(-1, -2, -3, sqrt(14));
}

void test_simple_normalize()
{
    TUPLES_Vector v = TUPLES_vector(4, 0, 0);
    v = TUPLES_normalize(v);
    TEST_ASSERT_EQUAL_DOUBLE(1, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, v.z);
}

void test_less_simple_normalize()
{
    TUPLES_Vector v = TUPLES_vector(1, 2, 3);
    v = TUPLES_normalize(v);
    TEST_ASSERT_EQUAL_DOUBLE(0.26726, v.x);
    TEST_ASSERT_EQUAL_DOUBLE(0.53452, v.y);
    TEST_ASSERT_EQUAL_DOUBLE(0.80178, v.z);
}

void test_magnitude_normalized_vector()
{
    TUPLES_Vector v = TUPLES_vector(15, 25, 37);
    v = TUPLES_normalize(v);
    TEST_ASSERT_EQUAL_DOUBLE(1, TUPLES_magnitude(v));
}

void test_simple_dot_product()
{
    TUPLES_Vector v1 = TUPLES_vector(1, 2, 3);
    TUPLES_Vector v2 = TUPLES_vector(2, 3, 4);
    double dot = TUPLES_dot(v1, v2);
    TEST_ASSERT_EQUAL_DOUBLE(20, dot);
}

void test_cross_product()
{
    TUPLES_Vector v1 = TUPLES_vector(1, 2, 3);
    TUPLES_Vector v2 = TUPLES_vector(2, 3, 4);
    TUPLES_Vector result = TUPLES_cross(v1, v2);
    TEST_ASSERT_EQUAL_DOUBLE(-1, result.x);
    TEST_ASSERT_EQUAL_DOUBLE(2, result.y);
    TEST_ASSERT_EQUAL_DOUBLE(-1, result.z);
    result = TUPLES_cross(v2, v1);
    TEST_ASSERT_EQUAL_DOUBLE(1, result.x);
    TEST_ASSERT_EQUAL_DOUBLE(-2, result.y);
    TEST_ASSERT_EQUAL_DOUBLE(1, result.z);
}

void test_color_init()
{
    TUPLES_Color c = TUPLES_color(-0.5, 0.4, 1.7);
    TEST_ASSERT_EQUAL_DOUBLE(-0.5, c.red);
    TEST_ASSERT_EQUAL_DOUBLE(0.4, c.green);
    TEST_ASSERT_EQUAL_DOUBLE(1.7, c.blue);
}

void test_color_add()
{
    TUPLES_Color c1 = TUPLES_color(0.9, 0.6, 0.75);
    TUPLES_Color c2 = TUPLES_color(0.7, 0.1, 0.25);
    TUPLES_Color result = TUPLES_add(c1, c2);
    TEST_ASSERT_EQUAL_DOUBLE(1.6, result.red);
    TEST_ASSERT_EQUAL_DOUBLE(0.7, result.green);
    TEST_ASSERT_EQUAL_DOUBLE(1.0, result.blue);
}

void test_color_subtract()
{
    TUPLES_Color c1 = TUPLES_color(0.9, 0.6, 0.75);
    TUPLES_Color c2 = TUPLES_color(0.7, 0.1, 0.25);
    TUPLES_Color result = TUPLES_subtract(c1, c2);
    TEST_ASSERT_EQUAL_DOUBLE(0.2, result.red);
    TEST_ASSERT_EQUAL_DOUBLE(0.5, result.green);
    TEST_ASSERT_EQUAL_DOUBLE(0.5, result.blue);
}

void test_color_multiply_by_scalar()
{
    TUPLES_Color c1 = TUPLES_color(0.2, 0.3, 0.4);
    TUPLES_Color result = TUPLES_multiply(c1, 2);
    TEST_ASSERT_EQUAL_DOUBLE(0.4, result.red);
    TEST_ASSERT_EQUAL_DOUBLE(0.6, result.green);
    TEST_ASSERT_EQUAL_DOUBLE(0.8, result.blue);
}

void test_color_multiply_by_color()
{
    TUPLES_Color c1 = TUPLES_color(1, 0.2, 0.4);
    TUPLES_Color c2 = TUPLES_color(0.9, 1, 0.1);
    TUPLES_Color result = TUPLES_multiply_colors(c1, c2);
    TEST_ASSERT_EQUAL_DOUBLE(0.9, result.red);
    TEST_ASSERT_EQUAL_DOUBLE(0.2, result.green);
    TEST_ASSERT_EQUAL_DOUBLE(0.04, result.blue);
}

void test_tuple_copy()
{
    TUPLES_Point p = TUPLES_point(1, 2, 3);
    TUPLES_Point q = p;
    TEST_ASSERT_EQUAL_DOUBLE(1, p.x);
    TEST_ASSERT_EQUAL_DOUBLE(2, p.y);
    TEST_ASSERT_EQUAL_DOUBLE(3, p.z);
    TEST_ASSERT_TRUE(TUPLES_is_point(q));
}

void test_reflect_vector_approaching_at_45_deg() {
    TUPLES_Vector v = TUPLES_vector(1, -1, 0);
    TUPLES_Vector n = TUPLES_vector(0, 1, 0);
    TUPLES_Vector r = TUPLES_reflect(v, n);
    TEST_ASSERT_EQUAL_DOUBLE(1, r.x);
    TEST_ASSERT_EQUAL_DOUBLE(1, r.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, r.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(r));
}

void test_reflect_vector_approaching_at_slanted_surface() {
    TUPLES_Vector v = TUPLES_vector(0, -1, 0);
    TUPLES_Vector n = TUPLES_vector(sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0);
    TUPLES_Vector r = TUPLES_reflect(v, n);
    TEST_ASSERT_EQUAL_DOUBLE(1, r.x);
    TEST_ASSERT_DOUBLE_WITHIN(0.000001, 0, r.y);
    TEST_ASSERT_EQUAL_DOUBLE(0, r.z);
    TEST_ASSERT_TRUE(TUPLES_is_vector(r));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_point_is_point);
    RUN_TEST(test_point_is_vector);
    RUN_TEST(test_add_vectors);
    RUN_TEST(test_add_vector_point);
    RUN_TEST(test_subtract_points);
    RUN_TEST(test_subtract_vectors);
    RUN_TEST(test_subtract_vector_from_point);
    RUN_TEST(test_negate_vector);
    RUN_TEST(test_multiply_tuple_by_scalar);
    RUN_TEST(test_multiply_tuple_by_fraction);
    RUN_TEST(test_divide_tuple_by_scalar);
    RUN_TEST(test_magnitude);
    RUN_TEST(test_simple_normalize);
    RUN_TEST(test_less_simple_normalize);
    RUN_TEST(test_magnitude_normalized_vector);
    RUN_TEST(test_simple_dot_product);
    RUN_TEST(test_cross_product);
    RUN_TEST(test_color_init);
    RUN_TEST(test_color_add);
    RUN_TEST(test_color_subtract);
    RUN_TEST(test_color_multiply_by_scalar);
    RUN_TEST(test_color_multiply_by_color);
    RUN_TEST(test_tuple_copy);
    RUN_TEST(test_reflect_vector_approaching_at_45_deg);
    RUN_TEST(test_reflect_vector_approaching_at_slanted_surface);
    return UNITY_END();
}

