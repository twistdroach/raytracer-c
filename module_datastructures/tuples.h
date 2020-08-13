#ifndef MODULE_DATASTRUCTURES_TUPLE
#define MODULE_DATASTRUCTURES_TUPLE

#include "utilities.h"
#include <stdbool.h>

/**
 * struct to represent a point or vector
 */
typedef union TUPLES_Tuple_ {
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

/**
 * Allocates and initializes a tuple.  Destroy must be called.
 * @param x
 * @param y
 * @param z
 * @return
 */
TUPLES_Point* TUPLES_new_point(double x, double y, double z);

/**
 * Allocates and initializes a tuple.  Destroy must be called.
 * @param x
 * @param y
 * @param z
 * @return
 */
TUPLES_Vector* TUPLES_new_vector(double x, double y, double z);

/**
 * Initializes a tuple as a vector, user responsible for destroy
 * @param v
 * @param x
 * @param y
 * @param z
 */
void TUPLES_init_vector(TUPLES_Vector* v, double x, double y, double z);
void TUPLES_init_color(TUPLES_Color* c, double red, double green, double blue);
bool TUPLES_is_vector(const TUPLES_Tuple *tuple);

/**
 * Initializes a tuple as a point, user responsible for destroy
 * @param v
 * @param x
 * @param y
 * @param z
 */
void TUPLES_init_point(TUPLES_Point* t, double x, double y, double z);
bool TUPLES_is_point(const TUPLES_Tuple *tuple);

/**
 * Adds two tuples
 * @param dest The destination of the addition operation.  Should be allocated.
 * @param t1 First operand
 * @param t2 Second operand
 */
void TUPLES_add(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, const TUPLES_Tuple* t2);

/**
 * Subtracts two tuples (t1 - t2)
 * @param dest The destination of the subtraction operation.  Should be allocated.
 * @param t1 First operand
 * @param t2 Second operand
 */
void TUPLES_subtract(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, const TUPLES_Tuple* t2);

/**
 * Negate a vector
 * @param tuple
 */
void TUPLES_negate(TUPLES_Vector* vec);

void TUPLES_multiply(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, double);
void TUPLES_divide(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, double);
double TUPLES_magnitude(const TUPLES_Vector* v);
void TUPLES_normalize(TUPLES_Vector* v);
double TUPLES_dot(const TUPLES_Vector* v1, const TUPLES_Vector* v2);
void TUPLES_cross(TUPLES_Vector* dest, const TUPLES_Vector* v1, const TUPLES_Vector* v2);
TUPLES_Vector* TUPLES_reflect(const TUPLES_Vector* v, const TUPLES_Vector* normal);
TUPLES_Color* TUPLES_new_color(double red, double green, double blue);

/**
 * Hadamard or Schur product
 * @param dest
 * @param c1
 * @param c2
 */
void TUPLES_multiply_colors(TUPLES_Color* dest, const TUPLES_Color* c1, const TUPLES_Color* c2);
bool TUPLES_is_equal(const TUPLES_Tuple* t1, const TUPLES_Tuple* t2);

void TUPLES_copy(TUPLES_Tuple* dest, const TUPLES_Tuple* src);
void TUPLES_destroy(TUPLES_Tuple* tuple);
#define TUPLES_destroy_all(...) Fn_apply(TUPLES_Tuple, TUPLES_destroy, __VA_ARGS__);
void TUPLES_delete(TUPLES_Tuple* tuple);
#define TUPLES_delete_all(...) Fn_apply(TUPLES_Tuple, TUPLES_delete, __VA_ARGS__);

#endif //MODULE_DATASTRUCTURES_TUPLE
