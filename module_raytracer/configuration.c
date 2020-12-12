#include <assert.h>
#include <logger.h>
#include <utilities.h>

#include "camera.h"
#include "canvas.h"
#include "configuration.h"

UTILITIES_Timer_Results CONFIGURATION_render(CONFIGURATION_Config *config) {
  assert(config);
  assert(config->world);
  assert(config->camera);
  //not asserting filename, if none is given, we just don't write one

  LOGGER_log(LOGGER_INFO, "Rendering...\n");
  UTILITIES_Timer* render_timer = UTILITIES_Timer_start();
  CANVAS_Canvas* canvas = CAMERA_render(config->camera, config->world);
  UTILITIES_Timer_Results render_results = UTILITIES_Timer_stop(render_timer);

  if (config->output_file) {
    LOGGER_log(LOGGER_INFO, "Writing file %s...\n", config->output_file);
    CANVAS_write_to_file(canvas, config->output_file);
  }

  return render_results;
}
