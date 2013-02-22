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
	array_init( &a, sizeof( int ), error_delegate );
	int i = 100;
	array_push_back( &a, &i, error_delegate );
	printf( "array.data[0] == %d\n", *(int *)a.data );
	int i2 = 0;
	array_debug_print( &a );
	fflush( stdout );
	i2 = array_get( &a, 0, int );
	printf( "i2 = %d\n", i2 );
	assert( i2 == 100 );
	return error;
}
