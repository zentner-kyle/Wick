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
	warray_push_front( &a, &i, error_wcall );
	int i2 = 0;
	i2 = warray_get( &a, 0, int );
	assert( i2 == 100 );
	return error;
}