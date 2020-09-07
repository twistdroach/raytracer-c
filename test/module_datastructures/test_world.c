#include <unity.h>
#include <plane.h>
#include "world.h"
#include "lights.h"
#include "testutils.h"
#include "test_world_utils.h"
#include "pattern.h"

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
    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 4.0, WORLD_get_object(world, 0));
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], ray, xs);
    TUPLES_Color c;
    WORLD_shade_hit(&c, world, comps, WORLD_default_ttl);

    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.38066, 0.47583, 0.2855);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&expected, &c));

    RAY_delete(ray);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
    RAY_delete_computations(comps);
}

void test_shading_an_intersection_from_inside() {
    //HMM this test fails because the hit is just barely far enough from zero to cause shadow to be triggered.
    //commenting out for now.
    WORLD_World* world = construct_test_world();

    TUPLES_Point light_origin;
    TUPLES_init_point(&light_origin, 0, 0.25, 0);
    TUPLES_Color light_color;
    TUPLES_init_color(&light_color, 1, 1, 1);
    LIGHTS_PointLight* light = LIGHTS_new_pointlight(&light_origin, &light_color);
    WORLD_set_light(world, light);

    RAY_Ray* ray = RAY_new(0, 0, 0, 0, 0, 1);

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 0.5, WORLD_get_object(world, 1));
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], ray, xs);
    TUPLES_Color c;
    WORLD_shade_hit(&c, world, comps, WORLD_default_ttl);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.90498, 0.90498, 0.90498);
    test_tuples(&expected, &c);

    RAY_delete(ray);
    destruct_test_world(world);
    LIGHTS_delete_pointlight(light);
    RAY_delete_intersections(xs);
    RAY_delete_computations(comps);
}

void test_color_when_ray_misses() {
    WORLD_World* world = construct_test_world();
    RAY_Ray* ray = RAY_new(0, 0, -5, 0, 1, 0);
    TUPLES_Color c;
    WORLD_color_at(&c, world, ray, WORLD_default_ttl);

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
    WORLD_color_at(&c, world, ray, WORLD_default_ttl);

    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.38066, 0.47583,0.2855);
    TEST_ASSERT(TUPLES_is_equal(&expected, &c));

    RAY_delete(ray);
    destruct_test_world(world);
}

void test_color_with_intersection_behind_ray() {
    WORLD_World* world = construct_test_world();
    SHAPE_get_material(WORLD_get_object(world, 0))->ambient = 1;
    MATERIAL_Material* inner_material = SHAPE_get_material(WORLD_get_object(world, 1));
    inner_material->ambient = 1;

    RAY_Ray* ray = RAY_new(0, 0, 0.75, 0, 0, -1);
    TUPLES_Color c;
    WORLD_color_at(&c, world, ray, WORLD_default_ttl);

    TEST_ASSERT_TRUE(TUPLES_is_equal(&inner_material->color, &c));

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

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 4.0, WORLD_get_object(world, 1));
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);
    TUPLES_Color c;
    WORLD_shade_hit(&c, world, comps, WORLD_default_ttl);
    TUPLES_Color expected;
    TUPLES_init_color(&expected, 0.1, 0.1, 0.1);
    test_tuples(&expected, &c);

    RAY_delete_intersections(xs);
    RAY_delete_computations(comps);
    WORLD_delete(world);
    SPHERE_delete(s1);
    SPHERE_delete(s2);
}

void test_reflected_color_for_nonreflective_material() {
    WORLD_World* world = construct_test_world();
    RAY_Ray ray;
    RAY_init(&ray, 0, 0, 0, 0, 0,1);
    SHAPE_Shape* s = WORLD_get_object(world, 1);
    SHAPE_get_material(s)->ambient = 1.0;
    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 1.0, s);
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0, 0, 0);
    WORLD_reflected_color(&got, world, comps, WORLD_default_ttl);
    test_tuples(&expected, &got);
    RAY_delete_intersections(xs);
    RAY_delete_computations(comps);
    destruct_test_world(world);
}

void test_reflected_color_for_reflective_material() {
    WORLD_World* world = construct_test_world();
    PLANE_Plane* plane = PLANE_new();
    MATRIX_Matrix* plane_translation = MATRIX_new_translation(0, -1, 0);
    PLANE_get_material(plane)->reflective = 0.5;
    PLANE_set_transform(plane, plane_translation);
    MATRIX_delete(plane_translation);
    WORLD_add_object(world, plane);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -3, 0, -sqrt(2)/2.0, sqrt(2)/2.0);

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, sqrt(2), plane);
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0.19032, 0.2379, 0.14274);
    WORLD_reflected_color(&got, world, comps, WORLD_default_ttl);
    test_tuples(&expected, &got);

    RAY_delete_intersections(xs);
    RAY_delete_computations(comps);
    destruct_test_world(world);
}

