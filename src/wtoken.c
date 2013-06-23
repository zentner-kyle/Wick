#include <wtoken.h>

whash whash_wtoken ( wval token ) {
  wtoken * self = ( wtoken * ) token.p;
  return self->family ^ wstr_hash ( self->text );
  };

int wcompare_wtoken ( wval token_a, wval token_b ) {
  wtoken * self = ( wtoken * ) token_a.p;
  wtoken * other = ( wtoken * ) token_b.p;
  if ( self->family == other->family ) {
    return wstr_compare ( self->text, other->text );
    }
  else {
    return self->family - other->family;
    }
  };
