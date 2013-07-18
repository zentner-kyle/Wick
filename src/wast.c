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
  if ( wobj_cast ( wast_prefix, w ) ) {
    wobj * c = wobj_cast ( wast_prefix, w )->child;
    wobj_cast ( wast_prefix, w )->child = NULL;
    return c;
    }
  else if ( wobj_cast ( wast_infix, w ) ) {
    wobj * c = wobj_cast ( wast_infix, w )->right;
    wobj_cast ( wast_infix, w )->right = NULL;
    return c;
    }
  else if ( wobj_cast ( wast_list, w ) ) {
    wobj * child = NULL;
    warray_wobj_ptr_pop_back ( wobj_cast ( wast_list, w )->children, &child );
    return child;
    }
  else {
    assert ( 0 );
    }
  }

werror * wast_add_rightmost ( wast * w, wobj * o ) {
  if ( wobj_cast ( wast_prefix, w ) ) {
    wobj_cast ( wast_prefix, w )->child = o;
    }
  else if ( wobj_cast ( wast_infix, w ) ) {
    wobj_cast ( wast_infix, w )->right = o;
    }
  else if ( wobj_cast ( wast_list, w ) ) {
    return warray_wobj_ptr_push_back ( wobj_cast ( wast_list, w )->children,
                                       o );    
    }
  else {
    return &werror_internal;
    }
    return w_ok;
  }

wast_list * wast_list_new ( wtoken * op ) {
  wast_list * w = walloc_simple ( wast_list, 1 );
  w->type = wtype_wast_list;
  w->parent = NULL;
  w->op = op;
  w->primary = NULL;
  w->children = warray_wobj_ptr_new ( );
  return w;
  }

void wprint_indent ( int indent ) {
  for ( int i = 0; i < indent; i++ ) {
    printf ( "  " );
    }
  }

void wast_print_inner ( wobj * w, int indent ) {
  wprint_indent ( indent );
  if ( ! w ) {
    printf ( "(null)\n" );
    return;
    }
  wvar_of ( wp, wast_prefix, w ) {
    printf ("\'");
    wstr_print ( *wp->op->text );
    printf ("'\n");
    wast_print_inner ( wp->child, indent + 1 );
    } wvar_end
  else wvar_of ( wi, wast_infix, w ) {
    printf ("\'");
    wstr_print ( *wi->op->text );
    printf ("'\n");
    wast_print_inner ( wi->left, indent + 1 );
    wast_print_inner ( wi->right, indent + 1 );
    } wvar_end
  else wvar_of ( wl, wast_list, w ) {
    printf ("\'");
    wstr_print ( *wl->op->text );
    printf ("'\n");
    wast_print_inner ( wl->primary, indent + 1 );
    warray_wobj_ptr_iter i = warray_wobj_ptr_start ( (( wast_list * ) w)->children );
    while ( warray_wobj_ptr_good ( &i ) ) {
      wast_print_inner ( warray_wobj_ptr_deref ( &i ), indent + 1 );
      warray_wobj_ptr_next ( &i );
      }
    } wvar_end
  else {
    printf ("\'");
    wstr_print ( * ( ( wtoken * ) w )->text );
    printf ("'\n");
    }
  }

void wast_print ( wast * w ) {
  wast_print_inner ( ( wobj * ) w, 0 );
  }
