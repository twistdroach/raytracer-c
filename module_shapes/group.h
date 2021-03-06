#ifndef DATA_STRUCTURES_GROUP_H
#define DATA_STRUCTURES_GROUP_H

#include "arrlist.h"
#include "ray.h"
#include "shape.h"

typedef struct GROUP_Group GROUP_Group;

const SHAPE_vtable GROUP_vtable;

GROUP_Group *GROUP_new();
void GROUP_init(GROUP_Group *group);
void GROUP_destroy(GROUP_Group *group);
void GROUP_delete(GROUP_Group *group);
void GROUP_delete_shape(SHAPE_Shape *shape);
void GROUP_partition_children(GROUP_Group *src_group, GROUP_Group *sub_group1, GROUP_Group *sub_group2);
void GROUP_remove_child(GROUP_Group *group, SHAPE_Shape *shape);

/**
 * \pure This function should never be called...it should be overridden by the members of the group.
 * @param local_normal
 * @param group
 * @param local_point
 * @param hit
 */
void GROUP_local_normal_at(TUPLES_Vector *local_normal, SHAPE_Shape *group, const TUPLES_Point *local_point, const RAY_Xs *hit);
void GROUP_local_intersect(RAY_Intersections *intersections, SHAPE_Shape *group, const RAY_Ray *local_ray);

void GROUP_set_material(GROUP_Group *group, const MATERIAL_Material *material);
#define GROUP_get_material(group) SHAPE_get_material((SHAPE_Shape *)group)
#define GROUP_set_transform(group, transform) SHAPE_set_transform((SHAPE_Shape *)group, transform)
#define GROUP_get_transform(group) SHAPE_get_transform((SHAPE_Shape *)group)

/**
 * The shape added will be freed when group is deleted...
 * @param group
 * @param shape must be a SHAPE_Shape or derivative thereof
 */
void GROUP_add_child(GROUP_Group *group, void *shape);
bool GROUP_is_empty(const GROUP_Group *group);
bool GROUP_contains(const GROUP_Group *group, const void *shape);

/**
 * Must cast to appropriate type...likely SHAPE_Shape* unless you know what you put in there.
 * This is really only used for testing at the moment.
 */
void *GROUP_get_child(GROUP_Group *group, unsigned int ndx);

#endif // DATA_STRUCTURES_GROUP_H
