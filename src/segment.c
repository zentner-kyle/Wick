#include <stdbool.h>
#include "segment.h"

/* const field_info char_info = FIELD_INFO(char, char, normal_field); */

const uint16_t normal_field = 0x0000;

const uint16_t repeated_field = 0x0001;

void rchar_init( vm * self ) {
}

void segment_init( vm * self ) {
	rchar_init( self );
}

bool last_field( field_info finfo ) {
	return finfo.size == 0;
}


size_t segment_info_size( segment_info * info ) {
	field_info * finfo = &info->fields;
	size_t total = 0;
	for ( int i = 0; ! last_field( finfo[i] ); i++ ) {
		total += finfo[i].size;
	}
	return total;
}
