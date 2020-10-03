#ifndef DATA_STRUCTURES_ARRLIST_H
#define DATA_STRUCTURES_ARRLIST_H

#include <stdbool.h>

typedef struct ARRLIST_List ARRLIST_List;

ARRLIST_List* ARRLIST_new();
void ARRLIST_add(ARRLIST_List* list, void* object);

/**
 * gets a point to an item in the list.  throws a E_INDEX_OUT_OF_BOUNDS
 * when an item that doesn't exist is accessed.
 * @param list
 * @param item
 * @return
 */
void* ARRLIST_safe_get(ARRLIST_List* list, unsigned int item);

/**
 * Removes an item from the list and moves the remaining ptrs up.
 * Doesn't bother to resize the array.
 * @param list
 * @param object
 */
void ARRLIST_remove(ARRLIST_List* list, const void* object);

bool ARRLIST_is_empty(const ARRLIST_List* list);

void ARRLIST_iterator(ARRLIST_List* list, void (*apply_each_ptr)(void* ptr, void* context), void* context);

/**
 * Performs linear search of the list.  Comparing only the ptr values!
 * @param list
 * @param object
 * @return
 */
bool ARRLIST_contains(const ARRLIST_List* list, const void* object);

/**
 * Returns the last item in the list
 * @param ARRLIST_List
 * @return
 */
void* ARRLIST_last(const ARRLIST_List* list);

/**
 * Frees the list and backing array, but doesn't free anything pointed to
 * the ptrs that have been added to the list.
 * @param list
 */
void ARRLIST_delete(ARRLIST_List* list);

#endif //DATA_STRUCTURES_ARRLIST_H
