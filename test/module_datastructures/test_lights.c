#include <unity.h>
#include <tuples.h>
#include <lights.h>

void setUp() {}
void tearDown() {}

void test_pointlight_has_position_and_intensity() {
    TUPLES_Color c = TUPLES_color(1, 1, 1);
    TUPLES_Point p = TUPLES_point(0, 0, 0);
    LIGHTS_PointLight* pl = LIGHTS_new_pointlight(p, c);
    TEST_ASSERT_TRUE(TUPLES_is_equal(&c, &pl->intensity));
    TEST_ASSERT_TRUE(TUPLES_is_equal(&p, &pl->position));
    LIGHTS_delete_pointlight(pl);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pointlight_has_position_and_intensity);
    return UNITY_END();
}
