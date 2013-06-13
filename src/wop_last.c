#define OP_NAME last

#include <wopcode_begin.c>

#ifdef OPCODE_BODY
  printf ( "Error: encountered dummy opcode "
  string_of_macro ( OP_NAME ) "." );
  fflush ( stdout );
  assert ( 0 );
  #endif

#include <wopcode_end.c>
