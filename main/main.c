#include "canvas.h"
#include <stdio.h>

int main(void)
{
    CANVAS_Canvas* canvas = CANVAS_new(900, 550);
    TUPLES_Point* p = TUPLES_new_point(0, 1, 0);
    TUPLES_Vector* v = TUPLES_new_vector(1, 1.8, 0);
    TUPLES_normalize(v);
    TUPLES_multiply(v, v, 11.25);

    TUPLES_Vector* graviwind = TUPLES_new_vector(-0.01, -0.1, 0);

    TUPLES_Color* red = TUPLES_new_color(1, 0,0);

    while (p->x < canvas->width && p->x >= 0 &&
            p->y < canvas->height && p->y >= 0) {
        CANVAS_write_pixel(canvas, p->x, canvas->height - p->y, red);
        TUPLES_add(p, p, v);
         TUPLES_add(v, v, graviwind);
         printf("%.0f %.0f\n", p->x, p->y);
    }

    char* filename = "test.ppm";
    printf("Writing file %s\n", filename);
    CANVAS_write_to_file(canvas, filename);
    printf("done\n");
    TUPLES_delete(v);
    TUPLES_delete(graviwind);
    TUPLES_delete(p);
    TUPLES_delete(red);
    CANVAS_delete(canvas);
    return 0;
}
