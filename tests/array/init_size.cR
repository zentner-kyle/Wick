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
	array_init_to_size( &a, wtype_int, 64, error_wcall );
	printf( "array->space = %zd\n", a.space );
	assert( a.space == 64 * sizeof( int ) );
	return error;
}
