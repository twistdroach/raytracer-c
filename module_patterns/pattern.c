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
  void (*delete)(PATTERN_Pattern* pattern);
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
  new_pattern->delete = new_pattern->delete;
  return new_pattern;
}

static void delete_base_pattern(PATTERN_Pattern *p) {
  assert(p);
  MATRIX_delete(p->transform);
  MATRIX_delete(p->inverse_transform);
  free(p);
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
  pattern->delete = &delete_base_pattern;
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
  dest->delete = src->delete;
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
  p->delete(p);
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
  UV_Pattern *uv_pattern;
  void (*coordinate_mapper)(double* u, double* v, const TUPLES_Point* point);
} PATTERN_Pattern_Map;

void map_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  PATTERN_Pattern_Map* pattern_map = (PATTERN_Pattern_Map*) pattern;
  double u, v;
  pattern_map->coordinate_mapper(&u, &v, point);
  UV_PATTERN_pattern_at(dest, pattern_map->uv_pattern, u, v);
}

void delete_pattern_map(PATTERN_Pattern* pattern) {
  PATTERN_Pattern_Map *pattern_map = (PATTERN_Pattern_Map*) pattern;
  UV_PATTERN_delete(pattern_map->uv_pattern);
  delete_base_pattern(pattern);
}

PATTERN_Pattern *copy_pattern_map(const PATTERN_Pattern *pattern) {
  assert(pattern);
  PATTERN_Pattern_Map *src = (PATTERN_Pattern_Map*) pattern;
  PATTERN_Pattern_Map *pattern_map = malloc(sizeof(PATTERN_Pattern_Map));
  if (!pattern_map) {
    Throw(E_MALLOC_FAILED);
  }
  pattern_map->uv_pattern = UV_PATTERN_copy(src->uv_pattern);
  copy_base_pattern_in_place(&pattern_map->pattern, pattern);
  pattern_map->coordinate_mapper = src->coordinate_mapper;
  return (PATTERN_Pattern*) pattern_map;
}

PATTERN_Pattern *PATTERN_new_map(const UV_Pattern* uv_pattern, void (*map)(double* u, double* v, const TUPLES_Point* point)) {
  assert(uv_pattern);
  PATTERN_Pattern_Map *pattern_map = malloc(sizeof(PATTERN_Pattern_Map));
  if (!pattern_map) {
    Throw(E_MALLOC_FAILED);
  }
  TUPLES_Color dummy_color;
  TUPLES_init_color(&dummy_color, 0, 0, 0);
  PATTERN_init(&pattern_map->pattern, &dummy_color, &dummy_color);
  pattern_map->uv_pattern = UV_PATTERN_copy(uv_pattern);
  pattern_map->coordinate_mapper = map;
  pattern_map->pattern.at = &map_at;
  pattern_map->pattern.copy = &copy_pattern_map;
  pattern_map->pattern.delete = &delete_pattern_map;
  return (PATTERN_Pattern*)pattern_map;
}

//---- cube map
typedef struct PATTERN_Pattern_Cube_Map {
  PATTERN_Pattern pattern;
  UV_Pattern *uv_pattern[UV_PATTERN_SIZE];
} PATTERN_Pattern_Cube_Map;

void cube_map_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  PATTERN_Pattern_Cube_Map* cube_map = (PATTERN_Pattern_Cube_Map*) pattern;
  double u, v;
  enum UV_PATTERN_Cube_Face face = UV_PATTERN_face_from_point(point);
  UV_PATTERN_cube_map(&u, &v, face, point);
  UV_PATTERN_pattern_at(dest, cube_map->uv_pattern[face], u, v);
}

static PATTERN_Pattern *copy_cube_map(const PATTERN_Pattern *pattern) {
  assert(pattern);
  PATTERN_Pattern_Cube_Map *cube_map = (PATTERN_Pattern_Cube_Map*)pattern;
  copy_base_pattern_in_place(&cube_map->pattern, pattern);
  return PATTERN_new_cube_map(cube_map->uv_pattern[UV_PATTERN_LEFT],
                              cube_map->uv_pattern[UV_PATTERN_FRONT],
                              cube_map->uv_pattern[UV_PATTERN_RIGHT],
                              cube_map->uv_pattern[UV_PATTERN_BACK],
                              cube_map->uv_pattern[UV_PATTERN_UP],
                              cube_map->uv_pattern[UV_PATTERN_DOWN]
                              );
}

