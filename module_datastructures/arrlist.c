#include <stdlib.h>
#include <assert.h>

#include "exceptions.h"

#include "arrlist.h"

typedef struct ARRLIST_List {
    const void** items;
    unsigned int count;
} ARRLIST_List;

ARRLIST_List* ARRLIST_new() {
    ARRLIST_List* list = malloc(sizeof(ARRLIST_List));
    if (!list) {
        Throw(E_MALLOC_FAILED);
    }
    list->items = NULL;
    list->count = 0;
    return list;
}

void ARRLIST_add(ARRLIST_List* list, const void* object) {
    assert(list);
    assert(object);
    const void** tmpptr = reallocarray(list->items, sizeof(void*), list->count + 1);
    if (!tmpptr) {
        Throw(E_MALLOC_FAILED);
    } else {
        list->items = tmpptr;
    }
    list->items[list->count] = object;
    list->count++;
}

void ARRLIST_remove(ARRLIST_List* list, const void* object) {
    assert(list);
    assert(object);
    bool found = false;
    for (unsigned int ndx = 0; ndx < list->count; ndx++) {
        if (list->items[ndx] == object) {
            found = true;
        }
        if (found) {
            if (ndx + 1 != list->count) {
                list->items[ndx] = list->items[ndx + 1];
            } else {
                list->items[ndx] = NULL;
            }
        }
    }
    if (found) {
        list->count--;
    }
}

bool ARRLIST_is_empty(const ARRLIST_List* list) {
    assert(list);
    return (list->count == 0);
}

bool ARRLIST_contains(const ARRLIST_List* list, const void* object) {
    assert(list);
    assert(object);
    for (unsigned int ndx = 0; ndx < list->count; ndx++) {
        if (list->items[ndx] == object) {
            return true;
        }
    }
    return false;
}

const void* ARRLIST_last(const ARRLIST_List* list) {
    assert(list);
    return list->items[list->count - 1];
}

void ARRLIST_delete(ARRLIST_List* list) {
    assert(list);
    free(list->items);
    free(list);
}
