#include <wparse.h>
#include <wcall.h>
#include <walloc.h>

wdefine_composite ( wparser );

const int family_whitespace = 0;
const int family_identifier = 1;
const int family_literal    = 2;
const int family_operator   = 3;

wstatus lex_comment ( wparser * self ) {
  wstr str;
  wstr_init_dynamic ( &str );
  str.start = self->text.start;
  while ( *self->text.start != '\n' ) {
    ++self->text.start;
    }
  str.past_end = self->text.start;
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
