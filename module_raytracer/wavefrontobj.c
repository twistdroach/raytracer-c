#include "wavefrontobj.h"
#include "exceptions.h"

#include <assert.h>
#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <triangle.h>
#include <unistd.h>

#include "tuples.h"

typedef struct parse_state {
  unsigned int line_number;
  GROUP_Group *current_group;
  char *line;
} parse_state;

bool check_for_slash_char(char *str) { return (strchr(str, '/') != NULL); }

typedef struct entry {
  unsigned int vertex;
  unsigned int normal;
  bool vertex_found;
  bool normal_found;
} entry;

static entry parse_face_entry(char *str, parse_state *state) {
  assert(str);
  entry e;
  e.normal_found = false;
  e.vertex_found = false;

  if (check_for_slash_char(str)) {
    // should always be three values.  we don't need the second since we don't implement textures yet
    char *first = strsep(&str, "/");
    strsep(&str, "/");
    char *third = strsep(&str, "/");

    char *err = NULL;
    e.vertex = strtoul(first, &err, 10);
    if (first == err) {
      LOGGER_log(LOGGER_ERROR, "Error parsing face vertex, line(%u), err(%s)\n", state->line_number, err);
    } else {
      e.vertex_found = true;
    };

    e.normal = strtoul(third, &err, 10);
    if (third == err) {
      LOGGER_log(LOGGER_ERROR, "Error parsing face normal, line(%u), err(%s)\n", state->line_number, err);
    } else {
      e.normal_found = true;
    };
  } else {
    char *err = NULL;
    e.vertex = strtoul(str, &err, 10);
    if (str == err) {
      LOGGER_log(LOGGER_ERROR, "Error parsing face vertex (no slashes), line(%u), err(%s)\n", state->line_number, err);
    } else {
      e.vertex_found = true;
    };
  }
  return e;
}

static void parse_face(WAVEFRONTOBJ_Obj *obj, parse_state *state) {
  assert(obj);
  assert(state);
  char *line = state->line;
  assert(line);

  char *saveptr = NULL;
  char *delim = " ";
  strtok_r(line, delim, &saveptr);

  char *first_vertex = strtok_r(NULL, delim, &saveptr);
  entry first_e = parse_face_entry(first_vertex, state);
  if (!first_e.vertex_found) {
    LOGGER_log(LOGGER_ERROR, "Error parsing first vertex on line (%u)\n", state->line_number);
    return;
  }
  unsigned int v1ndx = first_e.vertex;
  unsigned int n1ndx = 0;
  if (first_e.normal_found) {
    n1ndx = first_e.normal;
  }

  char *second_vertex = strtok_r(NULL, delim, &saveptr);
  entry second_e = parse_face_entry(second_vertex, state);
  if (!second_e.vertex_found) {
    LOGGER_log(LOGGER_ERROR, "Error parsing second vertex on line (%u)\n", state->line_number);
    return;
  }
  unsigned int v2ndx = second_e.vertex;
  unsigned int n2ndx = 0;
  if (second_e.normal_found) {
    n2ndx = second_e.normal;
  }

  CEXCEPTION_T e = E_NO_ERROR;
  TUPLES_Point *first, *previous, *current;
  TUPLES_Vector *first_n = NULL;
  TUPLES_Vector *previous_n = NULL;
  TUPLES_Vector *current_n = NULL;
  Try {
    first = WAVEFRONTOBJ_get_vertex(obj, v1ndx);
    previous = WAVEFRONTOBJ_get_vertex(obj, v2ndx);
    if (n1ndx) {
      first_n = WAVEFRONTOBJ_get_normal(obj, n1ndx);
    }
    if (n2ndx) {
      previous_n = WAVEFRONTOBJ_get_normal(obj, n2ndx);
    }

    char *current_vertex;
    while ((current_vertex = strtok_r(NULL, delim, &saveptr))) {
      entry current_vertex_e = parse_face_entry(current_vertex, state);
      if (!current_vertex_e.vertex_found) {
        LOGGER_log(LOGGER_ERROR, "Error parsing vertex on line (%u)\n", state->line_number);
        return;
      }

      current = WAVEFRONTOBJ_get_vertex(obj, current_vertex_e.vertex);
      if (current_vertex_e.normal_found) {
        current_n = WAVEFRONTOBJ_get_normal(obj, current_vertex_e.normal);
        TRIANGLE_SmoothTriangle *t = TRIANGLE_new_smooth_from_points(first, previous, current, first_n, previous_n, current_n);
        GROUP_add_child(state->current_group, t);
        previous_n = current_n;
      } else {
        TRIANGLE_Triangle *t = TRIANGLE_new_from_points(first, previous, current);
        GROUP_add_child(state->current_group, t);
      }

      obj->triangle_count++;
      previous = current;
    }
  }
  Catch(e) {
    if (e == E_INDEX_OUT_OF_BOUNDS) {
      LOGGER_log(LOGGER_ERROR, "Error retrieving vertex, line(%u)\n", state->line_number);
    } else {
      LOGGER_log(LOGGER_ERROR, "Unexpected error %s\n", EXCEPTIONS_strings[e]);
    }
  }
  obj->face_count++;
}

