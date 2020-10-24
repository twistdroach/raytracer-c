#include "csg.h"
#include <assert.h>

void CSG_bounds_of(const SHAPE_Shape *shape, BOUND_Box *box) {
  assert(shape);
  assert(box);
  CSG_Csg *csg = (CSG_Csg *)shape;
  BOUND_init(box);

  BOUND_Box left_box;
  SHAPE_parent_space_bounds_of(&left_box, csg->left);
  BOUND_add_box(box, &left_box);

  BOUND_Box right_box;
  SHAPE_parent_space_bounds_of(&right_box, csg->right);
  BOUND_add_box(box, &right_box);
}

bool CSG_shape_contains(const SHAPE_Shape *a, const SHAPE_Shape *b) {
  assert(a);
  assert(b);
  CSG_Csg *acsg = (CSG_Csg *)a;
  return acsg->left->vtable->contains(acsg->left, b) || acsg->right->vtable->contains(acsg->right, b);
}

void CSG_shape_divide(SHAPE_Shape *a_csg, unsigned int minimum_size) {
  assert(a_csg);
  CSG_Csg *csg = (CSG_Csg *)a_csg;
  SHAPE_divide(csg->left, minimum_size);
  SHAPE_divide(csg->right, minimum_size);
}

const SHAPE_vtable CSG_vtable = {&CSG_local_intersect, &CSG_delete_shape, &CSG_local_normal_at, &CSG_shape_contains, &CSG_bounds_of, &CSG_shape_divide};

bool CSG_intersection_allowed(CSG_Operation op, bool lhit, bool inl, bool inr) {
  switch (op) {
  case CSG_Union:
    return (lhit && !inr) || (!lhit && !inl);
  case CSG_Intersection:
    return (lhit && inr) || (!lhit && inl);
  case CSG_Difference:
    return (lhit && !inr) || (!lhit && inl);
  }
  assert(0); // should never get here
  return false;
}

RAY_Intersections *CSG_filter_intersections(const CSG_Csg *csg, const RAY_Intersections *intersections) {
  assert(csg);
  assert(intersections);

  bool inl = false;
  bool inr = false;

  RAY_Intersections *result = RAY_new_intersections();

  for (uint i = 0; i < intersections->count; i++) {
    const RAY_Xs *xs = &intersections->xs[i];
    bool lhit = csg->left->vtable->contains(csg->left, xs->object);

    if (CSG_intersection_allowed(csg->operation, lhit, inl, inr)) {
      RAY_add_intersection_tri(result, xs->t, xs->object, xs->u, xs->v);
    }

    if (lhit) {
      inl = !inl;
    } else {
      inr = !inr;
    }
  }

  return result;
}

CSG_Csg *CSG_new(CSG_Operation op, void *left, void *right) {
  assert(left);
  assert(right);
  CSG_Csg *csg = malloc(sizeof(CSG_Csg));
  if (!csg) {
    Throw(E_MALLOC_FAILED);
  }
  CSG_init(csg, op, left, right);
  return csg;
}

void CSG_init(CSG_Csg *csg, CSG_Operation op, void *left, void *right) {
  assert(csg);
  assert(left);
  assert(right);
  SHAPE_init(&csg->shape, &CSG_vtable);
  csg->left = left;
  SHAPE_set_parent(left, csg);
  csg->right = right;
  SHAPE_set_parent(right, csg);
  csg->operation = op;
  csg->bounds = NULL;
}

void CSG_destroy(CSG_Csg *csg) {
  assert(csg);
  SHAPE_delete_any_type(csg->left);
  SHAPE_delete_any_type(csg->right);
  SHAPE_destroy(&csg->shape);
  if (csg->bounds) {
    BOUND_delete(csg->bounds);
  }
}

void CSG_delete(CSG_Csg *csg) {
  assert(csg);
  CSG_destroy(csg);
  free(csg);
}

void CSG_delete_shape(SHAPE_Shape *shape) {
  assert(shape);
  CSG_Csg *csg = (CSG_Csg *)shape;
  CSG_delete(csg);
}

void CSG_local_normal_at(TUPLES_Vector *local_normal, SHAPE_Shape *csg, const TUPLES_Point *local_point, const RAY_Xs *hit) {
  assert(local_normal);
  assert(csg);
  assert(local_point);
  assert(hit);
  UNUSED(local_normal);
  UNUSED(csg);
  UNUSED(local_point);
  UNUSED(hit);
  assert(0);
}

void CSG_local_intersect(RAY_Intersections *intersections, SHAPE_Shape *shape, const RAY_Ray *local_ray) {
  assert(intersections);
  assert(shape);
  assert(local_ray);
  CSG_Csg *csg = (CSG_Csg *)shape;

  if (!csg->bounds) {
    csg->bounds = BOUND_new();
    CSG_bounds_of(shape, csg->bounds);
  }

  if (!BOUND_intersect(csg->bounds, local_ray)) {
    // we didn't hit the bounding box, so don't bother doing the below
    return;
  }

  RAY_Intersections *temp_xs = RAY_new_intersections();
  SHAPE_intersect(temp_xs, csg->left, local_ray);
  SHAPE_intersect(temp_xs, csg->right, local_ray);
  RAY_sort_intersections(temp_xs);
  RAY_Intersections *filtered_xs = CSG_filter_intersections(csg, temp_xs);
  RAY_add_intersections(intersections, filtered_xs);
  RAY_delete_intersections(temp_xs);
  RAY_delete_intersections(filtered_xs);
}
