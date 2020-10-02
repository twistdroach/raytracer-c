#include "logger.h"

#include "canvas.h"
#include "exceptions.h"
#include "ray.h"
#include <stdio.h>
#include <sphere.h>
#include <plane.h>
#include <camera.h>
#include "pattern.h"
#include <math.h>

CEXCEPTION_T e;
void build_sphere_at(WORLD_World* world, double x, double z) {
    SPHERE_Sphere* s = SPHERE_new();

    double scale_d = UTILITIES_random_double(0.1, .5);
    MATRIX_Matrix* scale = MATRIX_new_scaling(scale_d, scale_d, scale_d);

    double jitter_x = UTILITIES_random_double(-0.5, 0.5);
    double jitter_z = UTILITIES_random_double(-0.5, 0.5);
    x += jitter_x;
    z += jitter_z;
    MATRIX_Matrix* translation = MATRIX_new_translation(x, scale_d, z);

    MATRIX_Matrix* transform = MATRIX_multiply_many(translation, scale);

    SPHERE_set_transform(s, transform);

    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_init_color(&material->color,
        UTILITIES_random_double(0, 1),
        UTILITIES_random_double(0, 1),
        UTILITIES_random_double(0, 1)
    );
    material->reflective = UTILITIES_random_double(0, 1);
    material->refractive_index = 1.5;
    material->transparency = UTILITIES_random_double(0, 1);
    SPHERE_set_material(s, material);

    WORLD_add_object(world, s);
    MATRIX_delete_all(scale, translation, transform);
    MATERIAL_delete(material);
}

void build_world(WORLD_World* world) {
    TUPLES_Color red, green, blue;
    TUPLES_init_color(&red, 1, 0, 0);
    TUPLES_init_color(&green, 0, 1, 0);
    TUPLES_init_color(&blue, 0, 0, 1);
    PLANE_Plane* floor = PLANE_new();
    WORLD_add_object(world, floor);
    MATERIAL_Material* material = MATERIAL_new();
    material->specular = 0;
    PATTERN_Pattern* floor_pattern = PATTERN_new_stripe(&red, &green);
    MATRIX_Matrix* pattern_transform = MATRIX_new_rotation_y(-M_PI / 5);
    PATTERN_set_transform(floor_pattern, pattern_transform);
    MATRIX_delete(pattern_transform);
    MATERIAL_set_pattern(material, floor_pattern);
    PATTERN_delete(floor_pattern);
    PLANE_set_material(floor, material);
    MATERIAL_delete(material);

    for (int i=-10; i < 10; i+=2) {
        for (int j=-1; j < 30; j+=2) {
            build_sphere_at(world, i, j);
        }
    }

}

int main(void) {
    srand(3);
    Try {
                LOGGER_log(LOGGER_INFO, "Building world...\n");
                TUPLES_Point* light_position = TUPLES_new_point(-10, 10, -10);
                TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
                LIGHTS_PointLight* light = LIGHTS_new_pointlight(light_position, light_color);
                TUPLES_delete_all(light_position, light_color);

                CAMERA_Camera* camera = CAMERA_new(1920, 1080, M_PI / 3.0);
                TUPLES_Point* from = TUPLES_new_point(0, 3.5, -5);
                TUPLES_Point* to = TUPLES_new_point(0, 1.5, 0);
                TUPLES_Vector* up = TUPLES_new_vector(0, 1, 0);
                MATRIX_Matrix* camera_transform = CAMERA_view_transform(from, to, up);
                CAMERA_set_transform(camera, camera_transform);
                TUPLES_delete_all(from, to, up);
                MATRIX_delete(camera_transform);

                WORLD_World* world = WORLD_new(light);
                UTILITIES_Timer* build_timer = UTILITIES_Timer_start();
                build_world(world);
                UTILITIES_Timer_Results build_results = UTILITIES_Timer_stop(build_timer);
                LOGGER_log(LOGGER_INFO, "Wall: %.2f User: %.2f System: %.2f\n", build_results.wall_time_seconds,
                           build_results.user_time_seconds, build_results.system_time_seconds);
                LOGGER_log(LOGGER_INFO, "Rendering...\n");
                UTILITIES_Timer* render_timer = UTILITIES_Timer_start();
                CANVAS_Canvas* canvas = CAMERA_render(camera, world);
                UTILITIES_Timer_Results render_results = UTILITIES_Timer_stop(render_timer);

                char *filename = "random_spheres.ppm";
                LOGGER_log(LOGGER_INFO, "Writing file %s...\n", filename);
                CANVAS_write_to_file(canvas, filename);
                LOGGER_log(LOGGER_INFO, "Cleaning up...\n");
                WORLD_delete_all_objects(world);
                WORLD_delete(world);
                LIGHTS_delete_pointlight(light);
                CAMERA_delete(camera);
                CANVAS_delete(canvas);
                LOGGER_log(LOGGER_INFO, "Wall: %.2f User: %.2f System: %.2f\n", render_results.wall_time_seconds,
                           render_results.user_time_seconds, render_results.system_time_seconds);
            } Catch(e) {
        if (e == E_MALLOC_FAILED)
            printf("Malloc failed.  Exiting\n");
        else if (e == E_FILE_FAILED)
            printf("Failed to open test.ppm\n");
        else
            printf("Unknown exception %i occurred\n", e);
    }
    return 0;
}
