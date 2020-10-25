#include <assert.h>
#include <math.h>

#include "exceptions.h"
#include "matrix.h"
#include "pattern.h"
#include "tuples.h"

typedef struct PATTERN_Pattern {
  TUPLES_Color a, b;
  MATRIX_Matrix *transform;
  MATRIX_Matrix *inverse_transform;
  void (*at)(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point);
  PATTERN_Pattern *(*copy)(const PATTERN_Pattern *src);
} PATTERN_Pattern;

//forward declare
static PATTERN_Pattern *PATTERN_new(const TUPLES_Color *a, const TUPLES_Color *b);

static PATTERN_Pattern *copy_base_pattern(const PATTERN_Pattern* pattern) {
  assert(pattern);
  PATTERN_Pattern *new_pattern = PATTERN_new(&pattern->a, &pattern->b);
  MATRIX_copy(new_pattern->transform, pattern->transform);
  MATRIX_copy(new_pattern->inverse_transform, pattern->inverse_transform);
  new_pattern->at = pattern->at;
  new_pattern->copy = pattern->copy;
  return new_pattern;
}

static void PATTERN_init(PATTERN_Pattern *pattern, const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);

  TUPLES_copy(&pattern->a, a);
  TUPLES_copy(&pattern->b, b);
  pattern->transform = MATRIX_new_identity(4);
  pattern->inverse_transform = MATRIX_new_identity(4);
  MATRIX_inverse(pattern->inverse_transform, pattern->transform);
  pattern->copy = &copy_base_pattern;
}

static PATTERN_Pattern *PATTERN_new(const TUPLES_Color *a, const TUPLES_Color *b) {
  PATTERN_Pattern *pattern = malloc(sizeof(PATTERN_Pattern));
  if (!pattern) {
    Throw(E_MALLOC_FAILED);
  }
  PATTERN_init(pattern, a, b);
  return pattern;
}

static void copy_base_pattern_in_place(PATTERN_Pattern *dest, const PATTERN_Pattern *src) {
  assert(dest);
  assert(src);
  PATTERN_init(dest, &src->a, &src->b);
  MATRIX_copy(dest->transform, src->transform);
  MATRIX_copy(dest->inverse_transform, src->inverse_transform);
  dest->at = src->at;
  dest->copy = src->copy;
}

const TUPLES_Color *PATTERN_get_color_a(const PATTERN_Pattern *pattern) {
  assert(pattern);
  return &pattern->a;
}

const TUPLES_Color *PATTERN_get_color_b(const PATTERN_Pattern *pattern) {
  assert(pattern);
  return &pattern->b;
}

void PATTERN_delete(PATTERN_Pattern *p) {
  assert(p);
  MATRIX_delete(p->transform);
  MATRIX_delete(p->inverse_transform);
  free(p);
}

PATTERN_Pattern *PATTERN_new_copy(const PATTERN_Pattern *pattern) {
  assert(pattern);
  assert(pattern->transform);
  return pattern->copy(pattern);
}

void PATTERN_set_transform(PATTERN_Pattern *pattern, const MATRIX_Matrix *transformation) {
  assert(pattern);
  assert(transformation);
  MATRIX_destroy(pattern->transform);
  MATRIX_copy(pattern->transform, transformation);
  MATRIX_inverse(pattern->inverse_transform, pattern->transform);
}

void PATTERN_color_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *object_point) {
  assert(dest);
  assert(pattern);
  assert(object_point);
  pattern->at(dest, pattern, object_point);
}

void PATTERN_color_at_shape(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const SHAPE_Shape *shape, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(shape);
  assert(point);

  TUPLES_Point shape_point, pattern_point;
  SHAPE_world_to_object(&shape_point, shape, point);
  MATRIX_multiply_tuple(&pattern_point, pattern->inverse_transform, &shape_point);

  PATTERN_color_at(dest, pattern, &pattern_point);
}

// ------ stripe pattern
static void stripe_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  if ((int)floor(point->x) % 2 == 0) {
    TUPLES_copy(dest, &pattern->a);
  } else {
    TUPLES_copy(dest, &pattern->b);
  }
}

