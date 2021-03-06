#define _GNU_SOURCE

#ifdef __NetBSD__
#define _OPENBSD_SOURCE
#endif

#include "ray.h"
#include "arrlist.h"
#include <CException.h>
#include <assert.h>
#include <exceptions.h>
#include <math.h>
#include <shape.h>
#include <stdlib.h>

RAY_Ray *RAY_new(double origin_x, double origin_y, double origin_z, double direction_x, double direction_y, double direction_z) {
  RAY_Ray *ray = malloc(sizeof(RAY_Ray));
  if (!ray)
    Throw(E_MALLOC_FAILED);

  RAY_init(ray, origin_x, origin_y, origin_z, direction_x, direction_y, direction_z);
  return ray;
}

RAY_Ray *RAY_new_from_tuples(const TUPLES_Point *origin, const TUPLES_Vector *direction) {
  return RAY_new(origin->x, origin->y, origin->z, direction->x, direction->y, direction->z);
}

void RAY_init(RAY_Ray *ray, double origin_x, double origin_y, double origin_z, double direction_x, double direction_y, double direction_z) {
  assert(ray);
  TUPLES_init_point(&ray->origin, origin_x, origin_y, origin_z);
  TUPLES_init_vector(&ray->direction, direction_x, direction_y, direction_z);
}

void RAY_init_from_tuples(RAY_Ray *ray, const TUPLES_Point *origin, const TUPLES_Vector *direction) {
  RAY_init(ray, origin->x, origin->y, origin->z, direction->x, direction->y, direction->z);
}

void RAY_position(TUPLES_Point *pos, const RAY_Ray *ray, double t) {
  assert(pos);
  assert(ray);
  TUPLES_Vector direction_t;
  TUPLES_init_vector(&direction_t, 0, 0, 0);
  TUPLES_multiply(&direction_t, &ray->direction, t);
  TUPLES_add(pos, &ray->origin, &direction_t);
  TUPLES_destroy(&direction_t);
}

void RAY_transform(RAY_Ray *dest, const RAY_Ray *orig, const MATRIX_Matrix *matrix) {
  assert(dest);
  assert(orig);
  assert(matrix);
  assert(MATRIX_is_invertible(matrix));
  MATRIX_multiply_tuple(&dest->origin, matrix, &orig->origin);
  MATRIX_multiply_tuple(&dest->direction, matrix, &orig->direction);
}

void RAY_destroy(RAY_Ray *ray) {
  assert(ray);
  TUPLES_destroy(&ray->origin);
  TUPLES_destroy(&ray->direction);
}

void RAY_delete(RAY_Ray *ray) {
  assert(ray);
  RAY_destroy(ray);
  free(ray);
}

RAY_Intersections *RAY_new_intersections() {
  RAY_Intersections *intersections = malloc(sizeof(RAY_Intersections));
  if (!intersections)
    Throw(E_MALLOC_FAILED);

  intersections->count = 0;

  const size_t default_size = 100;
  intersections->xs = malloc(sizeof(RAY_Xs) * default_size);
  intersections->xs_size = default_size;

  return intersections;
}

static int compare_RAY_Xs(const void *a, const void *b) {
  double ad = ((RAY_Xs *)a)->t;
  double bd = ((RAY_Xs *)b)->t;
  if (ad > bd) {
    return 1;
  } else if (ad < bd) {
    return -1;
  } else {
    return 0;
  }
}

void RAY_sort_intersections(RAY_Intersections *intersections) { qsort(intersections->xs, intersections->count, sizeof(RAY_Xs), compare_RAY_Xs); }

RAY_Xs *RAY_hit(RAY_Intersections *intersections) {
  // TODO - improve this by just keeping a ptr to the smallest positive value on insertion
  for (uint ndx = 0; ndx < intersections->count; ndx++) {
    // TODO double_equal because our epsilon is less accurate than >, need to not detect small t values as hits
    if (intersections->xs[ndx].t > 0 && !double_equal(intersections->xs[ndx].t, 0.0)) {
      return &intersections->xs[ndx];
    }
  }
  return NULL;
}

RAY_Xs *RAY_shadow_hit(RAY_Intersections *intersections) {
  for (uint ndx = 0; ndx < intersections->count; ndx++) {
    if (intersections->xs[ndx].t > 0 && !double_equal(intersections->xs[ndx].t, 0.0) &&
        MATERIAL_casts_shadow(SHAPE_get_material(intersections->xs[ndx].object))) {
      return &intersections->xs[ndx];
    }
  }
  return NULL;
}

void RAY_iterate_intersections(RAY_Intersections *intersections, void (*intersection_iter)(RAY_Xs *ray, void *state), void *state) {
  assert(intersections);
  assert(intersection_iter);
  for (unsigned int ndx = 0; ndx < intersections->count; ndx++) {
    intersection_iter(&intersections->xs[ndx], state);
  }
}

