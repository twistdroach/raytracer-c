#ifndef SIMPLE_RAYTRACER_LINKEDLIST_H
#define SIMPLE_RAYTRACER_LINKEDLIST_H

#include <stdbool.h>

typedef struct LINKEDLIST_List LINKEDLIST_List;

LINKEDLIST_List *LINKEDLIST_new();
void LINKEDLIST_add(LINKEDLIST_List *list, void *object);

/**
 * gets a point to an item in the list.  throws a E_INDEX_OUT_OF_BOUNDS
 * when an item that doesn't exist is accessed.
 * @param list
 * @param item
 * @return
 */
void *LINKEDLIST_safe_get(const LINKEDLIST_List *list, unsigned int item);

unsigned int LINKEDLIST_item_count(const LINKEDLIST_List *list);

/**
 * Removes an item from the list and moves the remaining ptrs up.
 * Doesn't bother to resize the array.
 * @param list
 * @param object
 */
void LINKEDLIST_remove(LINKEDLIST_List *list, const void *object);

bool LINKEDLIST_is_empty(const LINKEDLIST_List *list);

/**
 * Execute the function on each node.  If the function returns false, then execution will stop early.
 * @param list
 * @param apply_each_ptr
 * @param context
 */
void LINKEDLIST_iterator(LINKEDLIST_List *list, bool (*apply_each_ptr)(void *ptr, void *context), void *context);

/**
 * Execute the function on each node.  If the function returns false, then remove the node from the list.
 * @param list
 * @param apply_each_ptr
 * @param context
 */
void LINKEDLIST_filter(LINKEDLIST_List *list, bool (*apply_each_ptr)(void *ptr, void *context), void *context);

/**
 * Performs linear search of the list.  Comparing only the ptr values!
 * @param list
 * @param object
 * @return
 */
bool LINKEDLIST_contains(const LINKEDLIST_List *list, const void *object);

/**
 * Returns the last item in the list
 * @param LINKEDLIST_List
 * @return
 */
void *LINKEDLIST_last(const LINKEDLIST_List *list);

/**
 * Frees the list and backing array, but doesn't free anything pointed to
 * the ptrs that have been added to the list.
 * @param list
 */
void LINKEDLIST_delete(LINKEDLIST_List *list);

#endif // SIMPLE_RAYTRACER_LINKEDLIST_H
