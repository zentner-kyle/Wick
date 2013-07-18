#include <stdio.h>

#include <wvm.h>
#include <wparse.h>

int main ( int argc, char *argv[] ) {
  printf ( "Starting Wick VM.\n" );
  wbuiltins_init ( );
  printf ( "src/test.wasm:\n%s\n", wstr_from_filename ( wstr_lit ( "src/test.wasm" ) ).start );
  opbunch * code = wbytecode_from_filename ( wstr_lit ( "src/test.wasm" ) );
  wexec_code ( code );
  /*wstr to_parse = wstr_lit ( "test <- testable + - 100\nprint ( test )" );*/
  wstr to_parse = wstr_from_filename ( wstr_lit ( "doc/61a.wick" ) );
  /*wstr to_parse = wstr_lit ( "test = (testable + 9) + 2 * 100  / - 1 ** 2 ** 3\ntest <- test+1\nprint ( test )" );*/
  /*wstr to_parse = wstr_lit ( "'test\\月'" );*/
  /*wstr to_parse = wstr_lit ( "test月" );*/
  /*wstr to_parse = wstr_lit ( "test\n" );*/
  /*wstr to_parse = wstr_lit ( "test[[[]]]\n" );*/
  /*wstr to_parse = wstr_lit ( "test\n" );*/
  /*wstr to_parse = wstr_lit ( "test\n()\n" );*/
  /*wstr to_parse = wstr_lit ( "k = strange syntax(1, 2, 3)" );*/
  /*wstr to_parse = wstr_lit ( "k = strange syntax(1, 2, 3)\nk <- k + 1" );*/
  /*wstr to_parse = wstr_lit ( "k <- \n  k" );*/
  /*wstr to_parse = wstr_lit ( "k ( a :: int, b :: int, )" );*/
  /*wstr to_parse = wstr_lit ( "k ( a ( , ), )" );*/
  /*wstr to_parse = wstr_lit ( "a0 =\n"*/
                             /*"  b0 =\n"*/
                             /*"    2\n"*/
                             /*"    2\n"*/
                             /*"a1 = \n"*/
                             /*"  b1 = {\n"*/
                             /*"2\n"*/
                             /*"2\n"*/
                             /*"}"*/
                             /*);*/
  /*wstr to_parse = wstr_lit ( "a0 =\n  1\n  2" );*/
  /*wstr to_parse = wstr_lit ( "a0 =\n  1\n  2" );*/
  wparser * parser = wparser_new ( &to_parse );
  for ( int i = 0; i < 1; i++ ) {
    wparser_parse ( parser );
    }
  printf ( "Final ast:\n" );
  wast_print ( ( wast * ) parser->root );
  return 0;
  }
