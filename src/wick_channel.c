#include "wick_alloc.h"
#include "wick_basic.h"
#include "wick_channel.h"
#include "wick_place.h"

wick_channel * wick_channel_new(NOARGS) {
	return WICK_ALLOC(wick_channel);
}

void wick_channel_send(wick_place * env, wick_channel * channel, wick_dyn msg) {
	/* TODO */
}

wick_channel * wick_channel_init(wick_place * env, wick_channel * self) {
	/* TODO */
	return self;
}
