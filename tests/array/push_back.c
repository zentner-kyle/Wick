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
	warray a;
	wcall error_wcall = { .func = report_error, .data = NULL };
	warray_init( &a, wtype_upcast(&wtype_int), error_wcall );
	int i = 100;
	warray_push_back( &a, &i, error_wcall );
	printf( "warray.data[0] == %d\n", *(int *)a.data );
	assert( *(int *)a.data == 100 );
	return error;
}