void test_shade_hit_for_reflective_material() {
    WORLD_World* world = construct_test_world();
    PLANE_Plane* plane = PLANE_new();
    MATRIX_Matrix* plane_translation = MATRIX_new_translation(0, -1, 0);
    PLANE_get_material(plane)->reflective = 0.5;
    PLANE_set_transform(plane, plane_translation);
    MATRIX_delete(plane_translation);
    WORLD_add_object(world, plane);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -3, 0, -sqrt(2)/2.0, sqrt(2)/2.0);

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, sqrt(2), plane);
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0.87677, 0.92436, 0.82918);
    WORLD_shade_hit(&got, world, comps, WORLD_default_ttl);
    test_tuples(&expected, &got);

    RAY_delete_intersections(xs);
    RAY_delete_computations(comps);
    destruct_test_world(world);
}

void test_color_at_with_mutually_reflective_surfaces() {
    TUPLES_Point pl_origin;
    TUPLES_Color pl_color;

    TUPLES_init_point(&pl_origin, 0, 0, 0);
    TUPLES_init_color(&pl_color, 1, 1, 1);

    LIGHTS_PointLight* light = LIGHTS_new_pointlight(&pl_origin, &pl_color);
    WORLD_World* world = WORLD_new(light);

    PLANE_Plane* lower = PLANE_new();
    MATRIX_Matrix* lower_transform = MATRIX_new_translation(0, -1.0, 0);
    PLANE_set_transform(lower, lower_transform);
    PLANE_get_material(lower)->reflective = 1.0;
    MATRIX_delete(lower_transform);
    WORLD_add_object(world, lower);

    PLANE_Plane* upper = PLANE_new();
    MATRIX_Matrix* upper_transform = MATRIX_new_translation(0, 1.0, 0);
    PLANE_set_transform(upper, upper_transform);
    PLANE_get_material(upper)->reflective = 1.0;
    MATRIX_delete(upper_transform);
    WORLD_add_object(world, upper);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, 0, 0, 1, 0);

    TUPLES_Color dest;
    WORLD_color_at(&dest, world, &ray, WORLD_default_ttl);

    WORLD_delete_all_objects(world);
    WORLD_delete(world);
    LIGHTS_delete_pointlight(light);
}

void test_reflected_color_at_max_recursion_depth() {
    WORLD_World* world = construct_test_world();
    PLANE_Plane* plane = PLANE_new();
    PLANE_get_material(plane)->reflective = 0.5;
    MATRIX_Matrix* transform = MATRIX_new_translation(0, -1, 0);
    PLANE_set_transform(plane, transform);
    MATRIX_delete(transform);
    WORLD_add_object(world, plane);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -3, 0, -sqrt(2)/2.0, sqrt(2)/2.0);

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, sqrt(2), plane);
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color reflected_color, black;
    TUPLES_init_color(&black, 0, 0, 0);
    WORLD_reflected_color(&reflected_color, world, comps, 0);

    test_tuples(&black, &reflected_color);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
}

void test_refracted_color_for_opaque_surface() {
    WORLD_World* world = construct_test_world();
    SHAPE_Shape* shape = WORLD_get_object(world, 0);
    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 4.0, shape);
    RAY_add_intersection(xs, 6.0, shape);
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0, 0, 0);

    WORLD_refracted_color(&got, world, comps, 5);

    test_tuples(&expected, &got);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
}

void test_refracted_color_at_max_recursive_depth() {
    WORLD_World* world = construct_test_world();
    SHAPE_Shape* shape = WORLD_get_object(world, 0);
    SHAPE_get_material(shape)->transparency = 1.0;
    SHAPE_get_material(shape)->refractive_index = 1.5;

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -5, 0, 0, 1);
    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, 4.0, shape);
    RAY_add_intersection(xs, 6.0, shape);
    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0, 0, 0);

    WORLD_refracted_color(&got, world, comps, 0);

    test_tuples(&expected, &got);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
}

void test_refracted_color_under_total_internal_reflection() {
    WORLD_World *world = construct_test_world();
    SHAPE_Shape *shape = WORLD_get_object(world, 0);
    SHAPE_get_material(shape)->transparency = 1.0;
    SHAPE_get_material(shape)->refractive_index = 1.5;

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, sqrt(2)/2.0, 0, 1, 0);
    RAY_Intersections *xs = RAY_new_intersections();
    RAY_add_intersection(xs, -sqrt(2)/2.0, shape);
    RAY_add_intersection(xs, sqrt(2)/2.0, shape);
    RAY_Computations *comps = RAY_prepare_computations(&xs->xs[1], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0, 0, 0);

    WORLD_refracted_color(&got, world, comps, 5);

    test_tuples(&expected, &got);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
}

