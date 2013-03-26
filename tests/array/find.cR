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
	wcall error_wcall = { .func = report_error, .data = NULL };
	array * a = array_new( wtype_int, error_wcall );
	int i = 1;
	array_push_front( a, &i, error_wcall );
	++i;
	array_push_front( a, &i, error_wcall );
	++i;
	array_push_front( a, &i, error_wcall );
	i = 2;
	assert( *(int *)array_find( a, &i ) == 2 );
	printf( "%p\n", a->data );
	return error;
}
