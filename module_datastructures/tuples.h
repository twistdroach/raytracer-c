#ifndef MODULE_DATASTRUCTURES_TUPLE
#define MODULE_DATASTRUCTURES_TUPLE

#include "utilities.h"
#include <stdbool.h>

/**
 * struct to represent a point or vector
 */
typedef union TUPLES_Tuple {
    struct {
        double x, y, z, w;
    };
    struct {
        double red, green, blue, alpha;
    };
} TUPLES_Tuple;

typedef TUPLES_Tuple TUPLES_Point;
typedef TUPLES_Tuple TUPLES_Vector;
typedef TUPLES_Tuple TUPLES_Color;

TUPLES_Point TUPLES_point(double x, double y, double z);
TUPLES_Vector TUPLES_vector(double x, double y, double z);
bool TUPLES_is_vector(const TUPLES_Tuple tuple);
bool TUPLES_is_point(const TUPLES_Tuple tuple);

TUPLES_Tuple TUPLES_add(const TUPLES_Tuple t1, const TUPLES_Tuple t2);
TUPLES_Tuple TUPLES_subtract(const TUPLES_Tuple t1, const TUPLES_Tuple t2);
TUPLES_Tuple TUPLES_negate(TUPLES_Vector vec);

TUPLES_Tuple TUPLES_multiply(const TUPLES_Tuple t1, double);
TUPLES_Tuple TUPLES_divide(const TUPLES_Tuple t1, double);
double TUPLES_magnitude(const TUPLES_Vector v);
TUPLES_Tuple TUPLES_normalize(TUPLES_Vector v);
double TUPLES_dot(const TUPLES_Vector v1, const TUPLES_Vector v2);
TUPLES_Tuple TUPLES_cross(const TUPLES_Vector v1, const TUPLES_Vector v2);
TUPLES_Vector TUPLES_reflect(const TUPLES_Vector v, const TUPLES_Vector normal);
TUPLES_Color TUPLES_color(double red, double green, double blue);
char* TUPLES_to_string(const TUPLES_Tuple*);

/**
 * Hadamard or Schur product
 * @param dest
 * @param c1
 * @param c2
 */
TUPLES_Color TUPLES_multiply_colors(const TUPLES_Color c1, const TUPLES_Color c2);
bool TUPLES_is_equal(const TUPLES_Tuple* t1, const TUPLES_Tuple* t2);

#endif //MODULE_DATASTRUCTURES_TUPLE
