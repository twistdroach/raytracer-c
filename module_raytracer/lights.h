#ifndef DATA_STRUCTURES_LIGHTS_H
#define DATA_STRUCTURES_LIGHTS_H

#include "tuples.h"
#include "sequences.h"

typedef struct WORLD_World WORLD_World;

typedef struct LIGHTS_Light {
    TUPLES_Point position;
    TUPLES_Color intensity;
    double (*intensity_at)(const struct LIGHTS_Light* light, const TUPLES_Point* point, const WORLD_World* world);
} LIGHTS_Light;

typedef struct LIGHTS_Light LIGHTS_PointLight;

typedef struct LIGHTS_AreaLight {
    union {
        LIGHTS_Light light;
        struct {
            TUPLES_Point position;
            TUPLES_Color intensity;
            double (*intensity_at)(const struct LIGHTS_Light* light, const TUPLES_Point* point, const WORLD_World* world);
        };
    };
    TUPLES_Vector uvec;
    unsigned int usteps;
    TUPLES_Vector vvec;
    unsigned int vsteps;
    unsigned int samples;
    TUPLES_Point corner;
    SEQUENCES_Sequence* sequence;
} LIGHTS_AreaLight;

LIGHTS_AreaLight* LIGHTS_new_arealight(const TUPLES_Point* corner, const TUPLES_Vector* uvec, unsigned int usteps, const TUPLES_Vector* vvec, unsigned int vsteps, const TUPLES_Color* color);
void LIGHTS_point_on_area_light(TUPLES_Point* dest, const LIGHTS_AreaLight* light, unsigned int u, unsigned int v);
void LIGHTS_set_jitter_on_area_light(LIGHTS_AreaLight* light, SEQUENCES_Sequence* seq);
void LIGHTS_delete_arealight(LIGHTS_AreaLight* pl);

LIGHTS_PointLight* LIGHTS_new_pointlight(const TUPLES_Point* p, const TUPLES_Color* c);
void LIGHTS_init_pointlight(LIGHTS_PointLight* pl, const TUPLES_Point* p, const TUPLES_Color* c);
void LIGHTS_destroy_pointlight(LIGHTS_PointLight* pl);
void LIGHTS_delete_pointlight(LIGHTS_PointLight* pl);
void LIGHTS_copy(LIGHTS_PointLight* dest, const LIGHTS_PointLight* src);
const TUPLES_Point* LIGHTS_get_origin(const LIGHTS_PointLight* light);
const TUPLES_Color* LIGHTS_get_color(const LIGHTS_PointLight* light);
double LIGHTS_intensity_at(const LIGHTS_Light* light, const TUPLES_Point* point, const WORLD_World* world);
#endif //DATA_STRUCTURES_LIGHTS_H
