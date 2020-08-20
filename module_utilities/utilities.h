#ifndef UTILITIES_UTILITIES_H
#define UTILITIES_UTILITIES_H
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdlib.h>  //for asprintf
#include <stdio.h>   //for free

#define EPSILON 0.00001

bool double_equal(double a, double b);

/**
 * safer asprintf - allows you to take a pointer (initialized to NULL)
 * and keep "appending" string data to it.  Not the most efficient,
 * but easy to use.
 * From: 21st Century C, 2nd edition Example 9-4
 */
#define Sasprintf(write_to,  ...) {           \
    char *tmp_string_for_extend = (write_to); \
    asprintf(&(write_to), __VA_ARGS__);       \
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

#endif //UTILITIES_UTILITIES_H
