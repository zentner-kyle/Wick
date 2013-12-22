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

werror wparser_bad_expr;
werror wparser_internal_error;
werror wparser_inactive;
werror wparser_invalid_token;

static werror * wparser_update ( wparser * self, wtoken * t );

static char * wparser_get_state_name ( enum wparser_state state ) {
  switch ( state ) {
    case wparser_prefix_context:
    return "prefix";
    case wparser_infix_context:
    return "infix";
    case wparser_line_start_context:
    return "line start";
    default:
      {}
    }
  return "unknown";
  }

static void wparser_set_state ( wparser * self, enum wparser_state state ) {
  /*char * old_state = wparser_get_state_name ( self->state );*/
  /*char * new_state = wparser_get_state_name ( state );*/
  /*printf ( "state change from %s to %s\n",*/
      /*old_state, new_state );*/
  self->state = state;
  }

static void wparser_debug_state ( wparser * self, wtoken * token ) {
  printf ( "pushing token '" );
  wstr_print ( *token->text );
  printf ( "'\n" );
  printf ( "Before update: \n" );
  wast_print ( ( wast * ) self->root );
  printf ( "self->accum = \n");
  wast_print ( self->accum );
  }

static werror * wparser_push_token ( wparser * self, wtoken * token ) {
  /*wparser_debug_state ( self, token );*/
  return wparser_update ( self, token );
  }

static bool is_ident_start_char ( uint32_t c ) {
  if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_')) {
    return true;
  } else {
#include <is_letter_tree.c>
    }
  }

static bool is_ident_char ( uint32_t c ) {
  return isdigit ( c ) || is_ident_start_char ( c );
  }

static werror * wparser_lex_comment ( wparser * restrict self ) {
  if ( *self->text.start != '#' ) {
    return &wparser_inactive;
    }
  wstr str = wstr_empty ();
  str.start = self->text.start;
  while ( *self->text.start != '\n' ) {
    ++self->text.start;
    }
  str.past_end = self->text.start;
  wtoken * token = wtoken_new ( wstr_copy ( &str ) );
  if ( token ) {
    return wparser_push_token ( self, token );
    }
  else {
    return &wick_out_of_memory;
    }
  }

