#include <wparse.h>
#include <wcall.h>
#include <walloc.h>
#include <werror.h>
#include <ctype.h>
#include <wutf8.h>
#include <assert.h>
#include <wobj.h>
#include <wast.h>

wdefine_composite ( wparser );

werror * wparser_update ( wparser * self, wtoken * t );

void wparser_error ( wparser * self, werror * error ) {
  self->stop = true;
  }

bool wparser_push_token ( wparser * self, wtoken * token ) {
  printf ( "pushing token '" );
  wstr_print ( *token->text );
  printf ( "'\n" );
  self->last_token = token;
  printf ( "Before update: \n" );
  wast_print ( ( wast * ) self->root );
  wparser_update ( self, token );
  return ! self->stop;
  }

bool is_ident_char ( int32_t c ) {
  if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_')) {
    return true;
  } else {
#include <is_letter_tree.c>
    }
  }

werror wparse_inactive;
werror invalid_token;

werror * wparser_lex_comment ( wparser * restrict self ) {
  if ( *self->text.start != '#' ) {
    return &wparse_inactive;
    }
  wstr str = wstr_empty ();
  str.start = self->text.start;
  while ( *self->text.start != '\n' ) {
    ++self->text.start;
    }
  str.past_end = self->text.start;
  wtoken * token = wtoken_new ( wstr_copy ( &str ) );
  if ( token ) {
    printf ( "comment " );
    wparser_push_token ( self, token );
    return w_ok;
    }
  else {
    return &wick_out_of_memory;
    }
  }

werror * wparser_unique_wtoken (
    wstr * str,
    wtoken ** token,
    wtable_wstr_ptr_to_wtoken_ptr * table ) {
  werror * e = w_ok;
  werr ( e, wtable_wstr_ptr_to_wtoken_ptr_get ( table, str, token ) );
  if ( e == w_ok ) {
    return w_ok;
    }
  else if ( e == &wtable_not_found ) {
    wstr * str_cpy = wstr_copy ( str );
    *token = wtoken_new ( str_cpy );
    if ( *token ) {
      wtable_wstr_ptr_to_wtoken_ptr_set ( table, str_cpy, *token, &null_wcall );
      return w_ok;
      }
    else {
      wstr_free ( str_cpy );
      return &wick_out_of_memory;
      }
    }
  else {
    return e;
    }
  }

werror * wparser_lex_indent ( wparser * restrict self ) {
  werror * e = w_ok;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start;
  while ( *str.past_end == ' ' || *str.past_end == '\t' ) {
    ++str.past_end;
    }
  werr ( e, wparser_unique_wtoken ( &str, &token, self->indent_table ) );
  if ( e == w_ok ) {
    self->text.start = str.past_end;
    printf ( "indent " );
    wparser_push_token ( self, token );
    }
  return e;
  }

werror * lex_from_trie ( wparser * self, wstr_trie * node ) {
  wstr_trie * prev_node = NULL;
  wtoken * token = NULL;
  const char * c = self->text.start;
  const char * last;

  while ( c != self->text.past_end ) {
    prev_node = node;
    node = wstr_trie_get_next ( node, *c );
    if ( node != prev_node ) {
      if ( node->val ) {
        last = c;
        token = ( wtoken * ) node->val;
        }
      ++c;
      }
    else
      {
      break;
      }
    }
  if ( token ) {
    self->text.start = last + 1;
    printf ( "op " );
    wparser_push_token ( self, token );
    return w_ok;
    }
  return &wparse_inactive;
  }

werror * lex_op_from_trie ( wparser * self ) {
  printf ( "in lex_op_from_trie\n" );
  switch ( self->state ) {
    case wparser_op_pending: {
      printf ("lexing from prefix trie!\n");
      return lex_from_trie ( self, self->prefix_table );
      }
    case wparser_expr_start: {
      printf ("lexing from infix trie!\n");
      return lex_from_trie ( self, self->infix_table );
      }
    default:
      printf ( "inconsistent state!\n" );
      assert ( false );
      return w_ok;
    }
  }

