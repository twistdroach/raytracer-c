#ifndef DATA_STRUCTURES_CAMERA_H
#define DATA_STRUCTURES_CAMERA_H

#include "tuples.h"
#include "matrix.h"

typedef struct CAMERA_Camera {
    unsigned int hsize;
    unsigned int vsize;
    double field_of_view;
    MATRIX_Matrix* transform;
} CAMERA_Camera;

CAMERA_Camera* CAMERA_new(unsigned int hsize, unsigned int vsize, double field_of_view);
void CAMERA_delete(CAMERA_Camera* camera);

MATRIX_Matrix* CAMERA_view_transform(const TUPLES_Point* from, const TUPLES_Point* to, const TUPLES_Vector* up);

#endif //DATA_STRUCTURES_CAMERA_H
