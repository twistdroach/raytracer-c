#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <string.h>

#include "exceptions.h"
#include "utilities.h"

inline bool double_equal(double a, double b) {
  // ghetto - but likely close enough, we'll see
  if (a == INFINITY && b == INFINITY)
    return true;
  if (a == -INFINITY && b == -INFINITY)
    return true;
  return (fabs(a - b) < EPSILON);
}

double UTILITIES_max(double x, double y, double z) {
  double xory = (x > y) ? x : y;
  return (xory > z) ? xory : z;
}

double UTILITIES_min(double x, double y, double z) {
  double xory = (x < y) ? x : y;
  return (xory < z) ? xory : z;
}

double UTILITIES_random_double(double min, double max) {
  assert(min <= max);
  if (min > max)
    Throw(E_INVALID_ARGUMENT);

  return min + (double)rand() / ((double)RAND_MAX / (max - min));
}

typedef struct UTILITIES_Timer {
  clock_t wall;
  clock_t user;
  clock_t sys;
} UTILITIES_Timer;

UTILITIES_Timer *UTILITIES_Timer_start() {
  UTILITIES_Timer *timer = malloc(sizeof(UTILITIES_Timer));
  if (!timer) {
    return NULL;
  }
  struct tms tmp_tms;
  clock_t walltime = times(&tmp_tms);
  if (walltime == ((clock_t)-1)) {
    free(timer);
    return NULL;
  }
  timer->wall = walltime;
  timer->user = tmp_tms.tms_utime;
  timer->sys = tmp_tms.tms_stime;

  return timer;
}

UTILITIES_Timer_Results UTILITIES_Timer_stop(UTILITIES_Timer *timer) {
  UTILITIES_Timer_Results results = {0};
  // if something failed creating timer, just return 0's
  if (!timer) {
    return results;
  }

  struct tms tmp_tms;
  clock_t walltime = times(&tmp_tms);
  if (walltime == ((clock_t)-1)) {
    free(timer);
    return results;
  }

  long ticks_per_sec = sysconf(_SC_CLK_TCK);
  if (ticks_per_sec < 0) {
    free(timer);
    return results;
  }

  results.wall_time_seconds = (walltime - timer->wall) / (double)ticks_per_sec;
  results.user_time_seconds = (tmp_tms.tms_utime - timer->user) / (double)ticks_per_sec;
  results.system_time_seconds = (tmp_tms.tms_stime - timer->sys) / (double)ticks_per_sec;

  free(timer);
  return results;
}

char* UTILITIES_concat(const char *s1, const char *s2) {
  assert(s1);
  assert(s2);
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  if (!result) {
    Throw(E_MALLOC_FAILED);
  }
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

char *UTILITIES_slurp(char *input_filename) {
  assert(input_filename);
  FILE *file = fopen(input_filename, "r");
  if (!file) {
    Throw(E_FILE_FAILED);
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    Throw(E_FILE_FAILED);
    return NULL;
  }

  long length = ftell(file);
  if (fseek(file, 0, SEEK_SET) != 0) {
    Throw(E_FILE_FAILED);
    return NULL;
  }

  char *buffer = calloc(length + 1, sizeof(char));
  if (!buffer) {
    Throw(E_MALLOC_FAILED);
    return NULL;
  }

  fread(buffer, sizeof(char), length, file);
  if (ferror(file) != 0) {
    Throw(E_FILE_FAILED);
    free(buffer);
    return NULL;
  }

  //not checked, not sure what I'd do if it failed?
  fclose(file);

  //terminate the string!
  buffer[length] = '\0';

  return buffer;
}
