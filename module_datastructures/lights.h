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


#endif //DATA_STRUCTURES_LIGHTS_H
