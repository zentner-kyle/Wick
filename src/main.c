#include <stdio.h>

#include <wvm.h>
#include <wparse.h>

int main ( int argc, char *argv[] ) {
  printf ( "Starting Wick VM.\n" );
  wbuiltins_init ( );
  printf ( "src/test.wasm:\n%s\n", wstr_from_filename ( wstr_lit ( "src/test.wasm" ) ).start );
  opbunch * code = wbytecode_from_filename ( wstr_lit ( "src/test.wasm" ) );
  wexec_code ( code );
  wstr to_parse = wstr_lit ( "test <- testable + 100" );
  /*wstr to_parse = wstr_from_filename ( wstr_lit ( "doc/61a.wick" ) );*/
  /*wstr to_parse = wstr_lit ( "'test\\月'" );*/
  /*wstr to_parse = wstr_lit ( "test\n" );*/
  wparser * parser = wparser_new ( &to_parse );
  wparser_lex ( parser );
  wtokens_print ( parser->tokens );
  return 0;
  }
