#include <string.h>
#include <assert.h>

#include <wstr.h>
#include <wtype.h>
#include <walloc.h>
#include <whash.h>

wdefine_composite ( wstr );

wtable_elem_interface wstr_wtable_i = {
  &whash_wstr_ptr,
  &wcompare_wstr_ptr
  };

size_t wstr_size ( const wstr str ) {
  return str.past_end - str.start;
  }

wstr wstr_from_static ( const char * str ) {
  wstr out;
  out.type = wtype_wstr;
  out.start = str;
  out.past_end = str + strlen ( str );
  out.alloc_type = wstr_static;
  return out;
  }

wstr wstr_from_dynamic ( const char * dynamic ) {
  wstr out;
  out.type = wtype_wstr;
  out.start = dynamic;
  out.past_end = dynamic + strlen ( dynamic );
  out.alloc_type = wstr_dynamic;
  return out;
  }

#ifndef WICK_MICRO
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
  out.type = wtype_wstr;
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
#endif

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
  to_init->type = wtype_wstr;
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

whash whash_wstr_ptr ( wval str ) {
  wstr self = *( wstr * ) str.pointer;
  return wstr_hash ( self );
  }

whash wstr_hash ( wstr self ) {
  return siphash_24 ( wstr_key, ( uint8_t * ) self.start, wstr_size ( self ) );
  }

int wcompare_wstr_ptr ( wval v_str_a, wval v_str_b ) {
  wstr str_a = * ( wstr * ) v_str_a.pointer;
  wstr str_b = * ( wstr * ) v_str_b.pointer;
  return wstr_compare ( str_a, str_b );
  }

void wstr_init_alloc ( wstr * self, const char * start, const char * past_end, enum wstr_alloc_type alloc_type ) {
  if ( ( ! past_end ) && start ) {
    past_end = start;
    while (*past_end) {
      past_end++;
      }
    }
  self->type = wtype_wstr;
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

wstr * wstr_from_llong ( long long input ) {
  char * text = malloc ( sizeof( char ) * 40 );
  snprintf ( text, 40, "%lld", input );
  return wstr_new ( text, NULL );
  }

wstr * wstr_copy ( wstr * original ) {
  size_t size = wstr_size ( *original );
  wstr * str = walloc_simple ( wstr, 1 );
  char * buf = malloc ( size );
  if ( ! str || ! buf ) {
    free ( str );
    free ( buf );
    return NULL;
    }
  memcpy ( buf, original->start, size );
  str->type = wtype_wstr;
  str->start = buf;
  str->past_end = buf + size;
  str->alloc_type = wstr_dynamic;
  return str;
  }

wstr wstr_empty ( void ) {
  wstr to_return;
  wstr_init_dynamic ( &to_return );
  to_return.start = NULL;
  to_return.past_end = NULL;
  return to_return;
  }

void wstr_free ( wstr * str ) {
    if ( str ) {
      if ( str->alloc_type == wstr_dynamic ) {
        free ( ( void * ) str->start );
        }
      free ( str );
      }
    }
