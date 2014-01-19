#include <stdio.h>

#include <wvm.h>
#include <wparse.h>

int main ( int argc, char *argv[] ) {
#ifndef WICK_MICRO
  wbuiltins_init ( );
  if ( argc < 2 ) {
    printf ( "%s: no input file.\n", argv[0] );
    return -1;
    }
  wstr to_parse = wstr_from_filename ( wstr_from_static ( argv[1] ) );
  wparser * parser = wparser_new ( &to_parse );
  werror * e = wparser_parse ( parser );;
  if ( e == w_ok ) {
    wast_print ( ( wast * ) parser->root );
    }
  else {
    printf ( "%s: could not parse input file.\n", argv[0] );
    }
#endif /* WICK_MICRO */
  return 0;
  }
