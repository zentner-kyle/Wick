#ifndef WERROR_H
#define WERROR_H
#include "wmacros.h"
#include "wstr.h"
#include "wtype.h"

def_struct( werror ) {
	wtype * type;
	wstr message;
};

void wreport_error( char * msg );

wtype werror_type;


#endif /* end of include guard: WERROR_H */
