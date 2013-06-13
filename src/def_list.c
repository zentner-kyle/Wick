/*
Copyright (c) 2012, Kyle Zentner
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/* TODO: slicing, context, new intrusive design, remove underscores, make list
 * actually container LIST_T not LIST_T*. 
 */

/* These arguments are relatively shared somewhat with array and other collections. */

#ifndef list_prefix
  #define list_prefix list_t
#endif
/* Example: int */

#ifndef list_postfix
  #define list_postfix _list
#endif

#define list_long_prefix macro_join( list_prefix, list_postfix )
/* Example: int_list */

#ifndef list_alloc
  #define list_alloc( bytecount ) malloc( bytecount )
#endif

#ifndef list_free
  #define list_free( elem ) free( elem )
#endif

#ifndef list_elem_move
  /* Only used in insertion on a *copying* list and in build. */
  #define list_elem_move( target, source ) *( target ) = *( source )
#endif

/* Used inside this file to define and access public list functions. */
#define method( name ) macro_join( list_long_prefix, macro_join( _, name ) )

/* Used inside this file to define and access private list functions. */
#define p_method( name ) macro_join( macro_join( macro_join( private, _ ), list_long_prefix ), macro_join( _, name ) )

#ifndef list_error_func
  void p_method( error_func )( char *msg ) {
    printf("Error in " macro_to_string( list_long_prefix ) ": %s\n", msg );
  }
  #define list_error_func p_method( error_func )
#endif

#ifndef list_iterator_t
  #define list_iterator_t macro_join( list_long_prefix, macro_join( _, iterator ) )
#endif


/* List specific arguments. */

#ifdef list_bidirectional
  #ifndef list_prev_name
    #define list_prev_name prev
  #endif
#endif

#ifndef list_alloc_cell
  #define list_alloc_cell list_alloc
#endif

#ifndef list_free_cell
  #define list_free_cell list_free
#endif

#ifndef list_alloc_handle
  #define list_alloc_handle list_alloc
#endif

#ifndef list_free_handle
  #define list_free_handle( to_free ) list_free( to_free )
#endif

#ifndef list_elem_name
#define list_elem_name element
#endif

#ifndef list_next_name
#define list_next_name next
#endif

#ifdef list_error_checks
  #define list_mem_check( memory_check_expr ) wick_mem_check( memory_check_expr )
#else
  #define list_mem_check( memory_check_expr ) ignore_return( memory_check_expr )
#endif


/* List / cons cell type. */

#ifdef list_intrusive
  #define list_cell_t list_t
  #define list_cell_to_elem( cell ) ( cell )
#else
  #define list_cell_to_elem( cell ) ( ( cell )->list_elem_name )
#endif

#ifdef list_use_handle
  /*  Make the name of the handle list_long_prefix.
   *  For example, int_list.
   *  This requires the cell type to have a different name.
   *  For example, int_list_cell.
   */
  #ifndef list_cell_t
    #define list_cell_t macro_join( list_long_prefix, _cell )
    #define list_define_cell_t
  #endif
  #ifndef list_handle_t
    #define list_handle_t list_long_prefix
    #define list_define_handle_t
  #endif
  #ifndef list_handle_cell_name
    #define list_handle_cell_name first_cell
  #endif
      /* *list_handle_t --> *list_cell_t */
  #define list_handle_to_cell( handle_name ) ( ( handle_name )->list_handle_cell_name )
#else
  /*  No handle, make list_cell_t have the name list_long_prefix.
   *  With no handles, list_handle_t == list_cell_t.
   *  This makes the function definitions sane.
   */
  #ifndef list_cell_t
    #define list_cell_t list_long_prefix
    #define list_define_cell_t
  #endif
  #define list_handle_t list_cell_t
  #define list_handle_to_cell( handle_name ) ( handle_name )
#endif

#ifndef list_intrusive
  #ifdef list_define_cell_t
  /*  The list cell type needs to be defined if it hasn't been by the user,
   *  which would be indicated by them defining list_cell_t.
   *  However, it doesn't need to be defined if the list is instrusive.
   */
    struct list_cell_t {
      list_t *list_elem_name;
      struct list_cell_t *list_next_name;
      #ifdef list_bidirectional
        struct list_cell_t *list_prev_name;
      #endif
    };

    typedef struct list_cell_t list_cell_t;
  #endif
#endif

