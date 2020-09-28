#ifndef DATA_STRUCTURES_WAVEFRONTOBJ_H
#define DATA_STRUCTURES_WAVEFRONTOBJ_H

#include <stdio.h>
#include "arrlist.h"
#include "group.h"

typedef struct WAVEFRONTOBJ_Obj {
    unsigned int ignored_lines;
    unsigned int vertex_count;
    unsigned int triangle_count;
    unsigned int face_count;
    unsigned int group_count;
    ARRLIST_List* vertices;
    GROUP_Group* default_group;
} WAVEFRONTOBJ_Obj;

#define WAVEFRONTOBJ_get_vertex(obj, ndx) ((TUPLES_Point*)(ARRLIST_safe_get((obj)->vertices, (ndx) - 1)))
#define WAVEFRONTOBJ_get_default_group(obj) ((obj)->default_group)

/**
 * Throws E_FILE_FAILED if the file can't be opened
 * @param file
 * @return returns a ptr to a parsed obj file
 */
WAVEFRONTOBJ_Obj* WAVEFRONTOBJ_parse_file_by_name(char* file);
WAVEFRONTOBJ_Obj* WAVEFRONTOBJ_parse_obj_stream(FILE* file);
void WAVEFRONTOBJ_delete(WAVEFRONTOBJ_Obj* obj);

#endif //DATA_STRUCTURES_WAVEFRONTOBJ_H
