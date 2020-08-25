#ifndef DATA_STRUCTURES_TEST_WORLD_UTILS_H
#define DATA_STRUCTURES_TEST_WORLD_UTILS_H

#include "unity.h"
#include "world.h"

SPHERE_Sphere* s1;
SPHERE_Sphere* s2;
LIGHTS_PointLight* pl;

WORLD_World* construct_test_world() {
    TUPLES_Point pl_origin;
    TUPLES_Color pl_color;

    TUPLES_init_point(&pl_origin, -10, 10, -10);
    TUPLES_init_color(&pl_color, 1, 1, 1);

    pl = LIGHTS_new_pointlight(&pl_origin, &pl_color);

    WORLD_World* world = WORLD_new(pl);

    s1 = SPHERE_new();
    MATERIAL_Material* s1_material = MATERIAL_new();
    TUPLES_init_color(&s1_material->color, 0.8, 1.0, 0.6);
    s1_material->diffuse = 0.7;
    s1_material->specular = 0.2;
    SHAPE_set_material(&s1->parent, s1_material);
    MATERIAL_delete(s1_material);
    WORLD_add_object(world, s1, SHAPEHOLDER_SPHERE);

    s2 = SPHERE_new();
    MATRIX_Matrix* s2_transform = MATRIX_new_scaling(0.5, 0.5, 0.5);
    SHAPE_set_transform(&s2->parent, s2_transform);
    MATRIX_delete(s2_transform);
    WORLD_add_object(world, s2, SHAPEHOLDER_SPHERE);

    return world;
}

void destruct_test_world(WORLD_World* world) {
    TEST_ASSERT_NOT_NULL(world);
    LIGHTS_delete_pointlight(pl);
    TEST_ASSERT_EQUAL_UINT(2, WORLD_get_object_count(world));
    TEST_ASSERT_NOT_NULL(WORLD_get_object_holder(world, 0));
    TEST_ASSERT_NOT_NULL(WORLD_get_object_holder(world, 1));
    SPHERE_delete(s1);
    SPHERE_delete(s2);
    WORLD_delete(world);
}

#endif //DATA_STRUCTURES_TEST_WORLD_UTILS_H