#ifdef list_define_handle_t
  struct list_handle_t {
    list_cell_t * list_handle_cell_name;
  };

  typedef struct list_handle_t list_handle_t;
#endif

/*  Iterators are more general, high-level mechanisms.
 *  They should act 'the same' under all collections.
 *  As such, there is no interface for customizing their internals.
 */

struct list_iterator_t {
  list_cell_t * cell;
};

typedef struct list_iterator_t list_iterator_t;


/* Low level list functions. */

error_number p_method( alloc )( list_cell_t **out_to_store ) {
  /* Allocates and nulls the pointers in just one cell. */
  list_cell_t *new_cell = list_alloc_cell( sizeof( list_cell_t ) );
  if( !new_cell ) {
    return WICK_ALLOC_ERROR;
  }
  new_cell->list_next_name = NULL;
  #ifndef list_intrusive
    new_cell->list_elem_name = NULL;
  #endif
  #ifdef list_bidirectional
    new_cell->list_prev_name = NULL;
  #endif
  *out_to_store = new_cell;
  return WICK_SUCCESS;
}

/* Connects two cells together.
   Also handles connecting to null for the beginning and end. */
void p_method( connect )( list_cell_t *a, list_cell_t *b ) {
  if ( a && b ) {
    a->list_next_name = b;
    #ifdef list_bidirectional
      b->list_prev_name = a;
    #endif
  } else if ( a ) {
    a->list_next_name = NULL;
  }
  #ifdef list_bidirectional
    else if ( b ) {
      b->list_prev_name = NULL;
    }
  #endif
}

void p_method( free_from_cell )( list_cell_t *to_free );

/* Allocates either both a cell and room for an elem, or neither. */
error_number p_method( parallel_alloc )
                ( list_t ** elem_to_alloc, list_cell_t ** cell_to_alloc ) {
  list_t * alloced_elem = list_alloc( sizeof( list_t ) );
  list_cell_t * alloced_cell;
  error_number err = p_method( alloc )( &alloced_cell );
  if ( err != WICK_SUCCESS || !alloced_elem ) {
    /* One allocation failed, but which one? */
    if ( err != WICK_SUCCESS && !alloced_elem ) {
      /* Both failed, undo neither. */
      return WICK_ALLOC_ERROR;
    }
    if ( err != WICK_SUCCESS ) {
      /* The cell allocation failed, undo the elem allocation. */
      list_free( alloced_elem );
      return WICK_ALLOC_ERROR;
    }
    if ( !alloced_elem ) {
      p_method( free_from_cell )( alloced_cell );
      return WICK_ALLOC_ERROR;
    }
  }
  *elem_to_alloc = alloced_elem;
  *cell_to_alloc = alloced_cell;
  return WICK_SUCCESS;
}

/*  Manages allocating all the memory needed for inserting a new elem,
 *  which is none in the case of intrusive lists.
 *  Also copies the element over into a freshly allocated elem space,
 *  unless the list is non-copying ( list_no_copy_elems is defined ).
 */
error_number p_method( new_cell )( list_t * element,
                                list_cell_t ** cell ) {
  #ifdef list_intrusive
    /* No allocation necessary, since the element is the cell.
     * Note that this is not a move, intrusive lists never copy.
     * Instead, the pointer is used.
     */
    *cell = element;
  #else
    #ifdef list_no_copy_elems
      WICK_THROW( p_method( alloc )( cell ) );
      ( *cell )->list_elem_name = element;
    #else
      list_t * new_element;
      WICK_THROW( p_method( parallel_alloc )( &new_element, cell ) );
      list_elem_move( new_element, element );
      ( *cell )->list_elem_name = new_element;
    #endif
  #endif
  return WICK_SUCCESS;
}

/*  Sets an iterator to the last position in the list.
 *  Note: O( n ) performance, clearly.
 */
void method( end )( list_handle_t * list, list_iterator_t *iter ) {
  list_mem_check( iter );
  list_cell_t * cell = list_handle_to_cell( list );
  if ( cell ) {
    while ( cell->list_next_name ) {
      cell = cell->list_next_name;
    }
  }
  iter->cell = cell;
}

/* High level sequence functions. */

