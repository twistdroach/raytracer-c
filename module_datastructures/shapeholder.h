#ifndef DATA_STRUCTURES_SHAPEHOLDER_H
#define DATA_STRUCTURES_SHAPEHOLDER_H

#include "ray.h"

typedef enum SHAPEHOLDER_Type {
    SHAPEHOLDER_BASE = 0,
    SHAPEHOLDER_TESTSHAPE,
    SHAPEHOLDER_SPHERE,
    SHAPEHOLDER_PLANE,
    SHAPEHOLDER_TYPE_COUNT
} SHAPEHOLDER_Type;

typedef struct SHAPEHOLDER_Shapeholder {
    void* shape;
    SHAPEHOLDER_Type type;
} SHAPEHOLDER_Shapeholder;

void SHAPEHOLDER_init(SHAPEHOLDER_Shapeholder* holder, void *shape, SHAPEHOLDER_Type type);
void SHAPEHOLDER_intersect(RAY_Intersections* intersections, const SHAPEHOLDER_Shapeholder* holder, const RAY_Ray* ray);
void SHAPEHOLDER_normal_at(TUPLES_Vector* dest, SHAPEHOLDER_Shapeholder* holder, const TUPLES_Point* point);

#endif //DATA_STRUCTURES_SHAPEHOLDER_H