static werror * wparser_unique_wtoken (
    wstr * str,
    wtoken ** token,
    wtoken * (* on_missing)( wstr * ),
    wtable_wstr_ptr_to_wtoken_ptr * table ) {
  werror * e = w_ok;
  werr ( e, wtable_wstr_ptr_to_wtoken_ptr_get ( table, str, token ) );
  if ( e == w_ok ) {
    return w_ok;
    }
  else if ( e == &wtable_not_found ) {
    wstr * str_cpy = wstr_copy ( str );
    *token = on_missing ( str_cpy );
    if ( *token ) {
      wtable_wstr_ptr_to_wtoken_ptr_set ( table, str_cpy, *token );
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

static werror * wparser_lex_indent ( wparser * restrict self ) {
  werror * e = w_ok;
  wtoken * token;
  if (self->state != wparser_line_start_context ) {
    return &wparser_inactive;
    }
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start;
  while ( *str.past_end == ' ' || *str.past_end == '\t' ) {
    ++str.past_end;
    }
  werr ( e, wparser_unique_wtoken ( &str, &token, wtoken_indent_new, self->indent_table ) );
  if ( e == w_ok ) {
    self->text.start = str.past_end;
    e = wparser_push_token ( self, token );
    }
  else {
    }
  return e;
  }

static werror * lex_from_trie ( wparser * self, wstr_trie * node ) {
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
    return wparser_push_token ( self, token );
    }
  return &wparser_inactive;
  }

static werror * lex_op_from_trie ( wparser * self ) {
  switch ( self->state ) {
    case wparser_infix_context: {
      return lex_from_trie ( self, self->infix_table );
      }
    case wparser_line_start_context: {
      wparser_set_state ( self, wparser_prefix_context );
      }
    case wparser_prefix_context: {
      return lex_from_trie ( self, self->prefix_table );
      }
    }
  return &werror_internal;
  }

static werror * lex_identifier ( wparser * self ) {
  werror * e = w_ok;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start;
  const char * c = str.start;
  uint32_t uc;
  if ( wutf8_get ( &uc, &c, self->text.past_end ) != w_ok ) {
    return &invalid_utf8;
    }
  if ( ! is_ident_start_char ( uc ) ) {
    return &wparser_inactive;
    }
  while ( str.past_end != self->text.past_end && is_ident_char ( uc ) ) {
    str.past_end = c;
    if ( wutf8_get ( &uc, &c, self->text.past_end ) != w_ok ) {
      if ( str.past_end == self->text.past_end) {
        break;
        }
      return &invalid_utf8;
      }
    }
    
  werr ( e, wparser_unique_wtoken ( &str, &token, wtoken_new, self->identifier_table ) );
  if ( e == w_ok ) {
    e = wparser_push_token ( self, token );
    self->text.start = str.past_end;
    }
  return e;
  }

static werror * wparser_lex_from_trie ( wparser * self ) {
  werror * status1 = lex_identifier ( self );
  werror * status2 = lex_op_from_trie ( self );
  if ( status1 == status2 ) {
    return status1;
    }
  else if ( status1 == &wparser_inactive ) {
    return status2;
    }
  else if ( status2 == &wparser_inactive ) {
    return status1;
    }
  else {
    return status1;
    }
  }

wtoken root;
wtoken_infix comma;
wtoken_infix space;
wtoken_left lparen;
wtoken_right rparen;
wtoken_left lbrace;
wtoken_right rbrace;
wtoken_left lbracket;
wtoken_right rbracket;

static werror * wstr_trie_insert_token ( wstr_trie * node, wtoken * token ) {
  node = wstr_trie_enter ( node, token->text );
  if ( ! node ) {
    return &wick_out_of_memory;
    }
  node->val = wobj_of ( token );
  return w_ok;
  }

static werror * wparser_insert_pair ( wparser * self, wtoken_left * left, wtoken_right * right ) {
  werror * e = w_ok;
  werr ( e, wstr_trie_insert_token ( self->prefix_table, ( wtoken * ) left ) );
  werr ( e, wstr_trie_insert_token ( self->infix_table, ( wtoken * ) left ) );
  werr ( e, wstr_trie_insert_token ( self->prefix_table, ( wtoken * ) right ) );
  werr ( e, wstr_trie_insert_token ( self->infix_table, ( wtoken * ) right ) );
  if ( e == w_ok ) {
    left->right = right;
    right->left = left;
    }
  return e;
  }

static werror * wparser_table_init ( wparser * self ) {
  werror * e = w_ok;
  struct {
    char * text;
    int lbp;
    int rbp;
    bool starts_indent;
  } infix_table[] = {
      { "="    ,  5 ,  4 ,  true } , 
      { "<-"   , 10 ,  9 ,  true } , 
      { "->"   , 10 ,  9 ,  true } , 
      { ":"    , 15 , 14 ,  true } , 
      { "\\"   , 10 , 10 , false } , 
      { "\\\\" , 10 , 10 , false } , 
      { "++"   , 60 , 60 , false } , 
      { "<+>"  , 60 , 60 , false } , 
      { "--"   , 60 , 60 , false } , 
      { "<->"  , 60 , 60 , false } , 
      { "**"   , 70 , 69 , false } , 
      { "<*>"  , 70 , 70 , false } , 
      { "/"    , 70 , 70 , false } , 
      { "//"   , 70 , 70 , false } , 
      { "</>"  , 70 , 70 , false } , 
      { "%"    , 70 , 70 , false } , 
      { "%%"   , 70 , 70 , false } , 
      { "<%>"  , 70 , 70 , false } , 
      { "<<"   , 60 , 60 , false } , 
      { ">>"   , 60 , 60 , false } , 
      { "<<<"  , 60 , 60 , false } , 
      { ">>>"  , 60 , 60 , false } , 
      { "<"    , 50 , 50 , false } , 
      { ">"    , 50 , 50 , false } , 
      { ">="   , 50 , 50 , false } , 
      { "<="   , 50 , 50 , false } , 
      { "=>"   , 50 , 50 , false } , 
      { "=="   , 50 , 50 , false } , 
      { "!="   , 50 , 50 , false } , 
      { "and"  , 30 , 30 , false } , 
      { "or"   , 30 , 30 , false } , 
      { "."    , 90 , 90 , false } , 
      { "::"   , 20 , 20 , false } , 
      { ".."   , 60 , 60 , false } , 
      { "*"    , 70 , 70 , false } , 
      { "+"    , 60 , 60 , false } , 
      { "-"    , 60 , 60 , false } , 
      { "!"    , 40 , 40 , false } , 
      { "!!"   , 40 , 40 , false } , 
      { "@"    , 20 , 20 , false } , 
      { "^"    , 40 , 40 , false } , 
      { "&"    , 40 , 40 , false } , 
      { "|"    , 40 , 40 , false } , 
      { "~"    , 40 , 40 , false } , 
      { "$"    , 20 , 20 , false } , 
      { ""     ,  0 ,  0 , false } ,
    };

  for ( int i = 0; infix_table[i].text[0]; i++ ) {
    wstr * str = wstr_new ( infix_table[i].text, 0);
    werr ( e, wstr_trie_insert_token (
        self->infix_table,
        ( wtoken * ) wtoken_infix_new ( str,
            infix_table[i].lbp, infix_table[i].rbp,
            infix_table[i].starts_indent ) ) );
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
      { ""     ,  0 } ,
    };

  for ( int i = 0; prefix_table[i].text[0]; i++ ) {
    wstr * str = wstr_new ( prefix_table[i].text, 0);
    werr ( e, wstr_trie_insert_token (
        self->prefix_table,
        ( wtoken * ) wtoken_prefix_new ( str, prefix_table[i].rbp ) ) );
    }

  struct {
    char * text;
    int rbp;
  } prefix_keywords[] = {
      { "if"      , 13 } ,
      { "for"     , 13 } ,
      { "import"  , 13 } ,
      { "while"   , 13 } ,
      { "var"     , 13 } ,
      { "assert"  , 13 } ,
      { ""        ,  0 } ,
    };

  for ( int i = 0; prefix_keywords[i].text[0]; i++ ) {
    wstr * str = wstr_new ( prefix_keywords[i].text, 0);
    if ( ! str ) {
      e = &wick_out_of_memory;
      }
    werr ( e, wtable_wstr_ptr_to_wtoken_ptr_set (
        self->identifier_table,
        str,
        ( wtoken * ) wtoken_prefix_new ( str, prefix_keywords[i].rbp ) ) );
    }
  wstr * ellipsis_str = wstr_new ( "...", 0 );
  werr ( e, wstr_trie_insert_token (
        self->prefix_table,
        wtoken_new ( ellipsis_str ) ) );

  
  int pair_lbp = 80;
  werr ( e, wtoken_left_init ( &lparen, wstr_new ( "(", 0 ), pair_lbp ) );
  werr ( e, wtoken_right_init ( &rparen, wstr_new ( ")", 0 ) ) );
  werr ( e, wparser_insert_pair ( self, &lparen, &rparen ) );

  werr ( e, wtoken_left_init ( &lbrace, wstr_new ( "{", 0 ), pair_lbp ) );
  werr ( e, wtoken_right_init ( &rbrace, wstr_new ( "}", 0 ) ) );
  werr ( e, wparser_insert_pair ( self, &lbrace, &rbrace ) );

  werr ( e, wtoken_left_init ( &lbracket, wstr_new ( "[", 0 ), pair_lbp ) );
  werr ( e, wtoken_right_init ( &rbracket, wstr_new ( "]", 0 ) ) );
  werr ( e, wparser_insert_pair ( self, &lbracket, &rbracket ) );

  werr ( e, wstr_trie_insert_token ( self->infix_table, ( wtoken * ) &comma ) );

  return e;
  } 

static werror * wparser_lex_quote ( wparser * self ) {
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
    return &wparser_invalid_token;
    }
  ++str.past_end;
  werr ( e, wparser_unique_wtoken ( &str, &token, wtoken_new, self->literal_table ) );
  if ( e == w_ok ) {
    e = wparser_push_token ( self, token );
    self->text.start = str.past_end;
    }
  return e;
  }

static werror * wparser_lex_number ( wparser * self ) {
  werror * e = w_ok;
  wtoken * token;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  str.past_end = str.start + 1;
  while ( str.past_end != self->text.past_end && isdigit ( *str.past_end) ) {
    ++str.past_end;
    }
  werr ( e, wparser_unique_wtoken ( &str, &token, wtoken_new, self->literal_table ) );
  if ( e == w_ok ) {
    e = wparser_push_token ( self, token );
    self->text.start = str.past_end;
    }
  return e;
  }

static werror * wparser_lex_literal ( wparser * self ) {
  if ( *self->text.start == '\'' || *self->text.start == '"' || *self->text.start == '`') {
    return wparser_lex_quote ( self );
    }
  else if ( isdigit ( *self->text.start ) ) {
    return wparser_lex_number ( self );
    }
  return &wparser_inactive;
  }

werror * wparser_parse ( wparser * self ) {
  self->text = self->all_text;
  bool active = true;
  bool inner_loop = true;
  while ( ! self->stop && wstr_size ( self->text ) && active ) {
    active = false;
    inner_loop = true;
    while ( *self->text.start == '\n' ) {
      ++self->text.start;
      wparser_set_state ( self, wparser_line_start_context );
      }
    active |= wparser_lex_indent ( self ) == w_ok;
    while (inner_loop) {
      inner_loop = wparser_lex_from_trie ( self ) == w_ok;
      if (self->stop) {
        return w_ok;
        }
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
  if ( wstr_size ( self->text ) != 0 ) {
    wstr_println ( self->text );
    return &wparser_bad_expr;
    }
  return w_ok;
  }

wparser * wparser_new ( wstr * text ) {
  werror * e = w_ok;
  werr ( e, wtoken_infix_init ( &space, wstr_new ( "(space)", 0 ), 90, 90, false ) );
  werr ( e, wtoken_init ( &root, wstr_new ( "(root)", 0 ) ) );
  werr ( e, wtoken_infix_init ( &comma, wstr_new ( ",", 0 ), 19, 19, false ) );
  if ( e != w_ok ) {
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
  if ( ! p->prefix_table ||
       ! p->infix_table ||
       ! p->literal_table ||
       ! p->identifier_table ||
       ! p->indent_table ) {
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
  p->state = wparser_line_start_context;

  wparser_table_init ( p );
  return p;
  }

static int wparser_get_rbp ( wtoken * t ) {
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

static werror * wparser_update_infix ( wparser * self, wtoken_infix * t ) {
  wast_infix * w = walloc_simple ( wast_infix, 1 );
  w->op = t;
  w->type = wtype_wast_infix;
  while ( ! wobj_cast ( wtoken_indent, self->accum->op ) && 
          wparser_get_rbp ( self->accum->op ) >= t->lbp ) {
    self->accum = self->accum->parent;
    }
  w->parent = self->accum;
  w->left = wast_remove_rightmost ( self->accum );
  w->right = NULL;
  wast_add_rightmost ( self->accum, wobj_of ( w ) );
  self->accum = ( wast * ) w;
  wparser_set_state ( self, wparser_prefix_context );
  return w_ok;
  }

static werror * wparser_update_prefix ( wparser * self, wtoken_prefix * t ) {
  wast_prefix * w = walloc_simple ( wast_prefix, 1 );
  w->type = wtype_wast_prefix;
  w->op = t;
  w->child = NULL;
  w->parent = self->accum;
  wast_add_rightmost ( self->accum, wobj_of ( w ) );
  self->accum = (wast *) w;
  return w_ok;
  }

static bool keep_comments = false;
static werror * wparser_update_comment ( wparser * self, wtoken * t ) {
  if ( keep_comments ) {
    wast * w = wast_parent_exprlist ( self->accum );
    return wast_add_rightmost ( w, wobj_of ( t ) );
    }
  else {
    return w_ok;
    }
  }

static werror * wparser_update_atom ( wparser * self, wtoken * t ) {
  switch ( self->state ) {
    case wparser_infix_context:
      wparser_update_infix ( self, &space );
    case wparser_line_start_context:
    case wparser_prefix_context: {
      wparser_set_state ( self, wparser_infix_context );
      return wast_add_rightmost ( self->accum, wobj_of ( t ) );
      }
    default:
        {}
    }
  return &werror_internal;
  }


static werror * wparser_update_left ( wparser * self, wtoken_left * t ) {
  wast_list * w = wast_list_new ( ( wtoken * ) t );
  if ( ! w ) {
    return &wick_out_of_memory;
    }
  switch ( self->state ) {
    case wparser_line_start_context:
    case wparser_infix_context: {
      while ( wparser_get_rbp ( self->accum->op ) >= t->lbp ) {
        self->accum = self->accum->parent;
        }
      w->primary = wast_remove_rightmost ( self->accum );
      wast_add_rightmost ( self->accum, wobj_of ( w ) );
      w->parent = self->accum;
      self->accum = ( wast * ) w;
      wparser_set_state ( self, wparser_prefix_context );
      break;
      }
    case wparser_prefix_context: {
      w->parent = self->accum;
      wast_add_rightmost ( self->accum, wobj_of ( w ) );
      self->accum = ( wast * ) w;
      wparser_set_state ( self, wparser_prefix_context );
      break;
      }
    }
  return w_ok;
  }

static werror * wparser_update_right ( wparser * self, wtoken_right * t ) {
  self->accum = wast_parent_exprlist ( self->accum );
  if ( self->accum->op == ( wtoken * ) &comma ) {
    self->accum = self->accum->parent;
    self->accum = wast_parent_exprlist ( self->accum );
    }
  if ( self->accum->op != ( wtoken * ) t->left ) {
    return &wparser_bad_expr;
    }
  self->accum = self->accum->parent;
  wparser_set_state ( self, wparser_infix_context );
  return w_ok;
  }

static werror * wparser_update_comma ( wparser * self, wtoken_infix * t ) {
  wast * parent_exprlist = wast_parent_exprlist ( self->accum );
  werror * e = w_ok;
  if (parent_exprlist->op == ( wtoken * ) &comma) {
    self->accum = parent_exprlist;
    }
  else {
    wast_list * w = wast_list_new ( ( wtoken * ) &comma );
    while ( wparser_get_rbp ( self->accum->op ) >= t->lbp ) {
      self->accum = self->accum->parent;
      }
    w->parent = self->accum;
    werr ( e, wast_add_rightmost ( ( wast * ) w,
        wast_remove_rightmost ( self->accum ) ) );
    werr ( e, wast_add_rightmost ( self->accum, wobj_of ( w ) ) );
    self->accum = ( wast * ) w;
    }
  wparser_set_state ( self, wparser_prefix_context );
  return e;
  }

static werror * wparser_update_indent ( wparser * self, wtoken_indent * t ) {
  switch ( self->state ) {
    case wparser_line_start_context: 
    case wparser_prefix_context:
    case wparser_infix_context: 
      {
      wvar_of ( wi, wast_infix, self->accum ) {
        if ( wi->op->starts_indent && wi->right == NULL ) {
          wast_list * w = wast_list_new ( ( wtoken * ) t );
          if ( ! w ) {
            return &wick_out_of_memory;
            }
          w->parent = self->accum;
          wi->right = wobj_of ( w );
          self->accum = ( wast * ) w;
          return w_ok;
          }
        } wvar_end
      if ( ! wobj_cast ( wtoken_indent, self->accum->op ) ) {
        self->accum = wast_parent_exprlist ( self->accum );
        }
      while ( ( wobj_cast ( wtoken_indent, self->accum->op ) &&
                self->accum->op != ( wtoken * ) t ) || 
              ( ( self->accum->op == ( wtoken * ) &comma ) &&
                ! wobj_cast ( wast_list, self->accum->parent ) ) ) {
        self->accum = self->accum->parent;
        self->accum = wast_parent_exprlist ( self->accum );
        }
      }
    }
  return w_ok;
  }

static werror * wparser_update ( wparser * self, wtoken * t ) {
  if ( t == ( wtoken * ) &comma ) {
    return wparser_update_comma ( self, &comma );
    }
  else if ( wstr_compare ( *t->text, wstr_lit ( "EOF" ) ) == 0 ) {
    self->stop = true;
    return w_ok;
    }
  else wvar_of ( wi, wtoken_infix, t ) {
    return wparser_update_infix ( self, wi );
    } wvar_end
  else wvar_of ( wp, wtoken_prefix, t ) {
    return wparser_update_prefix ( self, wp );
    } wvar_end
  else wvar_of ( windent, wtoken_indent, t ) {
    return wparser_update_indent ( self, windent );
    } wvar_end
  else wvar_of ( wl, wtoken_left, t ) {
    return wparser_update_left ( self, wl );
    } wvar_end
  else wvar_of ( wr, wtoken_right, t ) {
    return wparser_update_right ( self, wr );
    } wvar_end
  else if ( *t->text->start == '#' ) {
    wparser_update_comment ( self, t );
    }
  else {
    wparser_update_atom ( self, t );
    }
  return w_ok;
  }