/* Pushes a new elem onto the *front* of the list. */
error_number method( push )( list_handle_t **lst, list_t *elem ) {
  list_mem_check( elem && lst && *lst );
  list_cell_t * old_cell = list_handle_to_cell( *lst );
  WICK_THROW( p_method( new_cell )( elem, &( list_handle_to_cell( *lst ) )) );
  p_method( connect )( list_handle_to_cell( *lst ), old_cell );
  return WICK_SUCCESS;
}

/* Pops an elem off the *front* of the list. */
void method( pop )( list_handle_t **lst, list_t **elem ) {
  list_mem_check( elem && lst && *lst );
  /* *elem = list_handle_to_cell( *lst )->list_elem_name; */
  *elem = list_cell_to_elem( list_handle_to_cell( *lst ) );
  list_cell_t *next = list_handle_to_cell( *lst )->list_next_name;
  list_free_cell( list_handle_to_cell( *lst ) );
  #ifdef list_bidirectional
    next->list_prev_name = NULL;
  #endif
  list_handle_to_cell( *lst ) = next;
}

/* Pushes a new elem onto the *end* of the list.
 * Note: O( n ) performance, clearly.
 */
error_number method( append )( list_handle_t **lst, list_t *elem ) {
  list_mem_check( elem && lst && *lst );
  list_cell_t * new_cell = NULL;
  WICK_THROW( p_method( new_cell )( elem, &new_cell ) );
  list_iterator_t end;
  method( end )( *lst, &end );
  p_method( connect )( end.cell, new_cell );
  return WICK_SUCCESS;
}

/* Inserts a new element after a location given by an iterator.
 * Respects non-copying behavior.
 */
error_number method( insert_after )( list_handle_t **seq, list_iterator_t *iterator, list_t *element ) {
  list_mem_check( iterator && element );
  list_cell_t * new_cell;
  WICK_THROW( p_method( new_cell )( element, &new_cell ) );
  if ( iterator->cell ) {
    list_cell_t *start_cell = iterator->cell;
    p_method( connect )( new_cell, start_cell->list_next_name );
    p_method( connect )( start_cell, new_cell );
  } else {
    list_mem_check( seq );
    list_handle_to_cell( *seq ) = new_cell;
    iterator->cell = new_cell;
  }
  return WICK_SUCCESS;
}

#ifdef list_bidirectional
/* Inserts a new element before a location given by an iterator.
 * Respects non-copying behavior.
 */
error_number method( insert_before )( list_handle_t **seq, list_iterator_t *iterator, list_t *element ) {
  list_mem_check( iterator && element );
  list_cell_t * new_cell;
  WICK_THROW( p_method( new_cell )( element, &new_cell ) );
  if ( iterator->cell ) {
    list_cell_t *start_cell = iterator->cell;
    list_cell_t *previous_cell = iterator->cell->list_prev_name;
    p_method( connect )( new_cell, start_cell );
    p_method( connect )( previous_cell, new_cell );
  } else {
    list_mem_check( seq );
    list_handle_to_cell( *seq ) = new_cell;
    iterator->cell = new_cell;
  }
  return WICK_SUCCESS;
}
#endif

/* Free's a cell, and if, if the list is copying, the element.
 * Note: the argument is an actual cell, not a handle.
 * This function is used by all the remove functions to prevent bugs relating to non-copying.
 */
void p_method( delete_cell )( list_cell_t * cell ) {
  #ifndef list_no_copy_elems
    #ifndef list_intrusive
      list_free( cell->list_elem_name );
    #endif
  #endif
  list_free_cell( cell );
}

/* Removes the element after a location given by an iterator.
 * Respects non-copying behavior.
 */
void method( remove_next )( list_handle_t **seq, list_iterator_t *iterator ) {
  list_mem_check( seq && iterator && iterator->cell && iterator->cell->list_next_name );
  list_cell_t * next_cell = iterator->cell->list_next_name;
  p_method( connect )( iterator->cell, next_cell->list_next_name );
  p_method( delete_cell )( next_cell );
}

#ifdef list_bidirectional
/* Removes the element before a location given by an iterator.
 * Respects non-copying behavior.
 */
void method( remove_prev )( list_handle_t **seq, list_iterator_t *iterator ) {
  list_mem_check( seq && iterator && iterator->cell && iterator->cell->list_prev_name );
  list_cell_t * prev_cell = iterator->cell->list_prev_name;
  p_method( connect )( prev_cell->list_prev_name, iterator->cell );
  p_method( delete_cell )( prev_cell );
}

