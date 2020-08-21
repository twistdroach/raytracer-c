#include "tuples.h"
#include "utilities.h"
#include "exceptions.h"

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <CException.h>

#define VECTOR 0.0
#define POINT 1.0

TUPLES_Point TUPLES_point(double x, double y, double z) {
    return (TUPLES_Tuple) {
        .x = x,
        .y = y,
        .z = z,
        .w = POINT
    };
}

TUPLES_Vector TUPLES_vector(double x, double y, double z) {
    return (TUPLES_Vector) {
        .x = x,
        .y = y,
        .z = z,
        .w = VECTOR
    };
}

bool TUPLES_is_point(const TUPLES_Tuple tuple) {
    return double_equal(tuple.w, POINT);
}

bool TUPLES_is_vector(const TUPLES_Tuple tuple) {
    return double_equal(tuple.w, VECTOR);
}

TUPLES_Color TUPLES_color(double red, double green, double blue) {
    return (TUPLES_Color) {
        .red = red,
        .green = green,
        .blue = blue,
        .alpha = 0
    };
}

TUPLES_Tuple TUPLES_add(const TUPLES_Tuple t1, const TUPLES_Tuple t2) {
    return (TUPLES_Tuple) {
        .x = t1.x + t2.x,
        .y = t1.y + t2.y,
        .z = t1.z + t2.z,
        .w = t1.w + t2.w
    };
}

TUPLES_Tuple TUPLES_subtract(const TUPLES_Tuple t1, const TUPLES_Tuple t2) {
    return (TUPLES_Tuple) {
            .x = t1.x - t2.x,
            .y = t1.y - t2.y,
            .z = t1.z - t2.z,
            .w = t1.w - t2.w
    };
}

TUPLES_Vector TUPLES_negate(TUPLES_Vector vec)  {
    assert(TUPLES_is_vector(vec));
    return (TUPLES_Vector) {
        .x = 0.0 - vec.x,
        .y = 0.0 - vec.y,
        .z = 0.0 - vec.z,
        .w = VECTOR
    };
}

TUPLES_Vector TUPLES_reflect(const TUPLES_Vector v, const TUPLES_Vector normal) {
    // v - normal * 2 * dot(v, normal)
    return TUPLES_subtract(v, TUPLES_multiply(normal, 2.0 * TUPLES_dot(v, normal)));
}

TUPLES_Tuple TUPLES_multiply(const TUPLES_Tuple t1, const double mult) {
    return (TUPLES_Vector) {
            .x = t1.x * mult,
            .y = t1.y * mult,
            .z = t1.z * mult,
            .w = t1.w
    };
}

TUPLES_Tuple TUPLES_divide(const TUPLES_Tuple t1, const double div) {
    return (TUPLES_Vector) {
            .x = t1.x / div,
            .y = t1.y / div,
            .z = t1.z / div,
            .w = t1.w
    };
}

double TUPLES_magnitude(const TUPLES_Vector v) {
    assert(TUPLES_is_vector(v));
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

TUPLES_Vector TUPLES_normalize(const TUPLES_Vector v) {
    assert(TUPLES_is_vector(v));
    double magnitude = TUPLES_magnitude(v);
    assert(!double_equal(0, magnitude));
    return (TUPLES_Vector) {
        .x = v.x / magnitude,
        .y = v.y / magnitude,
        .z = v.z / magnitude,
        .w = VECTOR
    };
}

double TUPLES_dot(const TUPLES_Vector v1, const TUPLES_Vector v2) {
    assert(TUPLES_is_vector(v1));
    assert(TUPLES_is_vector(v2));
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

TUPLES_Vector TUPLES_cross(const TUPLES_Vector v1, const TUPLES_Vector v2) {
    assert(TUPLES_is_vector(v1) && TUPLES_is_vector(v2));
    return (TUPLES_Vector) {
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v1.x * v2.z,
        .z = v1.x * v2.y - v1.y * v2.x,
        .w = VECTOR
    };
}

// aka Hadamard or Schur product
TUPLES_Color TUPLES_multiply_colors(const TUPLES_Color c1, const TUPLES_Color c2) {
    return (TUPLES_Color) {
        .red = c1.red * c2.red,
        .green = c1.green * c2.green,
        .blue = c1.blue * c2.blue,
        .alpha = c1.alpha * c2.alpha
    };
}

bool TUPLES_is_equal(const TUPLES_Tuple* t1, const TUPLES_Tuple* t2) {
    return (double_equal(t1->x, t2->x) &&
            double_equal(t1->y, t2->y) &&
            double_equal(t1->z, t2->z) &&
            double_equal(t1->w, t2->w));
}

char* TUPLES_to_string(const TUPLES_Tuple* t) {
    assert(t);
    char* str = NULL;
    Sasprintf(str, "%.6f %.6f %.6f %.6f", t->x, t->y, t->z, t->w);
    return str;
}