werror * lex_identifier ( wparser * self ) {
  printf ( "in lex_identifier\n" );
  werror * e = w_ok;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start;
  const char * c;
  uint32_t uc;
  if ( wutf8_get ( &uc, &str.past_end, self->text.past_end ) != w_ok ) {
    return &invalid_utf8;
    }
  if ( ! is_ident_char ( uc ) ) {
    printf ( "was not identifier, started with '%c'\n", (char) uc );
    return &wparse_inactive;
    }
  while ( str.past_end != self->text.past_end && is_ident_char ( uc ) ) {
    c = str.past_end;
    if ( wutf8_get ( &uc, &str.past_end, self->text.past_end ) != w_ok ) {
      return &invalid_utf8;
      }
    }
  str.past_end = c;
    
  werr ( e, wparser_unique_wtoken ( &str, &token, self->identifier_table ) );
  if ( e == w_ok ) {
    printf ( "identifier " );
    wparser_push_token ( self, token );
    self->text.start = str.past_end;
    }
  return e;
  }

werror * wparser_lex_from_trie ( wparser * self ) {
  werror * status1 = lex_identifier ( self );
  werror * status2 = lex_op_from_trie ( self );
  if ( status1 == status2 ) {
    return status1;
    }
  else if ( status1 == &wparse_inactive ) {
    return status2;
    }
  else if ( status2 == &wparse_inactive ) {
    return status1;
    }
  else {
    return status1;
    }
  }

wtoken new_line;
wtoken start_of_file;
wtoken end_of_file;
wtoken root;
wtoken colon;
wtoken comma;

werror * wstr_trie_insert_token ( wstr_trie * node, wtoken * token ) {
  node = wstr_trie_enter ( node, token->text );
  if ( ! node ) {
    return &wick_out_of_memory;
    }
  node->val = wobj_of ( token );
  return w_ok;
  }

werror * wparser_insert_pair ( wparser * self, char * leftc, char * rightc ) {
  werror * e = w_ok;
  wtoken_left * left = wtoken_left_new ( wstr_new ( leftc, 0 ) );
  werr ( e, wstr_trie_insert_token ( self->prefix_table, ( wtoken * ) left ) );
  werr ( e, wstr_trie_insert_token ( self->infix_table, ( wtoken * ) left ) );
  wtoken_right * right = wtoken_right_new ( wstr_new ( rightc, 0 ) );
  werr ( e, wstr_trie_insert_token ( self->prefix_table, ( wtoken * ) right ) );
  werr ( e, wstr_trie_insert_token ( self->infix_table, ( wtoken * ) right ) );
  if ( e == w_ok ) {
    left->right = right;
    right->left = left;
    }
  return e;
  }

