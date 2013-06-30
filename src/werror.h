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

wtype * wtype_werror;


#endif /* end of include guard: WERROR_H */
