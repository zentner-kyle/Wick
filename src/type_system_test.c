#include <stdio.h>
#include <wick_core.h>
#include <wick_error.h>
#include <wick_place.h>
#include <wick_globals.h>

int main ( int argc, char ** argv ) {
  wick_core * c = wick_core_new ( );
  wick_place_signal_error ( c->global_place, &wick_global.error.success );
  return 0;
  }
