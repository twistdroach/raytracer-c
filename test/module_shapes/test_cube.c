#include <unity.h>
#include <cube.h>

void setUp() {}
void tearDown() {}

void helper_check_local_intersect_t(double ox, double oy, double oz,
                                    double dx, double dy, double dz,
                                    double t1, double t2) {
    CUBE_Cube* cube = CUBE_new();
    RAY_Ray ray;
    RAY_init(&ray, ox, oy, oz, dx, dy, dz);
    RAY_Intersections* xs = RAY_new_intersections();
    CUBE_local_intersect(xs, cube, &ray);
    TEST_ASSERT_EQUAL(2, xs->count);
    TEST_ASSERT_EQUAL_DOUBLE(t1, xs->xs[0].t);
    TEST_ASSERT_EQUAL_DOUBLE(t2, xs->xs[1].t);
    RAY_delete_intersections(xs);
    CUBE_delete(cube);
}

void test_ray_intersects_a_cube() {
    helper_check_local_intersect_t(5, 0.5, 0, -1, 0, 0, 4, 6);
    helper_check_local_intersect_t(-5, 0.5, 0, 1, 0, 0, 4, 6);
    helper_check_local_intersect_t(0.5, 5, 0, 0, -1, 0, 4, 6);
    helper_check_local_intersect_t(0.5, -5, 0, 0, 1, 0, 4, 6);
    helper_check_local_intersect_t(0.5, 0, 5, 0, 0, -1, 4, 6);
    helper_check_local_intersect_t(0.5, 0, -5, 0, 0, 1, 4, 6);
    helper_check_local_intersect_t(0, 0.5, 0, 0, 0, 1, -1, 1);
}

void helper_check_local_no_intersect_t(double ox, double oy, double oz,
                                    double dx, double dy, double dz) {
    CUBE_Cube* cube = CUBE_new();
    RAY_Ray ray;
    RAY_init(&ray, ox, oy, oz, dx, dy, dz);
    RAY_Intersections* xs = RAY_new_intersections();
    CUBE_local_intersect(xs, cube, &ray);
    TEST_ASSERT_EQUAL(0, xs->count);
    RAY_delete_intersections(xs);
    CUBE_delete(cube);
}

void test_ray_misses_a_cube() {
    helper_check_local_no_intersect_t(-2, 0, 0, 0.2673, 0.5345, 0.8018);
    helper_check_local_no_intersect_t(0, -2, 0, 0.8018, 0.2673, 0.5345);
    helper_check_local_no_intersect_t(0, 0, -2, 0.5345, 0.8018, 0.2673);
    helper_check_local_no_intersect_t(2, 0, 2, 0, 0, -1);
    helper_check_local_no_intersect_t(0, 2, 2, 0, -1, 0);
    helper_check_local_no_intersect_t(2, 2, 0, -1, 0, 0);
}

void helper_check_normal_at_point(double px, double py, double pz,
                                  double nx, double ny, double nz) {
    CUBE_Cube* cube = CUBE_new();
    TUPLES_Point p;
    TUPLES_init_point(&p, px, py, pz);
    TUPLES_Vector result;
    RAY_Intersections* intersections = RAY_new_intersections();
    CUBE_local_normal_at(&result, cube, &p, NULL);
    TEST_ASSERT_EQUAL_DOUBLE(nx, result.x);
    TEST_ASSERT_EQUAL_DOUBLE(ny, result.y);
    TEST_ASSERT_EQUAL_DOUBLE(nz, result.z);
    RAY_delete_intersections(intersections);
    CUBE_delete(cube);
}

void test_normal_on_surface_of_cube() {
    helper_check_normal_at_point(1, 0.5, -0.8, 1, 0, 0);
    helper_check_normal_at_point(-1, -0.2, 0.9, -1, 0, 0);
    helper_check_normal_at_point(-0.4, 1, -0.1, 0, 1, 0);
    helper_check_normal_at_point(0.3, -1, -0.7, 0, -1, 0);
    helper_check_normal_at_point(-0.6, 0.3, 1, 0, 0, 1);
    helper_check_normal_at_point(1, 1, 1, 1, 0, 0);
    helper_check_normal_at_point(-1, -1, -1, -1, 0, 0);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ray_intersects_a_cube);
    RUN_TEST(test_ray_misses_a_cube);
    RUN_TEST(test_normal_on_surface_of_cube);
    return UNITY_END();
}
