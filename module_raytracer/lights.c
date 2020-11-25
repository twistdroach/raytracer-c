#include "lights.h"
#include "world.h"
#include "yamlloader.h"
#include <assert.h>
#include <exceptions.h>
#include <logger.h>
#include <memory.h>

LIGHTS_Light *LIGHTS_new_pointlight(const TUPLES_Point *p, const TUPLES_Color *c) {
  assert(p);
  assert(c);
  TUPLES_Vector uvec, vvec;
  TUPLES_init_vector(&uvec, 0, 0, 0);
  TUPLES_init_vector(&vvec, 0, 0, 0);
  return LIGHTS_new_arealight(p, &uvec, 1, &vvec, 1, c);
}

const TUPLES_Color *LIGHTS_get_color(const LIGHTS_Light *light) {
  assert(light);
  return &light->intensity;
}
void LIGHTS_iterate_points_on_light(const LIGHTS_Light *light, void (*each_point)(TUPLES_Point *point, void *context), void *context) {
  assert(light);
  assert(each_point);
  TUPLES_Point p;
  for (unsigned int v = 0; v < light->vsteps; v++) {
    for (unsigned int u = 0; u < light->usteps; u++) {
      LIGHTS_point_on_area_light(&p, light, u, v);
      each_point(&p, context);
    }
  }
}

LIGHTS_Light *LIGHTS_new_arealight(const TUPLES_Point *corner, const TUPLES_Vector *uvec, unsigned int usteps, const TUPLES_Vector *vvec, unsigned int vsteps,
                                   const TUPLES_Color *color) {
  assert(corner);
  assert(uvec);
  assert(vvec);
  assert(color);
  LIGHTS_Light *light = malloc(sizeof(LIGHTS_Light));
  if (!light) {
    Throw(E_MALLOC_FAILED);
  }

  TUPLES_copy(&light->corner, corner);
  TUPLES_copy(&light->intensity, color);
  light->usteps = usteps;
  TUPLES_init_vector(&light->uvec, uvec->x / usteps, uvec->y / usteps, uvec->z / usteps);
  light->vsteps = vsteps;
  TUPLES_init_vector(&light->vvec, vvec->x / vsteps, vvec->y / vsteps, vvec->z / vsteps);
  light->samples = usteps * vsteps;
  light->sequence = NULL;
  return light;
}

void LIGHTS_point_on_area_light(TUPLES_Point *dest, const LIGHTS_Light *light, unsigned int u, unsigned int v) {
  assert(dest);
  assert(light);
  TUPLES_Vector uvec_tmp, vvec_tmp;
  double uvec_offset = u + (light->sequence ? SEQUENCES_next(light->sequence) : 0.5);
  double vvec_offset = v + (light->sequence ? SEQUENCES_next(light->sequence) : 0.5);
  TUPLES_multiply(&uvec_tmp, &light->uvec, uvec_offset);
  TUPLES_multiply(&vvec_tmp, &light->vvec, vvec_offset);
  TUPLES_add(dest, &light->corner, &uvec_tmp);
  TUPLES_add(dest, dest, &vvec_tmp);
}

void LIGHTS_destroy(LIGHTS_Light *l) {
  assert(l);
  if (l->sequence) {
    SEQUENCES_delete(l->sequence);
  }
}

void LIGHTS_delete(LIGHTS_Light *l) {
  assert(l);
  LIGHTS_destroy(l);
  free(l);
}

void LIGHTS_set_jitter_on_area_light(LIGHTS_Light *light, SEQUENCES_Sequence *seq) {
  assert(light);
  assert(seq);
  if (light->sequence) {
    SEQUENCES_delete(light->sequence);
  }
  light->sequence = SEQUENCES_copy(seq);
}

struct sum_area_context {
  double total;
  const WORLD_World *world;
  const TUPLES_Point *point;
};

static void sum_area_intensity(TUPLES_Point *light_point, void *context) {
  assert(light_point);
  assert(context);
  struct sum_area_context *c = (struct sum_area_context *)context;
  if (!WORLD_is_shadowed(c->world, light_point, c->point)) {
    c->total += 1.0;
  }
}

