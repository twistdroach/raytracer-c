#ifndef DATA_STRUCTURES_MATERIAL_H
#define DATA_STRUCTURES_MATERIAL_H

#include "tuples.h"
#include "lights.h"

typedef struct MATERIAL_Material {
    TUPLES_Color color;
    double ambient, diffuse, specular, shininess;
} MATERIAL_Material;

MATERIAL_Material* MATERIAL_new();
void MATERIAL_delete(MATERIAL_Material* m);
bool MATERIAL_is_equal(const MATERIAL_Material* m1, const MATERIAL_Material* m2);
void MATERIAL_copy(MATERIAL_Material* dest, const MATERIAL_Material* src);

TUPLES_Color MATERIAL_lighting(const MATERIAL_Material* material, const LIGHTS_PointLight* light, TUPLES_Point position, TUPLES_Vector eye_vector, TUPLES_Vector normal_vector, bool in_shadow);

#endif //DATA_STRUCTURES_MATERIAL_H
