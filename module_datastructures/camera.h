#ifndef DATA_STRUCTURES_CAMERA_H
#define DATA_STRUCTURES_CAMERA_H

#include "tuples.h"
#include "matrix.h"
#include "ray.h"
#include "world.h"
#include "canvas.h"

typedef struct CAMERA_Camera {
    unsigned int   hsize;           /** horizontal size in pixels */
    unsigned int   vsize;           /** vertical size in pixels   */
    double         half_width;
    double         half_height;
    double         field_of_view;   /** field of view in radians  */
    double         pixel_size;      /** pixel size in world units */
    MATRIX_Matrix* transform;       /** view transform            */
    MATRIX_Matrix* inv_transform;   /** inverse view transform    */
} CAMERA_Camera;

CAMERA_Camera* CAMERA_new(unsigned int hsize, unsigned int vsize, double field_of_view);
void CAMERA_delete(CAMERA_Camera* camera);

/**
 * Constructs and allocates a transformation matrix from 2 points and a vector pointing up
 * @param from
 * @param to
 * @param up
 * @return
 */
MATRIX_Matrix* CAMERA_view_transform(const TUPLES_Point from, const TUPLES_Point to, const TUPLES_Vector up);
void CAMERA_set_transform(CAMERA_Camera* camera, MATRIX_Matrix* transform);

void CAMERA_ray_for_pixel(RAY_Ray* dest, const CAMERA_Camera* camera, unsigned int px, unsigned int py);

CANVAS_Canvas* CAMERA_render(const CAMERA_Camera* camera, const WORLD_World* world);

#endif //DATA_STRUCTURES_CAMERA_H