void test_refracted_color_with_refracted_ray() {
    WORLD_World *world = construct_test_world();
    SHAPE_Shape *A = WORLD_get_object(world, 0);
    SHAPE_get_material(A)->ambient = 1.0;
    PATTERN_Pattern* A_test_pattern = PATTERN_new_test();
    MATERIAL_set_pattern(SHAPE_get_material(A), A_test_pattern);
    PATTERN_delete(A_test_pattern);

    SHAPE_Shape *B = WORLD_get_object(world, 1);
    SHAPE_get_material(B)->transparency = 1.0;
    SHAPE_get_material(B)->refractive_index = 1.5;

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, 0.1, 0, 1, 0);

    RAY_Intersections *xs = RAY_new_intersections();
    RAY_add_intersection(xs, -0.9899, A);
    RAY_add_intersection(xs, -0.4899, B);
    RAY_add_intersection(xs, 0.4899, B);
    RAY_add_intersection(xs, 0.9899, A);

    RAY_Computations *comps = RAY_prepare_computations(&xs->xs[2], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0, 0.99888, 0.04725);

    WORLD_refracted_color(&got, world, comps, 5);

    test_tuples(&expected, &got);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
}

void test_shade_hit_with_transparent_material() {
    WORLD_World* world = construct_test_world();
    PLANE_Plane* floor = PLANE_new();
    MATRIX_Matrix* floor_transform = MATRIX_new_translation(0, -1, 0);
    PLANE_set_transform(floor, floor_transform);
    MATRIX_delete(floor_transform);
    PLANE_get_material(floor)->transparency = 0.5;
    PLANE_get_material(floor)->refractive_index = 1.5;
    WORLD_add_object(world, floor);

    SPHERE_Sphere* ball = SPHERE_new();
    MATRIX_Matrix* ball_transform = MATRIX_new_translation(0, -3.5, -0.5);
    SPHERE_set_transform(ball, ball_transform);
    MATRIX_delete(ball_transform);
    SPHERE_get_material(ball)->ambient = 0.5;
    TUPLES_init_color(&SPHERE_get_material(ball)->color, 1, 0, 0);
    WORLD_add_object(world, ball);

    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -3, 0, -sqrt(2)/2.0, sqrt(2)/2.0);

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, sqrt(2), floor);

    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0.93642, 0.68642, 0.68642);

    WORLD_shade_hit(&got, world, comps, 5);

    test_tuples(&expected, &got);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
}

void test_shade_hit_with_a_reflective_transparent_material() {
    WORLD_World* world = construct_test_world();
    RAY_Ray ray;
    RAY_init(&ray, 0, 0, -3, 0, -sqrt(2)/2.0, sqrt(2)/2.0);
    PLANE_Plane* floor = PLANE_new();
    MATRIX_Matrix* floor_transform = MATRIX_new_translation(0, -1, 0);
    PLANE_set_transform(floor, floor_transform);
    MATRIX_delete(floor_transform);
    PLANE_get_material(floor)->reflective = 0.5;
    PLANE_get_material(floor)->transparency = 0.5;
    PLANE_get_material(floor)->refractive_index = 1.5;
    WORLD_add_object(world, floor);

    SPHERE_Sphere* ball = SPHERE_new();
    MATRIX_Matrix* ball_transform = MATRIX_new_translation(0, -3.5, -0.5);
    SPHERE_set_transform(ball, ball_transform);
    MATRIX_delete(ball_transform);
    SPHERE_get_material(ball)->ambient = 0.5;
    TUPLES_init_color(&SPHERE_get_material(ball)->color, 1, 0, 0);
    WORLD_add_object(world, ball);

    RAY_Intersections* xs = RAY_new_intersections();
    RAY_add_intersection(xs, sqrt(2), floor);

    RAY_Computations* comps = RAY_prepare_computations(&xs->xs[0], &ray, xs);

    TUPLES_Color expected, got;
    TUPLES_init_color(&expected, 0.93391, 0.69643, 0.69243);
    WORLD_shade_hit(&got, world, comps, 5);

    test_tuples(&expected, &got);

    RAY_delete_computations(comps);
    RAY_delete_intersections(xs);
    destruct_test_world(world);
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
    RUN_TEST(test_shade_hit_for_reflective_material);
    RUN_TEST(test_reflected_color_for_nonreflective_material);
    RUN_TEST(test_reflected_color_for_reflective_material);
    RUN_TEST(test_color_at_with_mutually_reflective_surfaces);
    RUN_TEST(test_reflected_color_at_max_recursion_depth);
    RUN_TEST(test_refracted_color_for_opaque_surface);
    RUN_TEST(test_refracted_color_at_max_recursive_depth);
    RUN_TEST(test_refracted_color_under_total_internal_reflection);
    RUN_TEST(test_refracted_color_with_refracted_ray);
    RUN_TEST(test_shade_hit_with_transparent_material);
    RUN_TEST(test_shade_hit_with_a_reflective_transparent_material);
    return UNITY_END();
}
