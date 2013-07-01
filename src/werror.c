#include <werror.h>
#include <stdio.h>

void wreport_error ( char * msg ) {
  printf ( "Wick Core Error: %s\n", msg );
  }

werror wcast_error;

wdefine_composite ( werror );