werror * wparser_table_init ( wparser * self ) {
  werror * e = w_ok;
  struct {
    char * text;
    int lbp;
    int rbp;
  } infix_table[] = {
      { "="    , 10 , 10 } , 
      { "++"   , 60 , 60 } , 
      { "<+>"  , 60 , 60 } , 
      { "--"   , 60 , 60 } , 
      { "<->"  , 60 , 60 } , 
      { "**"   , 70 , 70 } , 
      { "<*>"  , 70 , 70 } , 
      { "/"    , 70 , 70 } , 
      { "//"   , 70 , 70 } , 
      { "</>"  , 70 , 70 } , 
      { "%"    , 70 , 70 } , 
      { "%%"   , 70 , 70 } , 
      { "<%>"  , 70 , 70 } , 
      { "<-"   , 10 , 10 } , 
      { "->"   , 10 , 10 } , 
      { "<<"   , 60 , 60 } , 
      { ">>"   , 60 , 60 } , 
      { "<<<"  , 60 , 60 } , 
      { ">>>"  , 60 , 60 } , 
      { "<"    , 50 , 50 } , 
      { ">"    , 50 , 50 } , 
      { ">="   , 50 , 50 } , 
      { "<="   , 50 , 50 } , 
      { "=>"   , 50 , 50 } , 
      { "=="   , 50 , 50 } , 
      { "!="   , 50 , 50 } , 
      { "and"  , 30 , 30 } , 
      { "or"   , 30 , 30 } , 
      { "."    , 90 , 90 } , 
      { "::"   , 20 , 20 } , 
      { ".."   , 60 , 60 } , 
      { "*"    , 70 , 70 } , 
      { "+"    , 60 , 60 } , 
      { "-"    , 60 , 60 } , 
      { "!"    , 40 , 40 } , 
      { "!!"   , 40 , 40 } , 
      { "\\"   , 10 , 10 } , 
      { "\\\\" , 10 , 10 } , 
      { "@"    , 20 , 20 } , 
      { "^"    , 40 , 40 } , 
      { "&"    , 40 , 40 } , 
      { "|"    , 40 , 40 } , 
      { "~"    , 40 , 40 } , 
      { "$"    , 20 , 20 } , 
      { ":"    ,  0 ,  0 } , 
      { ","    ,  0 ,  0 } , 
      { ""     ,  0 ,  0 } ,
    };

  for ( int i = 0; infix_table[i].text[0]; i++ ) {
    wstr * str = wstr_new ( infix_table[i].text, 0);
    werr ( e, wstr_trie_insert_token (
        self->infix_table,
        ( wtoken * ) wtoken_infix_new ( str,
            infix_table[i].lbp, infix_table[i].rbp ) ) );
    }

  struct {
    char * text;
    int rbp;
  } prefix_table[] = {
      { "+"    , 60 } , 
      { "-"    , 60 } , 
      { "*"    , 70 } , 
      { "!"    , 40 } , 
      { "!!"   , 40 } , 
      { "\\"   , 10 } , 
      { "\\\\" , 10 } , 
      { "@"    , 20 } , 
      { "^"    , 40 } , 
      { "&"    , 40 } , 
      { "|"    , 40 } , 
      { "~"    , 40 } , 
      { ".."   , 60 } , 
      { "$"    , 20 } , 
      { "!"    , 40 } , 
      { "!!"   , 40 } , 
      { "\\"   , 10 } , 
      { "\\\\" , 10 } , 
      { "@"    , 20 } , 
      { "^"    , 40 } , 
      { "&"    , 40 } , 
      { "|"    , 40 } , 
      { "~"    , 40 } , 
      { "$"    , 20 } , 
      { ""     ,  0 } ,
    };

  for ( int i = 0; prefix_table[i].text[0]; i++ ) {
    wstr * str = wstr_new ( prefix_table[i].text, 0);
    werr ( e, wstr_trie_insert_token (
        self->prefix_table,
        ( wtoken * ) wtoken_prefix_new ( str, prefix_table[i].rbp ) ) );
    }

  werr ( e, wparser_insert_pair ( self, "(", ")" ) );
  werr ( e, wparser_insert_pair ( self, "[", "]" ) );
  werr ( e, wparser_insert_pair ( self, "{", "}" ) );
  werr ( e, wstr_trie_insert_token ( self->infix_table, &comma ) );
  werr ( e, wstr_trie_insert_token ( self->infix_table, &colon ) );

  return e;
  } 

werror * wparser_lex_quote ( wparser * self ) {
  werror * e = w_ok;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start + 1;
  while ( str.past_end < self->text.past_end && *str.past_end != *str.start ) {
    if ( *str.past_end == '\\' ) {
      ++str.past_end;
      }
    ++str.past_end;
    }
  if ( str.past_end >= self->text.past_end ) {
    return &invalid_token;
    }
  ++str.past_end;
  werr ( e, wparser_unique_wtoken ( &str, &token, self->literal_table ) );
  if ( e == w_ok ) {
    printf ( "quote " );
    wparser_push_token ( self, token );
    self->text.start = str.past_end;
    }
  return e;
  }

