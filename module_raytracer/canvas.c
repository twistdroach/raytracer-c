#include "canvas.h"
#include "exceptions.h"
#include <CException.h>
#include <assert.h>
#include <logger.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utilities.h>

static CEXCEPTION_T exception;

CANVAS_Canvas *CANVAS_new(uint width, uint height) {
  CANVAS_Canvas *canvas = (CANVAS_Canvas *)malloc(sizeof(CANVAS_Canvas));
  if (!canvas)
    Throw(E_MALLOC_FAILED);

  CANVAS_init(canvas, width, height);
  return canvas;
}

void CANVAS_init(CANVAS_Canvas *canvas, uint width, uint height) {
  canvas->width = width;
  canvas->height = height;
  canvas->data = (TUPLES_Color *)malloc((size_t)width * height * sizeof(TUPLES_Color));
  if (!canvas->data)
    Throw(E_MALLOC_FAILED);

  for (uint i = 0; i < width * height; i++) {
    TUPLES_init_color(&canvas->data[i], 0, 0, 0);
  }
}

static uint calculate_index(const CANVAS_Canvas *canvas, uint x, uint y) {
  assert(x < canvas->width);
  assert(y < canvas->height);
  return y * canvas->width + x;
}

TUPLES_Color *CANVAS_read_pixel(const CANVAS_Canvas *canvas, uint x, uint y) {
  assert(canvas);
  uint ndx = calculate_index(canvas, x, y);
  return &canvas->data[ndx];
}

void CANVAS_write_pixel(CANVAS_Canvas *canvas, uint x, uint y, const TUPLES_Color *color) {
  assert(canvas);
  assert(color);
  assert(x < canvas->width);
  assert(y < canvas->height);
  TUPLES_Color *dest = CANVAS_read_pixel(canvas, x, y);
  TUPLES_copy(dest, color);
}

char *CANVAS_get_ppm_header_string(const CANVAS_Canvas *canvas) {
  assert(canvas);
  char *retval;
  if (asprintf(&retval, "P3\n%u %u\n255\n", canvas->width, canvas->height) == -1)
    Throw(E_MALLOC_FAILED);
  return retval;
}

u_int8_t clampnscale_double(double d) {
  if (d > 1)
    d = 1;
  if (d < 0)
    d = 0;
  return round(d * 255.0);
}

char *CANVAS_get_ppm_body_string(const CANVAS_Canvas *canvas) {
  assert(canvas);
  char *buffer = NULL;
  for (uint j = 0; j < canvas->height; j++) {
    for (uint k = 0; k < canvas->width; k++) {
      TUPLES_Color *color = CANVAS_read_pixel(canvas, k, j);
      u_int8_t red = clampnscale_double(color->red);
      u_int8_t green = clampnscale_double(color->green);
      u_int8_t blue = clampnscale_double(color->blue);
      if (buffer) {
        Sasprintf(buffer, "%s%u %u %u ", buffer, red, green, blue);
      } else {
        Sasprintf(buffer, "%u %u %u ", red, green, blue);
      }
    }
    Sasprintf(buffer, "%s\n", buffer);
  }
  Sasprintf(buffer, "%s\n", buffer);
  return buffer;
}

void CANVAS_write_body_to_file(const CANVAS_Canvas *canvas, FILE *file) {
  assert(canvas);
  assert(file);
  for (uint j = 0; j < canvas->height; j++) {
    for (uint k = 0; k < canvas->width; k++) {
      TUPLES_Color *color = CANVAS_read_pixel(canvas, k, j);
      u_int8_t red = clampnscale_double(color->red);
      u_int8_t green = clampnscale_double(color->green);
      u_int8_t blue = clampnscale_double(color->blue);
      fprintf(file, "%u %u %u ", red, green, blue);
    }
    fprintf(file, "\n");
  }
  fprintf(file, "\n");
}

void CANVAS_write_to_file(const CANVAS_Canvas *canvas, const char *filename) {
  FILE *file = fopen(filename, "wb");
  if (!file)
    Throw(E_FILE_FAILED);
  char *header = CANVAS_get_ppm_header_string(canvas);
  fprintf(file, "%s", header);
  CANVAS_write_body_to_file(canvas, file);
  fclose(file);
  free(header);
}

void CANVAS_destroy(CANVAS_Canvas *canvas) {
  assert(canvas);
  assert(canvas->data);
  free(canvas->data);
}

void CANVAS_delete(CANVAS_Canvas *canvas) {
  assert(canvas);
  CANVAS_destroy(canvas);
  free(canvas);
}

struct parse_status {
  FILE *file;
  void (*next_step)(struct parse_status*);
  unsigned int line_number;
  CANVAS_Canvas *canvas;
  unsigned int scale;
  char *line;
  size_t len;
};

static bool is_comment(struct parse_status *status) {
  //TODO this is super lame - likely needs to be better, we'll see...
  return (status->line[0] == '#');
}

