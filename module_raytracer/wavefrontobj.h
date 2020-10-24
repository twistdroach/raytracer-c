#ifndef DATA_STRUCTURES_WAVEFRONTOBJ_H
#define DATA_STRUCTURES_WAVEFRONTOBJ_H

#include "arrlist.h"
#include "group.h"
#include <stdio.h>

typedef struct WAVEFRONTOBJ_Obj {
  GROUP_Group *default_group;
  unsigned int ignored_lines;
  unsigned int vertex_count;
  unsigned int normal_count;
  unsigned int triangle_count;
  unsigned int face_count;
  unsigned int group_count;
  ARRLIST_List *vertices;
  ARRLIST_List *normals;
} WAVEFRONTOBJ_Obj;

#define WAVEFRONTOBJ_get_vertex(obj, ndx) ((TUPLES_Point *)(ARRLIST_safe_get((obj)->vertices, (ndx)-1)))
#define WAVEFRONTOBJ_get_normal(obj, ndx) ((TUPLES_Vector *)(ARRLIST_safe_get((obj)->normals, (ndx)-1)))
#define WAVEFRONTOBJ_get_default_group(obj) ((obj)->default_group)

/**
 * Normalize obj to fit within a unit bounding box eg from (-1, -1, -1) to (1, 1, 1).
 * It does this by calculating and setting an appropriate transform matrix.
 * @param obj
 */
void WAVEFRONTOBJ_normalize(WAVEFRONTOBJ_Obj *obj);

/**
 * Throws E_FILE_FAILED if the file can't be opened
 * @param file
 * @return returns a ptr to a parsed obj file
 */
WAVEFRONTOBJ_Obj *WAVEFRONTOBJ_parse_file_by_name(char *file);
WAVEFRONTOBJ_Obj *WAVEFRONTOBJ_parse_obj_stream(FILE *file);
void WAVEFRONTOBJ_delete(WAVEFRONTOBJ_Obj *obj);

#endif // DATA_STRUCTURES_WAVEFRONTOBJ_H