void RAY_add_intersections(RAY_Intersections *dest_intersection, RAY_Intersections *src_intersections) {
  assert(dest_intersection);
  assert(src_intersections);
  for (unsigned int ndx = 0; ndx < src_intersections->count; ndx++) {
    RAY_Xs *xs = &src_intersections->xs[ndx];
    RAY_add_intersection(dest_intersection, xs->t, xs->object);
  }
}

void RAY_add_intersection(RAY_Intersections *intersections, double intersection, void *object) {
  assert(intersections);
  assert(object);
  if (intersections->count > intersections->xs_size) {
    RAY_Xs *tmpptr = realloc(intersections->xs, sizeof(RAY_Xs) * intersections->xs_size * 2);
    if (!tmpptr) {
      Throw(E_MALLOC_FAILED);
    } else {
      intersections->xs = tmpptr;
      intersections->xs_size *= 2;
    }
  }
  intersections->xs[intersections->count].t = intersection;
  intersections->xs[intersections->count].object = object;
  intersections->count++;
}

void RAY_add_intersection_tri(RAY_Intersections *intersections, double intersection, void *object, double u, double v) {
  RAY_add_intersection(intersections, intersection, object);
  intersections->xs[intersections->count - 1].u = u;
  intersections->xs[intersections->count - 1].v = v;
}

void RAY_delete_intersections(RAY_Intersections *intersections) {
  assert(intersections);
  if (intersections->xs) {
    free(intersections->xs);
  }
  free(intersections);
}

RAY_Computations *RAY_prepare_computations(const RAY_Xs *hit, const RAY_Ray *ray, const RAY_Intersections *xs) {
  assert(hit);
  assert(ray);
  assert(xs);
  RAY_Computations *comps = malloc(sizeof(RAY_Computations));
  // keep a handle on the object intersected
  comps->t = hit->t;
  comps->object = hit->object;

  // compute the point of intersection
  RAY_position(&comps->point, ray, comps->t);

  // compute the eye vector
  TUPLES_copy(&comps->eyev, &ray->direction);
  TUPLES_negate(&comps->eyev);

  // compute the normal @ the intersection
  SHAPE_normal_at(&comps->normalv, comps->object, &comps->point, hit);

  // Invert normal if inside
  if (TUPLES_dot(&comps->normalv, &comps->eyev) < 0) {
    comps->inside = true;
    TUPLES_negate(&comps->normalv);
  } else {
    comps->inside = false;
  }

  // compute the "over_point" to deal with floating point imprecision...
  TUPLES_multiply(&comps->over_point, &comps->normalv, EPSILON);
  TUPLES_add(&comps->over_point, &comps->over_point, &comps->point);

  // compute the "under_point"
  TUPLES_multiply(&comps->under_point, &comps->normalv, EPSILON);
  TUPLES_subtract(&comps->under_point, &comps->point, &comps->under_point);

  TUPLES_reflect(&comps->reflectv, &ray->direction, &comps->normalv);

  // Find
  ARRLIST_List *object_list = ARRLIST_new();
  for (unsigned int ndx = 0; ndx < xs->count; ndx++) {
    if (&xs->xs[ndx] == hit) {
      if (ARRLIST_is_empty(object_list)) {
        comps->n1 = 1.0;
      } else {
        comps->n1 = SHAPE_get_material((SHAPE_Shape *)ARRLIST_last(object_list))->refractive_index;
      }
    }

    if (ARRLIST_contains(object_list, xs->xs[ndx].object)) {
      ARRLIST_remove(object_list, xs->xs[ndx].object);
    } else {
      ARRLIST_add(object_list, xs->xs[ndx].object);
    }

    if (&xs->xs[ndx] == hit) {
      if (ARRLIST_is_empty(object_list)) {
        comps->n2 = 1.0;
      } else {
        comps->n2 = SHAPE_get_material((SHAPE_Shape *)ARRLIST_last(object_list))->refractive_index;
      }
      break;
    }
  }
  ARRLIST_delete(object_list);

  return comps;
}

void RAY_delete_computations(RAY_Computations *comps) { free(comps); }

double RAY_schlick(const RAY_Computations *comps) {
  assert(comps);
  double cos = TUPLES_dot(&comps->eyev, &comps->normalv);
  if (comps->n1 > comps->n2) {
    double n = comps->n1 / comps->n2;
    double sin2_t = pow(n, 2) * (1.0 - pow(cos, 2));
    if (sin2_t > 1.0) {
      return 1.0;
    }
    // when n1>n2 use cos(theta_t) instead
    cos = sqrt(1.0 - sin2_t);
  }
  double r0 = pow((comps->n1 - comps->n2) / (comps->n1 + comps->n2), 2);
  return r0 + (1 - r0) * pow(1 - cos, 5);
}
