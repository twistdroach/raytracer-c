#include <assert.h>
#include <exceptions.h>
#include <linkedlist.h>
#include "group.h"

typedef struct GROUP_Group {
    SHAPE_Shape         shape;
    LINKEDLIST_List*       list;
    BOUND_Box*          bounds;
} GROUP_Group;

static bool child_bounds_of(void *void_shape, void* void_context) {
    assert(void_shape);
    assert(void_context);
    SHAPE_Shape* child_shape = (SHAPE_Shape*)void_shape;
    BOUND_Box* box = (BOUND_Box*) void_context;
    BOUND_Box child_box;
    SHAPE_parent_space_bounds_of(&child_box, child_shape);
    BOUND_add_box(box, &child_box);
    return true;
}

void GROUP_bounds_of(const SHAPE_Shape* shape, BOUND_Box* box) {
    assert(shape);
    assert(box);
    GROUP_Group* a_group = (GROUP_Group*) shape;
    BOUND_init(box);
    LINKEDLIST_iterator(a_group->list, child_bounds_of, box);
}

typedef struct child_shape_contains_context {
    const SHAPE_Shape* b;
    bool found;
} child_shape_contains_context;

static bool child_shape_contains(void *void_shape, void* void_context) {
    SHAPE_Shape* shape = (SHAPE_Shape*)void_shape;
    child_shape_contains_context* context = (child_shape_contains_context*) void_context;
    if (shape->vtable->contains(shape, context->b)) {
        context->found = true;
        return false; //false tells the iterator not to continue
    }
    return true;
}

bool GROUP_shape_contains(const SHAPE_Shape* a, const SHAPE_Shape* b) {
    if (a == b) return true;
    GROUP_Group* a_group = (GROUP_Group*) a;
    child_shape_contains_context context = (child_shape_contains_context) {
        .b = b,
        .found = false
    };
    LINKEDLIST_iterator(a_group->list, child_shape_contains, &context);
    return context.found;
}

static bool divide_subshape(void *void_shape, void* void_context) {
    SHAPE_Shape* shape = (SHAPE_Shape*) void_shape;
    unsigned int minimum_size = *(unsigned int*)void_context;
    SHAPE_divide(shape, minimum_size);
    return true;
}

void GROUP_divide(SHAPE_Shape* shape, unsigned int minimum_size) {
    assert(shape);
    assert(minimum_size > 0);
    if (minimum_size == 0) return;
    GROUP_Group* group = (GROUP_Group*) shape;
    if (LINKEDLIST_item_count(group->list) > minimum_size) {
        GROUP_Group* left_group = GROUP_new();
        GROUP_Group* right_group = GROUP_new();
        GROUP_partition_children(group, left_group, right_group);
        if (!GROUP_is_empty(left_group)) {
            GROUP_add_child(group, left_group);
        }
        if (!GROUP_is_empty(right_group)) {
            GROUP_add_child(group, right_group);
        }
    }
    LINKEDLIST_iterator(group->list, divide_subshape, &minimum_size);
}

const SHAPE_vtable GROUP_vtable = {
        &GROUP_local_intersect,
        &GROUP_delete_shape,
        &GROUP_local_normal_at,
        &GROUP_shape_contains,
        &GROUP_bounds_of,
        &GROUP_divide
};

GROUP_Group* GROUP_new() {
    GROUP_Group* group = malloc(sizeof(GROUP_Group));
    if (!group) {
        Throw(E_MALLOC_FAILED);
    }
    GROUP_init(group);
    return group;
}

void GROUP_init(GROUP_Group* group) {
    assert(group);
    SHAPE_init(&group->shape, &GROUP_vtable);
    group->list = LINKEDLIST_new();
    group->bounds = NULL;
}

static bool delete_subshape(void* void_shape, void* context) {
    assert(void_shape);
    UNUSED(context);
    SHAPE_Shape* shape = (SHAPE_Shape*) void_shape;
    SHAPE_delete_any_type(shape);
    return true;
}

void GROUP_destroy(GROUP_Group* group) {
    assert(group);
    SHAPE_destroy(&group->shape);
    LINKEDLIST_iterator(group->list, delete_subshape, NULL);
    LINKEDLIST_delete(group->list);
    if (group->bounds) {
        BOUND_delete(group->bounds);
    }
}

void GROUP_delete(GROUP_Group* group) {
    assert(group);
    GROUP_destroy(group);
    free(group);
}

