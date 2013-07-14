#define wtable_source

#include <wtable.h>

wtable_elem_interface wtable_interface = {
  &wtable_key_hash,
  &wtable_key_compare
  };

wtable_struct * method ( new ) ( ) {
  return (wtable_struct *) wtable_new (
      wtable_key_wtype,
      wtable_val_wtype,
      &wtable_interface );
  }

bool method ( init_to_size ) (
    wtable_struct * self,
    size_t predicted_elems,
    wcall * on_error
  ) {
  return wtable_init_to_size (
      wtable_struct_to_table ( self ),
      wtable_key_wtype,
      wtable_val_wtype,
      predicted_elems,
      &wtable_interface,
      on_error );
  }

bool method ( init ) (
    wtable_struct * self,
    wcall * on_error
  ) {
  return wtable_init (
      wtable_struct_to_table ( self ),
      wtable_key_wtype,
      wtable_val_wtype,
      &wtable_interface,
      on_error );
  }

wtable_val_t method ( lookup ) ( wtable_struct * self, wtable_key_t key ) {
  return wtable_backcast_val ( wtable_lookup_hash (
      wtable_struct_to_table ( self ),
      wtable_cast_key ( key ),
      wtable_key_hash ( wtable_cast_key ( key ) )
      ) );
  }

werror * method ( get ) ( wtable_struct * self, wtable_key_t key, wtable_val_t * dst ) {
  return wtable_get_hash (
      wtable_struct_to_table ( self ),
      wtable_cast_key ( key ),
      ( wval * ) dst,
      wtable_key_hash ( wtable_cast_key ( key ) )
      );
  }

wtable_val_t method ( lookup_or_add ) (
    wtable_struct * self,
    wtable_key_t key,
    wcall * on_missing,
    wcall * on_error
  ) {
  return wtable_backcast_val ( wtable_lookup_or_add_hash (
      wtable_struct_to_table ( self ),
      wtable_cast_key ( key ),
      on_missing,
      on_error,
      wtable_key_hash ( wtable_cast_key ( key ) )
      ) );
  }

wtable_val_t method ( lookup_default ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t default_value,
    wcall * on_error
  ) {
  return wtable_backcast_val ( wtable_lookup_default_hash (
      wtable_struct_to_table ( self ),
      wtable_cast_key ( key ),
      wtable_cast_val ( default_value ),
      on_error,
      wtable_key_hash ( wtable_cast_key ( key ) )
      ) );
  }

void method ( set ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t val,
    wcall * on_error
  ) {
  wtable_set_hash (
      wtable_struct_to_table ( self ),
      wtable_cast_key ( key ),
      wtable_cast_val ( val ),
      on_error,
      wtable_key_hash ( wtable_cast_key ( key ) )
      );
  }

void method ( delete ) ( wtable_struct * self ) {
  wtable_delete ( wtable_struct_to_table ( self ) );
  }


#undef wtable_backcast_key
#undef wtable_backcast_val
#undef wtable_cast_key
#undef wtable_cast_val
#undef wtable_interface
#undef wtable_key_compare
#undef wtable_key_hash
#undef wtable_key_kind
#undef wtable_key_name
#undef wtable_key_t
#undef wtable_key_wtype
#undef wtable_name
#undef wtable_source
#undef wtable_struct
#undef wtable_struct_to_table
#undef wtable_val_kind
#undef wtable_val_name
#undef wtable_val_t
#undef wtable_val_wtype
#undef method
#undef wtable_no_struct
