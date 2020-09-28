#include <assert.h>
#include <exceptions.h>
#include "group.h"

const SHAPE_vtable GROUP_vtable = {
        &GROUP_local_intersect,
        &GROUP_delete_shape,
        &GROUP_local_normal_at
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
    group->list = ARRLIST_new();
}

void delete_subshape(void* void_shape, void* context) {
    assert(void_shape);
    UNUSED(context);
    SHAPE_Shape* shape = (SHAPE_Shape*) void_shape;
    SHAPE_delete_any_type(shape);
}

void GROUP_destroy(GROUP_Group* group) {
    assert(group);
    SHAPE_destroy(&group->shape);
    ARRLIST_iterator(group->list, delete_subshape, NULL);
    ARRLIST_delete(group->list);
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

void GROUP_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* group, const TUPLES_Point* local_point) {
    //should never get here...
    assert(local_normal);
    assert(group);
    assert(local_point);
    assert(0);
}

typedef struct check_shape_context {
    const RAY_Ray* local_ray;
    RAY_Intersections* intersections;
} check_shape_context;

void check_shape(void* void_shape, void* void_context) {
    check_shape_context* c = (check_shape_context*) void_context;
    SHAPE_Shape* shape = (SHAPE_Shape*) void_shape;
    SHAPE_intersect(c->intersections, shape, c->local_ray);
}

void GROUP_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* shape, const RAY_Ray* local_ray) {
    assert(intersections);
    assert(shape);
    assert(local_ray);
    GROUP_Group* group = (GROUP_Group*)shape;
    check_shape_context c = {
            .local_ray = local_ray,
            .intersections = intersections
    };
    ARRLIST_iterator(group->list, check_shape, &c);
    RAY_sort_intersections(intersections);
}

void GROUP_add_child(GROUP_Group* group, void* shape) {
    assert(group);
    assert(shape);
    ARRLIST_add(group->list, shape);
    SHAPE_set_parent(shape, group);
}

bool GROUP_is_empty(const GROUP_Group* group) {
    assert(group);
    return ARRLIST_is_empty(group->list);
}

bool GROUP_contains(const GROUP_Group* group, const void* shape) {
    assert(shape);
    return ARRLIST_contains(group->list, shape);
}

void set_shape_material(void* void_shape, void* void_material) {
    assert(void_shape);
    SHAPE_Shape* shape = (SHAPE_Shape*)void_shape;
    MATERIAL_Material* material = (MATERIAL_Material*)void_material;
    SHAPE_set_material(shape, material);
}

void GROUP_set_material(GROUP_Group* group, const MATERIAL_Material* material) {
    assert(group);
    assert(material);
    ARRLIST_iterator(group->list, set_shape_material, (void*)material);
}
