#ifndef SIMPLE_RAYTRACER_UV_PATTERN_H
#define SIMPLE_RAYTRACER_UV_PATTERN_H

#include <canvas.h>
#include <tuples.h>

typedef struct UV_Pattern UV_Pattern;

enum UV_PATTERN_Cube_Face {
  UV_PATTERN_LEFT,
  UV_PATTERN_RIGHT,
  UV_PATTERN_FRONT,
  UV_PATTERN_BACK,
  UV_PATTERN_UP,
  UV_PATTERN_DOWN,
  UV_PATTERN_SIZE
};

UV_Pattern *UV_PATTERN_new_align_check(const TUPLES_Color *main, const TUPLES_Color *ul, const TUPLES_Color* ur, const TUPLES_Color* bl, const TUPLES_Color* br);
UV_Pattern *UV_PATTERN_new_checkers(unsigned int u, unsigned int v, const TUPLES_Color *a, const TUPLES_Color *b);
UV_Pattern *UV_PATTERN_new_image(const CANVAS_Canvas *canvas);
void UV_PATTERN_delete(UV_Pattern *pattern);
#define UV_PATTERN_delete_all(...) Fn_apply(UV_Pattern, UV_PATTERN_delete, __VA_ARGS__);

UV_Pattern *UV_PATTERN_copy(const UV_Pattern *src);
void UV_PATTERN_pattern_at(TUPLES_Color* result, UV_Pattern* patter, double u, double v);

//map functions map 3d point on a shape to a 2d uv-pattern
void UV_PATTERN_spherical_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_planar_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_cylinder_map(double* u, double* v, const TUPLES_Point* point);
void UV_PATTERN_cube_map(double* u, double* v, enum UV_PATTERN_Cube_Face face, const TUPLES_Point* point);
enum UV_PATTERN_Cube_Face UV_PATTERN_face_from_point(const TUPLES_Point *point);

#endif // SIMPLE_RAYTRACER_UV_PATTERN_H