/* Removes the element *at* a location given by an iterator.
 * Respects non-copying behavior.
 */
void method( remove )( list_handle_t **seq, list_iterator_t *iterator ) {
  list_mem_check( seq && iterator && iterator->cell );
  list_cell_t * next_cell = iterator->cell->list_next_name;
  list_cell_t * prev_cell = iterator->cell->list_prev_name;
  p_method( connect )( prev_cell, next_cell );
  p_method( delete_cell )( iterator->cell );
  if ( iterator->cell == list_handle_to_cell( *seq ) ) {
    list_handle_to_cell( *seq ) = next_cell;
  }
  iterator->cell = next_cell;
}
#endif

/* Frees an entire list, given a cell as an argument.
 * Respects non-copying behavior.
 */
void p_method( free_from_cell )( list_cell_t *to_free ) {
  list_cell_t * next;
  while ( to_free ) {
    next = to_free->list_next_name;
    p_method( delete_cell )( to_free );
    to_free = next;
  }
}

/* Frees an entire list given a handle.
 * Respects non-copying behavior.
 */
void method( free )( list_handle_t *to_free ) {
  list_cell_t * current = list_handle_to_cell( to_free );
  p_method( free_from_cell )( current );
  list_free_handle( to_free );
}

/* Destructive, in-place map. */
void method( map )( list_handle_t *list, list_t *( *func )( list_t * ) ) {
  list_cell_t *cell = list_handle_to_cell( list );
  if ( func ) {
    while ( cell ) {
      list_cell_to_elem( cell ) = func( list_cell_to_elem( cell ) );
      cell = cell->list_next_name;
    }
  }
}

/* Prints a list in pseudo-json notation.
 * Uses list_elem_print_func if defined.
 * Otherwise, it prints a message of the form <int @ 0x7f7b62e9a290>.
 */
error_number method( print )( FILE *stream, list_handle_t *list ) {
  list_mem_check( stream );
  list_cell_t *cell = list_handle_to_cell( list );
  fprintf( stream, "[" );
  while ( cell ) {
    #ifdef list_elem_print_func
      list_elem_print_func( stream, list_cell_to_elem( cell ) );
    #else
      fprintf( stream, "<" macro_to_string( list_t ) " @ %p>", list_cell_to_elem( cell ) );
    #endif
    cell = cell->list_next_name;
    if ( cell ) {
      fprintf( stream, ", " );
    }
  }
  fprintf( stream, "]" );
  return WICK_SUCCESS;
}

/* Sets an iterator to the first position in the list. */
void method( start )( list_handle_t * list, list_iterator_t *iter ) {
  list_mem_check( iter );
  iter->cell = list_handle_to_cell( list );
}

/* Acts similarly to repeated calls to next and prev, with some exceptions.
 * Returns true if the iterator arrived at the indicated position, false otherwise.
 * If the movement fails, the iterator won't be modified.
 */
bool method( move )( list_iterator_t *iterator, int64_t steps ) {
  list_cell_t * target = iterator->cell;
  while ( steps > 0 && target ) {
    --steps;
    target = target->list_next_name;
  }
  #ifdef list_bidirectional
    while ( steps < 0 && target ) {
      ++steps;
      target = target->list_prev_name;
    }
  #else
    #ifdef list_error_checks
      if ( steps < 0 ) {
        return WICK_API_ERROR;
      }
    #endif
  #endif
  if ( target ) {
    iterator->cell = target;
    return true;
  } else {
    return false;
  }
}

/* Advances the iterator forward.
 * Returns true if movement succeeded, false otherwise.
 * If the movement fails, the iterator won't be modified.
 */
bool method( next )( list_iterator_t *iterator ) {
  /* NOTE: unchecked pointer dereferencing. */
  list_cell_t * target = iterator->cell->list_next_name;
  if ( target ) {
    iterator->cell = target;
    return true;
  } else {
    return false;
  }
}

#ifdef list_bidirectional
/* Moves the iterator backward.
 * Returns true if movement succeeded, false otherwise.
 * If the movement fails, the iterator won't be modified.
 */
bool method( prev )( list_iterator_t *iterator ) {
  /* NOTE: unchecked pointer dereferencing. */
  list_cell_t * target = iterator->cell->list_prev_name;
  if ( target ) {
    iterator->cell = target;
    return true;
  } else {
    return false;
  }
}
#endif

