#include <assert.h>
#include <wast.h>
#include <walloc.h>

wast * wast_parent_exprlist ( wast * ast ) {
  while ( ! wobj_cast ( wast_list, ast ) ) {
    ast = ( wast * ) ast->parent;
    }
  return ast;
  }

wobj * wast_remove_rightmost ( wast * w ) {
  if ( wobj_cast ( wast_unop, w ) ) {
    wobj * c = wobj_cast ( wast_unop, w )->child;
    wobj_cast ( wast_unop, w )->child = NULL;
    return c;
    }
  else if ( wobj_cast ( wast_binop, w ) ) {
    wobj * c = wobj_cast ( wast_binop, w )->right;
    wobj_cast ( wast_binop, w )->right = NULL;
    return c;
    }
  else if ( wobj_cast ( wast_list, w ) ) {
    return warray_wobj_ptr_pop_back ( wobj_cast ( wast_list, w )->children, &null_wcall );
    }
  else {
    assert ( 0 );
    }
  }

void wast_add_rightmost ( wast * w, wobj * o ) {
  if ( wobj_cast ( wast_unop, w ) ) {
    wobj_cast ( wast_unop, w )->child = o;
    }
  else if ( wobj_cast ( wast_binop, w ) ) {
    wobj_cast ( wast_binop, w )->right = o;
    }
  else if ( wobj_cast ( wast_list, w ) ) {
    warray_wobj_ptr_push_back ( wobj_cast ( wast_list, w )->children,
                                o,
                                &null_wcall );
    }
  else {
    assert ( 0 );
    }
  }

wast_list * wast_list_new ( wtoken * op ) {
  wast_list * w = walloc_simple ( wast_list, 1 );
  w->type = wtype_wast_list;
  w->parent = NULL;
  w->op = op;
  w->children = warray_wobj_ptr_new ( &null_wcall );
  return w;
  }

void wast_print_inner ( wast * w, int indent ) {
  if ( ! w ) {
    return;
    }
  if ( wobj_cast ( wast_unop, w ) ) {
    wstr_println ( *w->op->text );
    wast_print_inner ( wobj_cast ( wast, wobj_cast ( wast_unop, w )->child ), indent + 1 );
    }
  else if ( wobj_cast ( wast_binop, w ) ) {
    wstr_println ( *w->op->text );
    wast_print_inner ( wobj_cast ( wast, wobj_cast ( wast_binop, w )->left ), indent + 1 );
    wast_print_inner ( wobj_cast ( wast, wobj_cast ( wast_binop, w )->right ), indent + 1 );
    }
  else if ( wobj_cast ( wast_list, w ) ) {
    printf ( "wast_list!\n" );
    fflush ( stdout );
    wstr_println ( *w->op->text );
    wtokens_print ( wobj_cast ( wast_list, w )->children );
    }
  else {
    printf ( "type = " );
    wstr_println ( w->type->name );
    printf ( "&type = %p\n", w->type );
    printf ( "wast_list_wtype = %p\n", wtype_wast_list );
    assert ( 0 );
    }
  }

void wast_print ( wast * w ) {
  wast_print_inner ( w, 0 );
  }
