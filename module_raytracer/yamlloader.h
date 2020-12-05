#ifndef SIMPLE_RAYTRACER_YAMLLOADER_H
#define SIMPLE_RAYTRACER_YAMLLOADER_H

#include "camera.h"
#include "configuration.h"
#include <matrix.h>
#include <stdbool.h>
#include <tuples.h>
#include <world.h>


void YAMLLOADER_parse_double(char *data, double *value);
void YAMLLOADER_parse_uint(char *data, unsigned int *value);
void YAMLLOADER_parse_bool(char *data, bool *value);

// Tuples
void YAMLLOADER_parse_vector(char *data, TUPLES_Vector *vector);
void YAMLLOADER_parse_point(char *data, TUPLES_Point *point);
void YAMLLOADER_parse_color(char *data, TUPLES_Color *color);

// Matrices
MATRIX_Matrix *YAMLLOADER_parse_transform(char *data);
MATRIX_Matrix *YAMLLOADER_parse_transform_entry(char *data);

void YAMLLOADER_parse_map_entries(char *data, void (*process_entry)(char *key, char *value, void *context), void *context);
CONFIGURATION_Config *YAMLLOADER_parse(char *data);

#endif // SIMPLE_RAYTRACER_YAMLLOADER_H
