#include "uv_pattern.h"
#include <assert.h>
#include <math.h>

#define UV_PATTERN_FIELDS() \
  void (*pattern_at)(TUPLES_Color *result, UV_Pattern *pattern, double u, double v); \
  UV_Pattern *(*copy)(const UV_Pattern *src);

typedef struct UV_Pattern {
  UV_PATTERN_FIELDS()
} UV_Pattern;

/* ------- checker pattern ---------- */
typedef struct checker_pattern {
  union {
    UV_Pattern uv_pattern;
    struct { UV_PATTERN_FIELDS() };
  };
  TUPLES_Color a;
  TUPLES_Color b;
  unsigned int width;
  unsigned int height;
} checker_pattern;

static void checker_pattern_at(TUPLES_Color *result, UV_Pattern *uv_pattern, double u, double v) {
  assert(result);
  assert(uv_pattern);
  checker_pattern *pattern = (checker_pattern*)uv_pattern;
  int u2 = floor(u * pattern->width);
  int v2 = floor(v * pattern->height);

  if ((u2 + v2) % 2 == 0) {
    TUPLES_copy(result, &pattern->a);
  } else {
    TUPLES_copy(result, &pattern->b);
  }
}

static UV_Pattern *checker_copy(const UV_Pattern *src) {
  assert(src);
  const checker_pattern *src_pattern = (checker_pattern*) src;
  checker_pattern *dest = malloc(sizeof(checker_pattern));
  if (!dest) {
    Throw(E_MALLOC_FAILED);
  }
  *dest = *src_pattern;
  return (UV_Pattern*)dest;
}

UV_Pattern *UV_PATTERN_new_checkers(unsigned int u, unsigned int v, const TUPLES_Color *a, const TUPLES_Color *b) {
  assert(a);
  assert(b);
  checker_pattern *pattern = malloc(sizeof(checker_pattern));
  if (!pattern) {
    Throw(E_MALLOC_FAILED);
  }
  TUPLES_copy(&pattern->a, a);
  TUPLES_copy(&pattern->b, b);
  pattern->width = u;
  pattern->height = v;
  pattern->uv_pattern.pattern_at = &checker_pattern_at;
  pattern->uv_pattern.copy = &checker_copy;
  return (UV_Pattern*)pattern;
}

/* ------- align check pattern ---------- */
typedef struct align_check_pattern {
  union {
    UV_Pattern uv_pattern;
    struct { UV_PATTERN_FIELDS() };
  };
  TUPLES_Color main;
  TUPLES_Color ul;
  TUPLES_Color ur;
  TUPLES_Color bl;
  TUPLES_Color br;
} align_check_pattern;

/**
 * The point of this uv_pattern is to make a square with each
 * corner having a different color.  This let's us check cube
 * mapping.
 * @param result  color to be returned
 * @param uv_pattern a UV_Pattern* created by UV_Pattern_new_align_check()
 * @param u
 * @param v
 */
static void align_check_pattern_at(TUPLES_Color *result, UV_Pattern *uv_pattern, double u, double v) {
  assert(result);
  assert(uv_pattern);
  align_check_pattern *pattern = (align_check_pattern*)uv_pattern;
  if (v > 0.8) {
    if (u < 0.2) {
      TUPLES_copy(result, &pattern->ul);
    } else if (u > 0.8) {
      TUPLES_copy(result, &pattern->ur);
    }
  } else if(v < 0.2) {
    if (u < 0.2) {
      TUPLES_copy(result, &pattern->bl);
    } else if (u > 0.8) {
      TUPLES_copy(result, &pattern->br);
    }
  } else {
    TUPLES_copy(result, &pattern->main);
  }
}

static UV_Pattern *align_check_copy(const UV_Pattern *src) {
  assert(src);
  const align_check_pattern *src_pattern = (align_check_pattern*) src;
  align_check_pattern *dest = malloc(sizeof(align_check_pattern));
  if (!dest) {
    Throw(E_MALLOC_FAILED);
  }
  *dest = *src_pattern;
  return (UV_Pattern*)dest;
}

UV_Pattern *UV_PATTERN_new_align_check(const TUPLES_Color *main, const TUPLES_Color *ul, const TUPLES_Color* ur, const TUPLES_Color* bl, const TUPLES_Color* br) {
  assert(main);
  assert(ul);
  assert(ur);
  assert(bl);
  assert(br);
  align_check_pattern *pattern = malloc(sizeof(align_check_pattern));
  if (!pattern) {
    Throw(E_MALLOC_FAILED);
  }
  TUPLES_copy(&pattern->main, main);
  TUPLES_copy(&pattern->ul, ul);
  TUPLES_copy(&pattern->ur, ur);
  TUPLES_copy(&pattern->bl, bl);
  TUPLES_copy(&pattern->br, br);
  pattern->uv_pattern.pattern_at = &align_check_pattern_at;
  pattern->uv_pattern.copy = &align_check_copy;
  return (UV_Pattern*)pattern;
}

