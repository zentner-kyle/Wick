#include <wparse.h>
#include <wcall.h>
#include <walloc.h>
#include <werror.h>
#include <ctype.h>
#include <wutf8.h>
#include <assert.h>

wdefine_composite ( wparser );

void wparser_error ( wparser * self, werror * error ) {
  self->stop = true;
  }

bool wparser_push_token ( wparser * self, wtoken * token ) {
  printf ("Pushing token: '");
  wstr_print ( *token->text );
  printf ("'\n");
  warray_wtoken_ptr_push_back ( self->tokens, token, self->handle_error );
  return ! self->stop;
  }

const int family_stop = 0;
const int family_identifier = 1;
const int family_literal    = 2;
const int family_operator   = 3;
const int family_comment    = 4;
const int family_short      = 5;
const int family_long       = 6;
const int family_whitespace = 7;

bool is_ident_char ( int32_t c ) {
  if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_')) {
    printf ( "'%c' is an ident char!\n", (char) c );
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
  printf ( "Lexed indent of length %zd.\n.", wstr_size (str) );
  wparser_push_token ( self, token );
  return w_ok;
  }

werror * lex_op_from_trie ( wparser * self ) {
  printf ( "In lex op from trie.\n" );
  wstr_trie * node = self->token_table;
  wstr_trie * prev_node = NULL;
  wtoken * token = NULL;
  const char * c = self->text.start;
  printf ( "c = '%s'\n", c );
  const char * last;

  while ( c != self->text.past_end ) {
    prev_node = node;
    node = wstr_trie_get_next ( node, *c );
    if ( node != prev_node ) {
      printf ("found matching char\n");
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
      printf ( "could not find match for '%c'\n", *c );
      break;
      }
    }

  /*while ( c != self->text.past_end && node != prev_node ) {*/
    /*prev_node = node;*/
    /*node = wstr_trie_get_next ( node, *c );*/
    /*++c;*/
      /*}*/
    /*}*/
  if ( token ) {
    self->text.start = last + 1;
    wparser_push_token ( self, token );
    return w_ok;
    }
  return &wparse_inactive;
  }

werror * lex_identifier_from_trie ( wparser * self ) {
  printf ( "Lexing identifier from trie.\n" );
  printf ( "text ='");
  wstr_print ( self->text );
  printf ( "'\n");
  wstr_trie * node = self->token_table;
  wstr_trie * prev_node = NULL;
  wtoken * token = NULL;
  const char * c = self->text.start;
  const char * c1= c;
  int32_t uc; 
  if ( wutf8_get ( &uc, &c1, self->text.past_end ) != w_ok ) {
    printf ("Invalid\n");
    return &invalid_utf8;
    }
  if ( ! is_ident_char ( uc ) ) {
    printf ( "nothing to do!\n" );
    return &wparse_inactive;
    }
  while ( c != self->text.past_end ) {
    prev_node = node;
    node = wstr_trie_get_next ( node, *c );
    if ( node != prev_node ) {
      printf ("found matching char\n");
      ++c;
      }
    else
      {
      break;
      }
    }

  c1 = c;
  if ( wutf8_get ( &uc, &c1, self->text.past_end ) != w_ok ) {
    printf ("Invalid\n");
    return &invalid_utf8;
    }
  while ( c != self->text.past_end && is_ident_char ( uc ) ) {
    while ( c != c1 ) {
      printf ("|");
      node = wstr_trie_enter_next ( node, *c++ );
      printf ( "now c = '%s'\n", c );
      }
    if ( wutf8_get ( &uc, &c1, self->text.past_end ) != w_ok ) {
      printf ("Invalid\n");
      return &invalid_utf8;
      }
    }

  if ( node->val ) {
    printf ( "Node already has value %p!\n", node->val );
    token = wobj_cast ( wtoken, node->val );
    printf ( "token.text ='" );
    wstr_print ( *token->text );
    printf ("'\n");
    if ( ! token ) {
      printf ("cast error");
      return &wcast_error;
      }
    }
  else {
    printf ( "c = '%s'\n", c );
    token = wtoken_new ( family_identifier, wstr_new ( self->text.start, c ) );
    if ( ! token ) {
      printf ("out of memory error");
      return &wick_out_of_memory;
      }
    node->val = wobj_of ( token );
    }
  wparser_push_token ( self, token );
  self->text.start = c;
  printf ("ok, pushed token\n");
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
  /*if ( status1 == w_ok || status1 == &wparse_inactive ) {*/
    /*status2 = lex_op_from_trie ( self );*/
    /*if ( status2 == w_ok ) {*/
      /*return w_ok;*/
      /*}*/
    /*else if ( status2 == &wparse_inactive ) {*/
      /*if ( status1 == &wparse_inactive ) {*/
        /*return &wparse_inactive;*/
        /*}*/
      /*else {*/
        /*return w_ok;*/
        /*}*/
      /*}*/
    /*else {*/
      /*return status2;*/
      /*}*/
    /*}*/
  /*else {*/
    /*return status1;*/
    /*}*/
  }

