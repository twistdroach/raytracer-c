#include <assert.h>
#include <math.h>

#include "exceptions.h"
#include "world.h"
#include "shape.h"

typedef struct WORLD_World {
    const LIGHTS_PointLight* light;
    void**  objects;
    unsigned int object_count;
} WORLD_World;

unsigned int WORLD_default_ttl = 5;

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

void WORLD_add_object(WORLD_World* world, void* shape) {
    assert(world);
    assert(shape);
    void** tmpptr = reallocarray(world->objects, sizeof(void*), world->object_count + 1);
    if (!tmpptr) {
        Throw(E_MALLOC_FAILED);
    } else {
        world->objects = tmpptr;
    }
    world->objects[world->object_count] = shape;
    world->object_count++;
}

inline void* WORLD_get_object(const WORLD_World* world, unsigned int index) {
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
        SHAPE_intersect(intersections, WORLD_get_object(world, i), ray);
    }
    RAY_sort_intersections(intersections);
    return intersections;
}

void WORLD_shade_hit(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* computation, unsigned int ttl) {
    assert(world);
    assert(computation);

    double intensity = LIGHTS_intensity_at(WORLD_get_light(world), &computation->over_point, world);

    MATERIAL_lighting(dest,
                      (SHAPE_Shape*)computation->object,
                      world->light,
                      &computation->over_point,
                      &computation->eyev,
                      &computation->normalv,
                      intensity);

    TUPLES_Color reflected, refracted;
    WORLD_reflected_color(&reflected, world, computation, ttl);
    WORLD_refracted_color(&refracted, world, computation, ttl);
    const MATERIAL_Material* material = SHAPE_get_material(computation->object);
    if (material->reflective > 0 && material->transparency > 0) {
        double reflectance = RAY_schlick(computation);
        TUPLES_multiply(&reflected, &reflected, reflectance);
        TUPLES_multiply(&refracted, &refracted, 1 - reflectance);
    }
    TUPLES_add(dest, dest, &reflected);
    TUPLES_add(dest, dest, &refracted);
}

void WORLD_color_at(TUPLES_Color* dest, const WORLD_World* world, const RAY_Ray* ray, unsigned int ttl) {
    assert(dest);
    assert(world);
    assert(ray);
    RAY_Intersections* intersections = WORLD_intersect(world, ray);
    RAY_Xs* hit = RAY_hit(intersections);
    if (!hit) {
        TUPLES_init_color(dest, 0, 0, 0);
    } else {
        RAY_Computations* comps = RAY_prepare_computations(hit, ray, intersections);
        WORLD_shade_hit(dest, world, comps, ttl);
        RAY_delete_computations(comps);
    }
    RAY_delete_intersections(intersections);
}

bool WORLD_is_shadowed(const WORLD_World* world, const TUPLES_Point* light_position, const TUPLES_Point* point) {
    assert(world);
    assert(light_position);
    assert(point);

    TUPLES_Vector direction;
    TUPLES_subtract(&direction, light_position, point);
    double distance = TUPLES_magnitude(&direction);
    if (double_equal(0, distance)) return false;
    TUPLES_normalize(&direction);

    RAY_Ray ray;
    RAY_init_from_tuples(&ray, point, &direction);
    struct RAY_Intersections* intersections = WORLD_intersect(world, &ray);
    RAY_Xs* hit = RAY_shadow_hit(intersections);
    bool is_shadowed = false;
    if (hit && hit->t < distance) {
        is_shadowed = true;
    }

    RAY_delete_intersections(intersections);

    return is_shadowed;
}

void WORLD_delete_all_objects(WORLD_World* world) {
    assert(world);
    for (unsigned int ndx = 0; ndx < world->object_count; ndx++) {
        SHAPE_delete_any_type(world->objects[ndx]);
    }
}

void WORLD_reflected_color(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* comps, unsigned int ttl) {
    assert(dest);
    assert(world);
    assert(comps);
    const MATERIAL_Material* material = SHAPE_get_material(comps->object);
    if (ttl < 1 || double_equal(0.0, material->reflective)) {
        TUPLES_init_color(dest, 0, 0, 0);
    } else {
        RAY_Ray reflect_ray;
        RAY_init_from_tuples(&reflect_ray, &comps->over_point, &comps->reflectv);
        WORLD_color_at(dest, world, &reflect_ray, ttl - 1);
        TUPLES_multiply(dest, dest, material->reflective);
    }
}

void WORLD_refracted_color(TUPLES_Color* dest, const WORLD_World* world, const RAY_Computations* comps, unsigned int ttl) {
    assert(dest);
    assert(world);
    assert(comps);
    const MATERIAL_Material* material = SHAPE_get_material(comps->object);
    if (ttl < 1 || double_equal(0.0, material->transparency)) {
        TUPLES_init_color(dest, 0, 0, 0);
    } else {
        double n_ratio = comps->n1 / comps->n2;
        double cos_i = TUPLES_dot(&comps->eyev, &comps->normalv);
        double sin2_t = pow(n_ratio, 2) * (1 - pow(cos_i, 2));
        if (sin2_t > 1.0) {
            //total internal reflection
            TUPLES_init_color(dest, 0, 0, 0);
        } else {
            //calculate actual color
            double cos_t = sqrt(1.0 - sin2_t);
            TUPLES_Vector normalv_calc, eyev_calc, direction;
            TUPLES_multiply(&normalv_calc, &comps->normalv, n_ratio * cos_i - cos_t);
            TUPLES_multiply(&eyev_calc, &comps->eyev, n_ratio);
            TUPLES_subtract(&direction, &normalv_calc, &eyev_calc);

            RAY_Ray refract_ray;
            RAY_init_from_tuples(&refract_ray, &comps->under_point, &direction);

            WORLD_color_at(dest, world, &refract_ray, ttl - 1);
            TUPLES_multiply(dest, dest, material->transparency);
        }
    }
}
