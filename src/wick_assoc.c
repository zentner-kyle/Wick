#include "wick_assoc.h"
#include "wick_place.h"
#include "wick_thread.h"
#include "wick_globals.h"
#include "wick_dyn.h"


wick_assoc * wick_assoc_init(wick_place * env, wick_assoc * self) {
	self->start = 0;
	self->byte_length = 0;
	self->bytes_filled = 0;
	return self;
}

wick_assoc_iter * wick_assoc_iter_init(wick_place * env, wick_assoc_iter * self, wick_assoc * assoc) {
	self->assoc = assoc;
	self->byte_index = 0;
	return self;
}

bool wick_assoc_iter_valid(wick_place * env, wick_assoc_iter * self) {
	return self->byte_index <= self->assoc->bytes_filled;
}

wick_dyn wick_assoc_iter_deref(wick_place * env, wick_assoc_iter * self) {
	return wick_make_dyn(self->byte_index + (void *)self->assoc->start, &wick_global.type._assoc_cell);
}

bool wick_assoc_iter_next(wick_place * env, wick_assoc_iter * self) {
	++self->byte_index;
	return wick_assoc_iter_valid(env, self);
}

wick_dyn * wick_assoc_iter_key(wick_place * env, wick_assoc_iter * self) {
	return &((wick_assoc_cell *)wick_assoc_iter_deref(env, self).obj)->key;
}

wick_dyn * wick_assoc_iter_val(wick_place * env, wick_assoc_iter * self) {
	return &((wick_assoc_cell *)wick_assoc_iter_deref(env, self).obj)->key;
}
