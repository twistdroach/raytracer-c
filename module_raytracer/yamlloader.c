#include "yamlloader.h"
#include "configuration.h"
#include <assert.h>
#include <ctype.h>
#include <cube.h>
#include <logger.h>
#include <string.h>
#include <world.h>

/* ***********************************
 *
 * What follows is some terrible code
 * to parse yaml.
 *
 * I wanted to hack something together
 * and avoid pulling in any dependencies,
 * so here it is.  It is likely very unsafe
 * and will fail in all but the simplest
 * of cases.
 *
 * ***********************************/

// trim functions from https://stackoverflow.com/a/1431206
static char *ltrim(char *s)
{
  while(isspace(*s)) s++;
  return s;
}

static char *rtrim(char *s)
{
  char* back = s + strlen(s);
  while(isspace(*--back) && back > s);
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
    LOGGER_log(LOGGER_ERROR, "Failed parsing - colon delim not found (%s)\n", data);
    Throw(E_PARSE_FAILED);
  } else {
    *key = trim(*key);
  }

  *value = strtok_r(NULL, colon_delim, &saveptr);
  if (!*value) {
    LOGGER_log(LOGGER_ERROR, "Failed parsing - colon delim not found (%s)\n", data);
    Throw(E_PARSE_FAILED);
  } else {
    *value = trim(*value);
  }
}

