#ifndef WBUILTINS_H
#define WBUILTINS_H
#include <stdbool.h>
#include <wtype.h>


extern bool wbuiltins_initialized;
extern wtype_base wtype_int;

#define int_type ( ( wtype * ) &wtype_int )

void wbuiltins_init ( );


#endif /* end of include guard: WBUILTINS_H */
