#include "camera.h"
#include "tuples.h"
#include "matrix.h"

#include <assert.h>

CAMERA_Camera* CAMERA_new(unsigned int hsize, unsigned int vsize, double field_of_view) {
    CAMERA_Camera* camera = malloc(sizeof(CAMERA_Camera));
    camera->hsize = hsize;
    camera->vsize = vsize;
    camera->field_of_view = field_of_view;
    camera->transform = MATRIX_new_identity(4);
    return camera;
}

void CAMERA_delete(CAMERA_Camera* camera) {
    assert(camera);
    assert(camera->transform);
    MATRIX_delete(camera->transform);
    free(camera);
}

MATRIX_Matrix* CAMERA_view_transform(const TUPLES_Point* from, const TUPLES_Point* to, const TUPLES_Vector* up) {
    assert(from);
    assert(to);
    assert(up);
    TUPLES_Vector forward;
    TUPLES_subtract(&forward, to, from);
    TUPLES_normalize(&forward);

    TUPLES_Vector upn;
    TUPLES_copy(&upn, up);
    TUPLES_normalize(&upn);

    TUPLES_Vector left;
    TUPLES_cross(&left, &forward, &upn);

    TUPLES_Vector true_up;
    TUPLES_cross(&true_up, &left, &forward);

    MATRIX_Matrix orientation;
    MATRIX_init(&orientation, 4, 4);
    MATRIX_fill(&orientation, left.x,     left.y,     left.z,     0.0,
                              true_up.x,  true_up.y,  true_up.z,  0.0,
                              -forward.x, -forward.y, -forward.z, 0.0,
                              0.0,        0.0,        0.0,        1.0);

    MATRIX_Matrix* translation = MATRIX_new_translation(-from->x, -from->y, -from->z);
    MATRIX_Matrix* view = MATRIX_multiply(&orientation, translation);
    MATRIX_delete(translation);
    return view;
}