static void delete_cube_map(PATTERN_Pattern *pattern) {
  assert(pattern);
  PATTERN_Pattern_Cube_Map *cube_map = (PATTERN_Pattern_Cube_Map*)pattern;
  for (unsigned int ndx = 0; ndx < UV_PATTERN_SIZE; ndx++) {
    UV_PATTERN_delete(cube_map->uv_pattern[ndx]);
  }
  delete_base_pattern(&cube_map->pattern);
}

PATTERN_Pattern *PATTERN_new_cube_map(const UV_Pattern *left, const UV_Pattern *front, const UV_Pattern *right, const UV_Pattern *back, const UV_Pattern *up, const UV_Pattern *down) {
  assert(left);
  assert(front);
  assert(right);
  assert(back);
  assert(up);
  assert(down);
  PATTERN_Pattern_Cube_Map *map = malloc(sizeof(PATTERN_Pattern_Cube_Map));
  if (!map) {
    Throw(E_MALLOC_FAILED);
  }
  map->uv_pattern[UV_PATTERN_LEFT] = UV_PATTERN_copy(left);
  map->uv_pattern[UV_PATTERN_FRONT] = UV_PATTERN_copy(front);
  map->uv_pattern[UV_PATTERN_RIGHT] = UV_PATTERN_copy(right);
  map->uv_pattern[UV_PATTERN_BACK] = UV_PATTERN_copy(back);
  map->uv_pattern[UV_PATTERN_UP] = UV_PATTERN_copy(up);
  map->uv_pattern[UV_PATTERN_DOWN] = UV_PATTERN_copy(down);
  TUPLES_Color dummy_color;
  TUPLES_init_color(&dummy_color, 0, 0, 0);
  PATTERN_init(&map->pattern, &dummy_color, &dummy_color);
  map->pattern.at = &cube_map_at;
  map->pattern.copy = &copy_cube_map;
  map->pattern.delete = &delete_cube_map;
  return (PATTERN_Pattern*)map;
}

//---- blended patterns
typedef struct {
  PATTERN_Pattern pattern;
  PATTERN_Pattern *a;
  PATTERN_Pattern *b;
} PATTERN_Pattern_Blended;

void blended_map_at(TUPLES_Color *dest, const PATTERN_Pattern *pattern, const TUPLES_Point *point) {
  assert(dest);
  assert(pattern);
  assert(point);
  PATTERN_Pattern_Blended *bpat = (PATTERN_Pattern_Blended*)pattern;

  TUPLES_Point pattern_a_point, pattern_b_point;
  MATRIX_multiply_tuple(&pattern_a_point, bpat->a->inverse_transform, point);
  MATRIX_multiply_tuple(&pattern_b_point, bpat->b->inverse_transform, point);

  TUPLES_Color a_color, b_color;
  PATTERN_color_at(&a_color, bpat->a, &pattern_a_point);
  PATTERN_color_at(&b_color, bpat->b, &pattern_b_point);
  TUPLES_add(dest, &a_color, &b_color);
  TUPLES_divide(dest, dest, 2);
}

static PATTERN_Pattern *copy_blended_map(const PATTERN_Pattern *pattern) {
  assert(pattern);
  PATTERN_Pattern_Blended *bpat = (PATTERN_Pattern_Blended*)pattern;
  PATTERN_Pattern *a = PATTERN_new_copy(bpat->a);
  PATTERN_Pattern *b = PATTERN_new_copy(bpat->b);
  PATTERN_Pattern *copy = PATTERN_new_blended(a, b);
  copy_base_pattern_in_place(copy, pattern);
  return copy;
}

static void delete_blended_map(PATTERN_Pattern *pattern) {
  assert(pattern);
  PATTERN_Pattern_Blended *bpat = (PATTERN_Pattern_Blended*)pattern;
  PATTERN_delete(bpat->a);
  PATTERN_delete(bpat->b);
  delete_base_pattern(&bpat->pattern);
}

PATTERN_Pattern *PATTERN_new_blended(PATTERN_Pattern *a, PATTERN_Pattern *b) {
  assert(a);
  assert(b);
  PATTERN_Pattern_Blended *pattern = malloc(sizeof(PATTERN_Pattern_Blended));
  if (!pattern) {
    Throw(E_MALLOC_FAILED);
  }
  TUPLES_Color dummy_color;
  TUPLES_init_color(&dummy_color, 0, 0, 0);
  PATTERN_init(&pattern->pattern, &dummy_color, &dummy_color);
  pattern->pattern.at = &blended_map_at;
  pattern->pattern.copy = &copy_blended_map;
  pattern->pattern.delete = &delete_blended_map;
  pattern->a = a;
  pattern->b = b;
  return (PATTERN_Pattern*)pattern;
}
