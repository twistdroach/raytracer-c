#ifndef DATA_STRUCTURES_CYLINDER_H
#define DATA_STRUCTURES_CYLINDER_H

#include "ray.h"
#include "shape.h"


/**
 * \extends SHAPE_Shape
 */
typedef struct CYLINDER_Cylinder {
    union {
        SHAPE_Shape shape;
        struct {
            const SHAPE_vtable* vtable;
            MATRIX_Matrix*      transform;
            MATRIX_Matrix*      inverse;
            MATRIX_Matrix*      inverse_transpose;
            MATERIAL_Material*  material;
            SHAPE_Shape*        parent;
        };
    };
    double minimum, maximum;
    bool closed;
} CYLINDER_Cylinder;

const SHAPE_vtable CYLINDER_vtable;

CYLINDER_Cylinder* CYLINDER_new();
void CYLINDER_init(CYLINDER_Cylinder* cylinder);
void CYLINDER_destroy(CYLINDER_Cylinder* cylinder);
void CYLINDER_delete(CYLINDER_Cylinder* cylinder);
void CYLINDER_delete_shape(SHAPE_Shape* shape);

void CYLINDER_local_normal_at(TUPLES_Vector* local_normal, SHAPE_Shape* cylinder, const TUPLES_Point* local_point, const RAY_Xs* hit);
void CYLINDER_local_intersect(RAY_Intersections* intersections, SHAPE_Shape* cylinder, const RAY_Ray* local_ray);
bool CYLINDER_check_cap(const RAY_Ray* local_ray, double t, double radius);

#define CYLINDER_set_material(cylinder, material) SHAPE_set_material((SHAPE_Shape*)cylinder, material)
#define CYLINDER_get_material(cylinder) SHAPE_get_material((SHAPE_Shape*)cylinder)
#define CYLINDER_set_transform(cylinder, transform) SHAPE_set_transform((SHAPE_Shape*)cylinder, transform)

#endif //DATA_STRUCTURES_CYLINDER_H
