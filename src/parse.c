#include "common.h"
#include "utf8_string.c"
#include <utf8proc.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>

#define WICK_PARSE_CHECK_ARGS(_to_check_) \
  do{ \
    if(!(in_buffer && out_buffer && (_to_check_))) { \
      return WICK_MEMORY_ERROR; \
    } \
  } while(0)

#define _CHAR_NAME_ num_char
#define _COND_(character) \
  ('9' >= character && \
    character >= '0')
#include "def_char_func.c"

#define _WORD_NAME_ num_word
#define _COND_(character) \
  ('9' >= character && \
    character >= '0')
#include "def_word_func.c"

#define _WORD_NAME_ two_arg_word
#define _ARG_LIST_ uint8_t arg_char1, uint8_t arg_char2
#define _ARGS_ arg_char1, arg_char2
#define _COND_(character) \
  (character == arg_char1 || \
   character == arg_char2)
#include "def_word_func.c"

#define _CHAR_NAME_ ident_start_char
#define _COND_(character) \
    ('A' <= character && character <= 'Z') || \
    ('a' <= character && character <= 'z') || \
    (character == '_') || \
    (character > CHAR_MAX)
#include "def_char_func.c"

#define _CHAR_NAME_ ident_char
#define _COND_(character) \
    ('A' <= character && character <= 'Z') || \
    ('a' <= character && character <= 'z') || \
    ('9' >= character && character >= '0') || \
    (character == '_') || \
    (character > CHAR_MAX)
#include "def_char_func.c"

#define _WORD_NAME_ ident_word
#define _FUNC_ ident_char
#include "def_word_func.c"

#define _CHAR_NAME_ specific_char
#define _ARG_LIST_ char the_char
#define _COND_(character) \
    (character == (uint8_t)the_char)
#include "def_char_func.c"

#define _CHAR_NAME_ not_specific_char
#define _ARG_LIST_ char the_char
#define _COND_(character) \
    (character != (uint8_t)the_char)
#include "def_char_func.c"

#define _CHAR_NAME_ whitespace_char
#define _COND_(character) \
    (character == ' ' ) || \
    (character == '\t') || \
    (character == '\n')
#include "def_char_func.c"

#define _WORD_NAME_ whitespace
#define _FUNC_ whitespace_char
#include "def_word_func.c"

#define _CHAR_NAME_ opt_char
#define _COND_(character) \
    !(('A' <= character && character <= 'Z') || \
      ('a' <= character && character <= 'z') || \
      (character == '_') || \
      ('0' <= character && character <= '9') || \
      (character == ' ' ) || \
      (character == '\t') || \
      (character == '\n') || \
      (character > CHAR_MAX))
#include "def_char_func.c"

#define _CHAR_NAME_ escape_char
#define _COND_(character) \
    (character == '\\') || \
    (character == 'n' ) || \
    (character == 't' )
#include "def_char_func.c"

wick_error identifier(const utf8_string * in_buffer, utf8_string * out_buffer) {
  WICK_PARSE_CHECK_ARGS(1);
  utf8_string temp_buffer;
  WICK_THROW(ident_start_char(in_buffer, &temp_buffer));
  utf8_step(&temp_buffer, in_buffer);
  WICK_THROW(ident_word(&temp_buffer, &temp_buffer));
  out_buffer->start = in_buffer->start;
  out_buffer->past_end = temp_buffer.past_end;
  return WICK_SUCCESS;
}

wick_error match_string(const utf8_string * in_buffer, 
    utf8_string * out_buffer, char * the_string) {
  WICK_PARSE_CHECK_ARGS(the_string);
  /* This should be utf8 safe on valid utf8 strings,
   * since byte by bytes comparison should still be valid.
   */
  uint8_t * check_char = (uint8_t *)the_string;
  uint8_t * buffer_char = in_buffer->start;
  while(check_char && *check_char) {
    /* While there are potential non-matching char's. */
    if(buffer_char >= in_buffer->past_end ||
      /* Ran out of chars to match against. */
       *buffer_char != *check_char) {
      /* Input didn't match. */
      return WICK_FAILURE;
    }
    ++buffer_char;
    ++check_char;
  }
  out_buffer->start = in_buffer->start;
  out_buffer->past_end = buffer_char;
  return WICK_SUCCESS;
}

struct AST_Def {
  utf8_string name;
};

typedef struct AST_Def AST_Def;

wick_error parse_def(const utf8_string * in_buffer, 
    utf8_string * out_buffer, AST_Def * node) {
  WICK_PARSE_CHECK_ARGS(node);
  utf8_string remaining;
  WICK_THROW(match_string(in_buffer, &remaining, "def"));
  utf8_step(&remaining, in_buffer);
  whitespace(&remaining, &remaining);
  utf8_step(&remaining, in_buffer);
  WICK_THROW(identifier(&remaining, &remaining));
  out_buffer->start = remaining.past_end;
  out_buffer->past_end = in_buffer->past_end;
  node->name = remaining;
  return WICK_SUCCESS;
}

wick_error test_parse_number() {
  utf8_string buffer = utf8_string_from_char_array("123");
  utf8_string buffer2;
  printf("%d\n", two_arg_word(&buffer, &buffer2, 
        (uint8_t)'1', (uint8_t) '2') == WICK_SUCCESS);
  utf8_string_println(buffer2);
  return WICK_SUCCESS;
}


int main(int argv, char * argc[]) {
  if(argv != 2) {
      fprintf(stderr,
              "Incorrect usage:\n"
              "program should be called\n\n"
              "\t%s file_name\n", argc[0]);
      return 1;
  }
  utf8_string buffer = utf8_string_from_filename(argc[1]);
  utf8_string_println(buffer);
  printf("%c\n", *buffer.start);
  return 0;
}
