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
  return wstr_hash ( *self->text );
  };

int wcompare_wtoken ( wval token_a, wval token_b ) {
  wtoken * self = ( wtoken * ) token_a.pointer;
  wtoken * other = ( wtoken * ) token_b.pointer;
  return wstr_compare ( *self->text, *other->text );
  };

wtoken * wtoken_new ( wstr * text ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken * t = walloc_simple ( wtoken , 1 );
  if ( ! t ) {
    free ( text );
    return NULL;
    }
  t->type = wtype_wtoken;
  t->text = text;
  return t;
  }

wtoken_left * wtoken_left_new ( wstr * text ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken_left * t = walloc_simple ( wtoken_left , 1 );
  if ( ! t ) {
    free ( text );
    return NULL;
    }
  t->type = wtype_wtoken;
  t->text = text;
  t->right = NULL;
  return t;
  }

wtoken_right * wtoken_right_new ( wstr * text ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken_right * t = walloc_simple ( wtoken_right , 1 );
  if ( ! t ) {
    free ( text );
    return NULL;
    }
  t->type = wtype_wtoken;
  t->text = text;
  t->left = NULL;
  return t;
  }

werror * wtoken_init ( wtoken * self, wstr * text ) {
  if ( ! text ) {
    return &wick_out_of_memory;
    }
  self->type = wtype_wtoken;
  self->text = text;
  return w_ok;
  }

wtoken_prefix * wtoken_prefix_new ( wstr * text, int rbp ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken_prefix * t = walloc_simple ( wtoken_prefix , 1 );
  if ( ! t ) {
    free ( text );
    return NULL;
    }
  t->type = wtype_wtoken;
  t->text = text;
  t->rbp = rbp;
  return t;
  }

wtoken_infix * wtoken_infix_new ( wstr * text, int lbp, int rbp ) {
  if ( ! text ) {
    return NULL;
    }
  wtoken_infix * t = walloc_simple ( wtoken_infix , 1 );
  if ( ! t ) {
    free ( text );
    return NULL;
    }
  t->type = wtype_wtoken;
  t->text = text;
  t->lbp = lbp;
  t->rbp = rbp;
  return t;
  }

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
