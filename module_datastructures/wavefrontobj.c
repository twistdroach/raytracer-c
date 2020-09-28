#include "wavefrontobj.h"
#include "exceptions.h"

#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <triangle.h>
#include <logger.h>

#include "tuples.h"

typedef struct parse_state {
    unsigned int line_number;
    GROUP_Group* current_group;
    char* line;
} parse_state;

static void parse_face(WAVEFRONTOBJ_Obj* obj, parse_state* state) {
    assert(obj);
    assert(state);
    char* line = state->line;
    assert(line);

    char* err = NULL;
    char* saveptr = NULL;
    char* delim = " ";
    strtok_r(line, delim, &saveptr);

    char* first_vertex = strtok_r(NULL, delim, &saveptr);
    unsigned int v1ndx = strtoul(first_vertex, &err, 10);
    if (first_vertex == err) LOGGER_log(LOGGER_ERROR, "Error parsing vertex 1, line(%u), err(%s)\n", state->line_number, err);

    char* second_vertex = strtok_r(NULL, delim, &saveptr);
    unsigned int v2ndx = strtoul(second_vertex, &err, 10);
    if (second_vertex == err) LOGGER_log(LOGGER_ERROR, "Error parsing vertex 2, line(%u), err(%s)\n", state->line_number, err);

    CEXCEPTION_T e;
    TUPLES_Point *first, *previous, *current;
    Try {
                first = WAVEFRONTOBJ_get_vertex(obj, v1ndx);
                previous = WAVEFRONTOBJ_get_vertex(obj, v2ndx);
                char* current_vertex;
                while ((current_vertex = strtok_r(NULL, delim, &saveptr))) {
                    unsigned int current_vertex_ndx = strtoul(current_vertex, &err, 10);
                    if (current_vertex == err) {
                        LOGGER_log(LOGGER_ERROR, "Error parsing vertex, line(%u), err(%s)\n", state->line_number, err);
                    }
                    current = WAVEFRONTOBJ_get_vertex(obj, current_vertex_ndx);
                    TRIANGLE_Triangle* t = TRIANGLE_new_from_points(first, previous, current);
                    GROUP_add_child(state->current_group, t);
                    obj->triangle_count++;
                    previous = current;
                }

    }
    Catch(e) {
      LOGGER_log(LOGGER_ERROR, "Error retrieving vertex, line(%u)\n");
    }
    obj->face_count++;
}

static void parse_vertex(WAVEFRONTOBJ_Obj* obj, parse_state* state) {
    assert(obj);
    assert(state);
    char* line = state->line;
    assert(line);

    char* err = NULL;
    char* saveptr = NULL;
    char* delim = " ";
    strtok_r(line, delim, &saveptr);

    char* xstr = strtok_r(NULL, delim, &saveptr);
    double x = strtod(xstr, &err);
    if (xstr == err) LOGGER_log(LOGGER_ERROR, "Error parsing vertex x value, line(%u), err(%s)\n", state->line_number, err);

    char* ystr = strtok_r(NULL, delim, &saveptr);
    double y = strtod(ystr, &err);
    if (ystr == err) LOGGER_log(LOGGER_ERROR, "Error parsing vertex y value, line(%u), err(%s)\n", state->line_number, err);

    char* zstr = strtok_r(NULL, delim, &saveptr);
    double z = strtod(zstr, &err);
    if (zstr == err) LOGGER_log(LOGGER_ERROR, "Error parsing vertex z value, line(%u), err(%s)\n", state->line_number, err);

    ARRLIST_add(obj->vertices, TUPLES_new_point(x, y, z));
    obj->vertex_count++;
}

static void parse_group(WAVEFRONTOBJ_Obj* obj, parse_state* state) {
    assert(obj);
    assert(state);
    char* line = state->line;
    assert(line);

    char* saveptr = NULL;
    char* delim = " \n";
    strtok_r(line, delim, &saveptr);

    char* group_name = strtok_r(NULL, delim, &saveptr);
    LOGGER_log(LOGGER_DEBUG, "Found group (%s)\n", group_name);

    GROUP_Group* g = GROUP_new();
    state->current_group = g;
    GROUP_add_child(obj->default_group, g);
    obj->group_count++;
}

static void parse_stream(WAVEFRONTOBJ_Obj* obj, FILE* stream) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    parse_state state = (parse_state) {
        .current_group = obj->default_group,
        .line_number = 1,
        .line = NULL
    };
    while ((nread = getline(&line, &len, stream)) != -1) {
        if (nread > 0) {
            state.line = line;
            switch(line[0]) {
                case 'g':
                    parse_group(obj, &state);
                    break;
                case 'v':
                    parse_vertex(obj, &state);
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
    LOGGER_log(LOGGER_INFO, "Found groups(%u) faces(%u) triangles(%u) vertexes(%u)\n", obj->group_count, obj->face_count, obj->triangle_count, obj->vertex_count);
}

void WAVEFRONTOBJ_init(WAVEFRONTOBJ_Obj* obj, FILE* file) {
    assert(obj);
    assert(file);
    obj->ignored_lines = 0;
    obj->group_count = 0;
    obj->triangle_count = 0;
    obj->vertex_count = 0;
    obj->face_count = 0;
    obj->vertices = ARRLIST_new();
    obj->default_group = GROUP_new();
    parse_stream(obj, file);
}

WAVEFRONTOBJ_Obj* WAVEFRONTOBJ_parse_obj_stream(FILE* file) {
    assert(file);
    WAVEFRONTOBJ_Obj* obj = malloc(sizeof(WAVEFRONTOBJ_Obj));
    if (!obj) {
        Throw(E_MALLOC_FAILED);
    }
    WAVEFRONTOBJ_init(obj, file);
    return obj;
}

static void delete_vertex(void* vertex, void* context) {
    assert(vertex);
    UNUSED(context);

    TUPLES_Point* p = (TUPLES_Point*) vertex;
    TUPLES_delete(p);
}

WAVEFRONTOBJ_Obj* WAVEFRONTOBJ_parse_file_by_name(char* filename) {
    assert(filename);
    FILE* objfile = fopen(filename, "r");
    if (!objfile) {
        char* cwd = getcwd(NULL, 0);
        LOGGER_log(LOGGER_ERROR, "fopen failed to open (%s), cwd is (%s)\n", filename, cwd);
        free(cwd);
        Throw(E_FILE_FAILED);
    } else {
        LOGGER_log(LOGGER_INFO, "Opened file %s for parsing as obj\n", filename);
    }
    WAVEFRONTOBJ_Obj* obj = WAVEFRONTOBJ_parse_obj_stream(objfile);
    fclose(objfile);
    return obj;
}

void WAVEFRONTOBJ_destroy(WAVEFRONTOBJ_Obj* obj) {
    assert(obj);
    ARRLIST_iterator(obj->vertices, delete_vertex, NULL);
    ARRLIST_delete(obj->vertices);
    GROUP_delete(obj->default_group);
}

void WAVEFRONTOBJ_delete(WAVEFRONTOBJ_Obj* obj) {
    assert(obj);
    WAVEFRONTOBJ_destroy(obj);
    free(obj);
}
