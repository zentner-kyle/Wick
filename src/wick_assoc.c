#include <wick_assoc.h>
#include <wick_place.h>
#include <wick_thread.h>
#include <wick_globals.h>
#include <wick_dyn.h>
#include <wick_array.h>


wick_assoc * wick_assoc_init ( wick_place * env, wick_assoc * self ) {
  wick_array_init ( env, &self->array );
  return self;
  }

wick_assoc_iter * wick_assoc_iter_init ( wick_place * env, wick_assoc_iter * self, wick_assoc * assoc ) {
  wick_array_iter_init ( env, &self->iter, &assoc->array );
  return self;
  }

bool wick_assoc_iter_valid ( wick_place * env, wick_assoc_iter * self ) {
  return wick_array_iter_valid ( env, &self->iter );
  }

wick_dyn wick_assoc_iter_deref ( wick_place * env, wick_assoc_iter * self ) {
  return wick_array_iter_deref ( env, &self->iter );
  }

bool wick_assoc_iter_next ( wick_place * env, wick_assoc_iter * self ) {
  return wick_array_iter_next ( env, &self->iter );
  }

wick_dyn * wick_assoc_iter_key ( wick_place * env, wick_assoc_iter * self ) {
  return & ( ( wick_assoc_cell * )wick_assoc_iter_deref ( env, self ).obj )->key;
  }

wick_dyn * wick_assoc_iter_val ( wick_place * env, wick_assoc_iter * self ) {
  return & ( ( wick_assoc_cell * )wick_assoc_iter_deref ( env, self ).obj )->key;
  }
