//
// Created by zrowitsch on 8/11/20.
//

#include <assert.h>
#include <exceptions.h>
#include "lights.h"

LIGHTS_PointLight* LIGHTS_new_pointlight(const TUPLES_Point* p, const TUPLES_Color* c) {
    assert(p);
    assert(c);
    LIGHTS_PointLight* pl = malloc(sizeof(LIGHTS_PointLight));
    if (!pl)
        Throw(E_MALLOC_FAILED);
    LIGHTS_init_pointlight(pl, p, c);
    return pl;
}

void LIGHTS_init_pointlight(LIGHTS_PointLight* pl, const TUPLES_Point* p, const TUPLES_Color* c) {
    assert(pl);
    assert(p);
    assert(c);
    TUPLES_copy(&pl->position, p);
    TUPLES_copy(&pl->intensity, c);
}

void LIGHTS_destroy_pointlight(LIGHTS_PointLight* pl) {
    assert(pl);
    TUPLES_destroy(&pl->position);
    TUPLES_destroy(&pl->intensity);
}

void LIGHTS_delete_pointlight(LIGHTS_PointLight* pl) {
    assert(pl);
    LIGHTS_destroy_pointlight(pl);
    free(pl);
}
void LIGHTS_copy(LIGHTS_PointLight* dest, const LIGHTS_PointLight* src) {
    assert(dest);
    assert(src);
    TUPLES_copy(&dest->position, &src->position);
    TUPLES_copy(&dest->intensity, &src->intensity);
}
