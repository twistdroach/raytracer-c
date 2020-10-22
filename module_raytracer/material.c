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

struct calculate_lighting_context {
    TUPLES_Color sum;
    double intensity;
    const MATERIAL_Material* material;
    const TUPLES_Point* position;
    const TUPLES_Vector* normal_vector;
    const TUPLES_Vector* eye_vector;
    const TUPLES_Color* effective_color;
    const TUPLES_Color* light_color;
};

static void calculate_lighting(TUPLES_Point* light_point, void* context) {
    struct calculate_lighting_context* c = (struct calculate_lighting_context*) context;

    TUPLES_Color diffuse;
    TUPLES_Color specular;

    TUPLES_Vector lightv;
    TUPLES_subtract(&lightv, light_point, c->position);
    TUPLES_normalize(&lightv);

    double light_dot_normal = TUPLES_dot(&lightv, c->normal_vector);
    if (light_dot_normal < 0) {
        // negative means light is on other side of surface
        TUPLES_init_color(&diffuse, 0, 0, 0);
        TUPLES_init_color(&specular, 0, 0, 0);
    } else {
        TUPLES_multiply(&diffuse, c->effective_color, c->material->diffuse * light_dot_normal);

        TUPLES_negate(&lightv);
        TUPLES_Vector reflectv;
        TUPLES_reflect(&reflectv, &lightv, c->normal_vector);
        double reflect_dot_eye = TUPLES_dot(&reflectv, c->eye_vector);

        if (reflect_dot_eye < 0 || double_equal(0.0, reflect_dot_eye)) {
            // light is reflecting away
            TUPLES_init_color(&specular, 0, 0, 0);
        } else {
            double factor = pow(reflect_dot_eye, c->material->shininess);
            TUPLES_multiply(&specular, c->light_color, c->material->specular * factor);
        }
    }

    TUPLES_add(&c->sum, &c->sum, &diffuse);
    TUPLES_add(&c->sum, &c->sum, &specular);

}

void MATERIAL_lighting(TUPLES_Color* dest, const SHAPE_Shape* shape, const LIGHTS_Light* light, const TUPLES_Point* position, const TUPLES_Vector* eye_vector, const TUPLES_Vector* normal_vector, double intensity) {
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

    /* calculate ambient contribution */
    TUPLES_multiply(dest, &effective_color, material->ambient);

    struct calculate_lighting_context context = (struct calculate_lighting_context) {
        .intensity = intensity,
        .material = material,
        .position = position,
        .normal_vector = normal_vector,
        .eye_vector = eye_vector,
        .effective_color = &effective_color,
        .light_color = &light->intensity,
    };
    TUPLES_init_color(&context.sum, 0, 0, 0);

    LIGHTS_iterate_points_on_light(light, &calculate_lighting, &context);

    TUPLES_divide(&context.sum, &context.sum, light->samples);
    TUPLES_multiply(&context.sum, &context.sum, intensity);
    TUPLES_add(dest, dest, &context.sum);
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
