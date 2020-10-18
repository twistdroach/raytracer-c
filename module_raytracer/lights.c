#include <assert.h>
#include <exceptions.h>
#include "lights.h"
#include "world.h"

double pointlight_intensity_at(const LIGHTS_PointLight* light, const TUPLES_Point* point, const WORLD_World* world) {
    assert(light);
    assert(point);
    assert(world);
    if (WORLD_is_shadowed(world, &light->position, point)) {
        return 0.0;
    }
    return 1.0;
}

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
    pl->intensity_at = &pointlight_intensity_at;
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

double arealight_intensity_at(const LIGHTS_Light* gen_light, const TUPLES_Point* point, const WORLD_World* world) {
    assert(gen_light);
    assert(point);
    assert(world);
    const LIGHTS_AreaLight* light = (const LIGHTS_AreaLight*) gen_light;
    double total = 0.0;
    TUPLES_Point light_point;
    for (unsigned int v = 0; v<light->vsteps; v++) {
        for (unsigned int u = 0; u<light->usteps; u++) {
            LIGHTS_point_on_area_light(&light_point, light, u, v);
            if (!WORLD_is_shadowed(world, &light_point, point)) {
                total += 1.0;
            }
        }
    }
    return total / light->samples;
}

LIGHTS_AreaLight* LIGHTS_new_arealight(const TUPLES_Point* corner, const TUPLES_Vector* uvec, unsigned int usteps, const TUPLES_Vector* vvec, unsigned int vsteps, const TUPLES_Color* color) {
    assert(corner);
    assert(uvec);
    assert(vvec);
    assert(color);
    LIGHTS_AreaLight* light = malloc(sizeof(LIGHTS_AreaLight));
    if (!light) {
        Throw(E_MALLOC_FAILED);
    }

    TUPLES_copy(&light->corner, corner);
    TUPLES_copy(&light->intensity, color);
    light->usteps = usteps;
    TUPLES_init_vector(&light->uvec, uvec->x / usteps,
                                     uvec->y / usteps,
                                     uvec->z / usteps);
    light->vsteps = vsteps;
    TUPLES_init_vector(&light->vvec, vvec->x / vsteps,
                       vvec->y / vsteps,
                       vvec->z / vsteps);
    light->samples = usteps * vsteps;

    TUPLES_Vector uvec_tmp, vvec_tmp;
    TUPLES_multiply(&uvec_tmp, uvec, 0.5);
    TUPLES_multiply(&vvec_tmp, vvec, 0.5);

    TUPLES_add(&light->position, corner, &uvec_tmp);
    TUPLES_add(&light->position, &light->position, &vvec_tmp);
    light->intensity_at = arealight_intensity_at;
    return light;
}

void LIGHTS_point_on_area_light(TUPLES_Point* dest, const LIGHTS_AreaLight* light, unsigned int u, unsigned int v) {
    assert(dest);
    assert(light);
    TUPLES_Vector uvec_tmp, vvec_tmp;
    TUPLES_multiply(&uvec_tmp, &light->uvec, u + 0.5);
    TUPLES_multiply(&vvec_tmp, &light->vvec, v + 0.5);
    TUPLES_add(dest, &light->corner, &uvec_tmp);
    TUPLES_add(dest, dest, &vvec_tmp);
}

void LIGHTS_delete_arealight(LIGHTS_AreaLight* l) {
    assert(l);
    free(l);
}
