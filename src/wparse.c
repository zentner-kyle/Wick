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

enum token_family {
  family_stop,
  family_identifier,
  family_literal,
  family_infix,
  family_prefix,
  family_op,
  family_left_op,
  family_right_op,
  family_comment,
  family_whitespace,
  family_special,
  };

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
  wtoken * token = wtoken_new ( family_comment, wstr_copy ( &str ) );
  if ( token ) {
    printf ( "comment " );
    wparser_push_token ( self, token );
    return w_ok;
    }
  else {
    return &wick_out_of_memory;
    }
  }

werror * wparser_lex_indent ( wparser * restrict self ) {
  wstr_trie * node = self->token_table;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  while ( *self->text.start == ' ' || *self->text.start == '\t' ) {
    node = wstr_trie_enter_next ( node, *self->text.start );
    if ( ! node ) {
      return &wick_out_of_memory;
      }
    ++self->text.start;
    }
  str.past_end = self->text.start;
  if ( ! node->val ) {
    token = wtoken_new ( family_whitespace, wstr_copy ( &str ) );
    if ( ! token ) {
      return &wick_out_of_memory;
      }
    node->val = wobj_of ( token );
    }
  else {
    token = wobj_cast ( wtoken, node->val );
    if ( ! token ) {
      return &wcast_error;
      }
    }
  printf ( "indent " );
  wparser_push_token ( self, token );
  return w_ok;
  }

