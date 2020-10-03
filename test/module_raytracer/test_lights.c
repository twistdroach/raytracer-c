#include <unity.h>
#include <tuples.h>
#include <lights.h>

void setUp() {}
void tearDown() {}

void test_pointlight_has_position_and_intensity() {
    TUPLES_Color* c = TUPLES_new_color(1, 1, 1);
    TUPLES_Point* p = TUPLES_new_point(0, 0, 0);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, &pl->intensity));
    TEST_ASSERT_TRUE(TUPLES_is_equal(p, &pl->position));
    TUPLES_delete_all(c, p);
    LIGHTS_delete_pointlight(pl);
}

void test_light_copy() {
    TUPLES_Color* c = TUPLES_new_color(0.1, 0.2, 0.3);
    TUPLES_Point* p = TUPLES_new_point(0.4, 0.5, 0.6);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    LIGHTS_PointLight pl_copy;
    LIGHTS_copy(&pl_copy, pl);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, &pl_copy.intensity));
    TEST_ASSERT_TRUE(TUPLES_is_equal(p, &pl_copy.position));
    TUPLES_delete_all(c, p);
    LIGHTS_delete_pointlight(pl);
}

void test_get_color() {
    TUPLES_Color* c = TUPLES_new_color(0.1, 0.2, 0.3);
    TUPLES_Point* p = TUPLES_new_point(0.4, 0.5, 0.6);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    const TUPLES_Color* c_returned = LIGHTS_get_color(pl);
    TEST_ASSERT_TRUE(TUPLES_is_equal(c, c_returned));
    TUPLES_delete_all(c, p);
    LIGHTS_delete_pointlight(pl);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pointlight_has_position_and_intensity);
    RUN_TEST(test_light_copy);
    RUN_TEST(test_get_color);
    return UNITY_END();
}
