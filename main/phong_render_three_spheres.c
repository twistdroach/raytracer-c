#include "logger.h"

#include "canvas.h"
#include "exceptions.h"
#include "ray.h"
#include <stdio.h>
#include <sphere.h>
#include <camera.h>

#include <math.h>
#include <assert.h>

void build_world(WORLD_World* world) {
    SPHERE_Sphere* floor = SPHERE_new();
    WORLD_add_object(world, floor, SHAPEHOLDER_SPHERE);
    MATRIX_Matrix* floor_transform = MATRIX_new_scaling(10, 0.01, 10);
    MATERIAL_Material* material = MATERIAL_new();
    TUPLES_init_color(&material->color, 1, 0.9, 0.9);
    material->specular = 0;
    SPHERE_set_material(floor, material);
    SPHERE_set_transform(floor, floor_transform);
    MATRIX_delete(floor_transform);

    SPHERE_Sphere* left_wall = SPHERE_new();
    WORLD_add_object(world, left_wall, SHAPEHOLDER_SPHERE);
    MATRIX_Matrix* lwtranslation = MATRIX_new_translation(0, 0, 5);
    MATRIX_Matrix* lwroty = MATRIX_new_rotation_y(-M_PI_4);
    MATRIX_Matrix* lwrotx = MATRIX_new_rotation_x(M_PI_2);
    MATRIX_Matrix* lwscaling = MATRIX_new_scaling(10, 0.01, 10);
    MATRIX_Matrix* tmp1 = MATRIX_multiply(lwtranslation, lwroty);
    MATRIX_Matrix* tmp2 = MATRIX_multiply(tmp1, lwrotx);
    MATRIX_Matrix* tmp3 = MATRIX_multiply(tmp2, lwscaling);
    SPHERE_set_transform(left_wall, tmp3);
    MATRIX_delete_all(lwtranslation, lwroty, lwrotx, lwscaling, tmp1, tmp2, tmp3);
    SPHERE_set_material(left_wall, material);

    SPHERE_Sphere* right_wall = SPHERE_new();
    WORLD_add_object(world, right_wall, SHAPEHOLDER_SPHERE);
    MATRIX_Matrix* rwtranslation = MATRIX_new_translation(0, 0, 5);
    MATRIX_Matrix* rwroty = MATRIX_new_rotation_y(M_PI_4);
    MATRIX_Matrix* rwrotx = MATRIX_new_rotation_x(M_PI_2);
    MATRIX_Matrix* rwscaling = MATRIX_new_scaling(10, 0.01, 10);
    tmp1 = MATRIX_multiply(rwtranslation, rwroty);
    tmp2 = MATRIX_multiply(tmp1, rwrotx);
    tmp3 = MATRIX_multiply(tmp2, rwscaling);
    SPHERE_set_transform(right_wall, tmp3);
    MATRIX_delete_all(rwtranslation, rwroty, rwrotx, rwscaling, tmp1, tmp2, tmp3);
    SPHERE_set_material(right_wall, material);

    MATERIAL_delete(material);

    SPHERE_Sphere* middle = SPHERE_new();
    WORLD_add_object(world, middle, SHAPEHOLDER_SPHERE);
    MATRIX_Matrix* middle_transform = MATRIX_new_translation(-0.5, 1, 0.5);
    SPHERE_set_transform(middle, middle_transform);
    MATRIX_delete(middle_transform);
    MATERIAL_Material* middle_material = MATERIAL_new();
    TUPLES_init_color(&middle_material->color, 0.1, 1, 0.5);
    middle_material->diffuse = 0.7;
    middle_material->specular = 0.3;
    SPHERE_set_material(middle, middle_material);
    MATERIAL_delete(middle_material);

    SPHERE_Sphere* right = SPHERE_new();
    WORLD_add_object(world, right, SHAPEHOLDER_SPHERE);
    MATRIX_Matrix* right_translation = MATRIX_new_translation(1.5, 0.5, -0.5);
    MATRIX_Matrix* right_scaling = MATRIX_new_scaling(0.5, 0.5, 0.5);
    MATRIX_Matrix* right_transform = MATRIX_multiply(right_translation, right_scaling);
    SPHERE_set_transform(right, right_transform);
    MATRIX_delete_all(right_translation, right_scaling, right_transform);
    MATERIAL_Material* right_material = MATERIAL_new();
    TUPLES_init_color(&right_material->color, 0.5, 1, 0.1);
    right_material->diffuse = 0.7;
    right_material->specular = 0.3;
    SPHERE_set_material(right, right_material);
    MATERIAL_delete(right_material);

    SPHERE_Sphere* left = SPHERE_new();
    WORLD_add_object(world, left, SHAPEHOLDER_SPHERE);
    MATRIX_Matrix* left_translation = MATRIX_new_translation(-1.5, 0.33, -0.75);
    MATRIX_Matrix* left_scaling = MATRIX_new_scaling(0.33, 0.33, 0.33);
    MATRIX_Matrix* left_transform = MATRIX_multiply(left_translation, left_scaling);
    SPHERE_set_transform(left, left_transform);
    MATRIX_delete_all(left_translation, left_scaling, left_transform);
    MATERIAL_Material* left_material = MATERIAL_new();
    TUPLES_init_color(&left_material->color, 1, 0.8, 0.1);
    left_material->diffuse = 0.7;
    left_material->specular = 0.3;
    SPHERE_set_material(left, left_material);
    MATERIAL_delete(left_material);
}

int main(void) {
    CEXCEPTION_T e;
    Try {
                LOGGER_log(LOGGER_INFO, "Building world...\n");
                TUPLES_Point* light_position = TUPLES_new_point(-10, 10, -10);
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

                char *filename = "phong_render_three_spheres.ppm";
                LOGGER_log(LOGGER_INFO, "Writing file %s...\n", filename);
                CANVAS_write_to_file(canvas, filename);
                LOGGER_log(LOGGER_INFO, "Cleaning up...\n");
                for (unsigned int ndx = 0; ndx < WORLD_get_object_count(world); ndx++) {
                    SHAPEHOLDER_Shapeholder* holder = WORLD_get_object_holder(world, ndx);
                    SPHERE_delete(holder->shape);
                }
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
