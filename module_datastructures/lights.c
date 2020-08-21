//
// Created by zrowitsch on 8/11/20.
//

#include <assert.h>
#include <exceptions.h>
#include "lights.h"

LIGHTS_PointLight* LIGHTS_new_pointlight(TUPLES_Point p, TUPLES_Color c) {
    LIGHTS_PointLight* pl = malloc(sizeof(LIGHTS_PointLight));
    if (!pl)
        Throw(E_MALLOC_FAILED);
    LIGHTS_init_pointlight(pl, p, c);
    return pl;
}

void LIGHTS_init_pointlight(LIGHTS_PointLight* pl, TUPLES_Point p, TUPLES_Color c) {
    assert(pl);
    pl->position = p;
    pl->intensity = c;
}

void LIGHTS_destroy_pointlight(LIGHTS_PointLight* pl) {
    assert(pl);
}

void LIGHTS_delete_pointlight(LIGHTS_PointLight* pl) {
    assert(pl);
    LIGHTS_destroy_pointlight(pl);
    free(pl);
}
void LIGHTS_copy(LIGHTS_PointLight* dest, const LIGHTS_PointLight* src) {
    assert(dest);
    assert(src);
    *dest = *src;
}

TUPLES_Point LIGHTS_get_origin(const LIGHTS_PointLight* light) {
    assert(light);
    return light->position;
}

TUPLES_Color LIGHTS_get_color(const LIGHTS_PointLight* light) {
    assert(light);
    return light->intensity;

}
