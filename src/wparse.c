#include <wparse.h>
#include <wcall.h>
#include <walloc.h>

wdefine_composite ( wparser );

const int family_whitespace = 0;
const int family_identifier = 1;
const int family_literal    = 2;
const int family_operator   = 3;

wstatus add_token (
    int family,
    int lbp,
    int rbp,
    wstr ** text,
    wtoken ** to_store
  ) {
    wtoken * token = walloc_simple ( wtoken, 1 );
    wstr * new_str = wstr_copy ( *text );
    if ( ! token || ! new_str ) {
      free ( token );
      free ( new_str );
      return W_ERROR;
      }
    token->type = wtype_wtoken;
    token->family = family;
    token->lbp = lbp;
    token->rbp = rbp;
    token->text = new_str;
    *text = new_str;
    *to_store = token;
    return W_OK;
  }

#define wcall_name add_token
#define wcall_args \
  (int) arg[0].integer, \
  (int) arg[1].integer, \
  (int) arg[2].integer, \
  (wstr **) arg[3].pointer, \
  (wtoken **) arg[4].pointer
#define wcall_argc 5
#include <wcall_define.c>

wstatus lex_comment ( wparser * self ) {
  wcall add_comment;
  wstr str;
  if ( W_OK == wcall_clone ( &add_comment, &wcall_add_token ) ) {
    wcall_push ( &add_comment, warg ( int, integer, family_whitespace ) );
    wcall_push ( &add_comment, warg ( int, integer, 0 ) );
    wcall_push ( &add_comment, warg ( int, integer, 0 ) );
    }
  else {
    return W_ERROR;
    }
  wstr_init_dynamic ( &str );
  str.start = self->text.start;
  while ( *self->text.start != '\n' ) {
    ++self->text.start;
    }
  str.past_end = self->text.start;
  wtable_wstr_ptr_to_wtoken_ptr_lookup_or_add ( self->token_table,
                                                &str,
                                                &add_comment,
                                                self->handle_error );
  wcall_deinit ( &add_comment );
  return W_OK;
  }

wstatus lexer ( wparser * self ) {
  while ( *self->text.start++ == ' ' ) { }
  while ( *self->text.start++ == '\t' ) { }
  if ( *self->text.start == '#' ) {
    return lex_comment ( self );
    }
  if ( *self->text.start == '\n' ) {
    /*return lex_indent ( self );*/
    }
  else {
    /*return lex_ident ( self );*/
    }
  return W_OK;
  }
