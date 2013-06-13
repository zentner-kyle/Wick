#include <dyn.h>
#include <wick_array.h>
#include <wick_string.h>
#include <stdarg.h>

#define WICK_PARSER_WHITESPACE_MATCHER_TAG 0x8000
#define WICK_PARSER_STRING_MATCHER_TAG 0x8001
#define WICK_PARSER_FUNCTION_MATCHER_TAG 0x8002
#define WICK_PARSER_C_FUNCTION_PTR_TAG 0x8004
#define WICK_PARSER_OPTION_COMBINATOR_TAG 0x8008
#define WICK_PARSER_SEQUENCE_COMBINATOR_TAG 0x8009

struct wick_parser_matcher {
  extended_id id;
  };

typedef struct wick_parser_matcher wick_parser_matcher;

typedef wick_error ( *wick_parse_func ) ( wick_string * in, wick_string * out, void * data );

struct wick_parser_function_matcher {
  extended_id id;
  wick_parse_func func;
  void * data;
  };

typedef struct wick_parser_function_matcher wick_parser_function_matcher;


struct wick_parser_options_combinator {
  extended_id id;
  wick_array array;
  };

typedef struct wick_parser_options_combinator wick_parser_options_combinator;


struct wick_parser_sequence_combinator {
  extended_id id;
  wick_array array;
  };

typedef struct wick_parser_sequence_combinator wick_parser_sequence_combinator;

struct wick_parser_string_matcher {
  extended_id id;
  wick_string str;
  };

typedef struct wick_parser_string_matcher wick_parser_string_matcher;



wick_parse_matcher * wick_parser_whitespace ( void ) {
  wick_parser_matcher * out = WICK_ALLOC ( wick_parser_matcher );
  WICK_FAIL_ON_ERROR ( NULL );
  out->id = WICK_PARSER_WHITESPACE_MATCHER_TAG;
  return ( wick_parser_matcher * )out;
  }

wick_parser_matcher * wick_parser_string ( char * str ) {
  wick_parser_string_matcher * out = WICK_ALLOC ( wick_parser_string_matcher );
  WICK_FAIL_ON_ERROR ( NULL );
  wick_string_init ( out->str, str );
  out->id = WICK_PARSER_STRING_MATCHER_TAG;
  return ( wick_parser_matcher * )out;
  }

wick_parser_matcher * wick_parser_function_data ( wick_parse_func func, void * data ) {
  wick_parser_function_matcher * out = WICK_ALLOC ( wick_parser_function_matcher );
  WICK_FAIL_ON_ERROR ( NULL );
  out->id = WICK_PARSER_FUNCTION_MATCHER_TAG;
  out->func = func;
  out->data = data;
  return ( wick_parser_matcher * )out;
  }

wick_parser_matcher * wick_parser_function ( wick_parse_func func ) {
  return wick_parser_function_data ( func, NULL );
  }

wick_parser_matcher * wick_parser_options ( wick_parser_matcher * first, ... ) {
  wick_parser_options_combinator * out =
    WICK_ALLOC ( wick_parser_options_combinator );
  WICK_FAIL_ON_ERROR ( NULL );
  va_list argptr;
  va_start ( argptr, first );
  }
