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
        TUPLES_Color* red = TUPLES_new_color(1, 0, 0);

        TUPLES_Point* ray_origin = TUPLES_new_point(0, 0, -5);
        SPHERE_Sphere* sphere = SPHERE_new();
        for (uint y=0; y < canvas_pixels - 1; y++) {
            double world_y = half - pixel_size * y;
            for (uint x=0; x < canvas_pixels - 1; x++) {
                double world_x = -half + pixel_size * x;
                TUPLES_Point position;
                TUPLES_init_point(&position, world_x, world_y, wall_z);
                TUPLES_Vector v_tmp;
                TUPLES_subtract(&v_tmp, ray_origin, &position);
                TUPLES_normalize(&v_tmp);
                RAY_Ray r;
                RAY_init_from_tuples(&r, &position, &v_tmp);
                RAY_Intersections* xs = SPHERE_intersect(sphere, &r, NULL);
                if (RAY_hit(xs)) {
                    CANVAS_write_pixel(canvas, x, y, red);
                }
                RAY_delete_intersections(xs);
                TUPLES_destroy_all(&position, &v_tmp);
                RAY_destroy(&r);
            }
        }
        char *filename = "sphere_shadow.ppm";
        printf("Writing file %s...", filename);
        CANVAS_write_to_file(canvas, filename);
        printf("done\n");
        SPHERE_delete(sphere);
        CANVAS_delete(canvas);
        TUPLES_delete_all(red, ray_origin);
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
