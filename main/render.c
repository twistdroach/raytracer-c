#include <configuration.h>
#include <logger.h>
#include <stdlib.h>
#include <utilities.h>
#include <yamlloader.h>

CEXCEPTION_T global_exception;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file.yaml>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  Try {
        char *input_filename = argv[1];
        char *output_filename = UTILITIES_concat(input_filename, ".ppm");

        char *data = UTILITIES_slurp(input_filename);

        CONFIGURATION_Config *config = YAMLLOADER_parse(data);
        config->output_file = output_filename;

        UTILITIES_Timer_Results results = CONFIGURATION_render(config);
        LOGGER_log(LOGGER_INFO, "Render Time - Wall: %.2f User: %.2f System: %.2f\n",
                   results.wall_time_seconds, results.user_time_seconds,
                   results.system_time_seconds);

        CAMERA_delete(config->camera);
        WORLD_delete(config->world);
        free(config);
        free(output_filename);
  }
  Catch(global_exception) {
    LOGGER_log(LOGGER_ERROR, "Caught exception: %s\n", EXCEPTIONS_strings[global_exception]);
    exit(EXIT_FAILURE);
  }
}
