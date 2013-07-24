#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <wvm.h>
#include <walloc.h>
#include <werror.h>

#define warray_elem_t opbunch
#define warray_elem_kind integer
#include <warray.h>

#define WICK_STACK_SIZE 256
#define WICK_ENV_SIZE 256

#if !defined ( HAVE_COMPUTED_GOTOS ) && defined ( USE_COMPUTED_GOTOS )
  #error "Cannot use computed goto with this compiler."
  #endif

#ifdef USE_COMPUTED_GOTOS
  #define DISPATCH goto *jump_targets[op_bunch & 0xff];
#else
  #define DISPATCH switch ( op_bunch & 0xff )
  #endif

wdefine_base ( opbunch );

static const char wopcode_args[] = {
  #define OPCODE_LIST
  #define OPCODE_ARGS
  #include <wopcodes.c>
  };


void wexec_code ( opbunch * c ) {
  opbunch op_bunch = *c;
  wasm_arg args[4];
  wval * stack_start = walloc_simple ( wval, WICK_STACK_SIZE );
  wval * stack_end = stack_start;
  wval * env = walloc_simple ( wval, WICK_ENV_SIZE );
  wval acc = { 0 };
#define get_arg( a ) \
( stack_start + a )
  /* #define get_arg( a )                                      \ */
  /*   ( ( a & ( 1 << 7 ) ) ? ( stack_end + a ) : ( stack_start + a ) ) */
  #define arg_0 ( *get_arg ( args[0] ) )
  #define arg_1 ( *get_arg ( args[1] ) )
  #define arg_2 ( *get_arg ( args[2] ) )

  #ifdef USE_COMPUTED_GOTOS
    static const void * jump_targets[] = {
      #define OPCODE_TARGET
      #include <wopcodes.c>
      };
    #endif

  while ( true ) {
    DISPATCH {
      #define OPCODE_LABEL
      #define OPCODE_BODY
      #include <wopcodes.c>
      }
    }
exit:
  printf ( "Exiting wexec_code\n" );
  }

#ifndef WICK_MICRO
static const char * wopcode_names[] = {
  #define OPCODE_LIST
  #define OPCODE_NAME
  #include <wopcodes.c>
  };


void print_opcodes ( void ) {
  for ( int i = 0; i < 0xff; i++ ) {
    if ( strcmp ( wopcode_names[ i ], "last" ) == 0 ) {
      break;
      }
    else {
      printf ( "%i, %s\n", i, wopcode_names[ i ] );
      }
    }
  }

void skip_wspace ( wstr * line ) {
  while ( ( isspace ( line->start[ 0 ] ) || line->start[ 0 ] == '\n' ) &&
          line->start != line->past_end ) {
    ++line->start;
    }
  }

uint8_t get_opcode ( wstr name ) {
  for ( int i = 0; i < 0xff; i++ ) {
    if ( strcmp ( wopcode_names[ i ], "last" ) == 0 ) {
      break;
      }
    else if ( wstr_compare ( wstr_from_static ( wopcode_names[ i ] ), name ) == 0 ) {
      return i;
      }
    }
  return 0;
  }

void get_opcode_name ( wstr line, wstr * name, wstr * rest ) {
  skip_wspace ( &line );
  const char * c = line.start;

  while ( c != line.past_end && ! isspace ( *c ) ) { ++c;   }
  wstr_init_dynamic ( name );
  wstr_init_dynamic ( rest );
  name->start = line.start;
  name->past_end = c;
  rest->start = c;
  rest->past_end = line.past_end;
  }

void get_next_line ( wstr file, wstr * line, wstr * rest ) {
  const char * c = file.start;
  while ( c != file.past_end && *c != '\n' ) {
    ++c;
    }
  wstr_init_dynamic ( line );
  wstr_init_dynamic ( rest );
  if ( c == file.past_end ) {
    line->start = file.start;
    line->past_end = file.past_end;
    rest->start = NULL;
    rest->past_end = NULL;
    }
  else {
    assert ( *c == '\n' );
    line->start = file.start;
    line->past_end = c;
    rest->start = c + 1;
    rest->past_end = file.past_end;
    }
  }

uint8_t hex_digit ( char c ) {
#  define CASE( c, i )                          \
    case c: return i
  switch ( c ) {
    CASE ( '0', 0x0 );
    CASE ( '1', 0x1 );
    CASE ( '2', 0x2 );
    CASE ( '3', 0x3 );
    CASE ( '4', 0x4 );
    CASE ( '5', 0x5 );
    CASE ( '6', 0x6 );
    CASE ( '7', 0x7 );
    CASE ( '8', 0x8 );
    CASE ( '9', 0x9 );
    CASE ( 'a', 0xa );
    CASE ( 'b', 0xb );
    CASE ( 'c', 0xc );
    CASE ( 'd', 0xd );
    CASE ( 'e', 0xe );
    CASE ( 'f', 0xf );
    default:
      return 0;
    }
#undef CASE
  }