static void parse_normal(WAVEFRONTOBJ_Obj *obj, parse_state *state) {
  assert(obj);
  assert(state);
  char *line = state->line;
  assert(line);

  char *err = NULL;
  char *saveptr = NULL;
  char *delim = " ";
  strtok_r(line, delim, &saveptr);

  char *xstr = strtok_r(NULL, delim, &saveptr);
  double x = strtod(xstr, &err);
  if (xstr == err)
    LOGGER_log(LOGGER_ERROR, "Error parsing normal x value, line(%u), err(%s)\n", state->line_number, err);

  char *ystr = strtok_r(NULL, delim, &saveptr);
  double y = strtod(ystr, &err);
  if (ystr == err)
    LOGGER_log(LOGGER_ERROR, "Error parsing normal y value, line(%u), err(%s)\n", state->line_number, err);

  char *zstr = strtok_r(NULL, delim, &saveptr);
  double z = strtod(zstr, &err);
  if (zstr == err)
    LOGGER_log(LOGGER_ERROR, "Error parsing normal z value, line(%u), err(%s)\n", state->line_number, err);

  ARRLIST_add(obj->normals, TUPLES_new_vector(x, y, z));
  obj->normal_count++;
}

static void parse_vertex(WAVEFRONTOBJ_Obj *obj, parse_state *state) {
  assert(obj);
  assert(state);
  char *line = state->line;
  assert(line);

  char *err = NULL;
  char *saveptr = NULL;
  char *delim = " ";
  strtok_r(line, delim, &saveptr);

  char *xstr = strtok_r(NULL, delim, &saveptr);
  if (!xstr) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex x value - no token found, line(%u), err(%s)\n", state->line_number, err);
    return;
  }
  double x = strtod(xstr, &err);
  if (xstr == err) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex x value, line(%u), err(%s)\n", state->line_number, err);
    return;
  }

  char *ystr = strtok_r(NULL, delim, &saveptr);
  if (!ystr) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex y value - no token found, line(%u), err(%s)\n", state->line_number, err);
    return;
  }
  double y = strtod(ystr, &err);
  if (ystr == err) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex y value, line(%u), err(%s)\n", state->line_number, err);
    return;
  }

  char *zstr = strtok_r(NULL, delim, &saveptr);
  if (!zstr) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex z value - no token found, line(%u), err(%s)\n", state->line_number, err);
    return;
  }
  double z = strtod(zstr, &err);
  if (zstr == err) {
    LOGGER_log(LOGGER_ERROR, "Error parsing vertex z value, line(%u), err(%s)\n", state->line_number, err);
    return;
  }

  ARRLIST_add(obj->vertices, TUPLES_new_point(x, y, z));
  obj->vertex_count++;
}

static void parse_group(WAVEFRONTOBJ_Obj *obj, parse_state *state) {
  assert(obj);
  assert(state);
  char *line = state->line;
  assert(line);

  char *saveptr = NULL;
  char *delim = " \n";
  strtok_r(line, delim, &saveptr);

  char *group_name = strtok_r(NULL, delim, &saveptr);
  LOGGER_log(LOGGER_DEBUG, "Found group (%s)\n", group_name);

  GROUP_Group *g = GROUP_new();
  state->current_group = g;
  GROUP_add_child(obj->default_group, g);
  obj->group_count++;
}

