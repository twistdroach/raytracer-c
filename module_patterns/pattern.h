#ifndef DATA_STRUCTURES_PATTERNS_H
#define DATA_STRUCTURES_PATTERNS_H

#include <tuples.h>
#include <matrix.h>
#include <shape.h>

typedef struct PATTERN_Pattern PATTERN_Pattern;

/**
 * Initialize a test pattern that just sets colors
 * equal to the point x/y/z
 * @param pattern
 */
PATTERN_Pattern* PATTERN_new_test();

/**
 * Stripes between given colors along local x axis
 * @param a
 * @param b
 * @return
 */
PATTERN_Pattern* PATTERN_new_stripe(TUPLES_Color* a, TUPLES_Color* b);


const TUPLES_Color* PATTERN_get_color_a(const PATTERN_Pattern* pattern);
const TUPLES_Color* PATTERN_get_color_b(const PATTERN_Pattern* pattern);
void PATTERN_delete(PATTERN_Pattern* p);
void PATTERN_color_at(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const TUPLES_Point* point);
void PATTERN_color_at_shape(TUPLES_Color* dest, const PATTERN_Pattern* pattern, const SHAPE_Shape* shape, const TUPLES_Point* point);
PATTERN_Pattern* PATTERN_new_copy(const PATTERN_Pattern* pattern);
void PATTERN_set_transform(PATTERN_Pattern* pattern, const MATRIX_Matrix* transformation);
#endif //DATA_STRUCTURES_PATTERNS_H