static void parse_tuple_values(char* data, double *x, double *y, double *z) {
  assert(data);
  assert(x);
  assert(y);
  assert(z);

  //skip opening [
  char *skip_opening_bracket = strchr(data, '[');
  if (skip_opening_bracket) {
    data = skip_opening_bracket + 1;
  }

  size_t data_len = strlen(data);
  if (data[data_len - 1] != ']') {
    //handle parse error
    LOGGER_log(LOGGER_ERROR, "Parse error - expected ] at end of data in parse_point while parsing(%s)\n", data);
    Throw(E_PARSE_FAILED);
  }
  data[data_len - 1] = '\0';

  char *err = NULL;
  char *saveptr = NULL;
  char *comma_delim = ",";
  char *x_str = strtok_r(data, comma_delim, &saveptr);
  if (!x_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex x value while parsing(%s)\n", data);
    Throw(E_PARSE_FAILED);
  }
  *x = strtod(x_str, &err);

  char *y_str = strtok_r(NULL, comma_delim, &saveptr);
  if (!y_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex y value while parsing(%s)\n", data);
    Throw(E_PARSE_FAILED);
  }
  *y = strtod(y_str, &err);

  char *z_str = strtok_r(NULL, comma_delim, &saveptr);
  if (!z_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex z value while parsing(%s)\n", data);
    Throw(E_PARSE_FAILED);
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
  //TODO check err
  char *err = NULL;
  *value = strtoul(data, &err, 10);
}

void YAMLLOADER_parse_double(char *data, double *value) {
  assert(data);
  assert(value);
  //TODO check err
  char *err = NULL;
  *value = strtod(data, &err);
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

MATRIX_Matrix *YAMLLOADER_parse_transform_entry(char *data) {
  assert(data);

  //skip opening [
  char *skip_opening_bracket = strchr(data, '[');
  if (skip_opening_bracket) {
    data = skip_opening_bracket + 1;
  }

  char *saveptr = NULL;
  char *comma_delim = ",";
  char *type_str = strtok_r(data, comma_delim, &saveptr);
  if (!type_str) {
    LOGGER_log(LOGGER_ERROR, "Error parsing transform entry type (scale, translate, etc) value: %s\n", data);
    Throw(E_PARSE_FAILED);
  }
  type_str = trim(type_str);
  MATRIX_Matrix *mat = NULL;
  double x, y, z;
  if (strcmp("scale", type_str) == 0) {
    parse_tuple_values(saveptr, &x, &y, &z);
    mat = MATRIX_new_scaling(x, y, z);
  } else if (strcmp("translate", type_str) == 0 ) {
    parse_tuple_values(saveptr, &x, &y, &z);
    mat = MATRIX_new_translation(x, y, z);
  } else if (strcmp("rotate-x", type_str) == 0){
    YAMLLOADER_parse_double(saveptr, &x);
    mat = MATRIX_new_rotation_x(x);
  } else if (strcmp("rotate-y", type_str) == 0){
    YAMLLOADER_parse_double(saveptr, &y);
    mat = MATRIX_new_rotation_y(y);
  } else if (strcmp("rotate-z", type_str) == 0){
    YAMLLOADER_parse_double(saveptr, &z);
    mat = MATRIX_new_rotation_z(z);
  } else {
    LOGGER_log(LOGGER_ERROR, "Error parsing transform entry type (scale, translate, etc) unknown value: %s\n", type_str);
    Throw(E_PARSE_FAILED);
  }
  return mat;
}

MATRIX_Matrix *YAMLLOADER_parse_transform(char *data) {
  assert(data);
  MATRIX_Matrix *mat = MATRIX_new_identity(4);
  char *current_line = data;
  while(current_line) {
    char *next_line = strchr(current_line, '\n');
    if (next_line) {
      *next_line = '\0';
    }

    if (*current_line) {
      MATRIX_Matrix *line_matrix = YAMLLOADER_parse_transform_entry(current_line);
      MATRIX_Matrix *tmp = mat;
      mat = MATRIX_multiply(line_matrix, mat);
      MATRIX_delete_all(tmp, line_matrix);
    }

    current_line = next_line ? (next_line + 1) : NULL;
  }
  return mat;
}

void YAMLLOADER_parse_shape_info(SHAPE_Shape *shape, char *buffer) {
  assert(shape);
  assert(buffer);
  if (!*trim(buffer)) {
    //do nothing if we are handed nothing but whitespace
    return;
  }
  char *next_line = NULL;
  if (strncmp("transform:", ltrim(buffer), 10) == 0 && (next_line = strchr(buffer, ':'))) {
      MATRIX_Matrix* transform = YAMLLOADER_parse_transform(next_line + 1);
      SHAPE_set_transform(shape, transform);
      MATRIX_delete(transform);
  } else {
    LOGGER_log(LOGGER_ERROR, "Error parsing shape information (%s)\n", buffer);
    Throw(E_PARSE_FAILED);
    return;
  }
}

static void create_object(char *type, char *buffer, CONFIGURATION_Config *config) {
  assert(type);
  assert(buffer);
  assert(config);
  if (strcmp("light", type) == 0) {
    LIGHTS_Light *light = WORLD_get_light(config->world);
    if (light) {
      LIGHTS_delete(light);
    }
    WORLD_set_light(config->world, LIGHTS_parse_light(buffer));
  } else if (strcmp("camera", type) == 0) {
    config->camera = CAMERA_parse_camera(buffer);
  } else if (strcmp("cube", type) == 0) {
    CUBE_Cube *cube = CUBE_new();
    YAMLLOADER_parse_shape_info(cube, buffer);
    WORLD_add_object(config->world, cube);
  } else {
    LOGGER_log(LOGGER_ERROR, "Unknown type found when parsing: %s\n", type);
    free(buffer);
    Throw(E_PARSE_FAILED);
    return;
  }
  free(buffer);
}

static char *handle_array_item(char *first_entry, char *data, CONFIGURATION_Config *config) {
  assert(first_entry);
  assert(data);
  assert(config);

  // make sure we are handling an add (only thing supported now)
  char *first_function, *first_type;
  parse_map_entry(first_entry, &first_function, &first_type);
  if (!first_function || !first_type || (strcmp("- add", first_function) != 0)) {
    if (first_type) {
      LOGGER_log(LOGGER_ERROR, "Unknown yaml function: %s\n", first_function);
    } else {
      LOGGER_log(LOGGER_ERROR, "Failed parsing yaml: %s\n", first_entry);
    }
    Throw(E_PARSE_FAILED);
    return NULL;
  }

  char *buffer = NULL;
  Sasprintf(buffer, "\n");
  char *current_line = data;
  while(current_line) {
    char *next_line = strchr(current_line, '\n');
    if (*current_line) {
      char *trimmed_current_line = ltrim(current_line);
      if (strncmp("- add", trimmed_current_line, 5) == 0) {
        //reached next object, parse the buffer and move on
        create_object(first_type, buffer, config);
        return current_line;
      } else {
        if (next_line) {
          *next_line = '\0';
        }
      }
      if (*trimmed_current_line == '#') {
        //found a comment
      } else {
        //add line to buffer to be interpreted by specific parser (ie for light or cube or ...)
        Sasprintf(buffer, "%s\n%s", buffer, current_line);
      }
    }
    current_line = next_line ? (next_line + 1) : NULL;
  }
  //hit the end of data
  create_object(first_type, buffer, config);
  return current_line;
}

CONFIGURATION_Config *YAMLLOADER_parse(char *data) {
  assert(data);

  CONFIGURATION_Config *config = malloc(sizeof(CONFIGURATION_Config));
  if (!config) {
    Throw(E_MALLOC_FAILED);
  }
  //TODO this will leak on a throw of E_PARSE_FAILED in deeper code
  config->world = WORLD_new(NULL);

  char *current_line = data;
  while(current_line) {
    char *next_line = strchr(current_line, '\n');
    if (next_line) {
      *next_line = '\0';
    }

    if (*current_line) {
      char *first_char = ltrim(current_line);
      if (*first_char == '#') {
        //NOP - it's a comment
      } else if (*first_char == '-') {
        if (next_line) {
          current_line = handle_array_item(current_line, next_line + 1, config);
          continue;
        } else {
          LOGGER_log(LOGGER_ERROR, "Error parsing (%s), expected multiple line object\n", current_line);
          Throw(E_PARSE_FAILED);
        }
      }
    }

    current_line = next_line ? (next_line + 1) : NULL;
  }
  return config;
}