static void parse_stream(WAVEFRONTOBJ_Obj *obj, FILE *stream) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  parse_state state = (parse_state){.current_group = obj->default_group, .line_number = 1, .line = NULL};
  while ((nread = getline(&line, &len, stream)) != -1) {
    if (nread > 0) {
      state.line = line;
      switch (line[0]) {
      case 'g':
        parse_group(obj, &state);
        break;
      case 'v':
        if (line[1] == 'n') {
          parse_normal(obj, &state);
        } else if (line[1] == ' ') {
          parse_vertex(obj, &state);
        }
        break;
      case 'f':
        parse_face(obj, &state);
        break;
      default:
        obj->ignored_lines++;
      }
      state.line_number++;
    }
  }
  free(line);
  LOGGER_log(LOGGER_INFO, "Found groups(%u) faces(%u) triangles(%u) vertexes(%u) normals(%u)\n", obj->group_count, obj->face_count, obj->triangle_count,
             obj->vertex_count, obj->normal_count);
}

void WAVEFRONTOBJ_init(WAVEFRONTOBJ_Obj *obj, FILE *file) {
  assert(obj);
  assert(file);
  obj->ignored_lines = 0;
  obj->group_count = 0;
  obj->triangle_count = 0;
  obj->vertex_count = 0;
  obj->face_count = 0;
  obj->normal_count = 0;
  obj->vertices = ARRLIST_new();
  obj->normals = ARRLIST_new();
  obj->default_group = GROUP_new();
  parse_stream(obj, file);
}

WAVEFRONTOBJ_Obj *WAVEFRONTOBJ_parse_obj_stream(FILE *file) {
  assert(file);
  WAVEFRONTOBJ_Obj *obj = malloc(sizeof(WAVEFRONTOBJ_Obj));
  if (!obj) {
    Throw(E_MALLOC_FAILED);
  }
  WAVEFRONTOBJ_init(obj, file);
  return obj;
}

static void delete_tuple(void *vertex, void *context) {
  assert(vertex);
  UNUSED(context);

  TUPLES_Point *p = (TUPLES_Point *)vertex;
  TUPLES_delete(p);
}

WAVEFRONTOBJ_Obj *WAVEFRONTOBJ_parse_file_by_name(char *filename) {
  assert(filename);
  FILE *objfile = fopen(filename, "r");
  if (!objfile) {
    char *cwd = getcwd(NULL, 0);
    LOGGER_log(LOGGER_ERROR, "fopen failed to open (%s), cwd is (%s)\n", filename, cwd);
    free(cwd);
    Throw(E_FILE_FAILED);
  } else {
    LOGGER_log(LOGGER_INFO, "Opened file %s for parsing as obj\n", filename);
  }
  WAVEFRONTOBJ_Obj *obj = WAVEFRONTOBJ_parse_obj_stream(objfile);
  fclose(objfile);
  return obj;
}

void WAVEFRONTOBJ_destroy(WAVEFRONTOBJ_Obj *obj) {
  assert(obj);
  ARRLIST_iterator(obj->vertices, delete_tuple, NULL);
  ARRLIST_delete(obj->vertices);
  ARRLIST_iterator(obj->normals, delete_tuple, NULL);
  ARRLIST_delete(obj->normals);
  GROUP_delete(obj->default_group);
}

void WAVEFRONTOBJ_delete(WAVEFRONTOBJ_Obj *obj) {
  assert(obj);
  WAVEFRONTOBJ_destroy(obj);
  free(obj);
}

void WAVEFRONTOBJ_normalize(WAVEFRONTOBJ_Obj *obj) {
  assert(obj);
  BOUND_Box box;
  SHAPE_bounds_of(obj->default_group, &box);
  double dx = box.max.x - box.min.x;
  double dy = box.max.y - box.min.y;
  double dz = box.max.z - box.min.z;

  TUPLES_Vector delta;
  TUPLES_Point origin;
  TUPLES_init_point(&origin, 0, 0, 0);
  TUPLES_Point middle_of_object;
  TUPLES_init_point(&middle_of_object, box.min.x + (dx / 2), box.min.y, box.min.z + (dz / 2));
  TUPLES_subtract(&delta, &origin, &middle_of_object);
  MATRIX_Matrix *translation = MATRIX_new_translation(delta.x, delta.y, delta.z);

  double scalefactor = 1 / (UTILITIES_max(dx, dy, dz) / 2.0);
  MATRIX_Matrix *scale_matrix = MATRIX_new_scaling(scalefactor, scalefactor, scalefactor);
  MATRIX_Matrix *new_transform = MATRIX_multiply_many(translation, scale_matrix);
  GROUP_set_transform(obj->default_group, new_transform);
  MATRIX_delete_all(scale_matrix, new_transform, translation);
}
