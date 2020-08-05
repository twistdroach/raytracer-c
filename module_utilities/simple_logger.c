#include <stdio.h>
#include <stdarg.h>

#include "simple_logger.h"

/**
 * Based on https://github.com/VKoskiv/c-ray/blob/master/src/utils/logging.c
 */
void SIMPLE_LOGGER_log(LogLevel level, const char* fmt, ...) {
    //Bold
    printf("\x1b[1m");
    switch(level) {
        case ERROR:
            //Red
            printf("\x1b[31mError: ");
            break;
        case WARN:
            //Yellow
            printf("\x1b[33mWarning: ");
            break;
        case INFO:
            printf("Info: ");
            break;
        case DEBUG:
            printf("Debug: ");
            break;
    }
    //Reset formatting
    printf("\x1b[0m");

    char buf[512];
    va_list vargs;
    va_start(vargs, fmt);
    vsnprintf(buf, sizeof(buf), fmt, vargs);
    va_end(vargs);
    printf("%s", buf);
}
