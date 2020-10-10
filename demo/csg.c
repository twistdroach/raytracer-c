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
#include <cube.h>
#include <csg.h>

CEXCEPTION_T global_exception;
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

    SPHERE_Sphere* middle = SPHERE_new();
    MATRIX_Matrix* middle_transform = MATRIX_new_translation(-0.5, 1, 0.5);
    SPHERE_set_transform(middle, middle_transform);
    MATRIX_delete(middle_transform);
    MATERIAL_Material* middle_material = MATERIAL_new();
    PATTERN_Pattern* middle_pattern = PATTERN_new_gradient(&red, &blue);
    MATRIX_Matrix* middle_pattern_translate = MATRIX_new_translation(0.5, 0, 0);
    MATRIX_Matrix* middle_pattern_scale = MATRIX_new_scaling(2, 2, 2);
    MATRIX_Matrix* middle_pattern_transform = MATRIX_multiply(middle_pattern_scale, middle_pattern_translate);
    PATTERN_set_transform(middle_pattern, middle_pattern_transform);
    MATRIX_delete_all(middle_pattern_scale, middle_pattern_transform, middle_pattern_translate);
    MATERIAL_set_pattern(middle_material, middle_pattern);
    PATTERN_delete(middle_pattern);
    middle_material->diffuse = 0.7;
    middle_material->specular = 0.3;
    SPHERE_set_material(middle, middle_material);
    MATERIAL_delete(middle_material);

    CUBE_Cube* cube = CUBE_new();
    MATRIX_Matrix* cube_translate = MATRIX_new_translation(0.5, 0.5, 0.35);
    MATRIX_Matrix* cube_rot = MATRIX_new_rotation_y(M_PI_4);
    MATRIX_Matrix* cube_transform = MATRIX_multiply_many(cube_rot, cube_translate);
    CUBE_set_transform(cube, cube_transform);
    MATRIX_delete_all(cube_transform, cube_rot, cube_translate);
    MATERIAL_Material* cube_mat = MATERIAL_new();
    cube_mat->transparency = 1.0;
    cube_mat->ambient = 0.0;
    cube_mat->diffuse = 0.0;
    cube_mat->specular = 0.0;
    cube_mat->shadow_calc = false;
    CUBE_set_material(cube, cube_mat);
    MATERIAL_delete(cube_mat);

    CSG_Csg* csg = CSG_new(CSG_Difference, middle, cube);
    WORLD_add_object(world, csg);
}

int main(void) {
    Try {
                LOGGER_log(LOGGER_INFO, "Building world...\n");
                TUPLES_Point* light_position = TUPLES_new_point(10, 10, -10);
                TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
                LIGHTS_PointLight* light = LIGHTS_new_pointlight(light_position, light_color);
                TUPLES_delete_all(light_position, light_color);

                CAMERA_Camera* camera = CAMERA_new(1000, 500, M_PI / 3.0);
                TUPLES_Point* from = TUPLES_new_point(0, 1.5, -5);
                TUPLES_Point* to = TUPLES_new_point(0, 1, 0);
                TUPLES_Vector* up = TUPLES_new_vector(0, 1, 0);
                MATRIX_Matrix* camera_transform = CAMERA_view_transform(from, to, up);
                CAMERA_set_transform(camera, camera_transform);
                TUPLES_delete_all(from, to, up);
                MATRIX_delete(camera_transform);

                WORLD_World* world = WORLD_new(light);
                build_world(world);
                LOGGER_log(LOGGER_INFO, "Rendering...\n");
                UTILITIES_Timer* render_timer = UTILITIES_Timer_start();
                CANVAS_Canvas* canvas = CAMERA_render(camera, world);
                UTILITIES_Timer_Results render_results = UTILITIES_Timer_stop(render_timer);

                char *filename = "csg.ppm";
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
            } Catch(global_exception) {
        if (global_exception == E_MALLOC_FAILED)
            printf("Malloc failed.  Exiting\n");
        else if (global_exception == E_FILE_FAILED)
            printf("Failed to open test.ppm\n");
        else
            printf("Unknown exception %i occurred\n", global_exception);
    }
    return 0;
}
