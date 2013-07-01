#include <wparse.h>
#include <wcall.h>
#include <walloc.h>
#include <werror.h>
#include <ctype.h>

wdefine_composite ( wparser );

void wparser_error ( wparser * self, werror * error ) {
  self->stop = true;
  }

bool wparser_push_token ( wparser * self, wtoken * token ) {
  warray_wtoken_ptr_push_back ( self->tokens, token, self->handle_error );
  return ! self->stop;
  }

const int family_whitespace = 0;
const int family_identifier = 1;
const int family_literal    = 2;
const int family_operator   = 3;
const int family_comment    = 4;
const int family_short      = 5;
const int family_long       = 6;

werror wparse_inactive;

werror * lex_comment ( wparser * restrict self ) {
  if ( *self->text.start != '#' ) {
    return &wparse_inactive;
    }
  wstr str = wstr_empty ();
  str.start = self->text.start;
  while ( *self->text.start != '\n' ) {
    ++self->text.start;
    }
  str.past_end = self->text.start;
  wtoken * token = wtoken_new ( family_comment, wstr_copy ( &str ), 0, 0 );
  if ( token ) {
    wparser_push_token ( self, token );
    return w_ok;
    }
  else {
    return &wick_out_of_memory;
    }
  }

werror * lex_indent ( wparser * restrict self ) {
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
    token = wtoken_new ( family_whitespace, wstr_copy ( &str ), 0, 0 );
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
  wparser_push_token ( self, token );
  return w_ok;
  }

werror * lex_op_from_table ( wparser * self ) {
  wstr_trie * node = self->token_table;
  wstr_trie * prev_node = node;
  wtoken * token = NULL;
  char * c = self->text.start;
  while ( c != self->text.past_end && node != prev_node ) {
    prev_node = node;
    node = wstr_trie_get_next ( prev_node, *c );
    if ( node->val ) {
      token = wobj_cast ( wtoken, node->val );
      if ( ! token ) {
        return &wcast_error;
        }
      }
    ++c;
    }
  if ( token ) {
    self->text.start = c;
    wparser_push_token ( self, token );
    return w_ok;
    }
  return &wparse_inactive;
  }

werror * lex_from_trie ( wparser * self, wstr_trie * trie ) {
  wstr_trie * node = self->token_table;
  wstr_trie * prev_node = node;
  wtoken * token = NULL;
  char * c = self->text.start;
  while ( c != self->text.past_end && node != prev_node ) {
    prev_node = node;
    node = wstr_trie_get_next ( prev_node, *c );
    if ( node->val ) {
      token = wobj_cast ( wtoken, node->val );
      if ( ! token ) {
        return &wcast_error;
        }
      }
    ++c;
    }
  if ( token ) {
    self->text.start = c;
    wparser_push_token ( self, token );
    return w_ok;
    }
  return &wparse_inactive;
  }

wtoken new_line;
wtoken start_of_file;

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
    wstr * text,
    wtoken * token
  } table[] = {
      /*{ wstr_new ( "\n", 0 ), &new_line },*/
      { wstr_new ( "", 0 ), NULL },
    };
  int i = 0;
  while ( table[i].token ) {
    werror * e = wstr_trie_insert_token ( self->token_table, table[i].text, table[i].token );
    if ( e != w_ok ) {
      return e;
      }
    }
  } 

werror * lex_quote ( wparser * self ) {
  wstr * str = wstr_new ();
  str->start = self->text.start;
  char quote = *str->start++;
  str->past_end = self->text.start;
  while ( str->past_end < self->text.past_end && str->past_end != quote ) {
    }
  }

werror * lex_literal ( wparser * self ) {
  if ( *self->text.start == '\'' || *self->text.start == '"' || *self->text.start == '`') {
    return lex_quote ( self );
    }
  else if ( isdigit ( *self->text.start ) ) {
    return lex_number ( self );
    }
  return &wparse_inactive;
  }

werror * lexer ( wparser * self ) {
  if ( *self->text.start == '\n' ) {
    if ( last_token != &new_line ) {
      wparser_push_token ( self, &new_line );
      }
    }
  wtoken * last_token = warray_wtoken_ptr_index ( self, -1 );
  if ( last_token == &start_of_file || last_token == &new_line ) {
    return lex_indent ( self );
    }
  while ( true ) {
    if ( lex_from_tries ( self ) != w_ok ) {
      if ( !isspace ( *self->text.start ) ) {
        ++self->text.start;
        }
      else {
        break;
        }
      }
    else {
      break;
      }
    }
  if ( *self->text.start == '#' ) {
    return lex_comment ( self );
    }
  lex_literal ( self );
  lex_identifier ( self );
  return w_ok;
  }