PATTERN_Pattern *PATTERN_new_stripe(const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);
  PATTERN_Pattern *pattern = PATTERN_new(a, b);
  pattern->at = &stripe_at;
  return pattern;
}

// ------ test pattern
static void test_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  UNUSED(pattern);
  TUPLES_init_color(dest, point->x, point->y, point->z);
}

PATTERN_Pattern *PATTERN_new_test() {
  TUPLES_Color black;
  TUPLES_init_color(&black, 0, 0, 0);
  PATTERN_Pattern *pattern = PATTERN_new(&black, &black);
  pattern->at = &test_at;
  return pattern;
}

// ------ gradient pattern

void gradient_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
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

PATTERN_Pattern *PATTERN_new_gradient(const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);
  PATTERN_Pattern *pattern = PATTERN_new(a, b);
  pattern->at = &gradient_at;
  return pattern;
}

// ------ ring pattern

void ring_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);

  // stripe in 2 dimensions
  if ((int)floor(sqrt(pow(point->x, 2) + pow(point->z, 2))) % 2 == 0) {
    TUPLES_copy(dest, &pattern->a);
  } else {
    TUPLES_copy(dest, &pattern->b);
  }
}

PATTERN_Pattern *PATTERN_new_ring(const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);
  PATTERN_Pattern *pattern = PATTERN_new(a, b);
  pattern->at = &ring_at;
  return pattern;
}

// ------ checkers pattern

void checkers_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);

  if (((int)(floor(point->x) + floor(point->y) + floor(point->z))) % 2 == 0) {
    TUPLES_copy(dest, &pattern->a);
  } else {
    TUPLES_copy(dest, &pattern->b);
  }
}

PATTERN_Pattern *PATTERN_new_checkers(const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);
  PATTERN_Pattern *pattern = PATTERN_new(a, b);
  pattern->at = &checkers_at;
  return pattern;
}

// ------ solid pattern
void solid_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  UNUSED(point);
  TUPLES_copy(dest, &pattern->a);
}

PATTERN_Pattern *PATTERN_new_solid(const TUPLES_Color *a) {
  assert(a);
  PATTERN_Pattern *pattern = PATTERN_new(a, a);
  pattern->at = &solid_at;
  return pattern;
}

// ------ map
typedef struct PATTERN_Pattern_Map {
  PATTERN_Pattern pattern;
  UV_Pattern uv_pattern;
  void (*map)(double* u, double* v, const TUPLES_Point* point);
} PATTERN_Pattern_Map;

void map_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  PATTERN_Pattern_Map* pattern_map = (PATTERN_Pattern_Map*) pattern;
  double u, v;
  pattern_map->map(&u, &v, point);
  UV_PATTERN_pattern_at(dest, &pattern_map->uv_pattern, u, v);
}

PATTERN_Pattern *copy_pattern_map(const PATTERN_Pattern *pattern) {
  assert(pattern);
  PATTERN_Pattern_Map *src = (PATTERN_Pattern_Map*) pattern;
  PATTERN_Pattern_Map *pattern_map = malloc(sizeof(PATTERN_Pattern_Map));
  if (!pattern_map) {
    Throw(E_MALLOC_FAILED);
  }
  UV_PATTERN_copy(&pattern_map->uv_pattern, &src->uv_pattern);
  copy_base_pattern_in_place(&pattern_map->pattern, pattern);
  pattern_map->map = src->map;
  return (PATTERN_Pattern*) pattern_map;
}

PATTERN_Pattern *PATTERN_new_map(const UV_Pattern* uv_pattern, void (*map)(double* u, double* v, const TUPLES_Point* point)) {
  assert(uv_pattern);
  PATTERN_Pattern_Map *pattern_map = malloc(sizeof(PATTERN_Pattern_Map));
  if (!pattern_map) {
    Throw(E_MALLOC_FAILED);
  }
  UV_PATTERN_copy(&pattern_map->uv_pattern, uv_pattern);
  PATTERN_init(&pattern_map->pattern, &uv_pattern->a, &uv_pattern->b);
  pattern_map->map = map;
  pattern_map->pattern.at = &map_at;
  pattern_map->pattern.copy = copy_pattern_map;
  return (PATTERN_Pattern*)pattern_map;
}
