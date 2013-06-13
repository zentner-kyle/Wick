/* This file shouldn't be included directly. */

/*
 * An example of correct use of this file is as follows:
 *
 * #define _ARG_LIST_ uint8_t *arg_char
 * #define _ARG_CHECK_( arg_char != NULL )
 * #define _COND_( character ) ( character == *arg_char )
 * #define _CHAR_NAME_ arg_char_match
 * #include char_func.c
 * 
 * Both _ARG_LIST_ and _ARG_CHECK_ are optional arguments.
 * The defined arguments are undef'ed in this file.
 */

#ifdef _ARG_LIST_
  wick_error _CHAR_NAME_ (
      const utf8_string * in_buffer,
      utf8_string * out_buffer, _ARG_LIST_
      ) {
  #undef _ARG_LIST_
#else
  wick_error _CHAR_NAME_ (
      const utf8_string * in_buffer,
      utf8_string * out_buffer
      ) {
  #endif

#ifdef _ARG_CHECK_
  WICK_PARSE_CHECK_ARGS ( _ARG_CHECK_ );
  #undef _ARG_CHECK_
  #endif

  int32_t character = 0xFFFFFFFF;
  /* Way out of Unicode range to make errors more obvious. */
  ssize_t bytes_read = utf8proc_iterate (
      in_buffer->start,
      utf8_string_size ( * in_buffer ),
      &character );
  if ( _COND_ ( character ) ) {
    /* Does the character fit the conditions? */
    out_buffer->start = in_buffer->start;
    out_buffer->past_end = out_buffer->start + bytes_read;
    return WICK_SUCCESS;
    }
  else {
    return WICK_FAILURE;
    }
  }

#undef _COND_
#undef _CHAR_NAME_
