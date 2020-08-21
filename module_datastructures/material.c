#include <assert.h>
#include <math.h>
#include "material.h"

MATERIAL_Material* MATERIAL_new() {
    MATERIAL_Material* m = malloc(sizeof(MATERIAL_Material));
    m->color = TUPLES_color(1, 1, 1);
    m->ambient = 0.1;
    m->diffuse = 0.9;
    m->specular = 0.9;
    m->shininess = 200.0;
    return m;
}
void MATERIAL_delete(MATERIAL_Material* m) {
    assert(m);
    free(m);
}

bool MATERIAL_is_equal(const MATERIAL_Material* m1, const MATERIAL_Material* m2) {
    return TUPLES_is_equal(&m1->color, &m2->color) &&
            double_equal(m1->diffuse, m2->diffuse) &&
            double_equal(m1->shininess, m2->shininess) &&
            double_equal(m1->ambient, m2->ambient) &&
            double_equal(m1->specular, m2->specular);
}

void MATERIAL_copy(MATERIAL_Material* dest, const MATERIAL_Material* src) {
    //this assumes we don't really need to init dest->color, we can just copy the struct
    *dest = *src;
}

TUPLES_Color MATERIAL_lighting(const MATERIAL_Material* material, const LIGHTS_PointLight* light, TUPLES_Point position, TUPLES_Vector eye_vector, TUPLES_Vector normal_vector, bool in_shadow) {
    TUPLES_Color effective_color = TUPLES_multiply_colors(material->color, light->intensity);

    TUPLES_Vector lightv = TUPLES_normalize(TUPLES_subtract(light->position, position));

    //compute ambient light
    TUPLES_Color ambient = TUPLES_multiply(effective_color, material->ambient);
    if (!in_shadow) {
        TUPLES_Color diffuse;
        TUPLES_Color specular;
        double light_dot_normal = TUPLES_dot(lightv, normal_vector);
        if (light_dot_normal < 0) {
            // negative means light is on other side of surface
            diffuse = TUPLES_color(0, 0, 0);
            specular = TUPLES_color(0, 0, 0);
        } else {
            diffuse = TUPLES_multiply(effective_color, material->diffuse * light_dot_normal);

            lightv = TUPLES_negate(lightv);
            TUPLES_Vector reflectv = TUPLES_reflect(lightv, normal_vector);
            double reflect_dot_eye = TUPLES_dot(reflectv, eye_vector);

            if (reflect_dot_eye <= 0) {
                // light is reflecting away
                specular = TUPLES_color(0, 0, 0);
            } else {
                double factor = pow(reflect_dot_eye, material->shininess);
                specular = TUPLES_multiply(light->intensity, material->specular * factor);
            }
        }

        ambient = TUPLES_add(TUPLES_add(ambient, diffuse), specular);
    }
    return ambient;
}
