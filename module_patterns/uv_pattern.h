#ifndef SIMPLE_RAYTRACER_UV_PATTERN_H
#define SIMPLE_RAYTRACER_UV_PATTERN_H

#include <tuples.h>

typedef struct UV_Pattern {
  unsigned int width;
  unsigned int height;
  TUPLES_Color a;
  TUPLES_Color b;
} UV_Pattern;

void UV_PATTERN_copy(UV_Pattern* dest, const UV_Pattern* src);
void UV_PATTERN_init(UV_Pattern* pattern, unsigned int u, unsigned int v, const TUPLES_Color* a, const TUPLES_Color* b);
void UV_PATTERN_pattern_at(TUPLES_Color* result, UV_Pattern* patter, double u, double v);

void UV_PATTERN_spherical_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_planar_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_cylinder_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_cube_map(double* u, double* v, const TUPLES_Point* point);

#endif // SIMPLE_RAYTRACER_UV_PATTERN_H
