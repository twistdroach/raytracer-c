#ifndef DATA_STRUCTURES_PATTERNS_H
#define DATA_STRUCTURES_PATTERNS_H

#include "uv_pattern.h"
#include <matrix.h>
#include <shape.h>
#include <tuples.h>

typedef struct PATTERN_Pattern PATTERN_Pattern;

/**
 * Initialize a test pattern that just sets colors
 * equal to the point x/y/z
 * @param pattern
 */
PATTERN_Pattern *PATTERN_new_test();

/**
 * Stripes between given colors along local x axis
 * @param a
 * @param b
 * @return
 */
PATTERN_Pattern *PATTERN_new_stripe(const TUPLES_Color *a, const TUPLES_Color *b);

PATTERN_Pattern *PATTERN_new_gradient(const TUPLES_Color *a, const TUPLES_Color *b);
PATTERN_Pattern *PATTERN_new_ring(const TUPLES_Color *a, const TUPLES_Color *b);
PATTERN_Pattern *PATTERN_new_checkers(const TUPLES_Color *a, const TUPLES_Color *b);
PATTERN_Pattern *PATTERN_new_solid(const TUPLES_Color *a);
PATTERN_Pattern *PATTERN_new_map(const UV_Pattern* uv_pattern, void (*map)(double* u, double* v, const TUPLES_Point* point));
PATTERN_Pattern *PATTERN_new_cube_map(const UV_Pattern *left, const UV_Pattern *front, const UV_Pattern *right, const UV_Pattern *back, const UV_Pattern *up, const UV_Pattern *down);
PATTERN_Pattern *PATTERN_new_blended(PATTERN_Pattern *a, PATTERN_Pattern *b);

const TUPLES_Color *PATTERN_get_color_a(const PATTERN_Pattern *pattern);
const TUPLES_Color *PATTERN_get_color_b(const PATTERN_Pattern *pattern);
void PATTERN_delete(PATTERN_Pattern *p);
void PATTERN_color_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point);
void PATTERN_color_at_shape(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const SHAPE_Shape *shape, const TUPLES_Point *point);
PATTERN_Pattern *PATTERN_new_copy(const PATTERN_Pattern *pattern);
void PATTERN_set_transform(PATTERN_Pattern *pattern, const MATRIX_Matrix *transformation);
#endif // DATA_STRUCTURES_PATTERNS_H
