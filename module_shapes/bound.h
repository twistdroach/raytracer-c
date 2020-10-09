#ifndef SIMPLE_RAYTRACER_BOUND_H
#define SIMPLE_RAYTRACER_BOUND_H

#include <tuples.h>
#include <matrix.h>
#include <ray.h>

typedef struct BOUND_Box {
    TUPLES_Point min;
    TUPLES_Point max;
} BOUND_Box;

BOUND_Box* BOUND_new();
void BOUND_init(BOUND_Box* box);

void BOUND_delete(BOUND_Box* box);

void BOUND_add_point_from_tuple(BOUND_Box* box, const TUPLES_Point* point);
void BOUND_add_point(BOUND_Box* box, double x, double y, double z);
void BOUND_add_box(BOUND_Box* dest, const BOUND_Box* src);
bool BOUND_contains_point(const BOUND_Box* box, const TUPLES_Point* point);
bool BOUND_contains_box(const BOUND_Box* outer, const BOUND_Box* inner);
void BOUND_transform(BOUND_Box* dest, const BOUND_Box* box, const MATRIX_Matrix* transformation);
bool BOUND_intersect(const BOUND_Box* box, const RAY_Ray* ray);
void BOUND_split(const BOUND_Box* orig_box, BOUND_Box* dest_left, BOUND_Box* dest_right);
#endif //SIMPLE_RAYTRACER_BOUND_H
