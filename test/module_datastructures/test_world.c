#include <unity.h>
#include "world.h"
#include "lights.h"
#include "test_world_utils.h"

void setUp() {}
void tearDown() {}

void test_world_creation() {
    LIGHTS_PointLight pl;
    TUPLES_Point pl_origin;
    TUPLES_Color pl_color;

    TUPLES_init_point(&pl_origin, -10, 10, -10);
    TUPLES_init_color(&pl_color, 1, 1, 1);
    LIGHTS_init_pointlight(&pl, &pl_origin, &pl_color);

    WORLD_World* world = WORLD_new(&pl);
    TEST_ASSERT_NOT_NULL(world);
    WORLD_delete(world);
}

void test_construct_default_world() {
    WORLD_World* world = construct_test_world();
    TEST_ASSERT_EQUAL_PTR(pl, WORLD_get_light(world));
    TEST_ASSERT_EQUAL_UINT(2, WORLD_get_object_count(world));
    TEST_ASSERT_EQUAL_PTR(s1, WORLD_get_object(world,0));
    TEST_ASSERT_EQUAL_PTR(s2, WORLD_get_object(world, 1));
    destruct_test_world(world);
}

void test_intersect_world_with_ray() {
    WORLD_World* world = construct_test_world();
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    RAY_Intersections* intersections = WORLD_intersect(world, ray);
    TEST_ASSERT_EQUAL(4, intersections->count);
    TEST_ASSERT_EQUAL_DOUBLE(4.0, intersections->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(4.5, intersections->xs[1].t);
    TEST_ASSERT_EQUAL_DOUBLE(5.5, intersections->xs[2].t);
    TEST_ASSERT_EQUAL_DOUBLE(6.0, intersections->xs[3].t);
    RAY_delete_intersections(intersections);
    RAY_delete(ray);
    destruct_test_world(world);
}

void test_shading_an_intersection() {
    WORLD_World* world = construct_test_world();
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    SPHERE_Sphere* s = WORLD_get_object(world, 0);
    RAY_Xs i;
    i.t = 4.0;
    i.object = s;

    RAY_Computations* comps = RAY_prepare_computations(&i, ray);
    TUPLES_Color c;
    WORLD_shade_hit(&c, world, comps);

    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.38066, 0.47583, 0.2855);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, &c));

    RAY_delete(ray);
    destruct_test_world(world);
    RAY_delete_computations(comps);
}

void test_shading_an_intersection_from_inside() {
    WORLD_World* world = construct_test_world();

    TUPLES_Point light_origin;
    TUPLES_init_point(&light_origin, 0, 0.25, 0);
    TUPLES_Color light_color;
    TUPLES_init_color(&light_color, 1, 1, 1);
    LIGHTS_PointLight* light = LIGHTS_new_pointlight(&light_origin, &light_color);
    WORLD_set_light(world, light);

    RAY_Ray* ray = RAY_new(0, 0, 0, 0, 0, 1);
    SPHERE_Sphere* s = WORLD_get_object(world, 1);
    RAY_Xs i;
    i.t = 0.5;
    i.object = s;

    RAY_Computations* comps = RAY_prepare_computations(&i, ray);
    TUPLES_Color c;
    WORLD_shade_hit(&c, world, comps);
//    printf("over_point (%s): is shadowed %u\n", TUPLES_to_string(&comps->over_point), WORLD_is_shadowed(world, &comps->over_point));
//    printf("point (%s): is shadowed %u\n", TUPLES_to_string(&comps->point), WORLD_is_shadowed(world, &comps->point));
//    printf("Got color: %s\n", TUPLES_to_string(&c));
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.90498, 0.90498, 0.90498);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, &c));

    RAY_delete(ray);
    destruct_test_world(world);
    LIGHTS_delete_pointlight(light);
    RAY_delete_computations(comps);
}

void test_color_when_ray_misses() {
    WORLD_World* world = construct_test_world();
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 1, 0);
    TUPLES_Color c;
    WORLD_color_at(&c, world, ray);

    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0, 0,0);
    TEST_ASSERT(TUPLES_is_equal(&expected, &c));

    RAY_delete(ray);
    destruct_test_world(world);
}

