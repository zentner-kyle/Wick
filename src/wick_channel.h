#ifndef WICK_CHANNEL_H
#define WICK_CHANNEL_H
#include "wick_dyn.h"

struct wick_array;
struct wick_place;

struct wick_channel {
	struct wick_array * queue;
};

typedef struct wick_channel wick_channel;

wick_channel * wick_channel_new(void);

void wick_channel_send(struct wick_place * env, wick_channel * channel, wick_dyn msg);

wick_channel * wick_channel_init(struct wick_place * env, wick_channel * self);


#endif /* WICK_CHANNEL_H */
