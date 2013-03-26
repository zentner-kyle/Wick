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
	array_push_front( &a, &i, error_wcall );
	int i2 = 0;
	array_pop_front( &a, &i2, error_wcall );
	assert( i2 == 100 );
	return error;
}