void test_color_when_ray_hits() {
    WORLD_World* world = construct_test_world();
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 0, 1);
    TUPLES_Color c;
    WORLD_color_at(&c, world, ray);

    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.38066, 0.47583,0.2855);
    TEST_ASSERT(TUPLES_is_equal(&expected, &c));

    RAY_delete(ray);
    destruct_test_world(world);
}

void test_color_with_intersection_behind_ray() {
    WORLD_World* world = construct_test_world();
    SPHERE_Sphere* outer = WORLD_get_object(world, 0);
    outer->material->ambient = 1;
    SPHERE_Sphere* inner = WORLD_get_object(world, 1);
    inner->material->ambient = 1;

    RAY_Ray* ray = RAY_new(0, 0, 0.75, 0, 0, -1);
    TUPLES_Color c;
    WORLD_color_at(&c, world, ray);

    TEST_ASSERT_TRUE(TUPLES_is_equal(&inner->material->color, &c));

    RAY_delete(ray);
    destruct_test_world(world);
}

void test_no_shadow_when_nothing_collinear_with_point_and_light() {
    WORLD_World* world = construct_test_world();
    TUPLES_Point p;
    TUPLES_init_point(&p, 0, 10, 0);
    TEST_ASSERT_FALSE(WORLD_is_shadowed(world, &p));
    destruct_test_world(world);
}

void test_shadow_when_object_is_between_point_and_light() {
    WORLD_World* world = construct_test_world();
    TUPLES_Point p;
    TUPLES_init_point(&p, 10, -10, 10);
    TEST_ASSERT_TRUE(WORLD_is_shadowed(world, &p));
    destruct_test_world(world);
}

void test_shadow_when_object_is_behind_the_light() {
    WORLD_World* world = construct_test_world();
    TUPLES_Point p;
    TUPLES_init_point(&p, -20, 20, -20);
    TEST_ASSERT_FALSE(WORLD_is_shadowed(world, &p));
    destruct_test_world(world);
}

void test_shadow_when_object_is_behind_the_point() {
    WORLD_World* world = construct_test_world();
    TUPLES_Point p;
    TUPLES_init_point(&p, -2, 2, -2);
    TEST_ASSERT_FALSE(WORLD_is_shadowed(world, &p));
    destruct_test_world(world);
}

void test_shade_hit_with_a_point_in_shadow() {
    TUPLES_Point light_p;
    TUPLES_Color light_c;
    TUPLES_init_point(&light_p, 0, 0, -10);
    TUPLES_init_color(&light_c, 1, 1, 1);
    LIGHTS_PointLight light;
    LIGHTS_init_pointlight(&light, &light_p, &light_c);
    WORLD_World* world = WORLD_new(&light);

    SPHERE_Sphere* s1 = SPHERE_new();
    WORLD_add_object(world, s1);

    SPHERE_Sphere* s2 = SPHERE_new();
    WORLD_add_object(world, s2);
    MATRIX_Matrix* transform = MATRIX_new_translation(0, 0, 10);
    SPHERE_set_transform(s2, transform);
    MATRIX_delete(transform);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, 5, 0, 0, 1);

    RAY_Xs i;
    i.t = 4.0;
    i.object = s2;

    RAY_Computations* comps = RAY_prepare_computations(&i, &ray);
    TUPLES_Color c;
    WORLD_shade_hit(&c, world, comps);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.1, 0.1, 0.1);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, &c));

    RAY_delete_computations(comps);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_world_creation);
    RUN_TEST(test_construct_default_world);
    RUN_TEST(test_intersect_world_with_ray);
    RUN_TEST(test_shading_an_intersection);
    RUN_TEST(test_shading_an_intersection_from_inside);
    RUN_TEST(test_color_when_ray_misses);
    RUN_TEST(test_color_when_ray_hits);
    RUN_TEST(test_color_with_intersection_behind_ray);
    RUN_TEST(test_no_shadow_when_nothing_collinear_with_point_and_light);
    RUN_TEST(test_shadow_when_object_is_between_point_and_light);
    RUN_TEST(test_shadow_when_object_is_behind_the_light);
    RUN_TEST(test_shadow_when_object_is_behind_the_point);
    RUN_TEST(test_shade_hit_with_a_point_in_shadow);
    return UNITY_END();
}
