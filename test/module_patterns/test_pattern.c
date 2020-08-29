#include "unity.h"
#include "testutils.h"

#include "tuples.h"
#include "pattern.h"
#include "sphere.h"

TUPLES_Color* white;
TUPLES_Color* black;

void setUp() {
    white = TUPLES_new_color(1, 1, 1);
    black = TUPLES_new_color(0, 0, 0);
}
void tearDown() {
    TUPLES_delete(white);
    TUPLES_delete(black);
}

void test_create_pattern() {
    PATTERN_Pattern* p = PATTERN_new_stripe(white, black);
    test_tuples(white, PATTERN_get_color_a(p));
    test_tuples(black, PATTERN_get_color_b(p));
    PATTERN_delete(p);
}

void test_stripe_pattern_is_constant_in_y() {
    PATTERN_Pattern* p = PATTERN_new_stripe(white, black);
    TUPLES_Color got;
    TUPLES_Point p1, p2, p3;
    TUPLES_init_point(&p1, 0, 0, 0);
    TUPLES_init_point(&p2, 0, 1, 0);
    TUPLES_init_point(&p3, 0, 2, 0);
    PATTERN_color_at(&got, p, &p1);
    test_tuples(white, &got);
    PATTERN_color_at(&got, p, &p2);
    test_tuples(white, &got);
    PATTERN_color_at(&got, p, &p3);
    test_tuples(white, &got);
    PATTERN_delete(p);
}

void test_stripe_pattern_is_constant_in_z() {
    PATTERN_Pattern* p = PATTERN_new_stripe(white, black);
    TUPLES_Color got;
    TUPLES_Point p1, p2, p3;
    TUPLES_init_point(&p1, 0, 0, 0);
    TUPLES_init_point(&p2, 0, 0, 1);
    TUPLES_init_point(&p3, 0, 0, 2);
    PATTERN_color_at(&got, p, &p1);
    test_tuples(white, &got);
    PATTERN_color_at(&got, p, &p2);
    test_tuples(white, &got);
    PATTERN_color_at(&got, p, &p3);
    test_tuples(white, &got);
    PATTERN_delete(p);
}

void test_stripe_pattern_alternates_in_x() {
    PATTERN_Pattern* p = PATTERN_new_stripe(white, black);
    TUPLES_Color got;
    TUPLES_Point point;

    TUPLES_init_point(&point, 0, 0, 0);
    PATTERN_color_at(&got, p, &point);
    test_tuples(white, &got);

    TUPLES_init_point(&point, 0.9, 0, 0);
    PATTERN_color_at(&got, p, &point);
    test_tuples(white, &got);

    TUPLES_init_point(&point, 1, 0, 0);
    PATTERN_color_at(&got, p, &point);
    test_tuples(black, &got);

    TUPLES_init_point(&point, -0.1, 0, 0);
    PATTERN_color_at(&got, p, &point);
    test_tuples(black, &got);

    TUPLES_init_point(&point, -1, 0, 0);
    PATTERN_color_at(&got, p, &point);
    test_tuples(black, &got);

    TUPLES_init_point(&point, -1.1, 0, 0);
    PATTERN_color_at(&got, p, &point);
    test_tuples(white, &got);

    PATTERN_delete(p);
}

void test_stripes_with_an_object_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* transform = MATRIX_new_scaling(2, 2, 2);
    SPHERE_set_transform(s, transform);
    PATTERN_Pattern* pattern = PATTERN_new_stripe(white, black);
    TUPLES_Point p;
    TUPLES_init_point(&p, 1.5, 0, 0);
    TUPLES_Color dest;
    PATTERN_color_at_shape(&dest, pattern, s, &p);
    test_tuples(white, &dest);

    PATTERN_delete(pattern);
    MATRIX_delete(transform);
    SPHERE_delete(s);
}

