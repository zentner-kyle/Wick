#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include "segment.h"
#include "context.h"

void * walloc_bytes( context * ctxt, size_t num_bytes );

object * walloc_obj( context * ctxt, segment_info * info );

struct object {
  segment_info * info;
  /* Implicit extra space for fields. */
};

#define object_fields(object) \
  (((uint8_t *) object) + sizeof(object))

#endif /* OBJECT_H */
