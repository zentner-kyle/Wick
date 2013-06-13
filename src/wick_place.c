#include <wick_place.h>
#include <wick_assoc.h>
#include <wick_array.h>
#include <wick_alloc.h>
#include <wick_channel.h>
#include <wick_globals.h>
#include <wick_dyn.h>

wick_place * wick_place_new ( NOARGS ) {
  return WICK_ALLOC ( wick_place );
  }

wick_place * wick_place_init ( wick_place * env, wick_place * self ) {
  if ( self ) {
    self->parent = env;
    self->error_channel = wick_channel_new ( );
    if ( self->error_channel ) {
      wick_channel_init ( env, self->error_channel );
      }
    else {
      if ( env ) {
        wick_place_signal_error ( env, &wick_global.error.alloc );
        wick_host_free ( self );
        }
      else {
        /* Root unhandled error case. */
        }
      return NULL;
      }
    }
  return self;
  }

void wick_place_signal_error ( wick_place * self, wick_error * error ) {
  wick_channel_send ( self, self->error_channel, wick_make_dyn ( error, &wick_global.type._error ) );
  }
