#include "canvas.h"
#include "exceptions.h"
#include <stdio.h>

int main(void) {
    CEXCEPTION_T e;
    Try {
        CANVAS_Canvas *canvas = CANVAS_new(900, 550);
        TUPLES_Point p = TUPLES_point(0, 1, 0);
        TUPLES_Vector v = TUPLES_multiply(TUPLES_normalize(TUPLES_vector(1, 1.8, 0)), 11.25);

        TUPLES_Vector graviwind = TUPLES_vector(-0.01, -0.1, 0);

        TUPLES_Color red = TUPLES_color(1, 0, 0);

        while (p.x < canvas->width && p.x >= 0 &&
               p.y < canvas->height && p.y >= 0) {
            CANVAS_write_pixel(canvas, p.x, canvas->height - p.y, red);
            p = TUPLES_add(p, v);
            v = TUPLES_add(v, graviwind);
            printf("%.0f %.0f\n", p.x, p.y);
        }

        char *filename = "parabola.ppm";
        printf("Writing file %s...", filename);
        CANVAS_write_to_file(canvas, filename);
        printf("done\n");
        CANVAS_delete(canvas);
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
