#include <wtoken.h>
#include <stdint.h>
#include <wtype_h.h>
#include <wstr.h>
#include <wmacros.h>

whash whash_wtoken ( wval token ) {
  wtoken * self = ( wtoken * ) token.pointer;
  return self->family ^ wstr_hash ( *self->text );
  };

int wcompare_wtoken ( wval token_a, wval token_b ) {
  wtoken * self = ( wtoken * ) token_a.pointer;
  wtoken * other = ( wtoken * ) token_b.pointer;
  if ( self->family == other->family ) {
    return wstr_compare ( *self->text, *other->text );
    }
  else {
    return self->family - other->family;
    }
  };

wdefine_composite ( wtoken );
