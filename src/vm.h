#include <segment.h>
#include <vm_h.h>

/* Struct declarations, typedefs. */

struct vm {

  field_info char_info;
  field_info int_info;

  field_info rchar_info;

  /* Not statically known size. */
  segment_info * string_info;

  identifier last_identifier;
  };
