#ifndef WICK_EQUAL_H
#define WICK_EQUAL_H
#include <assert.h>
#include <string.h>
#include "wick_equal.h"
#include "wick_dyn.h"
#include "wick_globals.h"
#include "wick_string.h"
#include "wick_array.h"
#include "wick_assoc.h"



static bool wick_string_equal(wick_string a, wick_string b) {
  /* Check both strings are the same length. */
  size_t length_a = a.past_end - a.start;
  size_t length_b = b.past_end - b.start;
  if (length_a != length_b) {
    return false;
  } else {
    /* Is const void * portable? Might need to extract this to a header. */
    return memcmp((const void *)a.start, (const void *)b.start, length_a);
  }
}

static bool wick_error_equal(wick_error a, wick_error b) {
  return a.error_num == b.error_num;
}

static bool wick_array_equal(wick_place * env, wick_array a, wick_array b) {
  if (a.bytes_filled != b.bytes_filled) {
    return false;
  } else if (a.start == b.start) {
    return true;
  } else if (!a.start || !b.start) {
    return false;
  }
  wick_dyn * a_end = a.bytes_filled + (void *)a.start;
  while (a.start != a_end) {
    if (wick_equal(env, *a.start, *b.start)) {
      ++a.start;
      ++b.start;
    } else {
      return false;
    }
  }
  return true;
}

static bool wick_assoc_cell_equal(wick_place * env,
                                  wick_assoc_cell a,
                                  wick_assoc_cell b) {
  return wick_equal(env, a.key, b.key) && wick_equal(env, a.val, b.val);
}

static bool wick_assoc_equal(wick_place * env, wick_assoc a,
                             wick_assoc b) {
  return wick_array_equal(env, a.array, b.array);
  /* if (a.array.bytes_filled != b.array.bytes_filled) { */
  /*   return false; */
  /* } else if (a.start == b.start) { */
  /*   return true; */
  /* } */
  /* wick_assoc_iter ai; */
  /* wick_assoc_iter_init(env, &ai, &a); */
  /* wick_assoc_iter bi; */
  /* wick_assoc_iter_init(env, &bi, &b); */
  /* do { */
  /*   if (!wick_equal(env, */
  /*                   wick_assoc_iter_deref(env, &ai), */
  /*                   wick_assoc_iter_deref(env, &bi))) { */
  /*     return false; */
  /*   } */
  /*   wick_assoc_iter_next(env, &bi); */
  /* } while (wick_assoc_iter_next(env, &ai)); */
  /* assert(!wick_assoc_iter_valid(env, &bi)); */
  /* return true; */
}

bool wick_equal(wick_place * env, wick_dyn a, wick_dyn b) {
  if (a.type != b.type) {
    return false;
  } else if (a.type == &wick_global.type._error) {
    return wick_error_equal(*(wick_error *)a.obj, *(wick_error *)b.obj);
  } else if (a.type == &wick_global.type._int) {
    return *(int *)a.obj == *(int *)b.obj;
  } else if (a.type == &wick_global.type._double) {
    return *(double *)a.obj == *(double *)b.obj;
  } else if (a.type == &wick_global.type._dyn) {
    return wick_equal(env, *(wick_dyn *)a.obj, *(wick_dyn *)b.obj);
  } else if (a.type == &wick_global.type._assoc_cell) {
    return wick_assoc_cell_equal(env, *(wick_assoc_cell *)a.obj, *(wick_assoc_cell *)b.obj);
  } else if (a.type == &wick_global.type._assoc) {
    return wick_assoc_equal(env, *(wick_assoc *)a.obj, *(wick_assoc *)b.obj);
  } else if (a.type == &wick_global.type._string) {
    return wick_string_equal(*(wick_string *)a.obj, *(wick_string *)b.obj);
  } else if (a.type == &wick_global.type._array) {
    return wick_array_equal(env, *(wick_array *)a.obj, *(wick_array *)b.obj);
  }
  return false;
}

#endif /* WICK_EQUAL_H */
