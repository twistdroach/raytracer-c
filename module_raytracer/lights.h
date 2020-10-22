#ifndef DATA_STRUCTURES_LIGHTS_H
#define DATA_STRUCTURES_LIGHTS_H

#include "tuples.h"
#include "sequences.h"

typedef struct WORLD_World WORLD_World;

typedef struct LIGHTS_Light {
    TUPLES_Color intensity;
    TUPLES_Vector uvec;
    unsigned int usteps;
    TUPLES_Vector vvec;
    unsigned int vsteps;
    unsigned int samples;
    TUPLES_Point corner;
    SEQUENCES_Sequence* sequence;
} LIGHTS_Light;

LIGHTS_Light* LIGHTS_new_arealight(const TUPLES_Point* corner, const TUPLES_Vector* uvec, unsigned int usteps, const TUPLES_Vector* vvec, unsigned int vsteps, const TUPLES_Color* color);
void LIGHTS_point_on_area_light(TUPLES_Point* dest, const LIGHTS_Light* light, unsigned int u, unsigned int v);
void LIGHTS_set_jitter_on_area_light(LIGHTS_Light* light, SEQUENCES_Sequence* seq);

LIGHTS_Light* LIGHTS_new_pointlight(const TUPLES_Point* p, const TUPLES_Color* c);
void LIGHTS_destroy(LIGHTS_Light* pl);
void LIGHTS_delete(LIGHTS_Light* pl);
const TUPLES_Color* LIGHTS_get_color(const LIGHTS_Light* light);
double LIGHTS_intensity_at(const LIGHTS_Light* light, const TUPLES_Point* point, const WORLD_World* world);
void LIGHTS_iterate_points_on_light(const LIGHTS_Light* light, void (*each_point)(TUPLES_Point* point, void* context), void* context);
#endif //DATA_STRUCTURES_LIGHTS_H
