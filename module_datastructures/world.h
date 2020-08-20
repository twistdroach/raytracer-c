#ifndef DATA_STRUCTURES_WORLD_H
#define DATA_STRUCTURES_WORLD_H

#include "lights.h"
#include "sphere.h"
#include "intersections.h"

typedef struct WORLD_World WORLD_World;

WORLD_World* WORLD_new(const LIGHTS_PointLight* light);
void WORLD_delete(WORLD_World* world);

/** Add a sphere to the world.  In the future this will be
 * expanded to other object types.
 *
 * Ownership of the actual sphere remains with the caller, this
 * just adds a reference to it.
 *
 * @param world
 * @param sphere
 */
void WORLD_add_object(WORLD_World* world, SPHERE_Sphere* sphere);

/**
 * Gets a point to an object in the world
 * @param world
 * @param index
 * @return
 */
SPHERE_Sphere* WORLD_get_object(const WORLD_World* world, unsigned int index);
unsigned int WORLD_get_object_count(const WORLD_World* world);
const LIGHTS_PointLight* WORLD_get_light(const WORLD_World* world);
void WORLD_set_light(WORLD_World* world, const LIGHTS_PointLight*);
RAY_Intersections* WORLD_intersect(const WORLD_World* world, const RAY_Ray* ray);
void WORLD_shade_hit(TUPLES_Color* dest, const WORLD_World* world, const INTERSECTION_Intersection* computation);
void WORLD_color_at(TUPLES_Color* dest, const WORLD_World* world, const RAY_Ray* ray);
bool WORLD_is_shadowed(const WORLD_World* world, const TUPLES_Point* point);
#endif //DATA_STRUCTURES_WORLD_H
