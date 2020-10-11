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
#include <wavefrontobj.h>

CEXCEPTION_T global_exception;
static void build_world(WORLD_World* world) {
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
}

static void log_perf(UTILITIES_Timer_Results results, char* prefix) {
    LOGGER_log(LOGGER_INFO, "%sWall: %.2f User: %.2f System: %.2f\n", prefix,  results.wall_time_seconds,
               results.user_time_seconds, results.system_time_seconds);
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        LOGGER_log(LOGGER_ERROR, "Must specify obj file to load.\n");
        exit(EXIT_FAILURE);
    }
    char* input_file = argv[1];
    double obj_scale = 1.0;
    if (argc == 3) {
        char* scale_string = argv[2];
        char* err = NULL;
        obj_scale = strtod(scale_string, &err);
        if (scale_string == err) {
            LOGGER_log(LOGGER_ERROR, "Error parsing scale factor\n");
            exit(EXIT_FAILURE);
        }
    }

    Try {
                LOGGER_log(LOGGER_INFO, "Building world...\n");
                TUPLES_Point* light_position = TUPLES_new_point(-10, 10, -10);
                TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
                LIGHTS_PointLight* light = LIGHTS_new_pointlight(light_position, light_color);
                TUPLES_delete_all(light_position, light_color);

                CAMERA_Camera* camera = CAMERA_new(1920,1080, M_PI / 3.0);
                TUPLES_Point* from = TUPLES_new_point(0, 1.5, -5);
                TUPLES_Point* to = TUPLES_new_point(0, 1, 0);
                TUPLES_Vector* up = TUPLES_new_vector(0, 1, 0);
                MATRIX_Matrix* camera_transform = CAMERA_view_transform(from, to, up);
                CAMERA_set_transform(camera, camera_transform);
                TUPLES_delete_all(from, to, up);
                MATRIX_delete(camera_transform);

                WORLD_World* world = WORLD_new(light);
                build_world(world);
                UTILITIES_Timer* parse_timer = UTILITIES_Timer_start();
                WAVEFRONTOBJ_Obj* obj = WAVEFRONTOBJ_parse_file_by_name(input_file);
                WAVEFRONTOBJ_normalize(obj);
                UTILITIES_Timer_Results parse_results = UTILITIES_Timer_stop(parse_timer);
                log_perf(parse_results, "Parse time: ");
                GROUP_Group* obj_group = WAVEFRONTOBJ_get_default_group(obj);
                MATRIX_Matrix* obj_group_scale = MATRIX_new_scaling(obj_scale, obj_scale, obj_scale);
                MATRIX_Matrix* current_transform = GROUP_get_transform(obj_group);
                MATRIX_Matrix* new_transform = MATRIX_multiply(obj_group_scale, current_transform);
                GROUP_set_transform(obj_group, new_transform);
                MATRIX_delete_all(obj_group_scale, new_transform);
                MATERIAL_Material* obj_group_material = MATERIAL_new();
                obj_group_material->refractive_index = 1.5;
                obj_group_material->ambient = 0;
                obj_group_material->diffuse = 0;
                obj_group_material->reflective = 1.0;
                obj_group_material->transparency = 1.0;
                obj_group_material->shininess = 400;
                GROUP_set_material(obj_group, obj_group_material);
                MATERIAL_delete(obj_group_material);
                LOGGER_log(LOGGER_INFO, "Computing bounds...\n");
                UTILITIES_Timer* divide_timer = UTILITIES_Timer_start();
                SHAPE_divide((SHAPE_Shape*)obj_group, 1000);
                UTILITIES_Timer_Results divide_results = UTILITIES_Timer_stop(divide_timer);
                log_perf(divide_results, "Divide time: ");
                WORLD_add_object(world, obj_group);
                LOGGER_log(LOGGER_INFO, "Rendering...\n");
                UTILITIES_Timer* render_timer = UTILITIES_Timer_start();
                CANVAS_Canvas* canvas = CAMERA_render(camera, world);
                UTILITIES_Timer_Results render_results = UTILITIES_Timer_stop(render_timer);
                log_perf(render_results, "Render time: ");

                char *filename = "render_obj.ppm";
                LOGGER_log(LOGGER_INFO, "Writing file %s...\n", filename);
                CANVAS_write_to_file(canvas, filename);
                LOGGER_log(LOGGER_INFO, "Cleaning up...\n");
                WORLD_delete_all_objects(world);
                WORLD_delete(world);
                LIGHTS_delete_pointlight(light);
                CAMERA_delete(camera);
                CANVAS_delete(canvas);
            } Catch(global_exception) {
        if (global_exception == E_MALLOC_FAILED) {
            printf("Malloc failed.  Exiting\n");
            exit(EXIT_FAILURE);
        } else if (global_exception == E_FILE_FAILED) {
            printf("Failed to open %s\n", input_file);
            exit(EXIT_FAILURE);
        } else {
            printf("Unknown exception %i occurred\n", global_exception);
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}
