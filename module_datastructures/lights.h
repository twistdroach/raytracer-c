#ifndef DATA_STRUCTURES_LIGHTS_H
#define DATA_STRUCTURES_LIGHTS_H

#include "tuples.h"

typedef struct LIGHTS_PointLight {
    TUPLES_Point position;
    TUPLES_Color intensity;
} LIGHTS_PointLight;

LIGHTS_PointLight* LIGHTS_new_pointlight(const TUPLES_Point* p, const TUPLES_Color* c);
void LIGHTS_init_pointlight(LIGHTS_PointLight* pl, const TUPLES_Point* p, const TUPLES_Color* c);
void LIGHTS_destroy_pointlight(LIGHTS_PointLight* pl);
void LIGHTS_delete_pointlight(LIGHTS_PointLight* pl);
void LIGHTS_copy(LIGHTS_PointLight* dest, const LIGHTS_PointLight* src);
const TUPLES_Point* LIGHTS_get_origin(const LIGHTS_PointLight* light);
const TUPLES_Color* LIGHTS_get_color(const LIGHTS_PointLight* light);

#endif //DATA_STRUCTURES_LIGHTS_H
