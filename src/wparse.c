#include <wparse.h>
#include <wcall.h>
#include <walloc.h>

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

wstatus lex_comment ( wparser * self ) {
  if ( *self->text.start != '#' ) {
    return W_ERROR;
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
    return W_OK;
    }
  else {
    return W_ERROR;
    }
  }

wstatus lex_indent ( wparser * restrict self ) {
  if ( *self->text.start != '\n' ) {
    return W_ERROR;
    }
  wstr_trie * node = self->token_table;
  wstr str = wstr_empty ();
  str.start = self->text.start;
  while ( *self->text.start == ' ' || *self->text.start == '\t' ) {
    node = wstr_trie_enter_next ( node, *self->text.start );
    if ( ! node ) {
      wparser_error ( self, &wick_out_of_memory );
      return W_ERROR;
      }
    ++self->text.start;
    }
  str.past_end = self->text.start;
  if ( ! node->val ) {
    wtoken * token = wtoken_new ( family_whitespace, wstr_copy ( &str ), 0, 0 );
    if ( ! token ) {
      return W_ERROR;
      }
    node->val = wobj_of ( token );
    }
  return W_OK;
  }

wstatus lexer ( wparser * self ) {
  while ( *self->text.start++ == '\t' ) { }
  while ( *self->text.start++ == ' ' ) { }
  if ( *self->text.start == '#' ) {
    return lex_comment ( self );
    }
  if ( *self->text.start == '\n' ) {
    return lex_indent ( self );
    }
  else {
    /*return lex_ident ( self );*/
    }
  return W_OK;
  }
