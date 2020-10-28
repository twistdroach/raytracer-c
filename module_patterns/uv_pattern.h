#ifndef SIMPLE_RAYTRACER_UV_PATTERN_H
#define SIMPLE_RAYTRACER_UV_PATTERN_H

#include <tuples.h>

typedef struct UV_Pattern UV_Pattern;

UV_Pattern *UV_PATTERN_copy(const UV_Pattern *src);
UV_Pattern *UV_PATTERN_new(unsigned int u, unsigned int v, const TUPLES_Color *a, const TUPLES_Color *b);
void UV_PATTERN_init(UV_Pattern* pattern, unsigned int u, unsigned int v, const TUPLES_Color* a, const TUPLES_Color* b);
void UV_PATTERN_delete(UV_Pattern *pattern);
void UV_PATTERN_pattern_at(TUPLES_Color* result, UV_Pattern* patter, double u, double v);

const TUPLES_Color *UV_PATTERN_get_color_a(const UV_Pattern *pattern);
const TUPLES_Color *UV_PATTERN_get_color_b(const UV_Pattern *pattern);

void UV_PATTERN_spherical_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_planar_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_cylinder_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_cube_map(double* u, double* v, const TUPLES_Point* point);

#endif // SIMPLE_RAYTRACER_UV_PATTERN_H
