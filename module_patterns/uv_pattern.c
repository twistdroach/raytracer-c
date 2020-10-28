#include "uv_pattern.h"
#include <assert.h>
#include <math.h>

typedef struct UV_Pattern {
  unsigned int width;
  unsigned int height;
  TUPLES_Color a;
  TUPLES_Color b;
} UV_Pattern;

const TUPLES_Color *UV_PATTERN_get_color_a(const UV_Pattern *pattern) {
  assert(pattern);
  return &pattern->a;
}

const TUPLES_Color *UV_PATTERN_get_color_b(const UV_Pattern *pattern) {
  assert(pattern);
  return &pattern->b;
}

void UV_PATTERN_pattern_at(TUPLES_Color *result, UV_Pattern *pattern, double u, double v) {
  assert(result);
  assert(pattern);
  int u2 = floor(u * pattern->width);
  int v2 = floor(v * pattern->height);

  if ((u2 + v2) % 2 == 0) {
    TUPLES_copy(result, &pattern->a);
  } else {
    TUPLES_copy(result, &pattern->b);
  }
}

UV_Pattern *UV_PATTERN_new(unsigned int u, unsigned int v, const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);
  UV_Pattern *ptn = malloc(sizeof(UV_Pattern));
  if (!ptn) {
    Throw(E_MALLOC_FAILED);
  }
  UV_PATTERN_init(ptn, u, v, a, b);
  return ptn;
}

void UV_PATTERN_init(UV_Pattern* pattern, unsigned int u, unsigned int v, const TUPLES_Color* a, const TUPLES_Color* b) {
  assert(pattern);
  assert(a);
  assert(b);
  TUPLES_copy(&pattern->a, a);
  TUPLES_copy(&pattern->b, b);
  pattern->width = u;
  pattern->height = v;
}

UV_Pattern *UV_PATTERN_copy(const UV_Pattern* src) {
  assert(src);
  UV_Pattern *p = malloc(sizeof(UV_Pattern));
  if (!p) {
    Throw(E_MALLOC_FAILED);
  }
  *p = *src;
  return p;
}

void UV_PATTERN_spherical_map(double* u, double* v, const TUPLES_Point* point) {
  assert(point);
  double theta = atan2(point->x, point->z);

  TUPLES_Vector vec;
  TUPLES_init_vector(&vec, point->x, point->y, point->z);
  double radius = TUPLES_magnitude(&vec);

  double phi = acos(point->y / radius);

  double raw_u = theta / (2 * M_PI);

  *u = 1 - (raw_u + 0.5);
  *v = 1 - (phi / M_PI);
}

void UV_PATTERN_planar_map(double* u, double* v, const TUPLES_Point* point) {
  assert(u);
  assert(v);
  assert(point);

  *u = fmod(point->x, 1);
  if (*u < 0)
    *u = 1 + *u;
  *v = fmod(point->z, 1);
  if (*v < 0)
    *v = 1 + *v;
}

//TODO - fix cylinder map end-caps
void UV_PATTERN_cylinder_map(double* u, double* v, const TUPLES_Point* point) {
  assert(u);
  assert(v);
  assert(point);

  double theta = atan2(point->x, point->z);
  double raw_u = theta / (2 * M_PI);
  *u = 1 - (raw_u + 0.5);

  *v = fmod(point->y, 1);
  if (*v < 0) {
    *v = 1 + *v;
  }
}

void UV_PATTERN_cube_map(double* u, double* v, const TUPLES_Point* point) {
  assert(u);
  assert(v);
  assert(point);
}

void UV_PATTERN_delete(UV_Pattern *pattern) {
  assert(pattern);
  free(pattern);
}
