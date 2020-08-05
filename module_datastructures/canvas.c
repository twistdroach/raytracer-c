//
// Created by zrowitsch on 8/3/20.
//
#define _GNU_SOURCE
#include <stdlib.h>
#include <CException.h>
#include <assert.h>
#include <stdio.h>
#include <utilities.h>
#include <math.h>
#include "exceptions.h"
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

uint calculate_index(const CANVAS_Canvas* canvas, uint x, uint y) {
    assert(x < canvas->width);
    assert(y < canvas->height);
    return y * canvas->width + x;
}

TUPLES_Color* CANVAS_read_pixel(const CANVAS_Canvas* canvas, uint x, uint y) {
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

char* CANVAS_get_ppm_header_string(const CANVAS_Canvas* canvas) {
    assert(canvas);
    char * retval;
    if (asprintf(&retval, "P3\n%u %u\n255\n", canvas->width, canvas->height) == -1)
        Throw(E_MALLOC_FAILED);
    return retval;
}

u_int8_t clampnscale_double(double d) {
    if (d > 1) d = 1;
    if (d < 0) d = 0;
    return round(d * 255.0);
}

char* CANVAS_get_ppm_body_string(const CANVAS_Canvas* canvas) {
    assert(canvas);
    char* buffer = NULL;
    for (uint j=0; j<canvas->height; j++) {
        for (uint k=0; k<canvas->width; k++) {
            TUPLES_Color* color = CANVAS_read_pixel(canvas, k, j);
            u_int8_t red = clampnscale_double(color->red);
            u_int8_t green = clampnscale_double(color->green);
            u_int8_t blue = clampnscale_double(color->blue);
            if (buffer) {
                Sasprintf(buffer, "%s%u %u %u ", buffer, red, green, blue);
            } else {
                Sasprintf(buffer, "%u %u %u ", red, green, blue);
            }
        }
        Sasprintf(buffer, "%s\n", buffer);
    }
    Sasprintf(buffer, "%s\n", buffer);
    return buffer;
}

void CANVAS_write_body_to_file(const CANVAS_Canvas* canvas, FILE* file) {
    assert(canvas);
    assert(file);
    for (uint j = 0; j < canvas->height; j++) {
        for (uint k = 0; k < canvas->width; k++) {
            TUPLES_Color *color = CANVAS_read_pixel(canvas, k, j);
            u_int8_t red = clampnscale_double(color->red);
            u_int8_t green = clampnscale_double(color->green);
            u_int8_t blue = clampnscale_double(color->blue);
            fprintf(file, "%u %u %u ", red, green, blue);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}

void CANVAS_write_to_file(const CANVAS_Canvas* canvas, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file)
        Throw(E_FILE_FAILED);
    char* header = CANVAS_get_ppm_header_string(canvas);
    fprintf(file, "%s", header);
    CANVAS_write_body_to_file(canvas, file);
    fclose(file);
    free(header);
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
