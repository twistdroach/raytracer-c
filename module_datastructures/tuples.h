#ifndef MODULE_DATASTRUCTURES_TUPLE
#define MODULE_DATASTRUCTURES_TUPLE

#include <stdbool.h>

/**
 * struct to represent a point or vector
 */
typedef struct TUPLES_Tuple_ {
    double x, y, z, w;
} TUPLES_Tuple;

typedef TUPLES_Tuple TUPLES_Point;
typedef TUPLES_Tuple TUPLES_Vector;

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

/**
 * Initializes a tuple as a point, user responsible for destroy
 * @param v
 * @param x
 * @param y
 * @param z
 */
void TUPLES_init_point(TUPLES_Point* t, double x, double y, double z);

bool TUPLES_is_point(const TUPLES_Tuple *tuple);
bool TUPLES_is_vector(const TUPLES_Tuple *tuple);

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


void TUPLES_multiply(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, const double);
void TUPLES_divide(TUPLES_Tuple* dest, const TUPLES_Tuple* t1, const double);
double TUPLES_magnitude(const TUPLES_Vector* v);
void TUPLES_normalize(TUPLES_Vector* v);
double TUPLES_dot(const TUPLES_Vector* v1, const TUPLES_Vector* v2);
void TUPLES_cross(TUPLES_Vector* dest, const TUPLES_Vector* v1, const TUPLES_Vector* v2);

void TUPLES_destroy(TUPLES_Tuple* tuple);

#endif //MODULE_DATASTRUCTURES_TUPLE
