#include <stdbool.h>
#include <math.h>

#include "utilities.h"

inline bool double_equal(double a, double b)
{
    //ghetto - but likely close enough, we'll see
    return (fabs(a - b) < EPSILON);
}

