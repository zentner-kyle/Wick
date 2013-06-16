#include <string.h>
#include <assert.h>

#include <wstr.h>
#include <wtype.h>
#include <walloc.h>
#include <hash.h>

int wstr_compare_void ( void * v_str_a, void * v_str_b );
whash_t wstr_hash_void ( void * str );

wtype wstr_type;

wtable_elem_interface wstr_wtable_i = {
  &wstr_hash_void,
  &wstr_compare_void
  };

size_t wstr_size ( const wstr str ) {
  return str.past_end - str.start;
  }

wstr wstr_from_literal ( const char * literal ) {
  wstr out;
  out.type = &wstr_type;
  out.start = literal;
  out.past_end = literal + strlen ( literal );
  out.alloc_type = wstr_static;
  return out;
  }

wstr wstr_from_dynamic ( const char * dynamic ) {
  wstr out;
  out.type = &wstr_type;
  out.start = dynamic;
  out.past_end = dynamic + strlen ( dynamic );
  out.alloc_type = wstr_dynamic;
  return out;
  }

long int wick_get_file_size ( FILE * file ) {
  fpos_t start_position;
  long int file_size;
  fgetpos ( file, &start_position );
  fseek ( file, 0, SEEK_END );
  file_size = ftell ( file );
  fsetpos ( file, &start_position );
  return file_size;
  }

wstr wstr_from_file ( FILE * file ) {
  wstr out;
  out.type = &wstr_type;
  out.alloc_type = wstr_dynamic;
  if ( file == NULL ) {
    out.start = NULL;
    out.past_end = NULL;
    }
  else {
    long int to_read = wick_get_file_size ( file );
    uint8_t * buffer = malloc ( to_read + 1 );
    if ( buffer ) {
      long int bytes_read = fread ( ( void * )buffer, sizeof ( char ), to_read, file );
      assert ( bytes_read == to_read );
      out.past_end = ( const char * ) buffer + bytes_read;
      buffer[bytes_read] = '\0';
      out.start = ( const char * ) buffer;
      }
    else {
      out.start = NULL;
      out.past_end = NULL;
      }
    }
  return out;
  }

wstr wstr_from_filename ( wstr filename ) {
  FILE * file = fopen ( filename.start, "r" );
  wstr out = wstr_from_file ( file );
  if ( file ) {
    fclose ( file );
    }
  return out;
  }

size_t wstr_code_length ( const wstr str ) {
  size_t count = 0;
  const char * cur = str.start;
  const char * ascii_start = cur;

  while ( cur < str.past_end && *cur <= 127 )
  ascii: ++cur;

  count += cur - ascii_start;
  while ( cur < str.past_end ) {
    if ( *cur <= 127 ) {
      ascii_start = cur;
      goto ascii;
      }
    else {
      switch ( 0xF0 & *cur ) {
        case 0xE0: cur += 3; break;
        case 0xF0: cur += 4; break;
        default:   cur += 2; break;
        }
      }
    ++count;
    }
  return count;
  }

int wstr_compare ( wstr a, wstr b ) {
  size_t a_size = wstr_size ( a );
  size_t b_size = wstr_size ( b );
  if ( a_size == b_size ) {
    return memcmp ( ( const void * )a.start, ( const void * )b.start, a_size );
    } else if ( a_size > b_size ) {
    int result = memcmp ( a.start, b.start, b_size );
    if ( result == 0 ) {
      return 1;
      }
    else {
      return result;
      }
    } else /* if ( b_size > a_size ) */ {
    int result = memcmp ( a.start, b.start, a_size );
    if ( result == 0 ) {
      return -1;
      }
    else {
      return result;
      }
    }
  }

void wstr_init_dynamic ( wstr * to_init ) {
  to_init->type = ( wtype * ) &wstr_type;
  to_init->alloc_type = wstr_dynamic;
  }

void wstr_print ( wstr str ) {
  for ( const char * c = str.start; c != str.past_end; c++ ) {
    putc ( *c, stdout );
    }
  }

void wstr_println ( wstr str ) {
  wstr_print ( str );
  printf ( "\n" );
  }

siphash_key wstr_key = { { 0xe8f35937acefffaa, 0x331e89da1849403f } };

whash_t wstr_hash_void ( void * str ) {
  wstr self = *( wstr * ) str;
  return wstr_hash ( self );
  }

whash_t wstr_hash ( wstr self ) {
  return siphash_24 ( wstr_key, ( uint8_t * ) self.start, wstr_size ( self ) );
  }

int wstr_compare_void ( void * v_str_a, void * v_str_b ) {
  wstr str_a = * ( wstr * ) v_str_a;
  wstr str_b = * ( wstr * ) v_str_b;
  return wstr_compare ( str_a, str_b );
  }

void wstr_init_alloc ( wstr * self, const char * start, const char * past_end, enum wstr_alloc_type alloc_type ) {
  if ( ( ! past_end ) && start ) {
    past_end = start;
    while (*past_end) {
      past_end++;
      }
    }
  self->type = &wstr_type;
  self->start = start;
  self->past_end = past_end;
  }

void wstr_init ( wstr * self, const char * start, const char * past_end ) {
  wstr_init_alloc ( self, start, past_end, wstr_dynamic );
  }

wstr * wstr_new_alloc ( const char * start, const char * past_end, enum wstr_alloc_type alloc_type ) {
  wstr * to_return = walloc_simple ( wstr, 1 );
  if ( ! to_return ) {
    return NULL;
    }
  wstr_init_alloc ( to_return, start, past_end, alloc_type );
  return to_return;
  }

wstr * wstr_new ( const char * start, const char * past_end ) {
  return wstr_new_alloc ( start, past_end, wstr_dynamic );
  }