void GROUP_delete_shape(SHAPE_Shape* shape) {
    assert(shape);
    GROUP_Group* group = (GROUP_Group*) shape;
    GROUP_delete(group);
}

void GROUP_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* group, const TUPLES_Point* local_point, const RAY_Xs* hit) {
    //should never get here...
    assert(local_normal);
    assert(group);
    assert(local_point);
    UNUSED(local_normal);
    UNUSED(group);
    UNUSED(local_point);
    UNUSED(hit);
    assert(0);
}

typedef struct check_shape_context {
    const RAY_Ray* local_ray;
    RAY_Intersections* intersections;
} check_shape_context;

static bool check_shape(void* void_shape, void* void_context) {
    check_shape_context* c = (check_shape_context*) void_context;
    SHAPE_Shape* shape = (SHAPE_Shape*) void_shape;
    SHAPE_intersect(c->intersections, shape, c->local_ray);
    return true;
}

static BOUND_Box* get_bounds(GROUP_Group* group) {
    assert(group);
    if (!group->bounds) {
        group->bounds = BOUND_new();
        GROUP_bounds_of((SHAPE_Shape*)group, group->bounds);
    }
    return group->bounds;
}

void GROUP_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);
    GROUP_Group* group = (GROUP_Group*)shape;

    if (!BOUND_intersect(get_bounds(group), local_ray)) {
        //if we don't hit the bounding box, no reason to hit all the children
        return;
    }

    check_shape_context c = {
            .local_ray = local_ray,
            .intersections = intersections
    };
    LINKEDLIST_iterator(group->list, check_shape, &c);
    RAY_sort_intersections(intersections);
}

void GROUP_add_child(GROUP_Group* group, void* shape) {
    assert(group);
    assert(shape);
    LINKEDLIST_add(group->list, shape);
    SHAPE_set_parent(shape, group);
}

bool GROUP_is_empty(const GROUP_Group* group) {
    assert(group);
    return LINKEDLIST_is_empty(group->list);
}

bool GROUP_contains(const GROUP_Group* group, const void* shape) {
    assert(shape);
    return LINKEDLIST_contains(group->list, shape);
}

static bool set_shape_material(void* void_shape, void* void_material) {
    assert(void_shape);
    SHAPE_Shape* shape = (SHAPE_Shape*)void_shape;
    MATERIAL_Material* material = (MATERIAL_Material*)void_material;
    SHAPE_set_material(shape, material);
    return true;
}

void GROUP_set_material(GROUP_Group* group, const MATERIAL_Material* material) {
    assert(group);
    assert(material);
    LINKEDLIST_iterator(group->list, set_shape_material, (void*)material);
}

void GROUP_remove_child(GROUP_Group* group, SHAPE_Shape* shape) {
    assert(group);
    assert(shape);
    LINKEDLIST_remove(group->list, shape);
}

typedef struct place_children_context {
    GROUP_Group* left_group;
    GROUP_Group* right_group;
    BOUND_Box* left_box;
    BOUND_Box* right_box;
} place_children_context;

static bool place_children(void* void_shape, void* void_context) {
    assert(void_shape);
    assert(void_context);
    SHAPE_Shape* shape = (SHAPE_Shape*) void_shape;
    place_children_context* context = (place_children_context*) void_context;
    BOUND_Box shape_box;
    SHAPE_parent_space_bounds_of(&shape_box, shape);
    if (BOUND_contains_box(context->left_box, &shape_box)) {
        GROUP_add_child(context->left_group, shape);
    } else if (BOUND_contains_box(context->right_box, &shape_box)) {
        GROUP_add_child(context->right_group, shape);
    } else {
        return true;
    }
    //we placed in a child group, so pull from the parent group
    return false;
}

void GROUP_partition_children(GROUP_Group* src_group, GROUP_Group* left_group, GROUP_Group* right_group) {
    assert(src_group);
    assert(left_group);
    assert(right_group);
    const BOUND_Box* box = get_bounds(src_group);
    BOUND_Box left_box, right_box;
    BOUND_split(box, &left_box, &right_box);
    place_children_context context = (place_children_context) {
        .left_box = &left_box,
        .right_box = &right_box,
        .left_group = left_group,
        .right_group = right_group
    };
    LINKEDLIST_filter(src_group->list, place_children, &context);
}

void* GROUP_get_child(GROUP_Group* group, unsigned int ndx) {
    assert(group);
    return LINKEDLIST_safe_get(group->list, ndx);
}
