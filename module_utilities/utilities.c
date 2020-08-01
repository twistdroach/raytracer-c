//
// Created by zrowitsch on 8/1/20.
//

#include <float.h>
#include <stdbool.h>
#include <math.h>

bool double_equal(double a, double b)
{
    //ghetto - but likely close enough, we'll see
    return (fabs(a - b) < 0.00001);
}