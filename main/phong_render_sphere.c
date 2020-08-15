#include "canvas.h"
#include "exceptions.h"
#include "ray.h"
#include <stdio.h>
#include <sphere.h>

int main(void) {
    CEXCEPTION_T e;
    Try {
                double wall_z = 10;
                double wall_size = 7;
                unsigned int canvas_pixels = 500;
                double pixel_size = wall_size / canvas_pixels;
                double half = wall_size / 2;

                CANVAS_Canvas* canvas = CANVAS_new(canvas_pixels, canvas_pixels);

                TUPLES_Point* ray_origin = TUPLES_new_point(0, 0, -5);

                SPHERE_Sphere* sphere = SPHERE_new();
                MATERIAL_Material* material = MATERIAL_new();
                TUPLES_init_color(&material->color, 1, 0.2, 1);
                SPHERE_set_material(sphere, material);
                MATERIAL_delete(material);

                TUPLES_Point* light_position = TUPLES_new_point(-10, 10, -10);
                TUPLES_Color* light_color = TUPLES_new_color(1, 1, 1);
                LIGHTS_PointLight* light = LIGHTS_new_pointlight(light_position, light_color);
                TUPLES_delete_all(light_position, light_color);

                for (uint y=0; y < canvas_pixels - 1; y++) {
                    double world_y = half - pixel_size * y;
                    for (uint x=0; x < canvas_pixels - 1; x++) {
                        double world_x = -half + pixel_size * x;
                        TUPLES_Point position;
                        TUPLES_Vector v_tmp;
                        RAY_Ray r;

                        TUPLES_init_point(&position, world_x, world_y, wall_z);
                        TUPLES_subtract(&v_tmp, &position, ray_origin);
                        TUPLES_normalize(&v_tmp);
                        RAY_init_from_tuples(&r, ray_origin, &v_tmp);

                        RAY_Intersections* xs = SPHERE_intersect(sphere, &r);
                        RAY_Xs* hit = RAY_hit(xs);

                        if (hit) {
                            TUPLES_Point point_of_intersection;
                            TUPLES_Vector normal;
                            TUPLES_Vector eyev;
                            TUPLES_Color color;

                            RAY_position(&point_of_intersection, &r, hit->t);

                            SPHERE_normal_at(&normal, (SPHERE_Sphere*)hit->object, &point_of_intersection);

                            TUPLES_copy(&eyev, &r.direction);
                            TUPLES_negate(&eyev);

                            MATERIAL_lighting(&color, SPHERE_get_material((SPHERE_Sphere*)hit->object), light, &point_of_intersection, &eyev, &normal);
                            CANVAS_write_pixel(canvas, x, y, &color);
                        }
                        RAY_delete_intersections(xs);
                        RAY_destroy(&r);
                    }
                }
                char *filename = "phong_render_sphere.ppm";
                printf("Writing file %s...", filename);
                CANVAS_write_to_file(canvas, filename);
                printf("done\n");
                LIGHTS_delete_pointlight(light);
                SPHERE_delete(sphere);
                CANVAS_delete(canvas);
                TUPLES_delete_all(ray_origin);
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
