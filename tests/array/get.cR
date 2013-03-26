#include <assert.h>
#include <stdio.h>
#include "warray.h"
#include "wcall.h"
#include "wbuiltins.h"

bool error = false;

void report_error( void * ignored ) {
	error = true;
}

int main() {
	wbuiltins_init();
	array a;
	wcall error_wcall = { .func = report_error, .data = NULL };
	array_init( &a, wtype_int, error_wcall );
	int i = 100;
	array_push_back( &a, &i, error_wcall );
	printf( "array.data[0] == %d\n", *(int *)a.data );
	int i2 = 0;
	array_debug_print( &a );
	fflush( stdout );
	i2 = array_get( &a, 0, int );
	printf( "i2 = %d\n", i2 );
	assert( i2 == 100 );
	return error;
}
