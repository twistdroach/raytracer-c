#include <assert.h>
#include <exceptions.h>
#include "lights.h"
#include "world.h"

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

const TUPLES_Point* LIGHTS_get_origin(const LIGHTS_PointLight* light) {
    assert(light);
    return &light->position;
}

const TUPLES_Color* LIGHTS_get_color(const LIGHTS_PointLight* light) {
    assert(light);
    return &light->intensity;

}

double LIGHTS_intensity_at(const LIGHTS_PointLight* light, const TUPLES_Point* point, const WORLD_World* world) {
    assert(light);
    assert(point);
    assert(world);
    if (WORLD_is_shadowed(world, &light->position, point)) {
        return 0.0;
    }
    return 1.0;
}
