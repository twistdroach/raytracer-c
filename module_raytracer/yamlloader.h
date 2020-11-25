#ifndef SIMPLE_RAYTRACER_YAMLLOADER_H
#define SIMPLE_RAYTRACER_YAMLLOADER_H

#include <stdbool.h>
#include <tuples.h>

void YAMLLOADER_parse_uint(char *data, unsigned int *value);
void YAMLLOADER_parse_bool(char *data, bool *value);
void YAMLLOADER_parse_vector(char *data, TUPLES_Vector *vector);
void YAMLLOADER_parse_point(char *data, TUPLES_Point *point);
void YAMLLOADER_parse_color(char *data, TUPLES_Color *color);

void YAMLLOADER_parse_map_entries(char *data, void (*process_entry)(char *key, char *value, void *context), void *context);

#endif // SIMPLE_RAYTRACER_YAMLLOADER_H
