/* This file shouldn't be included directly. */

/* 
 * An example of correct use of this file is as follows:
 *
 * #define _ARG_LIST_ uint8_t *arg_char
 * #define _ARGS_ arg_char
 * #define _ARG_CHECK_( arg_char != NULL )
 * #define _COND_( character ) ( character == *arg_char )
 * #define _WORD_NAME_ arg_token_match
 * #include token_func.c
 *
 * Both _ARG_LIST_ and _ARG_CHECK_ are optional arguments.
 * The defined arguments are undef'ed in this file.
 *
 * _COND_ can also be replaced with _FUNC_, which must be a
 * a function returning a wick_error, and taking as arguments
 * ( const utf8_string * in_buffer, utf8_string * out_buffer, 
 *  _ARG_LIST_ ).
 *
 */


#ifdef _ARG_LIST_
  #ifndef _FUNC_
    wick_error C_TOKEN_JOIN ( _WORD_NAME_, _char_match )
      ( const utf8_string * in_buffer, utf8_string * out_buffer, _ARG_LIST_ );
    #define _FUNC_ C_TOKEN_JOIN ( _WORD_NAME_, _char_match )
    #define _NEED_NEW_FUNC_
    #endif
  wick_error _WORD_NAME_
    ( const utf8_string * in_buffer, utf8_string * out_buffer, _ARG_LIST_ ) {
#else
  #ifndef _FUNC_
    wick_error C_TOKEN_JOIN ( _WORD_NAME_, _char_match )
      ( const utf8_string * in_buffer, utf8_string * out_buffer );
    #define _FUNC_ C_TOKEN_JOIN ( _WORD_NAME_, _char_match )
    #define _NEED_NEW_FUNC_
    #endif
  wick_error _WORD_NAME_ ( const utf8_string * in_buffer,
                            utf8_string * out_buffer ) {
  #endif

#ifdef _ARG_CHECK_
  WICK_PARSE_CHECK_ARGS ( _ARG_CHECK_ );
#else
  WICK_PARSE_CHECK_ARGS ( 1 );
  #endif

  utf8_string remaining = *in_buffer;
  utf8_string temp_match;
  /*utf8_string temp_match = *in_buffer;*/
  /* We need two buffers to avoid writing to out_buffer on failure. */
  while ( 1 ) {
#ifdef _ARGS_
    wick_error result = _FUNC_ ( &remaining, &temp_match, _ARGS_ );
#undef _ARGS_
#else
    wick_error result = _FUNC_ ( &remaining, &temp_match );
  #endif
    if ( result == WICK_SUCCESS ) {
      /* Move one character forward. */
      remaining.start = temp_match.past_end;
      } else if ( result != WICK_FAILURE ) {
      /* This is where we propagate genuine errors. */
      return result;
      }
    if ( result == WICK_FAILURE || temp_match.past_end >= remaining.past_end ) {
      /* The match ends. */
      if ( remaining.start != in_buffer->start ) {
        /* The match ended successfully. */
        out_buffer->start = in_buffer->start;
        out_buffer->past_end = remaining.start;
        return WICK_SUCCESS;
        }
      else {
        /* The match failed. */
        return WICK_FAILURE;
        }
      }
    }
  }


#undef _FUNC_
#ifdef _NEED_NEW_FUNC_
  #undef _NEED_NEW_FUNC_
  #define _CHAR_NAME_ C_TOKEN_JOIN ( _WORD_NAME_, _char_match )
  /* The other args are passed from this file implicitly.
   * C has special variables too.
   */
  #include <def_char_func.c>
  #undef _CHAR_NAME_
  #endif
#undef _WORD_NAME_
