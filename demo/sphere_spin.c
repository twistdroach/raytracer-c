#include "logger.h"

#include "canvas.h"
#include "exceptions.h"
#include "ray.h"
#include <stdio.h>
#include <sphere.h>
#include <cube.h>
#include <plane.h>
#include <camera.h>
#include "pattern.h"
#include <math.h>
#include <assert.h>

CEXCEPTION_T global_exception;
void build_world(WORLD_World* world) {
    TUPLES_Color red, green, blue, white, black;
    TUPLES_init_color(&red, 1, 0, 0);
    TUPLES_init_color(&green, 0, 1, 0);
    TUPLES_init_color(&blue, 0, 0, 1);
    TUPLES_init_color(&black, 0, 0, 0);
    TUPLES_init_color(&white, 1, 1, 1);
    PLANE_Plane* floor = PLANE_new();
    WORLD_add_object(world, floor);
    MATERIAL_Material* material = MATERIAL_new();
    material->specular = 0;
    PATTERN_Pattern* floor_pattern = PATTERN_new_checkers(&red, &green);
    MATRIX_Matrix* pattern_transform = MATRIX_new_rotation_y(-M_PI / 5);
    PATTERN_set_transform(floor_pattern, pattern_transform);
    MATRIX_delete(pattern_transform);
    MATERIAL_set_pattern(material, floor_pattern);
    PATTERN_delete(floor_pattern);
    PLANE_set_material(floor, material);
    MATERIAL_delete(material);

    SPHERE_Sphere* middle = SPHERE_new();
    WORLD_add_object(world, middle);
    MATRIX_Matrix* middle_transform = MATRIX_new_translation(-0.5, 1, 0.5);
    SPHERE_set_transform(middle, middle_transform);
    MATRIX_delete(middle_transform);
    MATERIAL_Material* middle_material = MATERIAL_new();
    middle_material->ambient = 0;
    middle_material->diffuse = 0;
    middle_material->refractive_index = 1.5;
    middle_material->refractive_index = 1.20;
    middle_material->refractive_index = 1.10;
    middle_material->transparency = 1.0;
    middle_material->reflective = 1.0;
    middle_material->shadow_calc = true;
    SPHERE_set_material(middle, middle_material);
    MATERIAL_delete(middle_material);
}

void generate_coordinates(TUPLES_Point* dest, double t) {
    assert(dest);
    assert(t > 0 || double_equal(t, 0));
    assert(t <= M_2_PI);
    //double y = 1.75;
    double y = 2.75;
    double x = 5 * cos(t) - 2.5;
    double z = 5 * sin(2 * t);
    TUPLES_init_point(dest, x, y, z);
}

int main(void) {
    Try {
                LOGGER_log(LOGGER_INFO, "Building world...\n");
                TUPLES_Point light_position;
                TUPLES_init_point(&light_position, -10, 10, -10);
                TUPLES_Color light_color;
                TUPLES_init_color(&light_color, 1, 1, 1);
                TUPLES_Vector v1, v2;
                TUPLES_init_vector(&v1, 4, 0, 0);
                TUPLES_init_vector(&v2, 0, 3, 0);

                LIGHTS_AreaLight* light = LIGHTS_new_arealight(&light_position, &v1, 8, &v2, 8, &light_color);
                SEQUENCES_Sequence* seq = SEQUENCES_new_random(128);
                LIGHTS_set_jitter_on_area_light(light, seq);
                SEQUENCES_delete(seq);

                WORLD_World* world = WORLD_new((LIGHTS_Light*)light);
                build_world(world);

                double fps = 60;
                double length_in_seconds = 30;
                double total_frames = fps * length_in_seconds;
                double t_per_frame = (2*M_PI) / total_frames;
                unsigned int frame = 0;

                UTILITIES_Timer* render_timer = UTILITIES_Timer_start();
                for (double t = 0; t < 2 * M_PI; t += t_per_frame) {

                    //CAMERA_Camera* camera = CAMERA_new(320, 240, M_PI / 3.0);
                    //CAMERA_Camera* camera = CAMERA_new(1920, 1080, M_PI / 3.0);
                    CAMERA_Camera* camera = CAMERA_new(3840, 2160, M_PI / 3.0);
                    TUPLES_Point from;
                    generate_coordinates(&from, t);
                    TUPLES_Point* to = TUPLES_new_point(0, 1, 0);
                    TUPLES_Vector* up = TUPLES_new_vector(0, 1, 0);
                    MATRIX_Matrix* camera_transform = CAMERA_view_transform(&from, to, up);
                    CAMERA_set_transform(camera, camera_transform);
                    TUPLES_delete_all(to, up);
                    MATRIX_delete(camera_transform);

                    LOGGER_log(LOGGER_INFO, "Rendering...\n");
                    CANVAS_Canvas* canvas = CAMERA_render(camera, world);

                    const char *filename_template = "sphere_spin_%03u.ppm";
                    char* filename = NULL;
                    Sasprintf(filename, filename_template, frame);
                    LOGGER_log(LOGGER_INFO, "Writing file %s...\n", filename);
                    CANVAS_write_to_file(canvas, filename);
                    LOGGER_log(LOGGER_INFO, "Cleaning up...\n");
                    CAMERA_delete(camera);
                    CANVAS_delete(canvas);
                    frame++;
                }
                UTILITIES_Timer_Results render_results = UTILITIES_Timer_stop(render_timer);
                LOGGER_log(LOGGER_INFO, "Wall: %.2f User: %.2f System: %.2f\n", render_results.wall_time_seconds,
                               render_results.user_time_seconds, render_results.system_time_seconds);
                WORLD_delete_all_objects(world);
                WORLD_delete(world);
                LIGHTS_delete_arealight(light);
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
