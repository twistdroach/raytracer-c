#include <assert.h>
#include <math.h>

#include "exceptions.h"
#include "matrix.h"
#include "tuples.h"
#include "pattern.h"

typedef struct PATTERN_Pattern {
    TUPLES_Color a, b;
    MATRIX_Matrix* transform;
    void (*at)(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point);
} PATTERN_Pattern;

static PATTERN_Pattern* PATTERN_new(const TUPLES_Color* a, const TUPLES_Color* b) {
    PATTERN_Pattern* pattern = malloc(sizeof(PATTERN_Pattern));
    if (!pattern) {
        Throw(E_MALLOC_FAILED);
    }
    TUPLES_copy(&pattern->a, a);
    TUPLES_copy(&pattern->b, b);
    pattern->transform = MATRIX_new_identity(4);
    return pattern;
}

const TUPLES_Color* PATTERN_get_color_a(const PATTERN_Pattern* pattern) {
    assert(pattern);
    return &pattern->a;
}

const TUPLES_Color* PATTERN_get_color_b(const PATTERN_Pattern* pattern) {
    assert(pattern);
    return &pattern->b;
}

void PATTERN_delete(PATTERN_Pattern* p) {
    assert(p);
    MATRIX_delete(p->transform);
    free(p);
}

PATTERN_Pattern* PATTERN_new_copy(const PATTERN_Pattern* pattern) {
    assert(pattern);
    assert(pattern->transform);
    PATTERN_Pattern* new_pattern = PATTERN_new(&pattern->a, &pattern->b);
    MATRIX_copy(new_pattern->transform, pattern->transform);
    new_pattern->at = pattern->at;
    return new_pattern;
}

void PATTERN_set_transform(PATTERN_Pattern* pattern, const MATRIX_Matrix* transformation) {
    assert(pattern);
    assert(transformation);
    MATRIX_destroy(pattern->transform);
    MATRIX_copy(pattern->transform, transformation);
}

void PATTERN_color_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* object_point) {
    assert(dest);
    assert(pattern);
    assert(object_point);
    pattern->at(dest, pattern, object_point);
}

void PATTERN_color_at_shape(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const SHAPE_Shape* shape, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(shape);
    assert(point);
    MATRIX_Matrix inverse_pattern_transform;
    MATRIX_inverse(&inverse_pattern_transform, pattern->transform);

    TUPLES_Point shape_point, pattern_point;
    MATRIX_multiply_tuple(&shape_point, SHAPE_get_inverse_transform(shape), point);
    MATRIX_multiply_tuple(&pattern_point, &inverse_pattern_transform, &shape_point);

    PATTERN_color_at(dest, pattern, &pattern_point);
}

// ------ stripe pattern
static void stripe_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(point);
    if ((int)floor(point->x) % 2 == 0) {
        TUPLES_copy(dest, &pattern->a);
    } else {
        TUPLES_copy(dest, &pattern->b);
    }
}

PATTERN_Pattern* PATTERN_new_stripe(const TUPLES_Color* a, const TUPLES_Color* b) {
    assert(a);
    assert(b);
    PATTERN_Pattern* pattern = PATTERN_new(a, b);
    pattern->at = &stripe_at;
    return pattern;
}

// ------ test pattern
static void test_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(point);
    TUPLES_init_color(dest, point->x, point->y, point->z);
}

PATTERN_Pattern* PATTERN_new_test() {
    TUPLES_Color black;
    TUPLES_init_color(&black, 0, 0, 0);
    PATTERN_Pattern* pattern = PATTERN_new(&black, &black);
    pattern->at = &test_at;
    return pattern;
}

// ------ gradient pattern

void gradient_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(point);

    // simple linear interpolation
    TUPLES_Color distance;
    TUPLES_subtract(&distance, &pattern->b, &pattern->a);
    double fraction = point->x - floor(point->x);
    TUPLES_multiply(&distance, &distance, fraction);
    TUPLES_add(dest, &pattern->a, &distance);
}

PATTERN_Pattern* PATTERN_new_gradient(const TUPLES_Color* a, const TUPLES_Color* b) {
    assert(a);
    assert(b);
    PATTERN_Pattern* pattern = PATTERN_new(a, b);
    pattern->at = &gradient_at;
    return pattern;
}

// ------ ring pattern

void ring_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(point);

    //stripe in 2 dimensions
    if ((int)floor(sqrt(pow(point->x, 2) + pow(point->z, 2))) % 2 == 0) {
        TUPLES_copy(dest, &pattern->a);
    } else {
        TUPLES_copy(dest, &pattern->b);
    }
}

PATTERN_Pattern* PATTERN_new_ring(const TUPLES_Color* a, const TUPLES_Color* b) {
    assert(a);
    assert(b);
    PATTERN_Pattern* pattern = PATTERN_new(a, b);
    pattern->at = &ring_at;
    return pattern;
}

// ------ checkers pattern

void checkers_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(point);

    if (((int)(floor(point->x) + floor(point->y) + floor(point->z))) % 2 == 0) {
        TUPLES_copy(dest, &pattern->a);
    } else {
        TUPLES_copy(dest, &pattern->b);
    }
}

PATTERN_Pattern* PATTERN_new_checkers(const TUPLES_Color* a, const TUPLES_Color* b) {
    assert(a);
    assert(b);
    PATTERN_Pattern* pattern = PATTERN_new(a, b);
    pattern->at = &checkers_at;
    return pattern;
}

// ------ solid pattern

void solid_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point) {
    assert(dest);
    assert(pattern);
    assert(point);
    TUPLES_copy(dest, &pattern->a);
}

PATTERN_Pattern* PATTERN_new_solid(const TUPLES_Color* a) {
    assert(a);
    PATTERN_Pattern* pattern = PATTERN_new(a, a);
    pattern->at = &solid_at;
    return pattern;
}
