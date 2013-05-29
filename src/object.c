#include "object.h"

void * walloc_bytes( context * ctxt, size_t num_bytes ) {
	return malloc(num_bytes);
}

object * walloc_obj( context * ctxt, segment_info * info ) {
	size_t to_alloc = segment_info_size( info );
	object * obj = (object *) walloc_bytes( ctxt, to_alloc );
	if ( ! obj ) {
		alloc_error( ctxt );
		return NULL;
	}
	uint8_t * ptr = object_fields(obj);
	field_info * finfo = &info->fields;
	obj->info = info;
	for ( int i = 0; ! last_field( finfo[i] ); i++ ) {
		/* init_field( self, finfo + i, (void *) ptr ); */
		ptr += finfo[i].size;
	}
	return obj;
}
