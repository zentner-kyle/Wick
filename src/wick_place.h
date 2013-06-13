#ifndef WICK_PLACE_H
#define WICK_PLACE_H
#include <wick_basic.h>
#include <wick_dyn.h>

struct wick_assoc;
struct wick_array;
struct wick_channel;
struct wick_error;

struct wick_place {
  struct wick_channel * error_channel;
  struct wick_place * parent;
  struct wick_assoc * env;
  struct wick_array * channels;
  };

typedef struct wick_place wick_place;

wick_place * wick_place_new ( NOARGS );

/* This looks like a great place for future bugs. */
wick_place * wick_place_init ( wick_place * env, wick_place * self );

void wick_place_signal_error ( wick_place * self, struct wick_error * error );


#endif /* WICK_PLACE_H */
