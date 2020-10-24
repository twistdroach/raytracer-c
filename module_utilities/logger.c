#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "logger.h"

/**
 * Based on https://github.com/VKoskiv/c-ray/blob/master/src/utils/logging.c
 */
void LOGGER_log(LOGGER_LEVEL level, const char *fmt, ...) {
  time_t local_time;       /* calendar time */
  local_time = time(NULL); /* get current cal time */
  char timestamp[26];
  struct tm now;
  localtime_r(&local_time, &now);
  asctime_r(&now, timestamp);
  printf("%.19s  ", timestamp);

  // Bold
  printf("\x1b[1m");
  switch (level) {
  case LOGGER_ERROR:
    // Red
    printf("\x1b[31mError: ");
    break;
  case LOGGER_WARN:
    // Yellow
    printf("\x1b[33mWarning: ");
    break;
  case LOGGER_INFO:
    printf("Info: ");
    break;
  case LOGGER_DEBUG:
    printf("Debug: ");
    break;
  }
  // Reset formatting
  printf("\x1b[0m");

  char buf[512];
  va_list vargs;
  va_start(vargs, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vargs);
  va_end(vargs);
  printf("%s", buf);
}
