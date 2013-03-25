#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "vm_h.h"

/* Struct declarations, typedefs. */

struct object;
typedef struct object object;

struct identifier;
typedef struct identifier identifier;

typedef identifier segment_id;

struct field_info;
typedef struct field_info field_info;

struct segment_info;
typedef struct segment_info segment_info;


/* Global variables. */

const extern uint16_t normal_field;
const extern uint16_t repeated_field;

/* Global functions. */

size_t segment_info_size( segment_info * );
bool last_field( field_info finfo );


/* Struct definitions. */

struct identifier {
	unsigned long long index;
};


struct field_info {
	object * name;
	size_t size;
	uint16_t kind;
};


struct segment_info {
	segment_id id;
	field_info fields;
	/* All fields are the same space and type, so &fields + index calculates their location. */
};

#endif /* SEGMENT_H */
