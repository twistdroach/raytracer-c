#ifndef UTILITIES_PROJ_EXCEPTIONS_H
#define UTILITIES_PROJ_EXCEPTIONS_H

#include <CException.h>

typedef enum EXCEPTIONS_exceptions {
    E_NO_ERROR = 0,
    E_MALLOC_FAILED,
    E_FILE_FAILED,
    E_INVALID_ARGUMENT,
    E_INDEX_OUT_OF_BOUNDS
} EXCEPTIONS_exceptions;

extern const char* EXCEPTIONS_strings[];

#endif //UTILITIES_PROJ_EXCEPTIONS_H