werror * wparser_lex_number ( wparser * self ) {
  werror * e = w_ok;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start + 1;
  while ( str.past_end != self->text.past_end && isdigit ( *str.past_end) ) {
    ++str.past_end;
    }
  werr ( e, wparser_unique_wtoken ( &str, &token, self->literal_table ) );
  if ( e == w_ok ) {
    printf ( "number " );
    wparser_push_token ( self, token );
    self->text.start = str.past_end;
    }
  return e;
  }

werror * wparser_lex_literal ( wparser * self ) {
  if ( *self->text.start == '\'' || *self->text.start == '"' || *self->text.start == '`') {
    return wparser_lex_quote ( self );
    }
  else if ( isdigit ( *self->text.start ) ) {
    return wparser_lex_number ( self );
    }
  return &wparse_inactive;
  }

werror * wparser_lex ( wparser * self ) {
  bool active = true;
  bool inner_loop = true;
  while ( ! self->stop && wstr_size ( self->text ) && active ) {
    active = false;
    inner_loop = true;
    if ( *self->text.start == '\n' ) {
      ++self->text.start;
      if ( self->last_token != &new_line ) {
        wparser_push_token ( self, &new_line );
        }
      }
    if ( self->last_token == &start_of_file || self->last_token == &new_line ) {
      active |= wparser_lex_indent ( self ) == w_ok;
      }
    while (inner_loop) {
      printf ("inner loop\n");
      inner_loop = wparser_lex_from_trie ( self ) == w_ok;
      active |= inner_loop;
      if ( *self->text.start == ' ' || *self->text.start == '\t' ) {
        ++self->text.start;
        active = true;
        inner_loop = true;
        }
      }
    if ( *self->text.start == '#' ) {
      active |= wparser_lex_comment ( self ) == w_ok;
      }
    active |= wparser_lex_literal ( self ) == w_ok;
    }
  if ( self->text.start == self->text.past_end ) {
    wparser_push_token ( self, &end_of_file );
    }
  printf ("returning from lex.\n");
  return w_ok;
  }

wparser * wparser_new ( wstr * text ) {
  werror * e = w_ok;
  werr ( e, wtoken_init ( &new_line, wstr_new ( "\n", 0 ) ) );
  werr ( e, wtoken_init ( &start_of_file, wstr_new ( "(start of file)", 0 ) ) );
  werr ( e, wtoken_init ( &end_of_file, wstr_new ( "(end of file)", 0 ) ) );
  werr ( e, wtoken_init ( &root, wstr_new ( "(root)", 0 ) ) );
  werr ( e, wtoken_init ( &comma, wstr_new ( ",", 0 ) ) );
  werr ( e, wtoken_init ( &colon, wstr_new ( ":", 0 ) ) );
  if ( e != w_ok )
    {
    return NULL;
    }
  wparser * p = walloc_simple ( wparser, 1 );
  if ( ! p ) {
    return NULL;
    }
  p->type = wtype_wparser;
  p->all_text = *text;
  p->text = *text;
  p->prefix_table = wstr_trie_new ();
  p->infix_table = wstr_trie_new ();
  p->literal_table = wtable_wstr_ptr_to_wtoken_ptr_new ();
  p->identifier_table = wtable_wstr_ptr_to_wtoken_ptr_new ();
  p->indent_table = wtable_wstr_ptr_to_wtoken_ptr_new ();
  p->stop = false;
  if ( ! p->prefix_table || ! p->infix_table || ! p->literal_table || ! p->identifier_table || ! p->indent_table ) {
    wstr_trie_free ( p->prefix_table );
    wstr_trie_free ( p->infix_table );
    wtable_wstr_ptr_to_wtoken_ptr_delete ( p->literal_table );
    wtable_wstr_ptr_to_wtoken_ptr_delete ( p->identifier_table );
    wtable_wstr_ptr_to_wtoken_ptr_delete ( p->indent_table );
    free (p);
    return NULL;
    }
  p->root = wast_list_new ( &root );
  p->accum = ( wast * ) p->root;
  printf ("Set root.\n");
  p->state = wparser_expr_start;

  wparser_table_init ( p );
  wparser_push_token ( p, &start_of_file );
  return p;
  }

werror wparser_bad_expr;
werror wparser_internal_error;

