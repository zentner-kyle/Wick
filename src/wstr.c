#include "wstr.h"
#include "wtype.h"
#include "walloc.h"
#include <string.h>
#include <assert.h>

wtype wstr_type;

wstr wstr_from_literal( const char * literal ) {
	wstr out;
	out.type = &wstr_type;
	out.text = literal;
	out.length = strlen( literal );
	out.alloc_type = wstr_static;
	return out;
}

wstr wstr_from_dynamic( const char * dynamic ) {
	wstr out;
	out.type = &wstr_type;
	out.text = dynamic;
	out.length = strlen( dynamic );
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
		out.text = NULL;
		out.length = 0;
	} else {
		long int to_read = wick_get_file_size( file );
		uint8_t * buffer = malloc( out.length + 1);
		if ( buffer ) {
			long int bytes_read = fread((void *)buffer, sizeof(char), to_read, file);
			assert(bytes_read == to_read);
			out.length = bytes_read;
			buffer[out.length] = '\0';
			out.text = (const char *)buffer;
		} else {
			out.text = NULL;
			out.length = 0;
		}
	}
	return out;
}

wstr wstr_from_filename( wstr filename ) {
	FILE * file = fopen( filename.text, "r" );
	wstr out = wstr_from_file( file );
	if ( file ) {
		fclose( file );
	}
	return out;
}

size_t wstr_code_length(const wstr str) {
	size_t count = 0;
	size_t index = 0;
	size_t ascii_start = 0;

	while ( index < str.length && str.text[index] <= 0x7f ) {
		ascii: ++index;
	}

	count += index - ascii_start;
	while ( index < str.length ) {
		if ( str.text[index] <= 0x7f ) {
			ascii_start = index;
			goto ascii;
		} else {
			switch ( 0xF0 & str.text[index] ) {
				case 0xe0: index += 3; break;
				case 0xf0: index += 4; break;
				default: index += 2; break;
			}
		}
		++count;
	}
	return count;
}

int wstr_compare( wstr a, wstr b) {
	if (a.length == b.length) {
		return memcmp( (const void *)a.text, (const void *)b.text, a.length);
	} else if (a.length > b.length) {
		int result = memcmp( a.text, b.text, b.length );
		if (result == 0) {
			return 1;
		} else {
			return result;
		}
	} else /* if ( b.length > a.length ) */ {
		int result = memcmp( a.text, b.text, a.length );
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
	for ( int i = 0; i < str.length; i++ ) {
		putc( str.text[i], stdout );
	}
}

void wstr_println(wstr str) {
	wstr_print( str );
	printf( "\n" );
}
