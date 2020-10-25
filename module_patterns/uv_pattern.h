#ifndef SIMPLE_RAYTRACER_UV_PATTERN_H
#define SIMPLE_RAYTRACER_UV_PATTERN_H

#include <tuples.h>

typedef struct UV_Pattern {
  unsigned int width;
  unsigned int height;
  TUPLES_Color a;
  TUPLES_Color b;
} UV_Pattern;

#endif // SIMPLE_RAYTRACER_UV_PATTERN_H
