#include <wtoken.h>
#include <stdint.h>
#include <wtype_h.h>
#include <wstr.h>
#include <wmacros.h>
#include <walloc.h>

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

wtoken * wtoken_new ( int family, wstr * text ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken * t = walloc_simple ( wtoken , 1 );
  t->type = wtype_wtoken;
  t->family = family;
  t->text = text;
  t->lbp = 0;
  t->rbp = 0;
  return t;
  }

wtoken * wtoken_new_op ( int family, wstr * text, int lbp, int rbp ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken * t = walloc_simple ( wtoken , 1 );
  t->type = wtype_wtoken;
  t->family = family;
  t->text = text;
  t->lbp = lbp;
  t->rbp = rbp;
  return t;
  }

wdefine_composite ( wtoken );
