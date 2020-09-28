//
// Created by zrowitsch on 5/25/19.
//

#ifndef UTILITIES_PROJ_EXCEPTIONS_H
#define UTILITIES_PROJ_EXCEPTIONS_H

#include <CException.h>

typedef enum {
    E_NO_ERROR,
    E_MALLOC_FAILED,
    E_FILE_FAILED,
    E_INVALID_ARGUMENT,
    E_INDEX_OUT_OF_BOUNDS
} EXCEPTIONS_exceptions;

#endif //UTILITIES_PROJ_EXCEPTIONS_H