double LIGHTS_intensity_at(const LIGHTS_Light *light, const TUPLES_Point *point, const WORLD_World *world) {
  assert(light);
  assert(point);
  assert(world);
  struct sum_area_context c = (struct sum_area_context){.point = point, .world = world, .total = 0.0};
  LIGHTS_iterate_points_on_light(light, sum_area_intensity, &c);
  return c.total / light->samples;
}

struct light_context {
  TUPLES_Point at, corner;
  TUPLES_Color intensity;
  TUPLES_Vector uvec, vvec;
  unsigned int usteps, vsteps;
  bool jitter;
  bool found_at, found_corner, found_intensity,
      found_uvec, found_vvec, found_usteps, found_vsteps,
      found_jitter;
};

static void parse_light_map_entry(char *key, char *value, void *context) {
  assert(context);
  struct light_context *lc = (struct light_context*)context;
  if (strcmp("at", key) == 0) {
    YAMLLOADER_parse_point(value, &lc->at);
    lc->found_at = true;
  } else if (strcmp("corner", key) == 0) {
    YAMLLOADER_parse_point(value, &lc->corner);
    lc->found_corner = true;
  } else if (strcmp("intensity", key) == 0) {
    YAMLLOADER_parse_color(value, &lc->intensity);
    lc->found_intensity = true;
  } else if (strcmp("uvec", key) == 0) {
    YAMLLOADER_parse_vector(value, &lc->uvec);
    lc->found_uvec = true;
  } else if (strcmp("vvec", key) == 0) {
    YAMLLOADER_parse_vector(value, &lc->vvec);
    lc->found_vvec = true;
  } else if (strcmp("jitter", key) == 0) {
    YAMLLOADER_parse_bool(value, &lc->jitter);
    lc->found_jitter = true;
  } else if (strcmp("usteps", key) == 0) {
    YAMLLOADER_parse_uint(value, &lc->usteps);
    lc->found_usteps = true;
  } else if (strcmp("vsteps", key) == 0) {
    YAMLLOADER_parse_uint(value, &lc->vsteps);
    lc->found_vsteps = true;
  } else {
    LOGGER_log(LOGGER_WARN, "Unrecognized map key while parsing light: %s\n", key);
  }
}

static bool validate_light_context(struct light_context *lc) {
  assert(lc);
  if (lc->found_at && lc->found_corner) {
    LOGGER_log(LOGGER_WARN, "Should not have at and corner defined for light\n");
    return false;
  }

  if (!lc->found_corner && !lc->found_at) {
    LOGGER_log(LOGGER_WARN, "Light must have either at or corner defined");
    return false;
  }

  if (!lc->found_intensity) {
    LOGGER_log(LOGGER_WARN, "Light must always define an intensity");
    return false;
  }

  if (lc->found_corner &&
      (!lc->found_vsteps || !lc->found_vvec ||
       !lc->found_usteps || !lc->found_uvec)) {
    LOGGER_log(LOGGER_WARN, "When defining area light - all of corner, vsteps, usteps, vvec, and uvec need to be defined");
    return false;
  }

  return true;
}

LIGHTS_Light *LIGHTS_parse_light(char *data) {
  assert(data);

  struct light_context lc = {0};

  YAMLLOADER_parse_map_entries(data, parse_light_map_entry, &lc);

  if (!validate_light_context(&lc)) {
    return NULL;
  }

  LIGHTS_Light *light = NULL;
  if (lc.found_at) {
    light = LIGHTS_new_pointlight(&lc.at, &lc.intensity);
  } else {
    light = LIGHTS_new_arealight(&lc.corner, &lc.uvec, lc.usteps, &lc.vvec, lc.vsteps, &lc.intensity);
    if (lc.found_jitter && lc.jitter) {
      SEQUENCES_Sequence* seq = SEQUENCES_new_random(4093);
      LIGHTS_set_jitter_on_area_light(light, seq);
      SEQUENCES_delete(seq);
    }
  }
  return light;
}
