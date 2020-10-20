#ifndef DATA_STRUCTURES_WORLD_H
#define DATA_STRUCTURES_WORLD_H

#include "lights.h"
#include "sphere.h"
#include "ray.h"

typedef struct WORLD_World WORLD_World;
unsigned int WORLD_default_ttl;

WORLD_World* WORLD_new(const LIGHTS_Light* light);
void WORLD_delete(WORLD_World* world);

/** Add a shape to the world.
 *
 * Ownership of the actual allocation remains with the caller, this
 * just adds a reference to it.
 *
 * @param world
 * @param sphere
 */
void WORLD_add_object(WORLD_World* world, void* shape);

/**
 * Gets a pointer to an object in the world
 * @param world
 * @param index
 * @return
 */
void* WORLD_get_object(const WORLD_World* world, unsigned int index);
unsigned int WORLD_get_object_count(const WORLD_World* world);
/**
 * Will call *_delete on each shape held by the world.
 * This is purely for convenience...
 * @param world
 */
void WORLD_delete_all_objects(WORLD_World* world);
const LIGHTS_PointLight* WORLD_get_light(const WORLD_World* world);
void WORLD_set_light(WORLD_World* world, const LIGHTS_PointLight*);
RAY_Intersections* WORLD_intersect(const WORLD_World* world, const RAY_Ray* ray);
void WORLD_shade_hit(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* computation, unsigned int ttl);
void WORLD_color_at(TUPLES_Color* dest, const WORLD_World* world, const RAY_Ray* ray, unsigned int ttl);
bool WORLD_is_shadowed(const WORLD_World* world, const TUPLES_Point* light_position, const TUPLES_Point* point);
void WORLD_reflected_color(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* comps, unsigned int ttl);
void WORLD_refracted_color(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* comps, unsigned int ttl);
#endif //DATA_STRUCTURES_WORLD_H
