#define wtable_source

#include <wtable.h>

wtable_elem_interface wtable_interface = {
  &wtable_key_hash,
  &wtable_key_compare
  };

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

wtable_val_t method ( lookup_or_add ) (
    wtable_struct * self,
    wtable_key_t key,
    wval ( *on_missing ) ( wval key ),
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