void test_stripes_with_a_pattern_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    PATTERN_Pattern* pattern = PATTERN_new_stripe(white, black);
    MATRIX_Matrix* transform = MATRIX_new_scaling(2, 2, 2);
    PATTERN_set_transform(pattern, transform);
    TUPLES_Point p;
    TUPLES_init_point(&p, 1.5, 0, 0);
    TUPLES_Color dest;
    PATTERN_color_at_shape(&dest, pattern, s, &p);
    test_tuples(white, &dest);

    PATTERN_delete(pattern);
    MATRIX_delete(transform);
    SPHERE_delete(s);
}

void test_stripes_with_object_and_pattern_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* scaling_transform = MATRIX_new_scaling(2, 2, 2);
    SPHERE_set_transform(s, scaling_transform);

    PATTERN_Pattern* pattern = PATTERN_new_stripe(white, black);
    MATRIX_Matrix* translation_transform = MATRIX_new_translation(0.5, 0, 0);
    PATTERN_set_transform(pattern, translation_transform);

    TUPLES_Point p;
    TUPLES_init_point(&p, 2.5, 0, 0);
    TUPLES_Color dest;
    PATTERN_color_at_shape(&dest, pattern, s, &p);
    test_tuples(white, &dest);

    PATTERN_delete(pattern);
    MATRIX_delete_all(scaling_transform, translation_transform);
    SPHERE_delete(s);
}

void test_pattern_with_an_object_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* t = MATRIX_new_scaling(2, 2, 2);
    SPHERE_set_transform(s, t);
    MATRIX_delete(t);

    PATTERN_Pattern* test_pattern = PATTERN_new_test();

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 1, 1.5, 2);

    TUPLES_Point p;
    TUPLES_init_point(&p, 2, 3, 4);
    PATTERN_color_at_shape(&got, test_pattern, s, &p);

    SPHERE_delete(s);
    PATTERN_delete(test_pattern);
}

void test_pattern_with_pattern_transformation() {
    SPHERE_Sphere* s = SPHERE_new();

    PATTERN_Pattern* test_pattern = PATTERN_new_test();
    MATRIX_Matrix* t = MATRIX_new_scaling(2, 2, 2);
    PATTERN_set_transform(test_pattern, t);
    MATRIX_delete(t);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 1, 1.5, 2);

    TUPLES_Point p;
    TUPLES_init_point(&p, 2, 3, 4);
    PATTERN_color_at_shape(&got, test_pattern, s, &p);

    SPHERE_delete(s);
    PATTERN_delete(test_pattern);
}

void test_pattern_with_pattern_and_object_transformation() {
    SPHERE_Sphere* s = SPHERE_new();
    MATRIX_Matrix* t = MATRIX_new_scaling(2, 2, 2);
    SPHERE_set_transform(s, t);
    MATRIX_delete(t);

    PATTERN_Pattern* test_pattern = PATTERN_new_test();
    MATRIX_Matrix* t2 = MATRIX_new_translation(0.5, 1, 1.5);
    PATTERN_set_transform(test_pattern, t2);
    MATRIX_delete(t2);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0.75, .5, 0.25);

    TUPLES_Point p;
    TUPLES_init_point(&p, 2.5, 3, 3.5);
    PATTERN_color_at_shape(&got, test_pattern, s, &p);

    SPHERE_delete(s);
    PATTERN_delete(test_pattern);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create_pattern);
    RUN_TEST(test_stripe_pattern_is_constant_in_y);
    RUN_TEST(test_stripe_pattern_is_constant_in_z);
    RUN_TEST(test_stripe_pattern_alternates_in_x);
    RUN_TEST(test_stripes_with_an_object_transformation);
    RUN_TEST(test_stripes_with_a_pattern_transformation);
    RUN_TEST(test_stripes_with_object_and_pattern_transformation);
    RUN_TEST(test_pattern_with_an_object_transformation);
    RUN_TEST(test_pattern_with_pattern_transformation);
    RUN_TEST(test_pattern_with_pattern_and_object_transformation);
    return UNITY_END();
}