uint8_t parse_arg ( wstr line, wstr * rest, werror * report ) {
  uint8_t arg = 0;
  skip_wspace ( &line );
  if ( line.start[0] == 's' ) {
    arg = 1 << 7;
    }
  else {
  if ( line.start[ 0 ] != 'r' ) {
  report->message = wstr_new ( "Expected argument to start with either 's' or 'r'.", 0 );
  return 0;
  }
    }
  if ( ! isxdigit ( line.start[ 1 ] ) ) {
    report->message = wstr_new ( "No hex digit in argument.", 0 );
    return 0;
    }
  int shift = 0;
  rest->start = line.start + 2;
  if ( isxdigit ( line.start[ 2 ] ) ) {
    shift = 4;
    arg |= hex_digit ( line.start[ 2 ] );
    rest->start += 1;
    }
  rest->past_end = line.past_end;
  uint8_t larger = hex_digit ( line.start[ 1 ] );
  if ( ( ( larger << shift ) & ( 1 << 7 ) ) || isxdigit ( rest->start[ 0 ] ) ) {
    report->message = wstr_new ( "Argument is too large.", 0 );
    }
  arg |= larger << shift;
  return arg;
  }

opbunch * wbytecode_from_filename ( wstr filename ) {
  wstr file = wstr_from_filename ( filename );
  wstr file_remaining = file;
  wstr line;
  size_t total_size = 0;
  int left_in_opcode = 4;
  wstr success = wstr_lit ( "success" );
  werror parse_error = { wtype_werror, & success };
  warray_opbunch * uncollapsed_code = warray_opbunch_new ( );
  if ( ! uncollapsed_code ) {
    return NULL;
    }
  while ( wstr_size ( file_remaining ) != 0 ) {
    get_next_line ( file_remaining, &line, &file_remaining );
    wstr name;
    get_opcode_name ( line, &name, &line );
    uint32_t opcode = get_opcode ( name );
    uint8_t size = wopcode_args[opcode];
    left_in_opcode -= size + 1;
    if ( left_in_opcode < 0 ) {
      total_size += 1;
      left_in_opcode = 4 - ( size + 1 );
      }
    for ( int i = 1; i <= size; i++ ) {
      skip_wspace ( &line );
      uint8_t arg = parse_arg ( line, &line, &parse_error );
      opcode |= ( uint32_t ) arg << ( i * 8 );
      if ( i != size ) {
        skip_wspace ( &line );
        assert ( line.start[ 0 ] == ',' );
        line.start++;
        }
      }
    skip_wspace ( &line );
    assert ( wstr_size ( line ) == 0 || line.start[ 0 ] == '#' );
    warray_opbunch_push_back ( uncollapsed_code, opcode );
    fflush (stdout);
    }
  ++total_size;
  opbunch * code = malloc ( sizeof ( opbunch ) * total_size );
  opbunch * code_i = code;
  opbunch opcode;
  uint8_t used_in_opcode = 0;
  while ( ! warray_opbunch_empty ( uncollapsed_code ) ) {
    if (warray_opbunch_pop_front ( uncollapsed_code, &opcode ) != w_ok) {
      free (code);
      return NULL;
      }
    uint8_t size = wopcode_args[opcode & 0xff] + 1;
    if ( used_in_opcode + size > sizeof ( opbunch ) ) {
      while ( used_in_opcode < sizeof ( opbunch ) ) {
        *code_i &= ~ ( 0xff << ( used_in_opcode * 8 ) );
        ++used_in_opcode;
        }
      ++code_i;
      used_in_opcode = 0;
      }
    for ( int i = 0; i < size; i++ ) {
      *code_i &= ~ ( 0xff << ( ( used_in_opcode + i ) * 8 ) );
      *code_i |= ( ( opcode >> ( i * 8 ) ) & 0xff ) << ( ( used_in_opcode + i ) * 8 );
      }
    used_in_opcode += size;
    }
  while ( used_in_opcode < sizeof ( opbunch ) ) {
    *code_i &= ~ ( 0xff << ( used_in_opcode * 8 ) );
    ++used_in_opcode;
    }
  printf ( "Compressed code: " );
  for ( int i = 0; i < total_size; i++ ) {
    printf ( "%.8x ", code[i] );
    }
  printf ( "\n" );
  return code;
  }
#endif /* WICK_MICRO */
