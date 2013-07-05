#include <stdio.h>

#include <wvm.h>
#include <wparse.h>

int main ( int argc, char *argv[] ) {
  printf ( "Starting Wick VM.\n" );
  printf ( "src/test.wasm:\n%s\n", wstr_from_filename ( wstr_lit ( "src/test.wasm" ) ).start );
  opbunch * code = wbytecode_from_filename ( wstr_lit ( "src/test.wasm" ) );
  wexec_code ( code );
  /*wparser * parser = wparser_new ( wstr_new ( "test <- testable + 100", 0 ) );*/
  wstr to_parse = wstr_from_filename ( wstr_lit ( "doc/61a.wick" ) );
  /*wstr to_parse = wstr_lit ( "'test\\月'" );*/
  /*wstr to_parse = wstr_lit ( "test\n" );*/
  wparser * parser = wparser_new ( &to_parse );
  wparser_lex ( parser );
  wparser_print_tokens ( parser );
  return 0;
  }
