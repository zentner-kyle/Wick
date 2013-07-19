#ifndef WTABLE_H
#define WTABLE_H
#include <stdbool.h>
#include <stdlib.h>
#include <wcall.h>
#include <wmacros.h>
#include <wtype_h.h>
#include <whash.h>
#include <wval.h>
#include <werror.h>

extern werror wtable_not_found;

def_struct ( wtable_elem_interface ) {
  whash_func_t * hash;
  whash_compare_func_t * compare;
  };

def_struct ( wtable_bucket ) {
  wval key;
  wval value;
  wtable_bucket * next;
  whash hash;
  };

def_struct ( wtable ) {
  wtype * type;
  wtable_elem_interface * interface;
  wtype * key_type;
  wtype * val_type;
  size_t mask;
  size_t num_elems;
  size_t space;
  wtable_bucket * data;
  };

wtable * wtable_new (
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface
    );

werror * wtable_init_to_size (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    size_t predicted_elems,
    wtable_elem_interface * hash_interface );

werror * wtable_init (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface );

werror * wtable_get ( wtable * self, wval key, wval * dst );
werror * wtable_get_hash ( wtable * self, wval key, wval * dst, whash hash );


werror * wtable_get_or_add (
    wtable * self,
    wval key,
    wval * dst,
    wcall * on_missing );

werror * wtable_get_or_add_hash (
    wtable * self,
    wval key,
    wval * dst,
    wcall * on_missing,
    whash hash );

werror * wtable_get_default (
    wtable * self,
    wval key,
    wval * dst,
    wval default_value );
werror * wtable_get_default_hash (
    wtable * self,
    wval key,
    wval * dst,
    wval default_value,
    whash hash );

werror * wtable_set (
    wtable * self,
    wval key,
    wval value );
werror * wtable_set_hash (
    wtable * self,
    wval key,
    wval value,
    whash hash );

werror * wtable_delete ( wtable * self );


#endif /* end of include guard: WTABLE_H */
