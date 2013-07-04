#include <werror.h>
#include <stdio.h>

void wreport_error ( char * msg ) {
  printf ( "Wick Core Error: %s\n", msg );
  }

werror wcast_error;

wdefine_composite ( werror );

werror * w_ok = 0;
werror werror_generic;
