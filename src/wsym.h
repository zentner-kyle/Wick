#ifndef WSYM_H
#define WSYM_H
#include <wtype_h.h>
#include <wmacros.h>
#include <wstr.h>
#include <wtable.h>

def_struct ( wsym ) {
  uint64_t id[2];
  wstr text;
  };

extern wtable_elem_interface wsym_wtable_i;

wsym wsym_of_wstr ( wstr s );

whash_t wsym_hash ( wsym self );
int wsym_compare ( wsym a, wsym b );

#define WSYM_LIT( literal ) \
  ( ( wsym ) { { 0, 0 }, WSTR_LIT ( literal )   } )

void wsym_init ( wsym * self, uint64_t id[2], const char * start, const char * past_end );
void wsym_init_alloc ( wsym * self, uint64_t id[2], const char * start, const char * past_end, enum wstr_alloc_type alloc_type );
wsym * wsym_new_alloc ( uint64_t id[2], const char * start, const char * past_end, enum wstr_alloc_type alloc_type );

#endif /* end of include guard: WSYM_H */
