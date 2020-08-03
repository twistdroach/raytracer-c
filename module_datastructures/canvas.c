//
// Created by zrowitsch on 8/3/20.
//

#include <stdlib.h>
#include <CException.h>
#include <assert.h>
#include "proj_exceptions.h"
#include "canvas.h"

CANVAS_Canvas* CANVAS_new(uint width, uint height) {
    CANVAS_Canvas* canvas = (CANVAS_Canvas*) malloc(sizeof(CANVAS_Canvas));
    if (!canvas)
       Throw(E_MALLOC_FAILED);

    CANVAS_init(canvas, width, height);
    return canvas;
}

void CANVAS_init(CANVAS_Canvas* canvas, uint width, uint height) {
    canvas->width = width;
    canvas->height = height;
    canvas->data = (TUPLES_Color*) malloc(width * height * sizeof(TUPLES_Color));
    if (!canvas->data)
        Throw(E_MALLOC_FAILED);

    for (uint i=0; i < width * height; i++) {
        TUPLES_init_color(&canvas->data[i], 0, 0, 0);
    }
}

uint calculate_index(CANVAS_Canvas* canvas, uint x, uint y) {
    assert(x < canvas->width);
    assert(y < canvas->height);
    return y * canvas->height + canvas->width;
}

TUPLES_Color* CANVAS_read_pixel(CANVAS_Canvas* canvas, uint x, uint y) {
    assert(canvas);
    uint ndx = calculate_index(canvas, x, y);
    return &canvas->data[ndx];
}

void CANVAS_write_pixel(CANVAS_Canvas* canvas, uint x, uint y, const TUPLES_Color* color) {
    assert(canvas);
    assert(color);
    TUPLES_Color* dest = CANVAS_read_pixel(canvas, x, y);
    TUPLES_copy(dest, color);
}

void CANVAS_destroy(CANVAS_Canvas* canvas) {
    assert(canvas);
    assert(canvas->data);
    free(canvas->data);
}

void CANVAS_delete(CANVAS_Canvas* canvas) {
    assert(canvas);
    CANVAS_destroy(canvas);
    free(canvas);
}