/* Returns true if the iterator is valid, false otherwise. */
bool method( valid_iter )( list_handle_t * list, list_iterator_t *iterator ) {
  if ( !iterator || !iterator->cell ) {
    return false;
  } else {
    return true;
  }
}

/* Returns the list_t * from an iterator.
 * For example: int_list_iterator * --> int *
 * Note that this function assumes the iterator is valid.
 */
list_t * method( deref )( list_iterator_t *iterator ) {
  return list_cell_to_elem( iterator->cell );
}

/* Returns the list_t * from an iterator.
 * For example: int_list_iterator * --> int *
 * If the iterator isn't valid, this function returns NULL.
 */
list_t * method( safe_deref )( list_handle_t * list, list_iterator_t *iterator ) {
  if (!method( valid_iter )( list, iterator ) ) {
    return NULL;
  }
  return method( deref )( iterator );
}

/* Creates a new list.
 * Without handles, this function just sets the list pointer to NULL.
 */
error_number method( new )( list_handle_t ** to_store ) {
  #ifdef list_use_handle
    list_handle_t *alloced_handle = list_alloc_handle( sizeof( list_handle_t ) );
    if ( alloced_handle ) {
      *to_store = alloced_handle;
      return WICK_SUCCESS;
    } else {
      return WICK_ALLOC_ERROR;
    }
  #else
    *to_store = NULL;
    return WICK_SUCCESS;
  #endif
}

/* Makes a copy of a list.
 * If the list is non-copying, this will be a 'shallow' copy.
 * Otherwise, new elements will be allocated, and list_elem_move will be called.
 * Warning: allocating the new list may fail, in which case all memory will be cleared,
 * but some elements will already have had list_elem_move called on them.
 */
error_number method( copy )( list_handle_t * source, list_handle_t ** destination ) {
  list_iterator_t source_iter;
  list_iterator_t destination_iter;
  list_handle_t * temp_dest;
  error_number e = WICK_SUCCESS;
  WICK_THROW( method( new )( &temp_dest ) );
  method( start )( source, &source_iter );
  method( start )( temp_dest, &destination_iter );
  do {
    e = method( insert_after )(&temp_dest, &destination_iter, method( deref )( &source_iter ) );
    method( next )( &destination_iter );
  } while ( e == WICK_SUCCESS && method( next )( &source_iter ) );
  if ( e != WICK_SUCCESS ) {
    method( free )( temp_dest );
  } else {
    *destination = temp_dest;
  }
  return e;
}

/* Builds a new list using varag syntax.
 * The first argument should be eqaul to the length of the resulting list.
 * In other words, the following example is correct.
 *
 *   int_list *list = NULL;
 *   WICK_THROW( int_list_build( 5, &list, 1, 2, 3, 4, 5 ) );
 *
 * Warning: allocating the new list may fail, in which case all memory will be cleared,
 * but some elements will already have had list_elem_move called on them.
 * Note that non-copying is respected, which may have surprising results occasionally.
 */
error_number method( build )( size_t arg_count, list_handle_t **to_store, ... ) {
  va_list argptr;
  va_start( argptr, to_store );
  WICK_THROW( method( new )( to_store ) );
  list_cell_t *start = NULL;
  list_cell_t *current = NULL;
  list_cell_t *next = NULL;
  error_number err;
  for ( size_t i = arg_count; i > 0; --i ) {
    list_t arg = va_arg( argptr, list_t );
    err = p_method( new_cell )( &arg, &next );
    if ( err != WICK_SUCCESS ) {
      /* If the list is partially allocated, free it. */
      /* Note that free handles NULL pointers fine. */
      /* The end of the list is also safe because list cells are alloced clear. */
      p_method( free_from_cell )( start );
      #ifdef list_use_handle
        list_free_handle( to_store );
      #endif
      return err;
    }
    if ( !start ) {
      start = next;
    }
    if ( current ) {
      p_method( connect )( current, next );
    }
    current = next;
  }
  list_handle_to_cell( *to_store ) = start;
  va_end( argptr );
  return WICK_SUCCESS;
}