/* ------- image pattern ---------- */
typedef struct image_pattern {
  union {
    UV_Pattern uv_pattern;
    struct {
      UV_PATTERN_FIELDS()
    };
  };
  const CANVAS_Canvas *canvas;
} image_pattern;

static void image_pattern_at(TUPLES_Color *result, UV_Pattern *uv_pattern, double u, double v) {
  assert(result);
  assert(uv_pattern);
  const image_pattern *pattern = (image_pattern*) uv_pattern;
  v = 1 - v;
  double x = u * (CANVAS_get_width(pattern->canvas) - 1);
  double y = v * (CANVAS_get_height(pattern->canvas) - 1);
  printf("Getting (%.2f, %.2f)\n", x, y);
  TUPLES_copy(result, CANVAS_read_pixel(pattern->canvas, (uint)round(x), (uint)round(y)));
}

static UV_Pattern *image_copy(const UV_Pattern *src) {
  assert(src);
  const image_pattern *src_pattern = (image_pattern*) src;
  image_pattern *dest = malloc(sizeof(image_pattern));
  if (!dest) {
    Throw(E_MALLOC_FAILED);
  }
  *dest = *src_pattern;
  dest->canvas = CANVAS_copy(src_pattern->canvas);
  return (UV_Pattern*)dest;
}

UV_Pattern *UV_PATTERN_new_image(const CANVAS_Canvas *canvas) {
  assert(canvas);
  image_pattern *pattern = malloc(sizeof(image_pattern));
  if (!pattern) {
    Throw(E_MALLOC_FAILED);
  }
  //TODO - we should likely make a copy of the canvas and delete it when the patteern is deleted
  pattern->canvas = canvas;
  pattern->pattern_at = &image_pattern_at;
  pattern->copy = &image_copy;
  return (UV_Pattern*)pattern;
}
/* ------- generic methods ---------- */

UV_Pattern *UV_PATTERN_copy(const UV_Pattern* src) {
  assert(src);
  return src->copy(src);
}

void UV_PATTERN_pattern_at(TUPLES_Color *result, UV_Pattern *pattern, double u, double v) {
  assert(result);
  assert(pattern);
  pattern->pattern_at(result, pattern, u, v);
}

void UV_PATTERN_delete(UV_Pattern *pattern) {
  assert(pattern);
  free(pattern);
}
/* ------- mapping functions ---------- */

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

void UV_PATTERN_cube_map(double* u, double* v, enum UV_PATTERN_Cube_Face face, const TUPLES_Point* point) {
  assert(u);
  assert(v);
  assert(point);
  switch (face) {
  case UV_PATTERN_LEFT:
    *u = fmod(point->z + 1, 2.0) / 2.0;
    *v = fmod(point->y + 1, 2.0) / 2.0;
    break;
  case UV_PATTERN_RIGHT:
    *u = fmod(1 - point->z, 2.0) / 2.0;
    *v = fmod(point->y + 1, 2.0) / 2.0;
    break;
  case UV_PATTERN_FRONT:
    *u = fmod(point->x + 1, 2.0) / 2.0;
    *v = fmod(point->y + 1, 2.0) / 2.0;
    break;
  case UV_PATTERN_BACK:
    *u = fmod(1 - point->x, 2.0) / 2.0;
    *v = fmod(point->y + 1, 2.0) / 2.0;
    break;
  case UV_PATTERN_UP:
    *u = fmod(point->x + 1, 2.0) / 2.0;
    *v = fmod(1 - point->z, 2.0) / 2.0;
    break;
  case UV_PATTERN_DOWN:
    *u = fmod(point->x + 1, 2.0) / 2.0;
    *v = fmod(point->z + 1, 2.0) / 2.0;
    break;
  default:
    assert(0);
    Throw(E_INVALID_ARGUMENT);
    *u = 0;
    *v = 0;
  }
}

enum UV_PATTERN_Cube_Face UV_PATTERN_face_from_point(const TUPLES_Point *point) {
  assert(point);
  double abs_x = fabs(point->x);
  double abs_y = fabs(point->y);
  double abs_z = fabs(point->z);
  double max = UTILITIES_max(abs_x, abs_y, abs_z);

  if (double_equal(max, point->x))  { return UV_PATTERN_RIGHT; }
  if (double_equal(max, -point->x)) { return UV_PATTERN_LEFT;  }
  if (double_equal(max, point->y))  { return UV_PATTERN_UP;    }
  if (double_equal(max, -point->y)) { return UV_PATTERN_DOWN;  }
  if (double_equal(max, point->z))  { return UV_PATTERN_FRONT; }

  return UV_PATTERN_BACK;
}

