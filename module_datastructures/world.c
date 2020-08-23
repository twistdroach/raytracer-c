#include <assert.h>
#include "exceptions.h"
#include "world.h"

typedef struct WORLD_World {
    const LIGHTS_PointLight* light;
    SPHERE_Sphere**  objects;
    unsigned int object_count;
} WORLD_World;

void WORLD_init(WORLD_World* world, const LIGHTS_PointLight* light) {
    assert(world);
    assert(light);
    world->light = light;
    world->objects = NULL;
    world->object_count = 0;
}

WORLD_World* WORLD_new(const LIGHTS_PointLight* light) {
    assert(light);
    WORLD_World* world = malloc(sizeof(WORLD_World));
    if (!world)
        Throw(E_MALLOC_FAILED);
    WORLD_init(world, light);
    return world;
}

void WORLD_destroy(WORLD_World* world) {
    assert(world);
    free(world->objects);
    world->objects = NULL;
    world->object_count = 0;
}

void WORLD_delete(WORLD_World* world) {
    assert(world);
    WORLD_destroy(world);
    free(world);
}

void WORLD_set_light(WORLD_World* world, const LIGHTS_PointLight* light) {
    assert(world);
    assert(light);
    world->light = light;
}

inline const LIGHTS_PointLight* WORLD_get_light(const WORLD_World* world) {
    assert(world);
    return world->light;
}

void WORLD_add_object(WORLD_World* world, SPHERE_Sphere* sphere) {
    assert(world);
    assert(sphere);
    SPHERE_Sphere** tmpptr = reallocarray(world->objects, sizeof(SPHERE_Sphere*), world->object_count + 1);
    if (!tmpptr) {
        Throw(E_MALLOC_FAILED);
    } else {
        world->objects = tmpptr;
    }
    world->objects[world->object_count] = sphere;
    world->object_count++;
}

inline SPHERE_Sphere* WORLD_get_object(const WORLD_World* world, unsigned int index) {
    assert(world);
    assert(index < world->object_count);
    return world->objects[index];
}

inline unsigned int WORLD_get_object_count(const WORLD_World* world) {
    assert(world);
    return world->object_count;
}

RAY_Intersections* WORLD_intersect(const WORLD_World* world, const RAY_Ray* ray) {
    assert(world);
    assert(ray);
    RAY_Intersections* intersections = RAY_new_intersections();
    for (unsigned int i=0; i<world->object_count; i++) {
        SPHERE_intersect(WORLD_get_object(world, i), ray, intersections);
    }
    RAY_sort_intersections(intersections);
    return intersections;
}

void WORLD_shade_hit(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* computation) {
    assert(world);
    assert(computation);

    bool shadowed = WORLD_is_shadowed(world, &computation->over_point);

    MATERIAL_lighting(dest,
                      SPHERE_get_material(computation->object),
                      world->light,
                      &computation->point,
                      &computation->eyev,
                      &computation->normalv,
                      shadowed);
}

void WORLD_color_at(TUPLES_Color* dest, const WORLD_World* world, const RAY_Ray* ray) {
    assert(dest);
    assert(world);
    assert(ray);
    RAY_Intersections* intersections = WORLD_intersect(world, ray);
    RAY_Xs* hit = RAY_hit(intersections);
    if (!hit) {
        TUPLES_init_color(dest, 0, 0, 0);
    } else {
        RAY_Computations* comps = RAY_prepare_computations(hit, ray);
        WORLD_shade_hit(dest, world, comps);
        RAY_delete_computations(comps);
    }
    RAY_delete_intersections(intersections);
}

bool WORLD_is_shadowed(const WORLD_World* world, const TUPLES_Point* point) {
    TUPLES_Vector direction;
    TUPLES_subtract(&direction, LIGHTS_get_origin(WORLD_get_light(world)), point);
    double distance = TUPLES_magnitude(&direction);
    TUPLES_normalize(&direction);

    RAY_Ray ray;
    RAY_init_from_tuples(&ray, point, &direction);
    struct RAY_Intersections* intersections = WORLD_intersect(world, &ray);
    RAY_Xs* hit = RAY_hit(intersections);
    bool is_shadowed = false;
    if (hit && hit->t < distance) {
        is_shadowed = true;
    }

    RAY_delete_intersections(intersections);

    return is_shadowed;
}
