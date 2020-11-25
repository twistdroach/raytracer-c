#include "yamlloader.h"
#include <assert.h>
#include <ctype.h>
#include <logger.h>
#include <string.h>

// trim functions from https://stackoverflow.com/a/1431206
static char *ltrim(char *s)
{
  while(isspace(*s)) s++;
  return s;
}

static char *rtrim(char *s)
{
  char* back = s + strlen(s);
  while(isspace(*--back));
  *(back+1) = '\0';
  return s;
}

static char *trim(char *s)
{
  return rtrim(ltrim(s));
}

static void parse_map_entry(char *data, char **key, char **value) {
  assert(data);
  assert(key);
  assert(value);

  char *saveptr = NULL;
  char *colon_delim = ":";
  *key = strtok_r(data, colon_delim, &saveptr);
  if (!*key) {
    //TODO handle parse error
  } else {
    *key = trim(*key);
  }

  *value = strtok_r(NULL, colon_delim, &saveptr);
  if (!*value) {
    //TODO handle parse error
  } else {
    *value = trim(*value);
  }
}

static void parse_tuple_values(char* data, double *x, double *y, double *z) {
  assert(data);
  assert(x);
  assert(y);
  assert(z);

  if (data[0] != '[') {
    //handle parse error
    LOGGER_log(LOGGER_ERROR, "Parse error - expected [ at beginning of data in parse_point\n");
  }
  data++;

  size_t data_len = strlen(data);
  if (data[data_len - 1] != ']') {
    //handle parse error
    LOGGER_log(LOGGER_ERROR, "Parse error - expected ] at end of data in parse_point\n");
  }
  data[data_len - 1] = '\0';

  char *err = NULL;
  char *saveptr = NULL;
  char *comma_delim = ",";
  char *x_str = strtok_r(data, comma_delim, &saveptr);
  if (!x_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex x value\n");
  }
  *x = strtod(x_str, &err);

  char *y_str = strtok_r(NULL, comma_delim, &saveptr);
  if (!y_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex y value\n");
  }
  *y = strtod(y_str, &err);

  char *z_str = strtok_r(NULL, comma_delim, &saveptr);
  if (!z_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex z value\n");
  }
  *z = strtod(z_str, &err);
}

void YAMLLOADER_parse_color(char *data, TUPLES_Color *color) {
  assert(data);
  assert(color);
  double x, y, z;
  parse_tuple_values(data, &x, &y, &z);
  TUPLES_init_color(color, x, y, z);
}

void YAMLLOADER_parse_point(char *data, TUPLES_Point *point) {
  assert(data);
  assert(point);
  double x, y, z;
  parse_tuple_values(data, &x, &y, &z);
  TUPLES_init_point(point, x, y, z);
}

void YAMLLOADER_parse_vector(char *data, TUPLES_Vector *vector) {
  assert(data);
  assert(vector);
  double x, y, z;
  parse_tuple_values(data, &x, &y, &z);
  TUPLES_init_vector(vector, x, y, z);
}

void YAMLLOADER_parse_bool(char *data, bool *value) {
  assert(data);
  assert(value);
  *value = (strcasecmp("true", data) == 0);
}

void YAMLLOADER_parse_uint(char *data, unsigned int *value) {
  assert(data);
  assert(value);
  char *err = NULL;
  *value = strtoul(data, &err, 10);
}

void YAMLLOADER_parse_map_entries(char *data, void (*process_entry)(char *key, char *value, void *context), void *context) {
  assert(data);
  assert(process_entry);

  char *current_line = data;
  while(current_line) {
    char *next_line = strchr(current_line, '\n');
    if (next_line) {
      *next_line = '\0';
    }

    if (*current_line) {
      char *key = NULL;
      char *value = NULL;
      parse_map_entry(current_line, &key, &value);
      process_entry(key, value, context);
    }

    current_line = next_line ? (next_line + 1) : NULL;
  }
}



