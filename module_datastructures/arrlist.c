#include <stdlib.h>
#include <assert.h>

#include "exceptions.h"

#include "arrlist.h"

typedef struct ARRLIST_List {
    void** items;
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

void ARRLIST_add(ARRLIST_List* list, void* object) {
    assert(list);
    assert(object);
    void** tmpptr = reallocarray(list->items, sizeof(void*), list->count + 1);
    if (!tmpptr) {
        Throw(E_MALLOC_FAILED);
    } else {
        list->items = tmpptr;
    }
    list->items[list->count] = object;
    list->count++;
}

unsigned int ARRLIST_item_count(const ARRLIST_List* list) {
    assert(list);
    return list->count;
}

void* ARRLIST_safe_get(const ARRLIST_List* list, unsigned int item) {
    assert(list);
    if (item >= list->count) {
        Throw(E_INDEX_OUT_OF_BOUNDS);
    }
    return list->items[item];
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

void ARRLIST_iterator(ARRLIST_List* list, void (*apply_each_ptr)(void* ptr, void* context), void* context) {
    assert(list);
    assert(apply_each_ptr);
    for (unsigned int ndx = 0; ndx < list->count; ndx++) {
        apply_each_ptr(list->items[ndx], context);
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

void* ARRLIST_last(const ARRLIST_List* list) {
    assert(list);
    assert(list->count > 0);
    if (list->count < 1) {
        return NULL;
    }
    return list->items[list->count - 1];
}

void ARRLIST_delete(ARRLIST_List* list) {
    assert(list);
    free(list->items);
    free(list);
}
