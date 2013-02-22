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
	array_push_front( &a, &i, error_delegate );
	int i2 = 0;
	array_pop_front( &a, &i2, error_delegate );
	assert( i2 == 100 );
	return error;
}
