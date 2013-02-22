#include <assert.h>
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
	return error;
}
