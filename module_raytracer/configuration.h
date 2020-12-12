#ifndef SIMPLE_RAYTRACER_CONFIGURATION_H
#define SIMPLE_RAYTRACER_CONFIGURATION_H

#include "camera.h"
#include "world.h"

typedef struct CONFIGURATION_Config {
  WORLD_World   *world;
  CAMERA_Camera *camera;
  char          *output_file;
} CONFIGURATION_Config;

UTILITIES_Timer_Results CONFIGURATION_render(CONFIGURATION_Config *config);

#endif // SIMPLE_RAYTRACER_CONFIGURATION_H
