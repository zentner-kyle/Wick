#ifndef WERROR_H
#define WERROR_H
#include <wmacros.h>
#include <wtype_h.h>

struct wstr;

def_struct ( werror ) {
  wtype * type;
  struct wstr * message;
  };

void wreport_error ( char * msg );

wdeclare_composite ( werror );

extern werror wcast_error;
extern werror * w_ok;
extern werror werror_generic;


#endif /* end of include guard: WERROR_H */
