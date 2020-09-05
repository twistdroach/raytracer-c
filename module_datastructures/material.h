#ifndef DATA_STRUCTURES_MATERIAL_H
#define DATA_STRUCTURES_MATERIAL_H

//#include "pattern.h"
#include "tuples.h"
#include "lights.h"
typedef struct PATTERN_Pattern PATTERN_Pattern;
typedef struct SHAPE_Shape SHAPE_Shape;

typedef struct MATERIAL_Material {
    TUPLES_Color color;
    double ambient, diffuse, specular, shininess, reflective;
    PATTERN_Pattern* pattern;
} MATERIAL_Material;

MATERIAL_Material* MATERIAL_new();
void MATERIAL_delete(MATERIAL_Material* m);
bool MATERIAL_is_equal(const MATERIAL_Material* m1, const MATERIAL_Material* m2);
MATERIAL_Material* MATERIAL_new_copy(const MATERIAL_Material* src);
void MATERIAL_lighting(TUPLES_Color* dest, const SHAPE_Shape* shape, const LIGHTS_PointLight* light, const TUPLES_Point* position, const TUPLES_Vector* eye_vector, const TUPLES_Vector* normal_vector, bool in_shadow);
void MATERIAL_set_pattern(MATERIAL_Material* material, const PATTERN_Pattern* pattern);

#endif //DATA_STRUCTURES_MATERIAL_H
