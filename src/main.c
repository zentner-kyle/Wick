#include <stdio.h>

#include <wvm.h>

int main ( int argc, char *argv[] ) {
  printf ( "Starting Wick VM.\n" );
  printf ( "src/test.wasm:\n%s\n", wstr_from_filename ( wstr_lit ( "src/test.wasm" ) ).start );
  opbunch * code = wbytecode_from_filename ( wstr_lit ( "src/test.wasm" ) );
  wexec_code ( code );
  return 0;
  }