#ifdef list_compare_func
/* Performs the work in the inner loop of merge. */
error_number p_method( sort_get_earlier_cell )( list_cell_t **a,
                                                    list_cell_t **b,
                                                    list_cell_t **r ) {
  if ( !*a ) {
    *r = *b;
    *b = ( *b )->list_next_name;
  } else if ( !*b ) {
    *r = *a;
    *a = ( *a )->list_next_name;
  } else {
    int32_t compare_val = 0;
    WICK_THROW( list_compare_func( list_cell_to_elem( *a ), list_cell_to_elem( *b ), &compare_val ) );
    if ( compare_val <= 0 ) {
      *r = *a;
      *a = ( *a )->list_next_name;
    } else {
      *r = *b;
      *b = ( *b )->list_next_name;
    }
  }
  return WICK_SUCCESS;
}

/* Used for sort to perform a merge sort. */
error_number p_method( merge )( list_handle_t * a, list_handle_t * b, list_handle_t ** result ) {
  list_cell_t * cell_a = list_handle_to_cell( a );
  list_cell_t * cell_b = list_handle_to_cell( b );
  list_cell_t * cell_prev = NULL;
  list_cell_t * cell_current = NULL;
  list_cell_t * result_start;
  WICK_THROW( p_method( sort_get_earlier_cell )( &cell_a, &cell_b, &result_start ) );
  cell_prev = result_start;
  while ( cell_a || cell_b ) {
    WICK_THROW( p_method( sort_get_earlier_cell )( &cell_a, &cell_b, &cell_current ) );
    p_method( connect )( cell_prev, cell_current );
    cell_prev = cell_prev->list_next_name;
  }
  list_handle_to_cell( *result ) = result_start;
  return WICK_SUCCESS;
}

/* Performs a merge sort.
 * TODO: avoid traversing the front parts of the list repeatedly.
 */
error_number method( sort )( list_handle_t **list ) {
  list_mem_check( list );
  if (!*list || !( *list )->list_next_name ) {
    return WICK_SUCCESS;
  }
  list_cell_t * counter = list_handle_to_cell( *list )->list_next_name;
  list_cell_t * halfway = list_handle_to_cell( *list );
  list_cell_t * first_half = list_handle_to_cell( *list );
  list_cell_t * second_half = NULL;
  while ( counter ) {
    counter = counter->list_next_name;
    if ( counter ) {
      counter = counter->list_next_name;
      halfway = halfway->list_next_name;
    }
  }
  second_half = halfway->list_next_name;
  fflush( stdout );
  p_method( connect( NULL, second_half ) );
  p_method( connect( halfway, NULL ) );
  WICK_THROW( method( sort )( &second_half ) );
  WICK_THROW( method( sort )( &first_half ) );
  WICK_THROW( p_method( merge )( first_half, second_half, list ) );
  return WICK_SUCCESS;
}

/* Compares two lists with the same type using the list_compare_func
 * to determine if they're equal.
 */
error_number method( equal )( list_handle_t *a, list_handle_t *b, bool * result ) {
  list_iterator_t a_iter;
  list_iterator_t b_iter;
  method( start )( a, &a_iter );
  method( start )( b, &b_iter );
  bool a_next = false;
  bool b_next = false;
  int32_t temp_result = 0;
  do {
    WICK_THROW( list_compare_func( method( deref )( &a_iter ), method( deref )( &b_iter ), &temp_result ) );
    if ( temp_result != 0 ) {
      result = false;
      return WICK_SUCCESS;
    }
    a_next = method( next )( &a_iter );
    b_next = method( next )( &b_iter );
  } while ( a_next && b_next );
  if ( a_next != b_next ) {
    *result = false;
  } else {
    *result = true;
  }
  return WICK_SUCCESS;
}
#endif

#undef list_t
#undef list_prefix
#undef list_postfix
#undef list_long_prefix
#undef list_alloc
#undef list_free
#undef list_error_func
#undef list_bidirectional
#undef list_iterator_t
#undef list_alloc_cell
#undef list_free_cell
#undef list_cell_t
#undef list_elem_name
#undef list_next_name
#undef list_prev_name
#undef list_mem_check
#undef list_elem_move
#undef list_no_copy_elems
#undef list_define_cell_t
#undef list_define_handle_t
#undef list_handle_t
#undef list_handle_to_cell
#undef list_handle_cell_name
#undef list_free_handle
#undef list_alloc_handle
#undef list_compare_func
#undef list_intrusive
#undef list_cell_to_elem
#undef list_elem_print_func
#undef list_use_handle
#undef list_error_checks
#undef method
#undef p_method
