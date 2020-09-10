#include <assert.h>
#include <math.h>
#include "material.h"
#include "pattern.h"

MATERIAL_Material* MATERIAL_new() {
    MATERIAL_Material* m = malloc(sizeof(MATERIAL_Material));
    TUPLES_init_color(&m->color, 1, 1, 1);
    m->ambient = 0.1;
    m->diffuse = 0.9;
    m->specular = 0.9;
    m->shininess = 200.0;
    m->reflective = 0.0;
    m->transparency = 0.0;
    m->refractive_index = 1.0;
    m->pattern = NULL;
    m->shadow_calc = true;
    return m;
}
void MATERIAL_delete(MATERIAL_Material* m) {
    assert(m);
    TUPLES_destroy(&m->color);
    if (m->pattern) {
        PATTERN_delete(m->pattern);
    }
    free(m);
}

bool MATERIAL_is_equal(const MATERIAL_Material* m1, const MATERIAL_Material* m2) {
    return TUPLES_is_equal(&m1->color, &m2->color) &&
            double_equal(m1->diffuse, m2->diffuse) &&
            double_equal(m1->shininess, m2->shininess) &&
            double_equal(m1->ambient, m2->ambient) &&
            double_equal(m1->specular, m2->specular);
}

MATERIAL_Material* MATERIAL_new_copy(const MATERIAL_Material* src) {
    assert(src);
    MATERIAL_Material* dest = MATERIAL_new();
    *dest = *src;
    if (src->pattern) {
        dest->pattern = PATTERN_new_copy(src->pattern);
    }
    return dest;
}

void MATERIAL_lighting(TUPLES_Color* dest, const SHAPE_Shape* shape, const LIGHTS_PointLight* light, const TUPLES_Point* position, const TUPLES_Vector* eye_vector, const TUPLES_Vector* normal_vector, bool in_shadow) {
    assert(dest);
    assert(shape);
    assert(light);
    assert(position);
    assert(eye_vector);
    assert(normal_vector);
    assert(shape->material);
    TUPLES_Color color_or_pattern;
    const MATERIAL_Material* material = shape->material;
    if (material->pattern) {
        PATTERN_color_at_shape(&color_or_pattern, material->pattern, shape, position);
    } else {
        TUPLES_copy(&color_or_pattern, &shape->material->color);
    }

    TUPLES_Color effective_color;
    TUPLES_multiply_colors(&effective_color, &color_or_pattern, &light->intensity);

    TUPLES_Vector lightv;
    TUPLES_subtract(&lightv, &light->position, position);
    TUPLES_normalize(&lightv);

    TUPLES_Color ambient;
    TUPLES_multiply(&ambient, &effective_color, material->ambient);

    if (in_shadow) {
        TUPLES_copy(dest, &ambient);
    } else {
        TUPLES_Color diffuse;
        TUPLES_Color specular;
        double light_dot_normal = TUPLES_dot(&lightv, normal_vector);
        if (light_dot_normal < 0) {
//        if (light_dot_normal < 0 && !double_equal(0.0, light_dot_normal)) {
            // negative means light is on other side of surface
            TUPLES_init_color(&diffuse, 0, 0, 0);
            TUPLES_init_color(&specular, 0, 0, 0);
        } else {
            TUPLES_multiply(&diffuse, &effective_color, material->diffuse * light_dot_normal);

            TUPLES_negate(&lightv);
            TUPLES_Vector reflectv;
            TUPLES_reflect(&reflectv, &lightv, normal_vector);
            double reflect_dot_eye = TUPLES_dot(&reflectv, eye_vector);

            if (reflect_dot_eye < 0 || double_equal(0.0, reflect_dot_eye)) {
                // light is reflecting away
                TUPLES_init_color(&specular, 0, 0, 0);
            } else {
                double factor = pow(reflect_dot_eye, material->shininess);
                TUPLES_multiply(&specular, &light->intensity, material->specular * factor);
            }
        }

        TUPLES_add(dest, &ambient, &diffuse);
        TUPLES_add(dest, dest, &specular);
    }
}

void MATERIAL_set_pattern(MATERIAL_Material* material, const PATTERN_Pattern* pattern) {
    assert(material);
    assert(pattern);
    if (material->pattern) {
        PATTERN_delete(material->pattern);
    }
    material->pattern = PATTERN_new_copy(pattern);
}

bool MATERIAL_casts_shadow(const MATERIAL_Material* m) {
    assert(m);
    return m->shadow_calc;
}