wtoken new_line;
wtoken start_of_file;

werror * wstr_trie_insert_token ( wstr_trie * node, wstr * str, wtoken * token ) {
  node = wstr_trie_enter ( node, str );
  if ( ! node ) {
    return &wick_out_of_memory;
    }
  node->val = wobj_of ( token );
  printf ( "val for '%s' is now '%s'\n", str->start, wobj_cast ( wtoken, node->val )->text->start );
  return w_ok;
  }

werror * wparser_table_init ( wparser * self ) {
  struct {
    char * text;
    int family;
    int lbp;
    int rbp;
    wtoken * token;
  } table[] = {
      { "=", family_operator, 1, 1 },
      { "+", family_operator, 2, 2 },
      { "-", family_operator, 2, 2 },
      { "*", family_operator, 3, 3 },
      { "/", family_operator, 3, 3 },
      { "%", family_operator, 3, 3 },
      { "<-", family_operator, 1, 1 },
      { "<<", family_operator, 1, 1 },
      { "(", family_operator, 0, 1 },
      { ")", family_operator, 1, 0 },
      { "", 0, 0, 0 },
    };
  wstr * str;
  int i = 0;
  while ( table[i].family != family_stop ) {
    wstr_trie_print ( self->token_table );
    str = wstr_new ( table[i].text, 0);
    printf ( "adding token '%s'\n", table[i].text );
    werror * e = wstr_trie_insert_token (
        self->token_table,
        str,
        wtoken_new_op ( table[i].family, str, 0, 0 ) );
    ++i;
    if ( e != w_ok ) {
      return e;
      }
    }
  return w_ok;
  } 

werror * wparser_lex_quote ( wparser * self ) {
  const char * start = self->text.start;
  wstr_trie * node = self->token_table;
  wtoken * token = NULL;
  const char * c = self->text.start;
  const char * c1;
  wstr * str;
  int32_t uc; 
  while ( c != self->text.past_end && *c != *start ) {
    node = wstr_trie_enter_next ( node, *c );
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

  /* 
   * node is pointed at the last filled node, c is pointed to the next
   * character.
   */

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
    printf ("Hi!");
    active = false;
    inner_loop = true;
    wtoken * last_token = *warray_wtoken_ptr_index ( self->tokens, -1 );
    if ( *self->text.start == '\n' ) {
      ++self->text.start;
      if ( last_token != &new_line ) {
        wparser_push_token ( self, &new_line );
        }
      }
    if ( last_token == &start_of_file || last_token == &new_line ) {
      printf ( "Going to lex indent.\n" );
      active |= wparser_lex_indent ( self ) == w_ok;
      }
    while (inner_loop) {
      inner_loop = wparser_lex_from_trie ( self ) == w_ok;
      active |= inner_loop;
      if ( isspace ( *self->text.start ) ) {
        printf ("skipping space.\n");
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
  return w_ok;
  }

wparser * wparser_new ( wstr * text ) {
  new_line.type = wtype_wtoken;
  new_line.text = wstr_new ( "(newline)", 0 );
  new_line.family = family_literal;
  new_line.lbp = 0;
  new_line.rbp = 0;
  wparser * p = walloc_simple ( wparser, 1 );
  if ( ! p ) {
    return NULL;
    }
  p->type = wtype_wparser;
  p->all_text = *text;
  p->text = *text;
  p->handle_error = &null_wcall;
  p->token_table = wstr_trie_new ();
  p->tokens = warray_wtoken_ptr_new ( &null_wcall );
  p->stop = false;
  if ( ! p->tokens || ! p->token_table ) {
    warray_wtoken_ptr_delete ( p->tokens );
    wstr_trie_free ( p->token_table );
    free (p);
    return NULL;
    }
  else {
    wparser_table_init ( p );
    wstr * str = wstr_new ( "=", 0 );
    wstr_trie * node = wstr_trie_get_longest ( p->token_table, str );
    assert ( node );
    printf ("Here!\n");
    fflush (stdout);
    printf ("node = %p\n", node);
    printf ("node->val = %p\n", node->val);
    wstr_trie_print ( node );
    assert ( wobj_cast ( wtoken, node->val ) );
    assert ( strcmp ( wobj_cast ( wtoken, node->val )->text->start, "=" ) == 0 );
    wparser_push_token ( p, &new_line );
    }
  return p;
  }

void wparser_print_tokens ( wparser * self ) {
  warray_wtoken_ptr_debug_print ( self->tokens );
  warray_wtoken_ptr_iter i = warray_wtoken_ptr_start ( self->tokens );
  while ( warray_wtoken_ptr_good ( &i ) ) {
    wstr_println ( *warray_wtoken_ptr_deref ( &i )->text );
    warray_wtoken_ptr_next ( &i );
    }
  }
