#ifndef DATA_STRUCTURES_CANVAS_H
#define DATA_STRUCTURES_CANVAS_H

#include "tuples.h"
#include <stdlib.h>

typedef struct CANVAS_Canvas_ {
   uint width, height;
   TUPLES_Color* data;
} CANVAS_Canvas;

CANVAS_Canvas* CANVAS_new(uint width, uint height);
void CANVAS_init(CANVAS_Canvas* canvas, uint width, uint height);
void CANVAS_destroy(CANVAS_Canvas* canvas);
void CANVAS_delete(CANVAS_Canvas* canvas);

TUPLES_Color* CANVAS_read_pixel(CANVAS_Canvas* canvas, uint x, uint y);
void CANVAS_write_pixel(CANVAS_Canvas* canvas, uint x, uint y, const TUPLES_Color* color);

#endif //DATA_STRUCTURES_CANVAS_H
