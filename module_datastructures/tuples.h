#ifndef MODULE_DATASTRUCTURES_TUPLE
#define MODULE_DATASTRUCTURES_TUPLE

#include <stdbool.h>

/**
 * struct to represent a point or vector
 */
typedef struct TUPLES_Tuple_ {
    double x, y, z, w;
} TUPLES_Tuple;

typedef TUPLES_Tuple* TUPLES_Point;
typedef TUPLES_Tuple* TUPLES_Vector;


TUPLES_Point TUPLES_new_point(double x, double y, double z);
TUPLES_Vector TUPLES_new_vector(double x, double y, double z);

bool TUPLES_is_point(TUPLES_Tuple *tuple);
bool TUPLES_is_vector(TUPLES_Tuple *tuple);

void TUPLES_destroy(TUPLES_Tuple* tuple);

#endif //MODULE_DATASTRUCTURES_TUPLE
