#ifndef DATA_STRUCTURES_CANVAS_H
#define DATA_STRUCTURES_CANVAS_H

#include "tuples.h"
#include <stdlib.h>

#define uint unsigned int

typedef struct CANVAS_Canvas {
  uint width, height;
  TUPLES_Color *data;
} CANVAS_Canvas;

CANVAS_Canvas *CANVAS_new(uint width, uint height);
void CANVAS_init(CANVAS_Canvas *canvas, uint width, uint height);
void CANVAS_destroy(CANVAS_Canvas *canvas);
void CANVAS_delete(CANVAS_Canvas *canvas);
CANVAS_Canvas *CANVAS_copy(const CANVAS_Canvas *canvas);
#define CANVAS_get_width(canvas) ((canvas)->width)
#define CANVAS_get_height(canvas) ((canvas)->height)

/**
 * Returns a ptr to color of the pixel in the canvas.  Should not be freed - canvas will free when deleted.
 * @param canvas
 * @param x
 * @param y
 * @return
 */
TUPLES_Color *CANVAS_read_pixel(const CANVAS_Canvas *canvas, uint x, uint y);
void CANVAS_write_pixel(CANVAS_Canvas *canvas, uint x, uint y, const TUPLES_Color *color);
char *CANVAS_get_ppm_header_string(const CANVAS_Canvas *canvas);
char *CANVAS_get_ppm_body_string(const CANVAS_Canvas *canvas);
void CANVAS_write_to_file(const CANVAS_Canvas *canvas, const char *filename);

CANVAS_Canvas *CANVAS_parse_file_by_name(char *file);
CANVAS_Canvas *CANVAS_parse_stream(FILE *file);
#endif // DATA_STRUCTURES_CANVAS_H
