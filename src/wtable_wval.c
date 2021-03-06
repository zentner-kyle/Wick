#include <assert.h>
#include <string.h>
#include <wtable_wval.h>
#include <wbits.h>
#include <walloc.h>
#include <wstr.h>
#include <wval.h>
#include <wtype.h>

werror wtable_not_found;

bool wtable_needs_grow ( wtable * self );

bool wtable_needs_shrink ( wtable * self );

werror * wtable_grow ( wtable * restrict self );

#define SELECT_BUCKET( \
    macro_self, \
    macro_key, \
    macro_hashed, \
    macro_buckets, \
    macro_when_first, \
    macro_when_not_first, \
    macro_when_missing_first, \
    macro_when_missing_not_first ) \
  do { \
    const whash hashed = macro_hashed; \
    wtable_bucket * macro_the_bucket = ( macro_buckets ) + ( ( hashed ) & ( macro_self )->mask ); \
    if ( ! macro_the_bucket->hash ) { \
      macro_when_missing_first ( macro_the_bucket ); \
      } \
    else if ( macro_self->interface->compare ( macro_the_bucket->key, macro_key ) == 0 ) { \
      macro_when_first ( macro_the_bucket ); \
      } \
    else { \
      while ( macro_the_bucket->next ) { \
        macro_the_bucket = macro_the_bucket->next; \
        if ( macro_self->interface->compare ( macro_the_bucket->key, macro_key ) == 0 ) { \
          macro_when_not_first ( macro_the_bucket ); \
          } \
        } \
      macro_when_missing_not_first ( macro_the_bucket ); \
      } \
    } while ( 0 )

const size_t wtable_default_size = 8;

size_t wtable_count_elems ( wtable * self ) {
  int out = 0;
  for ( int i = 0; i < self->space; i++ ) {
    wtable_bucket * b = self->data + i;
    while ( b ) {
      if (b->hash) {
        out += 1;
        }
      b = b->next;
      }
    }
  return out;
  }

werror * wtable_init_to_size (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    size_t predicted_elems,
    wtable_elem_interface * hash_interface
    ) {
  assert ( self );
  predicted_elems = round_up_power_2 ( predicted_elems );
  self->key_type = key_type;
  self->val_type = val_type;
  self->interface = hash_interface;
  self->mask = mask_of_pow2 ( predicted_elems );
  self->space = predicted_elems;
  self->num_elems = 0;
  self->data = walloc_simple ( wtable_bucket, predicted_elems );
  if ( self->data == NULL ) {
    return &wick_out_of_memory;
    }
  else {
    memset ( ( void * ) self->data, 0, predicted_elems * sizeof ( wtable_bucket ) );
    return w_ok;
    }
  }

werror * wtable_init (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface
    ) {
  return wtable_init_to_size (
      self,
      key_type,
      val_type,
      wtable_default_size,
      hash_interface );
  }

wtable * wtable_new (
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface
  ) {
  wtable * self = walloc_simple ( wtable, 1 );
  if ( ! self ) {
    return NULL;
    }
  if ( wtable_init ( self,
                     key_type,
                     val_type,
		     hash_interface ) == w_ok ) {
    return self;
    }
  else {
    free ( self );
    return NULL;
    }
  }

werror * wtable_get ( wtable * self, wval key, wval * dst ) {
  return wtable_get_hash ( self, key, dst, self->interface->hash ( key ) );
  }
werror * wtable_get_hash ( wtable * self, wval key, wval * dst, whash hash ) {
  #define SET_DST( bucket ) \
    *dst = bucket->value; \
    return w_ok;
  #define RETURN_ERROR( bucket ) \
    return &wtable_not_found;
  SELECT_BUCKET ( self,
                  key,
                  hash,
                  self->data,
                  SET_DST,
                  SET_DST,
                  RETURN_ERROR,
                  RETURN_ERROR );
  #undef SET_DST
  #undef RETURN_ERROR
  }

werror * wtable_get_or_add (
    wtable * self,
    wval key,
    wval * dst,
    wcall * on_missing
    ) {
  return wtable_get_or_add_hash (
      self,
      key,
      dst,
      on_missing,
      self->interface->hash ( key ) );
  }
werror * wtable_get_or_add_hash (
    wtable * self,
    wval key,
    wval * dst,
    wcall * on_missing,
    whash hash
    ) {
    werror * e = w_ok;
  #define RETURN_FOUND( bucket ) \
    *dst = ( bucket )->value; \
    return w_ok;
  #define ADD_FIRST( bucket ) \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    wcall_push ( on_missing, self->key_type->ptr_of, (wval) { .pointer = ( wobj * ) &( bucket )->key } ); \
    wcall_push ( on_missing, self->val_type->ptr_of, (wval) { .pointer = ( wobj * ) &( bucket )->value } ); \
    if ( winvoke ( on_missing ) != w_ok ) { \
      wval macro_value = ( bucket )->value; \
      ( bucket )->hash = 0; \
      *dst = macro_value; \
      return w_ok; \
      } \
    self->num_elems++; \
    if ( wtable_needs_grow ( self ) ){ \
      e = wtable_grow ( self ); \
      } \
    *dst = ( bucket )->value; \
    return e;
  #define ADD_NOT_FIRST( bucket ) \
    wtable_bucket * new_bucket = walloc_simple ( wtable_bucket, 1 ); \
    if ( new_bucket == NULL ) { \
      return &wick_out_of_memory; \
      } \
    ( bucket )->next = new_bucket;  \
    new_bucket->next = NULL; \
    ( new_bucket )->hash = hashed; \
    ( new_bucket )->key = key; \
    wcall_push ( on_missing, self->key_type->ptr_of, (wval) { .pointer = ( wobj * ) &( new_bucket )->key } ); \
    wcall_push ( on_missing, self->val_type->ptr_of, (wval) { .pointer = ( wobj * ) &( new_bucket )->value } ); \
    e = winvoke ( on_missing ); \
    if ( e != w_ok ) { \
      free ( new_bucket ); \
      ( bucket )->next = NULL; \
      return e; \
      } \
    self->num_elems++; \
    *dst = ( new_bucket )->value; \
    if ( wtable_needs_grow ( self ) ){ \
      return wtable_grow ( self ); \
      } \
    return w_ok;
  SELECT_BUCKET (
      self,
      key,
      hash,
      self->data,
      RETURN_FOUND,
      RETURN_FOUND,
      ADD_FIRST,
      ADD_NOT_FIRST );
      #undef ADD_NOT_FIRST
  }

