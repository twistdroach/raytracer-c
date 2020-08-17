#ifndef DATA_STRUCTURES_WORLD_H
#define DATA_STRUCTURES_WORLD_H

#include "lights.h"
#include "sphere.h"

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
SPHERE_Sphere* WORLD_get_object(const WORLD_World* world, unsigned int index);
unsigned int WORLD_get_object_count(const WORLD_World* world);
const LIGHTS_PointLight* WORLD_get_light(const WORLD_World* world);
RAY_Intersections* WORLD_intersect(WORLD_World* world, RAY_Ray* ray);

#endif //DATA_STRUCTURES_WORLD_H
