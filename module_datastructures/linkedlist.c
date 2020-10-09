#include "linkedlist.h"
#include <stdlib.h>
#include <exceptions.h>
#include <assert.h>

typedef struct LINKEDLIST_Node {
    void* data;
    struct LINKEDLIST_Node* next;
} LINKEDLIST_Node;

typedef struct LINKEDLIST_List {
    LINKEDLIST_Node* head;
    LINKEDLIST_Node* tail;
    unsigned int count;
} LINKEDLIST_List;

LINKEDLIST_List* LINKEDLIST_new() {
    LINKEDLIST_List* list = malloc(sizeof(LINKEDLIST_List));
    if (!list) {
        Throw(E_MALLOC_FAILED);
    }
    list->count = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}
void LINKEDLIST_add(LINKEDLIST_List* list, void* object) {
    assert(list);
    assert(object);
    LINKEDLIST_Node* new_node = malloc(sizeof(LINKEDLIST_Node));
    if (!new_node) {
        Throw(E_MALLOC_FAILED);
    }
    new_node->data = object;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->count++;
}

void* LINKEDLIST_safe_get(const LINKEDLIST_List* list, unsigned int item) {
    assert(list);
    if (item >= list->count) {
        Throw(E_INDEX_OUT_OF_BOUNDS);
    }
    LINKEDLIST_Node* node_ptr = list->head;
    for (unsigned int ndx = item; ndx != 0 && node_ptr != NULL; ndx --) {
        node_ptr = node_ptr->next;
    }
    if (node_ptr == NULL) {
        //possible if no Try/Catch block
        return NULL;
    }
    return node_ptr->data;
}

unsigned int LINKEDLIST_item_count(const LINKEDLIST_List* list) {
    assert(list);
    return list->count;
}

static LINKEDLIST_Node* find(const LINKEDLIST_List* list, const void* object) {
    assert(list);
    assert(object);

    LINKEDLIST_Node* node_ptr = list->head;
    while (node_ptr != NULL) {
        if (node_ptr->data == object) {
            return node_ptr;
        }
        node_ptr = node_ptr->next;
    }
    return NULL;
}

static void destroy_node(LINKEDLIST_Node* node) {
    assert(node);
    free(node);
}

static void remove_node(LINKEDLIST_List* list, LINKEDLIST_Node* node_ptr, LINKEDLIST_Node* node_ptr_last) {
    assert(list);
    assert(node_ptr);
    if (node_ptr == list->head && node_ptr == list->tail) {
        list->count = 0;
        list->head = NULL;
        list->tail = NULL;
        destroy_node(node_ptr);
        return;
    }
    if (node_ptr == list->head) {
        list->head = node_ptr->next;
    } else if (node_ptr == list->tail) {
        list->tail = node_ptr_last;
        node_ptr_last->next = NULL;
    } else {
        node_ptr_last->next = node_ptr->next;
    }
    destroy_node(node_ptr);
    list->count--;
}

void LINKEDLIST_remove(LINKEDLIST_List* list, const void* object) {
    assert(list);
    assert(object);
    LINKEDLIST_Node* node_ptr = list->head;
    LINKEDLIST_Node* node_ptr_last = NULL;
    bool found = false;

    while (node_ptr != NULL && !found) {
        if (node_ptr->data == object) {
            found = true;
        } else {
            node_ptr_last = node_ptr;
            node_ptr = node_ptr->next;
        }
    }

    if (found) {
        remove_node(list, node_ptr, node_ptr_last);
    }
}

void LINKEDLIST_filter(LINKEDLIST_List* list, bool (*apply_each_ptr)(void* ptr, void* context), void* context) {
    assert(list);
    assert(apply_each_ptr);
    LINKEDLIST_Node* node_ptr = list->head;
    LINKEDLIST_Node* node_ptr_last = NULL;
    while (node_ptr != NULL) {
        if (apply_each_ptr(node_ptr->data, context)) {
            node_ptr_last = node_ptr;
            node_ptr = node_ptr->next;
        } else {
            LINKEDLIST_Node* tmp = node_ptr->next;
            remove_node(list, node_ptr, node_ptr_last);
            node_ptr = tmp;
        }
    }
}

bool LINKEDLIST_is_empty(const LINKEDLIST_List* list) {
    assert(list);
    return (list->count == 0);
}

void LINKEDLIST_iterator(LINKEDLIST_List* list, bool (*apply_each_ptr)(void* ptr, void* context), void* context) {
  assert(list);
  assert(apply_each_ptr);
  LINKEDLIST_Node* node = list->head;
  while (node != NULL) {
      if (!apply_each_ptr(node->data, context))
          return;
      node = node->next;
  }
}

bool LINKEDLIST_contains(const LINKEDLIST_List* list, const void* object) {
    assert(list);
    assert(object);

    if (find(list, object)) {
        return true;
    }
    return false;
}

void* LINKEDLIST_last(const LINKEDLIST_List* list) {
    assert(list);
    if (list->tail) {
        return list->tail->data;
    } else {
        return NULL;
    }
}

void LINKEDLIST_delete(LINKEDLIST_List* list) {
    assert(list);
    LINKEDLIST_Node* node = list->head;
    LINKEDLIST_Node* next_node = NULL;
    while (node != NULL) {
        next_node = node->next;
        destroy_node(node);
        node = next_node;
    }
    free(list);
}
