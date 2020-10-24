#include "lights.h"
#include "world.h"
#include <assert.h>
#include <exceptions.h>

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