werror * wparser_update_atom ( wparser * self, wtoken * t ) {
  printf ( "in wparser_update_atom\n" );
  switch ( self->state ) {
    case wparser_expr_start: {
      printf ( "wparser_expr_start\n" );
      self->accum = wast_parent_exprlist ( self->accum );
      printf ("self->accum = %p\n", self->accum);
      wast_add_rightmost ( self->accum, wobj_of ( t ) );
      break;
      }
    case wparser_op_pending: {
      wast_prefix * prefix = wobj_cast ( wast_prefix, self->accum );
      if ( prefix ) {
        prefix->child = wobj_of ( t );
        self->state = wparser_expr_start;
        break;
        }
      wast_infix * infix = wobj_cast ( wast_infix, self->accum );
      if ( infix ) {
        infix->right = wobj_of ( t );
        self->state = wparser_expr_start;
        break;
        }
      assert ( false );
      }
    }
  return w_ok;
  }

werror * wparser_update_whitespace ( wparser * self, wtoken * t ) {
  return w_ok;
  }

werror * wparser_update_special ( wparser * self, wtoken * t ) {
  return w_ok;
  }

int wparser_get_rbp ( wtoken * t ) {
  wvar_of ( wi, wtoken_infix, t ) {
    return wi->rbp;
    } wvar_end
  else wvar_of ( wp, wtoken_prefix, t ) {
    return wp->rbp;
    } wvar_end
  else {
    return 0;
    }
  }

werror * wparser_update_infix ( wparser * self, wtoken_infix * t ) {
  printf ("updating infix.\n");
  switch ( self->state ) {
    case wparser_expr_start: {
      wast_infix * w = walloc_simple ( wast_infix, 1 );
      w->op = t;
      w->type = wtype_wast_infix;
      while ( wparser_get_rbp ( self->accum->op ) >= t->lbp ) {
        self->accum = self->accum->parent;
        }
      w->parent = self->accum;
      w->left = wast_remove_rightmost ( self->accum );
      w->right = NULL;
      wast_add_rightmost ( self->accum, wobj_of ( w ) );
      self->accum = ( wast * ) w;
      printf ("now accum = \n");
      wast_print ( self->accum );

      self->state = wparser_op_pending;
      break;
      }
    case wparser_op_pending: {
      return &wparser_bad_expr;
      }
    default:
      assert ( false );
    }
  return w_ok;
  }

werror * wparser_update_prefix ( wparser * self, wtoken_prefix * t ) {
  wast_prefix * w = walloc_simple ( wast_prefix, 1 );
  w->type = wtype_wast_prefix;
  w->op = t;
  w->child = NULL;
  w->parent = self->accum;
  wast_add_rightmost ( self->accum, wobj_of ( w ) );
  self->accum = (wast *) w;
  return w_ok;
  }

werror * wparser_update_left ( wparser * self, wtoken_left * t ) {
  return w_ok;
  }

werror * wparser_update_right ( wparser * self, wtoken_right * t ) {
  return w_ok;
  }

werror * wparser_update ( wparser * self, wtoken * t ) {
  printf ("accum = \n");
  wast_print ( self->accum );

  wvar_of ( wi, wtoken_infix, t ) {
    return wparser_update_infix ( self, wi );
    } wvar_end
  else wvar_of ( wp, wtoken_prefix, t ) {
    return wparser_update_prefix ( self, wp );
    } wvar_end
  else wvar_of ( wl, wtoken_left, t ) {
    return wparser_update_left ( self, wl );
    } wvar_end
  else wvar_of ( wr, wtoken_right, t ) {
    return wparser_update_right ( self, wr );
    } wvar_end
  else {
    printf ("did not recognize token as any type.\n");
    printf ("type = ");
    wstr_println ( t->type->name );
    if ( *t->text->start == ' ' || *t->text->start == '\t' ) {
      /* Ignore spaces. */
      return w_ok;
      }
    else {
      wparser_update_atom ( self, t );
      }
    }
  return w_ok;
  }
