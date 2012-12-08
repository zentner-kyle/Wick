#include "wick_array.h"
#include <string.h>
#include "wick_alloc.h"
#include "wick_error.h"
#include "wick_globals.h"

#define WICK_DEFAULT_ARRAY_BYTE_SIZE (sizeof(wick_dyn) * 8)

wick_array * wick_array_init(wick_place * env, wick_array * self) {
	void * mem = wick_host_alloc(WICK_DEFAULT_ARRAY_BYTE_SIZE);
	if (!mem) {
		wick_signal_alloc_error(env);
		return NULL;
	}
	self->start = (wick_dyn *) mem;
	self->byte_length = WICK_DEFAULT_ARRAY_BYTE_SIZE;
	self->bytes_filled = 0;
	return self;
}

static uint64_t wick_array_next_size(uint64_t old_size) {
	return old_size * 2;
}

void wick_array_append(wick_place * env, wick_array * self, wick_dyn dynamic) {
	if (self->bytes_filled == self->byte_length) {
		/* The array needs to be resized. */
		uint64_t old_byte_length = self->byte_length; 
		uint64_t new_byte_length = wick_array_next_size(self->byte_length);
		void * new_mem = wick_host_alloc(new_byte_length);
		if (!new_mem) {
			wick_signal_alloc_error(env);
			return;
		}
		wick_dyn * old_start = self->start;
		memcpy(new_mem, self->start, old_byte_length);
		self->start = new_mem;
		self->byte_length = new_byte_length;
		wick_host_free(old_start);
	} 
	*(wick_dyn *)(self->bytes_filled + (void *) self->start) = dynamic;
	self->bytes_filled += sizeof(wick_dyn);
}

wick_dyn wick_array_index(wick_place * env, wick_array * self, uint64_t index) {
	if (index * sizeof(wick_dyn) > self->bytes_filled) {
		wick_place_signal_error(env, &wick_global.error.index);
		return wick_make_dyn(0, 0);
	} else {
		return self->start[index];
	}
}

wick_array_iter wick_array_new_iter(wick_place * env, wick_array * self) {
	return (wick_array_iter){self, 0};
}


bool wick_array_iter_valid(wick_place * env, wick_array_iter * self) {
	return self->byte_index < self->array->bytes_filled;
}

wick_dyn wick_array_iter_deref(wick_place * env, wick_array_iter * self) {
	return *((wick_dyn *) (((void *) self->array->start) + self->byte_index));
}

bool wick_array_iter_next(wick_place * env, wick_array_iter * self) {
	self->byte_index += sizeof(wick_dyn);
	return wick_array_iter_valid(env, self);
}
