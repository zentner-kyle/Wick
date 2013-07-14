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

bool wtable_init_to_size (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    size_t predicted_elems,
    wtable_elem_interface * hash_interface,
    wcall * on_error );

bool wtable_init (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface,
    wcall * on_error );

wval wtable_lookup ( wtable * self, wval key );
wval wtable_lookup_hash ( wtable * self, wval key, whash hash );

werror * wtable_gt ( wtable * self, wval key, wval * dst );
werror * wtable_get_hash ( wtable * self, wval key, wval * dst, whash hash );


wval wtable_lookup_or_add (
    wtable * self,
    wval key,
    wcall * on_missing,
    wcall * on_error );

wval wtable_lookup_or_add_hash (
    wtable * self,
    wval key,
    wcall * on_missing,
    wcall * on_error,
    whash hash );

wval wtable_lookup_default (
    wtable * self,
    wval key,
    wval default_value,
    wcall * on_error );
wval wtable_lookup_default_hash (
    wtable * self,
    wval key,
    wval default_value,
    wcall * on_error,
    whash hash );

void wtable_delete ( wtable * self );

void wtable_set (
    wtable * self,
    wval key,
    wval value,
    wcall * on_error );
void wtable_set_hash (
    wtable * self,
    wval key,
    wval value,
    wcall * on_error,
    whash hash );


#endif /* end of include guard: WTABLE_H */
