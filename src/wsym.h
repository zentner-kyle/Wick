#ifndef WSYM_H
#define WSYM_H
#include "wtype_h.h"
#include "wmacros.h"
#include "wstr.h"

def_struct( wsym ) {
    wstr text;
};

wsym wsym_of_wstr( wstr s );

#define WSYM_LIT( literal ) \
  ((wsym) { WSTR_LIT( literal ) })

#endif /* end of include guard: WSYM_H */
