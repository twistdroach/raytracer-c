//
// Created by zrowitsch on 8/1/20.
//

#ifndef UTILITIES_UTILITIES_H
#define UTILITIES_UTILITIES_H
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdlib.h>  //for asprintf
#include <stdio.h>   //for free


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

#endif //UTILITIES_UTILITIES_H
