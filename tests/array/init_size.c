#include <assert.h>
#include <stdio.h>
#include "array.h"
#include "delegate.h"

bool error = false;

void report_error( void * ignored ) {
	error = true;
}

int main() {
	array a;
	delegate error_delegate = { .func = report_error, .data = NULL };
	array_init_to_size( &a, sizeof( int ), 64, error_delegate );
	printf( "array->space = %zd\n", a.space );
	assert( a.space == 64 * sizeof( int ) );
	return error;
}