werror * lex_op_from_trie ( wparser * self ) {
  wstr_trie * node = self->token_table;
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
        token = wobj_cast ( wtoken, node->val );
        if ( ! token ) {
          return &wcast_error;
          }
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

werror * lex_identifier_from_trie ( wparser * self ) {
  wstr_trie * node = self->token_table;
  wstr_trie * prev_node = NULL;
  wtoken * token = NULL;
  const char * c = self->text.start;
  const char * c1= c;
  int32_t uc; 
  if ( wutf8_get ( &uc, &c1, self->text.past_end ) != w_ok ) {
    return &invalid_utf8;
    }
  if ( ! is_ident_char ( uc ) ) {
    return &wparse_inactive;
    }
  while ( c != self->text.past_end ) {
    prev_node = node;
    node = wstr_trie_get_next ( node, *c );
    if ( node != prev_node ) {
      ++c;
      }
    else
      {
      break;
      }
    }

  c1 = c;
  if ( wutf8_get ( &uc, &c1, self->text.past_end ) != w_ok ) {
    return &invalid_utf8;
    }
  while ( c != self->text.past_end && is_ident_char ( uc ) ) {
    while ( c != c1 ) {
      node = wstr_trie_enter_next ( node, *c++ );
      }
    if ( wutf8_get ( &uc, &c1, self->text.past_end ) != w_ok ) {
      return &invalid_utf8;
      }
    }

  if ( node->val ) {
    token = wobj_cast ( wtoken, node->val );
    if ( ! token ) {
      return &wcast_error;
      }
    }
  else {
    token = wtoken_new ( family_identifier, wstr_new ( self->text.start, c ) );
    if ( ! token ) {
      return &wick_out_of_memory;
      }
    node->val = wobj_of ( token );
    }
  printf ( "identifier " );
  wparser_push_token ( self, token );
  self->text.start = c;
  return w_ok;
  }

werror * wparser_lex_from_trie ( wparser * self ) {
  werror * status1 = lex_identifier_from_trie ( self );
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

werror * wstr_trie_insert_token ( wstr_trie * node, wstr * str, wtoken * token ) {
  node = wstr_trie_enter ( node, str );
  if ( ! node ) {
    return &wick_out_of_memory;
    }
  node->val = wobj_of ( token );
  return w_ok;
  }

werror * wparser_table_init ( wparser * self ) {
  struct {
    char * text;
    int family;
    int lbp;
    int rbp;
  } table[] = {
      { ","    , family_special  ,  0 ,  0 } , 
      { "="    , family_infix    , 10 , 10 } , 
      { "+"    , family_op       , 60 , 60 } , 
      { "++"   , family_infix    , 60 , 60 } , 
      { "<+>"  , family_infix    , 60 , 60 } , 
      { "-"    , family_op       , 60 , 60 } , 
      { "--"   , family_infix    , 60 , 60 } , 
      { "<->"  , family_infix    , 60 , 60 } , 
      { "*"    , family_op       , 70 , 70 } , 
      { "**"   , family_infix    , 70 , 70 } , 
      { "<*>"  , family_infix    , 70 , 70 } , 
      { "/"    , family_infix    , 70 , 70 } , 
      { "//"   , family_infix    , 70 , 70 } , 
      { "</>"  , family_infix    , 70 , 70 } , 
      { "%"    , family_infix    , 70 , 70 } , 
      { "%%"   , family_infix    , 70 , 70 } , 
      { "<%>"  , family_infix    , 70 , 70 } , 
      { "<-"   , family_infix    , 10 , 10 } , 
      { "->"   , family_infix    , 10 , 10 } , 
      { "<<"   , family_infix    , 60 , 60 } , 
      { ">>"   , family_infix    , 60 , 60 } , 
      { "<<<"  , family_infix    , 60 , 60 } , 
      { ">>>"  , family_infix    , 60 , 60 } , 
      { "<"    , family_infix    , 50 , 50 } , 
      { ">"    , family_infix    , 50 , 50 } , 
      { ">="   , family_infix    , 50 , 50 } , 
      { "<="   , family_infix    , 50 , 50 } , 
      { "=>"   , family_infix    , 50 , 50 } , 
      { "=="   , family_infix    , 50 , 50 } , 
      { "!="   , family_infix    , 50 , 50 } , 
      { "("    , family_left_op  ,  0 ,  0 } , 
      { ")"    , family_right_op ,  0 ,  0 } , 
      { "{"    , family_left_op  ,  0 ,  0 } , 
      { "}"    , family_right_op ,  0 ,  0 } , 
      { "["    , family_left_op  ,  0 ,  0 } , 
      { "]"    , family_right_op ,  0 ,  0 } , 
      { "and"  , family_infix    , 30 , 30 } , 
      { "or"   , family_infix    , 30 , 30 } , 
      { "."    , family_infix    , 90 , 90 } , 
      { ";"    , family_op       ,  0 ,  0 } , 
      { "!"    , family_op       , 40 , 40 } , 
      { "!!"   , family_op       , 40 , 40 } , 
      { "\\"   , family_op       , 10 , 10 } , 
      { "\\\\" , family_op       , 10 , 10 } , 
      { "::"   , family_infix    , 20 , 20 } , 
      { "@"    , family_op       , 20 , 20 } , 
      { "^"    , family_op       , 40 , 40 } , 
      { "&"    , family_op       , 40 , 40 } , 
      { "|"    , family_op       , 40 , 40 } , 
      { "~"    , family_op       , 40 , 40 } , 
      { ".."   , family_op       , 60 , 60 } , 
      { "$"    , family_op       , 20 , 20 } , 
      { ":"    , family_special  ,  0 ,  0 } , 
      { ""     , 0               ,  0 ,  0 } , 
    };
  wstr * str;
  int i = 0;
  while ( table[i].family != family_stop ) {
    str = wstr_new ( table[i].text, 0);
    werror * e = wstr_trie_insert_token (
        self->token_table,
        str,
        wtoken_new_op ( table[i].family, str, table[i].lbp, table[i].rbp ) );
    ++i;
    if ( e != w_ok ) {
      return e;
      }
    }
  return w_ok;
  } 

werror * wparser_lex_quote ( wparser * self ) {
  const char * start = self->text.start;
  wtoken * token = NULL;
  const char * c = self->text.start;
  const char * c1;
  wstr_trie * node = wstr_trie_enter_next ( self->token_table, *c++ );
  wstr * str;
  int32_t uc; 
  while ( c != self->text.past_end ) {
    node = wstr_trie_enter_next ( node, *c );
    if (*c == *start ) {
      node = wstr_trie_enter_next ( node, *c++ );
      break;
      }
    ++c;
    if ( c != self->text.past_end && *c == '\\' ) {
      if ( c + 1 == self->text.past_end ) {
        return &invalid_token;
        }
      node = wstr_trie_enter_next ( node, *c++ );
      c1 = c;
      if ( wutf8_get ( &uc, &c, self->text.past_end ) != w_ok ) {
        return &invalid_utf8;
        }
      while ( c1 != c ) {
        node = wstr_trie_enter_next ( node, *c1++ );
        }
      }
    }
  if ( node->val ) {
    token = wobj_cast ( wtoken, node->val );
    if ( ! token ) {
      return &wcast_error;
      }
    }
  else {
    str = wstr_new ( start, c );
    if ( ! str ) {
      return &wick_out_of_memory;
      }
    if ( ! wutf8_valid ( str ) ) {
      wstr_free ( str );
      return &invalid_utf8;
      }
    token = wtoken_new ( family_literal, str );
    if ( ! token ) {
      wstr_free ( str );
      return &wick_out_of_memory;
      }
    }
  printf ( "quote " );
  wparser_push_token ( self, token );
  self->text.start = c;
  return w_ok;
  }

werror * wparser_lex_number ( wparser * self ) {
  const char * start = self->text.start;
  wstr_trie * node = self->token_table;
  wtoken * token = NULL;
  const char * c = self->text.start;
  wstr * str;
  while ( c != self->text.past_end && isdigit ( *c ) ) {
    node = wstr_trie_enter_next ( node, *c++ );
    }
  if ( node->val ) {
    token = wobj_cast ( wtoken, node->val );
    if ( ! token ) {
      return &wcast_error;
      }
    }
  else {
    str = wstr_new ( start, c );
    token = wtoken_new ( family_literal, str );
    if ( ! token ) {
      wstr_free ( str );
      return &wick_out_of_memory;
      }
    }
  printf ( "number " );
  wparser_push_token ( self, token );
  self->text.start = c;
  return w_ok;
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
  return w_ok;
  }

wparser * wparser_new ( wstr * text ) {
  new_line.type = wtype_wtoken;
  new_line.text = wstr_new ( "\n", 0 );
  new_line.family = family_special;
  new_line.lbp = 0;
  new_line.rbp = 0;
  start_of_file.type = wtype_wtoken;
  start_of_file.text = wstr_new ( "(start of file)", 0 );
  start_of_file.family = family_special;
  start_of_file.lbp = 0;
  start_of_file.rbp = 0;
  end_of_file.type = wtype_wtoken;
  end_of_file.text = wstr_new ( "(end of file)", 0 );
  end_of_file.family = family_special;
  end_of_file.lbp = 0;
  end_of_file.rbp = 0;
  root.type = wtype_wtoken;
  root.text = wstr_new ( "(root)", 0 );
  root.family = family_special;
  root.lbp = 0;
  root.rbp = 0;
  wparser * p = walloc_simple ( wparser, 1 );
  if ( ! p ) {
    return NULL;
    }
  p->type = wtype_wparser;
  p->all_text = *text;
  p->text = *text;
  p->token_table = wstr_trie_new ();
  p->stop = false;
  if ( ! p->token_table ) {
    wstr_trie_free ( p->token_table );
    free (p);
    return NULL;
    }
  else {
    wparser_table_init ( p );
    wparser_push_token ( p, &start_of_file );
    }
  p->root = wast_list_new ( &root );
  p->accum = ( wast * ) p->root;
  p->state = wparser_expr_complete;
  return p;
  }

bool is_op ( wtoken * t ) {
  switch ( t->family ) {
    case family_op:
    case family_infix:
    case family_prefix:
    case family_left_op:
    case family_right_op:
    case family_special:
      return true;
    default:
      return false;
    }
  }

bool is_expr ( wtoken * t ) {
  switch ( t->family ) {
    case family_identifier:
    case family_literal:
      return true;
    default:
      return false;
    }
  }

werror wparser_bad_expr;
werror wparser_internal_error;

void wparser_remove_space_before ( warray_wobj_ptr_iter * i ) {
  while ( true ) {
    wtoken * potential_whitespace = wobj_cast ( wtoken, *warray_wobj_ptr_at ( i, -1 ) );
    if ( potential_whitespace && potential_whitespace->family == family_whitespace ) {
      warray_wobj_ptr_remove_at ( i, -1 );
      warray_wobj_ptr_prev ( i );
      }
    else {
      break;
      }
    }
  }

void wparser_remove_space_after ( warray_wobj_ptr_iter * i ) {
  while ( true ) {
    wtoken * potential_whitespace = wobj_cast ( wtoken, *warray_wobj_ptr_at ( i, 1 ) );
    if ( potential_whitespace && potential_whitespace->family == family_whitespace ) {
      warray_wobj_ptr_remove_at ( i, 1 );
      }
    else {
      break;
      }
    }
  }

werror * wparser_update_atom ( wparser * self, wtoken * t ) {
  switch ( self->state ) {
    case wparser_expr_complete: {
      self->accum = wast_parent_exprlist ( self->accum );
      wast_add_rightmost ( self->accum, wobj_of ( t ) );
      break;
      }
    case wparser_op_pending: {
      wast_prefix * prefix = wobj_cast ( wast_prefix, self->accum );
      if ( prefix ) {
        prefix->child = wobj_of ( t );
        self->state = wparser_expr_complete;
        break;
        }
      wast_infix * infix = wobj_cast ( wast_infix, self->accum );
      if ( infix ) {
        infix->right = wobj_of ( t );
        self->state = wparser_expr_complete;
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

werror * wparser_update_infix ( wparser * self, wtoken * t ) {
  switch ( self->state ) {
    case wparser_expr_complete: {
      wast_infix * w = walloc_simple ( wast_infix, 1 );
      w->op = t;
      w->type = wtype_wast_infix;
      while ( self->accum->op->rbp >= t->lbp ) {
        self->accum = self->accum->parent;
        }
      w->parent = self->accum;
      w->left = wast_remove_rightmost ( self->accum );
      w->right = NULL;
      wast_add_rightmost ( self->accum, wobj_of ( w ) );
      self->accum = ( wast * ) w;
      self->state = wparser_op_pending;
      break;
      }
    case wparser_op_pending:
      return &wparser_bad_expr;
    default:
      assert ( false );
    }
  return w_ok;
  }

werror * wparser_update_prefix ( wparser * self, wtoken * t ) {
  wast_prefix * w = walloc_simple ( wast_prefix, 1 );
  w->type = wtype_wast_prefix;
  w->op = t;
  w->child = NULL;
  wast_add_rightmost ( self->accum, wobj_of ( w ) );
  self->accum = (wast *) w;
  return w_ok;
  }

werror * wparser_update_left_op ( wparser * self, wtoken * t ) {
  return w_ok;
  }

werror * wparser_update_right_op ( wparser * self, wtoken * t ) {
  return w_ok;
  }

werror * wparser_update_op ( wparser * self, wtoken * t ) {
  switch ( self->state ) {
    case wparser_expr_complete:
      return wparser_update_infix ( self, t );
    case wparser_op_pending:
      return wparser_update_prefix ( self, t );
    default:
      assert ( false );
    }
  }

werror * wparser_update ( wparser * self, wtoken * t ) {
  switch ( t->family ) {
    case family_identifier:
    case family_literal:
      return wparser_update_atom ( self, t );
    case family_op:
      return wparser_update_op ( self, t );
    case family_infix:
      return wparser_update_infix ( self, t );
    case family_prefix:
      return wparser_update_prefix ( self, t );
    case family_left_op:
      return wparser_update_left_op ( self, t );
    case family_right_op:
      return wparser_update_right_op ( self, t );
    case family_whitespace:
      return wparser_update_whitespace ( self, t );
    case family_special:
      return wparser_update_special ( self, t );
    default:
      assert ( 0 );
      return w_ok;
    }
  }
