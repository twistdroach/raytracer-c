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

CANVAS_Canvas *texture_canvases[UV_PATTERN_SIZE];
UV_Pattern *uv_patterns[UV_PATTERN_SIZE];

static void build_patterns() {
  char *filenames[UV_PATTERN_SIZE];
  filenames[UV_PATTERN_LEFT] = "negx.ppm";
  filenames[UV_PATTERN_RIGHT] = "posx.ppm";
  filenames[UV_PATTERN_FRONT] = "posz.ppm";
  filenames[UV_PATTERN_BACK] = "negz.ppm";
  filenames[UV_PATTERN_UP] = "posy.ppm";
  filenames[UV_PATTERN_DOWN] = "negy.ppm";

  for (unsigned int ndx = 0; ndx < UV_PATTERN_SIZE; ndx++) {
    texture_canvases[ndx] = CANVAS_parse_file_by_name(filenames[ndx]);
    uv_patterns[ndx] = UV_PATTERN_new_image(texture_canvases[ndx]);
  }
}

static void build_world(WORLD_World* world) {
    TUPLES_Color red, green, blue, white, black;
    TUPLES_init_color(&red, 1, 0, 0);
    TUPLES_init_color(&green, 0, 1, 0);
    TUPLES_init_color(&blue, 0, 0, 1);
    TUPLES_init_color(&black, 0, 0, 0);
    TUPLES_init_color(&white, 1, 1, 1);

    CUBE_Cube *cube = CUBE_new();
    WORLD_add_object(world, cube);
    MATRIX_Matrix *matrix = MATRIX_new_scaling(1000, 1000, 1000);
    CUBE_set_transform(cube, matrix);
    MATRIX_delete(matrix);
    MATERIAL_Material *material = MATERIAL_new();
    material->diffuse = 0;
    material->specular = 0;
    material->ambient = 1;
    build_patterns();
    PATTERN_Pattern *pattern = PATTERN_new_cube_map(
        uv_patterns[UV_PATTERN_LEFT], uv_patterns[UV_PATTERN_FRONT],
        uv_patterns[UV_PATTERN_RIGHT], uv_patterns[UV_PATTERN_BACK],
        uv_patterns[UV_PATTERN_UP], uv_patterns[UV_PATTERN_DOWN]
        );
    MATERIAL_set_pattern(material, pattern);
    CUBE_set_material(cube, material);

    SPHERE_Sphere* middle = SPHERE_new();
    WORLD_add_object(world, middle);
    MATRIX_Matrix *middle_scale = MATRIX_new_scaling(0.75, 0.75, 0.75);
    MATRIX_Matrix *middle_translate = MATRIX_new_translation(0, 0, 5);
    MATRIX_Matrix* middle_transform = MATRIX_multiply_many(middle_translate, middle_scale);
    SPHERE_set_transform(middle, middle_transform);
    MATRIX_delete_all(middle_transform, middle_translate, middle_scale);
    MATERIAL_Material* middle_material = MATERIAL_new();
    middle_material->ambient = 0;
    middle_material->diffuse = 0.4;
#define nottransparent
#ifdef nottransparent
    middle_material->reflective = 0.6;
#else
  middle_material->reflective = 1.0;
  middle_material->refractive_index = 1.15;
  middle_material->transparency = 1.0;
#endif
    middle_material->shininess = 20;
    middle_material->shadow_calc = false;
    SPHERE_set_material(middle, middle_material);
    MATERIAL_delete(middle_material);
}

void generate_coordinates(TUPLES_Point* dest, double t) {
    assert(dest);
    assert(t > 0 || double_equal(t, 0));
    assert(t <= M_2_PI);
    //double y = 1.75;
//    double y = 2.75;
    double y = 1.1;
//    double x = 5 * cos(t) - 2.5;
  double x = 5 * cos(t) - 1.5;
  double z = 5 * sin(2 * t);
    TUPLES_init_point(dest, x, y, z);
}

int main(void) {
    Try {
                LOGGER_log(LOGGER_INFO, "Building world...\n");
                TUPLES_Point light_position;
                TUPLES_init_point(&light_position, -100, 100, -100);
                TUPLES_Color light_color;
                TUPLES_init_color(&light_color, 1, 1, 1);
//                TUPLES_Vector v1, v2;
//                TUPLES_init_vector(&v1, 4, 0, 0);
//                TUPLES_init_vector(&v2, 0, 3, 0);

//                LIGHTS_Light* light = LIGHTS_new_arealight(&light_position, &v1, 8, &v2, 8, &light_color);
//                SEQUENCES_Sequence* seq = SEQUENCES_new_random(2048);
//                LIGHTS_set_jitter_on_area_light(light, seq);
//                SEQUENCES_delete(seq);
                LIGHTS_Light *light = LIGHTS_new_pointlight(&light_position, &light_color);

                WORLD_World* world = WORLD_new((LIGHTS_Light*)light);
                build_world(world);

                double fps = 30;
                double length_in_seconds = 30;
                double total_frames = fps * length_in_seconds;
                double t_per_frame = (2*M_PI) / total_frames;
                unsigned int frame = 0;

                UTILITIES_Timer* render_timer = UTILITIES_Timer_start();
                for (double t = 0; t < 2 * M_PI; t += t_per_frame) {

                    UTILITIES_Timer* frame_timer = UTILITIES_Timer_start();
//                    CAMERA_Camera* camera = CAMERA_new(320, 240, M_PI / 3.0);
                    CAMERA_Camera* camera = CAMERA_new(1920, 1080, M_PI / 3.0);
//                    CAMERA_Camera* camera = CAMERA_new(3840, 2160, M_PI / 3.0);
                    TUPLES_Point from;
                    generate_coordinates(&from, t);
                    TUPLES_Point* to = TUPLES_new_point(0, 0, 5);
                    TUPLES_Vector* up = TUPLES_new_vector(0, 1, 0);
                    MATRIX_Matrix* camera_transform = CAMERA_view_transform(&from, to, up);
                    CAMERA_set_transform(camera, camera_transform);
                    TUPLES_delete_all(to, up);
                    MATRIX_delete(camera_transform);

                    LOGGER_log(LOGGER_INFO, "Rendering...\n");
                    CANVAS_Canvas* canvas = CAMERA_render(camera, world);

                    const char *filename_template = "texture_map_%03u.ppm";
                    char* filename = NULL;
                    Sasprintf(filename, filename_template, frame);
                    LOGGER_log(LOGGER_INFO, "Writing file %s...\n", filename);
                    CANVAS_write_to_file(canvas, filename);
                    LOGGER_log(LOGGER_INFO, "Cleaning up...\n");
                    CAMERA_delete(camera);
                    CANVAS_delete(canvas);
                    frame++;
                    UTILITIES_Timer_Results frame_results = UTILITIES_Timer_stop(frame_timer);
                    LOGGER_log(LOGGER_INFO, "Last frame - Wall: %.2f User: %.2f System: %.2f\n", frame_results.wall_time_seconds,
                               frame_results.user_time_seconds, frame_results.system_time_seconds);
                }
                UTILITIES_Timer_Results render_results = UTILITIES_Timer_stop(render_timer);
                LOGGER_log(LOGGER_INFO, "Wall: %.2f User: %.2f System: %.2f\n", render_results.wall_time_seconds,
                               render_results.user_time_seconds, render_results.system_time_seconds);
                WORLD_delete_all_objects(world);
                WORLD_delete(world);
                LIGHTS_delete(light);
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
