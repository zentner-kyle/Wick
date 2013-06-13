#include "wstr.h"
#include "wtype.h"
#include "walloc.h"
#include <string.h>
#include <assert.h>

wtype wstr_type;

size_t wstr_size( const wstr str ) {
  return str.past_end - str.start;
}

wstr wstr_from_literal( const char * literal ) {
  wstr out;
  out.type = &wstr_type;
  out.start = literal;
  out.past_end = literal + strlen( literal );
  out.alloc_type = wstr_static;
  return out;
}

wstr wstr_from_dynamic( const char * dynamic ) {
  wstr out;
  out.type = &wstr_type;
  out.start = dynamic;
  out.past_end = dynamic + strlen( dynamic );
  out.alloc_type = wstr_dynamic;
  return out;
}

long int wick_get_file_size( FILE * file ) {
  fpos_t start_position;
  long int file_size;
  fgetpos(file, &start_position);
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  fsetpos(file, &start_position);
  return file_size;
}

wstr wstr_from_file( FILE * file ) {
  wstr out;
  out.type = &wstr_type;
  out.alloc_type = wstr_dynamic;
  if ( file == NULL ) {
    out.start = NULL;
    out.past_end = NULL;
  } else {
    long int to_read = wick_get_file_size( file );
    uint8_t * buffer = malloc( to_read + 1);
    if ( buffer ) {
      long int bytes_read = fread((void *)buffer, sizeof(char), to_read, file);
      assert(bytes_read == to_read);
      out.past_end = (const char *) buffer + bytes_read;
      buffer[bytes_read] = '\0';
      out.start = (const char *) buffer;
    } else {
      out.start = NULL;
      out.past_end = NULL;
    }
  }
  return out;
}

wstr wstr_from_filename( wstr filename ) {
  FILE * file = fopen( filename.start, "r" );
  wstr out = wstr_from_file( file );
  if ( file ) {
    fclose( file );
  }
  return out;
}

size_t wstr_code_length(const wstr str) {
  size_t count = 0;
  const char * cur = str.start;
  const char * ascii_start = cur;

  while (cur < str.past_end && *cur <= 127)
  ascii: ++cur;

  count += cur - ascii_start;
  while (cur < str.past_end) {
    if (*cur <= 127) {
      ascii_start = cur;
      goto ascii;
    } else {
      switch (0xF0 & *cur) {
        case 0xE0: cur += 3; break;
        case 0xF0: cur += 4; break;
        default:   cur += 2; break;
      }
    }
    ++count;
  }
  return count;
}

int wstr_compare( wstr a, wstr b) {
  size_t a_size = wstr_size( a );
  size_t b_size = wstr_size( b );
  if (a_size == b_size) {
    return memcmp( (const void *)a.start, (const void *)b.start, a_size);
  } else if (a_size > b_size) {
    int result = memcmp( a.start, b.start, b_size );
    if (result == 0) {
      return 1;
    } else {
      return result;
    }
  } else /* if ( b_size > a_size ) */ {
    int result = memcmp( a.start, b.start, a_size );
    if (result == 0) {
      return -1;
    } else {
      return result;
    }
  }
}

void wstr_init_dynamic( wstr * to_init ) {
  to_init->type = (wtype *) &wstr_type;
  to_init->alloc_type = wstr_dynamic;
}

void wstr_print(wstr str) {
  for ( const char * c = str.start; c != str.past_end; c++ ) {
    putc( *c, stdout );
  }
}

void wstr_println(wstr str) {
  wstr_print( str );
  printf( "\n" );
}
