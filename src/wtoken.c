#include <wtoken.h>
#include <stdint.h>
#include <wtype_h.h>
#include <wstr.h>
#include <wmacros.h>
#include <walloc.h>
#include <wobj.h>
#include <wtype.h>

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

void wtokens_print ( warray_wobj_ptr * self ) {
  printf ( "[ " );
  fflush ( stdout );
  warray_wobj_ptr_iter i = warray_wobj_ptr_start ( self );
  while ( warray_wobj_ptr_good ( &i ) ) {
    wtoken * token = wobj_cast ( wtoken, warray_wobj_ptr_deref ( &i ) );
    if ( token ) {
      wstr_print ( *token->text );
      if ( warray_wobj_ptr_good ( &i ) ) {
        /*printf ( ", " );*/
        printf ( " " );
        }
      }
      warray_wobj_ptr_next ( &i );
    }
  printf ( " ]" );
  }