/* Note that these two functions share most of their implementations. */

werror * wtable_get_default (
    wtable * self,
    wval key,
    wval * dst,
    wval default_value
    ) {
  return wtable_get_default_hash (
      self,
      key,
      dst,
      default_value,
      self->interface->hash ( key ) );
  }

werror * wtable_get_default_hash (
    wtable * self,
    wval key,
    wval * dst,
    wval default_value,
    whash hash
    ) {
  #undef ADD_FIRST
  #define ADD_FIRST( bucket ) \
    self->num_elems++; \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    *dst = ( bucket )->value = default_value; \
    if ( wtable_needs_grow ( self ) ){ \
      return wtable_grow ( self ); \
      } \
    return w_ok;
  #define ADD_NOT_FIRST( bucket ) \
    wtable_bucket * new_bucket = walloc_simple ( wtable_bucket, 1 ); \
    if ( new_bucket == NULL ) { \
      return &wick_out_of_memory; \
      } \
    ( bucket )->next = new_bucket;  \
    new_bucket->next = NULL; \
    ADD_FIRST ( new_bucket );
  SELECT_BUCKET (
      self,
      key,
      hash,
      self->data,
      RETURN_FOUND,
      RETURN_FOUND,
      ADD_FIRST,
      ADD_NOT_FIRST );
  #undef RETURN_FOUND
  }

werror * wtable_set (
    wtable * self,
    wval key,
    wval value
    ) {
  return wtable_set_hash (
      self,
      key,
      value,
      self->interface->hash ( key ) );
  }
werror * wtable_set_hash (
    wtable * self,
    wval key,
    wval value,
    whash hash
    ) {
  #undef ADD_FIRST
  #define ADD_FIRST( bucket ) \
    self->num_elems++; \
    assert ( bucket ); \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    ( bucket )->value = value; \
    if ( wtable_needs_grow ( self ) ){ \
      return wtable_grow ( self ); \
      } \
    return w_ok;
  #define SET( bucket ) \
    ( bucket )->value = value; \
    return w_ok;
  SELECT_BUCKET (
      self,
      key,
      hash,
      self->data,
      SET,
      SET,
      ADD_FIRST,
      ADD_NOT_FIRST );
  #undef ADD_NOT_FIRST
  #undef SET
  }

bool wtable_needs_grow ( wtable * self ) {
  return self->num_elems >= (self->space >> 1);
  }

bool wtable_needs_shrink ( wtable * self ) {
  return self->num_elems <= (self->space >> 2);
  }

werror * wtable_delete ( wtable * self ) {
  if ( self ) {
    free ( self->data );
    }
  free ( self );
  return w_ok;
  }

werror * wtable_grow ( wtable * restrict self ) {
  wtable_bucket * new_buckets = walloc_simple ( wtable_bucket, self->space << 1 );
  if ( new_buckets == NULL ) {
    return &wick_out_of_memory;
    }
  memset ( ( void * ) new_buckets, 0, (self->space << 1) * sizeof ( wtable_bucket ) );
  wtable_bucket * old_bucket = self->data;
  self->mask = (self->mask << 1) | 1;
  for ( int i = 0; i < self->space; i++ ) {
    old_bucket = self->data + i;
    if ( old_bucket->hash ) {
      assert ( old_bucket->hash != 1 );
      #define IGNORE( bucket ) {}
      #define IMPOSSIBLE( bucket ) assert(0);
      #define MOVE_FIRST( bucket ) \
        memcpy ( bucket, old_bucket, sizeof ( wtable_bucket ) ); \
        ( bucket )->next = 0;
      SELECT_BUCKET (
          self,
          old_bucket->key,
          old_bucket->hash,
          new_buckets,
          IMPOSSIBLE,
          IMPOSSIBLE,
          MOVE_FIRST,
          IMPOSSIBLE );
      old_bucket = old_bucket->next;
      while ( old_bucket ) {
        #define MOVE_AND_FREE( bucket ) \
          memcpy ( bucket, old_bucket, sizeof ( wtable_bucket ) ); \
          free ( old_bucket ); \
          old_bucket = ( bucket )->next; \
          ( bucket )->next = 0;
        #define RELINK( bucket ) \
          ( bucket )->next = old_bucket; \
          old_bucket = old_bucket->next; \
          ( bucket )->next->next = 0;
        SELECT_BUCKET (
            self,
            old_bucket->key,
            old_bucket->hash,
            new_buckets,
            IMPOSSIBLE,
            IMPOSSIBLE,
            MOVE_AND_FREE,
            RELINK );
        }
      }
    }
  self->space <<= 1;
  free ( self->data );
  self->data = new_buckets;
  assert ( self->num_elems == wtable_count_elems ( self ) );
  return w_ok;
  }
