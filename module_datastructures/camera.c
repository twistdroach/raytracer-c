#include "camera.h"
#include "tuples.h"
#include "matrix.h"

#include <assert.h>
#include <math.h>

static void calculate_pixel_size(CAMERA_Camera* camera) {
    assert(camera);
    double half_view = tan(camera->field_of_view / 2.0);
    double aspect = camera->hsize / (double) camera->vsize;

    if (aspect >= 1) {
        camera->half_width = half_view;
        camera->half_height = half_view / aspect;
    } else {
        camera->half_width = half_view * aspect;
        camera->half_height = half_view;
    }
    camera->pixel_size = (camera->half_width * 2.0) / camera->hsize;
}

CAMERA_Camera* CAMERA_new(unsigned int hsize, unsigned int vsize, double field_of_view) {
    assert(hsize>0);
    assert(vsize>0);
    CAMERA_Camera* camera = malloc(sizeof(CAMERA_Camera));
    camera->hsize = hsize;
    camera->vsize = vsize;
    camera->field_of_view = field_of_view;
    camera->transform = MATRIX_new_identity(4);
    camera->inv_transform = MATRIX_new_identity(4);
    MATRIX_inverse(camera->inv_transform, camera->transform);
    calculate_pixel_size(camera);
    return camera;
}

void CAMERA_delete(CAMERA_Camera* camera) {
    assert(camera);
    assert(camera->transform);
    MATRIX_delete_all(camera->transform, camera->inv_transform);
    free(camera);
}

MATRIX_Matrix* CAMERA_view_transform(const TUPLES_Point from, const TUPLES_Point to, const TUPLES_Vector up) {
    TUPLES_Vector forward = TUPLES_subtract(to, from);
    forward = TUPLES_normalize(forward);

    TUPLES_Vector upn = TUPLES_normalize(up);

    TUPLES_Vector left = TUPLES_cross(forward, upn);

    TUPLES_Vector true_up = TUPLES_cross(left, forward);

    MATRIX_Matrix orientation;
    MATRIX_init(&orientation, 4, 4);
    MATRIX_fill(&orientation, left.x,     left.y,     left.z,     0.0,
                              true_up.x,  true_up.y,  true_up.z,  0.0,
                              -forward.x, -forward.y, -forward.z, 0.0,
                              0.0,        0.0,        0.0,        1.0);

    MATRIX_Matrix* translation = MATRIX_new_translation(-from.x, -from.y, -from.z);
    MATRIX_Matrix* view = MATRIX_multiply(&orientation, translation);
    MATRIX_delete(translation);
    return view;
}

void CAMERA_set_transform(CAMERA_Camera* camera, MATRIX_Matrix* transform) {
    assert(camera);
    assert(transform);
    assert(MATRIX_is_invertible(transform));
    MATRIX_copy(camera->transform, transform);
    MATRIX_inverse(camera->inv_transform, camera->transform);
}

void CAMERA_ray_for_pixel(RAY_Ray* dest, const CAMERA_Camera* camera, unsigned int px, unsigned int py) {
    assert(dest);
    assert(camera);
    assert(px < camera->hsize);
    assert(py < camera->vsize);

    double xoffset = (px + 0.5) * camera->pixel_size;
    double yoffset = (py + 0.5) * camera->pixel_size;

    double world_x = camera->half_width - xoffset;
    double world_y = camera->half_height - yoffset;

    TUPLES_Point ptmp = TUPLES_point(world_x, world_y, -1.0);
    TUPLES_Point pixel = MATRIX_multiply_tuple(camera->inv_transform, ptmp);

    TUPLES_Point origin = MATRIX_multiply_tuple(camera->inv_transform, TUPLES_point(0,0,0));

    TUPLES_Vector direction = TUPLES_normalize(TUPLES_subtract(pixel, origin));

    RAY_init_from_tuples(dest, origin, direction);
}

CANVAS_Canvas* CAMERA_render(const CAMERA_Camera* camera, const WORLD_World* world) {
    assert(camera);
    assert(world);
    CANVAS_Canvas* canvas = CANVAS_new(camera->hsize, camera->vsize);
    for (uint y = 0; y < camera->vsize - 1; y++) {
        for (uint x = 0; x < camera->hsize - 1; x++) {
            RAY_Ray ray;
            TUPLES_Color color;

            CAMERA_ray_for_pixel(&ray, camera, x, y);
            WORLD_color_at(&color, world, &ray);
            CANVAS_write_pixel(canvas, x, y, color);
        }
    }
    return canvas;
}