static void parse_pixels(struct parse_status *status) {
  ssize_t nread;
  unsigned int current_x = 0;
  unsigned int current_y = 0;
  status->next_step = NULL;
  long r = -1;
  long g = -1;
  long b = -1;
  while ((nread = getline(&status->line, &status->len, status->file)) != -1) {
    if (nread > 0) {
      status->line_number++;
      while (nread > 0 && is_comment(status)) {
        nread = getline(&status->line, &status->len, status->file);
        status->line_number++;
      }
      char *saveptr = NULL;
      char *delim = " \n";
      char* num_str = strtok_r(status->line, delim, &saveptr);
      while (num_str) {
        char *err = NULL;
        long num = strtol(num_str, &err, 10);
        if (num_str == err) {
          LOGGER_log(LOGGER_ERROR, "Error parsing pixel string, line_number (%u), line(%s), err(%s)\n", status->line_number, status->line, err);
          Throw(E_PARSE_FAILED);
        }
        if (r < 0) {
          r = num;
        } else if (g < 0) {
          g = num;
        } else if (b < 0) {
          b = num;
          TUPLES_Color color;
          TUPLES_init_color(&color, r/(double)status->scale, g/(double)status->scale, b/(double)status->scale);
          CANVAS_write_pixel(status->canvas, current_x, current_y, &color);
          if (++current_x == status->canvas->width) {
            ++current_y;
            current_x = 0;
          }
          r = -1;
          g = -1;
          b = -1;
        }
        num_str = strtok_r(NULL, delim, &saveptr);
      }
    }
  }
}

static void parse_scale(struct parse_status *status) {
  ssize_t nread = getline(&status->line, &status->len, status->file);
  status->next_step = NULL;
  if (nread > 0) {
    status->line_number++;
    while (nread > 0 && is_comment(status)) {
      nread = getline(&status->line, &status->len, status->file);
      status->line_number++;
    }
    char *saveptr = NULL;
    char *delim = " \n";
    char* scale_string = strtok_r(status->line, delim, &saveptr);
    char *err = NULL;
    unsigned long scale = strtoul(scale_string, &err, 10);
    if (scale_string == err) {
      LOGGER_log(LOGGER_ERROR, "Error parsing scale string, line(%s), err(%s)\n", status->line, err);
      Throw(E_PARSE_FAILED);
    }
    status->scale = scale;
    status->next_step = &parse_pixels;
  }
}

static void parse_dimensions(struct parse_status *status) {
  ssize_t nread = getline(&status->line, &status->len, status->file);
  status->next_step = NULL;
  if (nread > 0) {
    status->line_number++;
    while (nread > 0 && is_comment(status)) {
      nread = getline(&status->line, &status->len, status->file);
      status->line_number++;
    }
    //Should be width<space>height
    char *saveptr = NULL;
    char *delim = " \n";
    char* width_string = strtok_r(status->line, delim, &saveptr);
    char* height_string = strtok_r(NULL, delim, &saveptr);
    LOGGER_log(LOGGER_DEBUG, "Found width %s, height %s while parsing ppm\n", width_string, height_string);
    char *err = NULL;
    unsigned long width = strtoul(width_string, &err, 10);
    if (width_string == err) {
      LOGGER_log(LOGGER_ERROR, "Error parsing ppm width, line(%s), err(%s)\n", status->line, err);
      Throw(E_PARSE_FAILED);
    }
    err = NULL;
    unsigned long height = strtoul(height_string, &err, 10);
    if (height_string == err) {
      LOGGER_log(LOGGER_ERROR, "Error parsing ppm height, line(%s), err(%s)\n", status->line, err);
      Throw(E_PARSE_FAILED);
    }
    status->canvas = CANVAS_new(width, height);
    status->next_step = &parse_scale;
  }
}

static void parse_magic_number(struct parse_status *status) {
  ssize_t nread = getline(&status->line, &status->len, status->file);
  status->next_step = NULL;
  if (nread > 0) {
    status->line_number++;
    char *saveptr = NULL;
    char *delim = " \n";
    char* magic = strtok_r(status->line, delim, &saveptr);
    if (strncmp(magic, "P3", nread) != 0) {
      LOGGER_log(LOGGER_ERROR, "Error parsing PPM magic number: %s\n", status->line);
      Throw(E_PARSE_FAILED);
      return;
    }
  } else {
    LOGGER_log(LOGGER_ERROR, "Error getting line while parsing PPM magic number\n");
    Throw(E_PARSE_FAILED);
    return;
  }
  status->next_step = &parse_dimensions;
}

CANVAS_Canvas *CANVAS_parse_stream(FILE *file) {
  assert(file);

  struct parse_status status = {
      .line_number = 0,
      .file = file,
      .next_step = &parse_magic_number,
      .canvas = NULL,
      .len = 0,
      .line = NULL
  };

  Try {
    while (status.next_step) {
      status.next_step(&status);
    }
    free(status.line);
  }
  Catch(exception) {
    free(status.line);
    Throw(exception);
  }
  return status.canvas;
}

CANVAS_Canvas *CANVAS_parse_file_by_name(char *filename) {
  assert(filename);
  FILE *objfile = fopen(filename, "r");
  if (!objfile) {
    char *cwd = getcwd(NULL, 0);
    LOGGER_log(LOGGER_ERROR, "fopen failed to open (%s), cwd is (%s)\n", filename, cwd);
    free(cwd);
    Throw(E_FILE_FAILED);
  } else {
    LOGGER_log(LOGGER_INFO, "Opened file %s for parsing as ppm\n", filename);
  }
  CANVAS_Canvas *obj = CANVAS_parse_stream(objfile);
  fclose(objfile);
  return obj;
}
