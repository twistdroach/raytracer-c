#ifndef UTILITIES_UTILITIES_H
#define UTILITIES_UTILITIES_H
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdlib.h>  //for asprintf
#include <stdio.h>   //for free

#include "exceptions.h"

#define EPSILON 0.0001
bool double_equal(double a, double b);

double UTILITIES_max(double a, double b, double c);
double UTILITIES_min(double a, double b, double c);

/**
 * Mark a value as unsed and stop the compiler from warning about it.
 */
#define UNUSED(x) ((x)=(x))

/**
 * safer asprintf - allows you to take a pointer (initialized to NULL)
 * and keep "appending" string data to it.  Not the most efficient,
 * but easy to use.
 * From: 21st Century C, 2nd edition Example 9-4
 */
#define Sasprintf(write_to,  ...) {           \
    char *tmp_string_for_extend = (write_to); \
    if (!asprintf(&(write_to), __VA_ARGS__)) {  \
      Throw(E_MALLOC_FAILED);                   \
    }                                          \
    free(tmp_string_for_extend);              \
}

/**
 * Generic vectorization of functions that take a single pointer.
 * Example:
 * #define Free_all(...) Fn_apply(void, free, __VA_ARGS__);
 * From: 21st Century C, 2nd edition Example 10-6
 */
#define Fn_apply(type, fn, ...) {                                      \
    void *stopper_for_apply = (int[]){0};                              \
    type **list_for_apply = (type*[]){__VA_ARGS__, stopper_for_apply}; \
    for (int i=0; list_for_apply[i] != stopper_for_apply; i++)         \
    fn(list_for_apply[i]);                                        \
}

/**
 * Gets a random number of low precision.  Min must be less than max.
 * Asserts min < max and/or throws E_INVALID_ARGUMENT if assertions disabled.
 * @param min
 * @param max
 * @return a random double >= min && <= max
 */
double UTILITIES_random_double(double min, double max);

typedef struct UTILITIES_Timer UTILITIES_Timer;
typedef struct UTILITIES_Timer_Results {
    double user_time_seconds;
    double system_time_seconds;
    double wall_time_seconds;

} UTILITIES_Timer_Results;
UTILITIES_Timer* UTILITIES_Timer_start();
UTILITIES_Timer_Results UTILITIES_Timer_stop(UTILITIES_Timer* timer);

#endif //UTILITIES_UTILITIES_H
