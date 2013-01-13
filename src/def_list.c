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

#ifndef _LIST_PREFIX_
  #define _LIST_PREFIX_ _LIST_T_
#endif
/* Example: int */

#ifndef _LIST_POSTFIX_
  #define _LIST_POSTFIX_ _list
#endif

#define _LIST_LONG_PREFIX_ C_TOKEN_JOIN(_LIST_PREFIX_, _LIST_POSTFIX_)
/* Example: int_list */

#ifndef _LIST_ALLOC_
  #define _LIST_ALLOC_(_bytecount_) malloc(_bytecount_)
#endif

#ifndef _LIST_FREE_
  #define _LIST_FREE_(_elem_) free(_elem_)
#endif

#ifndef _LIST_ELEM_MOVE_
  /* Only used in insertion on a *copying* list and in build. */
  #define _LIST_ELEM_MOVE_(_target_, _source_) *(_target_) = *(_source_)
#endif

/* Used inside this file to define and access public list functions. */
#define LIST_FUNC(_func_postfix_) C_TOKEN_JOIN(_LIST_LONG_PREFIX_, _func_postfix_)

/* Used inside this file to define and access private list functions. */
#define P_LIST_FUNC(_func_postfix_) C_TOKEN_JOIN(_, C_TOKEN_JOIN(_LIST_LONG_PREFIX_, _func_postfix_))

#ifndef _LIST_ERROR_FUNC_
  void P_LIST_FUNC(_error_func)(char *msg) {
    printf("Error in " C_MACRO2STRING(_LIST_LONG_PREFIX_) ": %s\n", msg);
  }
  #define _LIST_ERROR_FUNC_ P_LIST_FUNC(_error_func)
#endif

#ifndef _LIST_ITERATOR_T_
  #define _LIST_ITERATOR_T_ C_TOKEN_JOIN(_LIST_LONG_PREFIX_, _iterator)
#endif


/* List specific arguments. */

#ifdef _LIST_BIDIRECTIONAL_
  #ifndef _LIST_PREV_NAME_
    #define _LIST_PREV_NAME_ prev
  #endif
#endif

#ifndef _LIST_ALLOC_CELL_
  #define _LIST_ALLOC_CELL_ _LIST_ALLOC_
#endif

#ifndef _LIST_FREE_CELL_
  #define _LIST_FREE_CELL_ _LIST_FREE_
#endif

#ifndef _LIST_ALLOC_HANDLE_
  #define _LIST_ALLOC_HANDLE_ _LIST_ALLOC_
#endif

#ifndef _LIST_FREE_HANDLE_
  #define _LIST_FREE_HANDLE_(_to_free_) _LIST_FREE_(_to_free_)
#endif

#ifndef _LIST_ELEM_NAME_
#define _LIST_ELEM_NAME_ element
#endif

#ifndef _LIST_NEXT_NAME_
#define _LIST_NEXT_NAME_ next
#endif

#ifdef _LIST_ERROR_CHECKS_
  #define _LIST_MEM_CHECK_(_memory_check_expr_) WICK_MEM_CHECK(_memory_check_expr_)
#else
  #define _LIST_MEM_CHECK_(_memory_check_expr_) IGNORE_RETURN(_memory_check_expr_)
#endif


/* List / cons cell type. */

#ifdef _LIST_INTRUSIVE_
  #define _LIST_CELL_T_ _LIST_T_
  #define _LIST_CELL_TO_ELEM_(_cell_) (_cell_)
#else
  #define _LIST_CELL_TO_ELEM_(_cell_) ((_cell_)->_LIST_ELEM_NAME_)
#endif

#ifdef _LIST_USE_HANDLE_
  /*  Make the name of the handle _LIST_LONG_PREFIX_.
   *  For example, int_list.
   *  This requires the cell type to have a different name.
   *  For example, int_list_cell.
   */
  #ifndef _LIST_CELL_T_
    #define _LIST_CELL_T_ C_TOKEN_JOIN(_LIST_LONG_PREFIX_, _cell)
    #define _LIST_DEFINE_CELL_T_
  #endif
  #ifndef _LIST_HANDLE_T_
    #define _LIST_HANDLE_T_ _LIST_LONG_PREFIX_
    #define _LIST_DEFINE_HANDLE_T_
  #endif
  #ifndef _LIST_HANDLE_CELL_NAME_
    #define _LIST_HANDLE_CELL_NAME_ first_cell
  #endif
      /* *_LIST_HANDLE_T_ --> *_LIST_CELL_T_ */
  #define _LIST_HANDLE_TO_CELL_(_handle_name_) ((_handle_name_)->_LIST_HANDLE_CELL_NAME_)
#else
  /*  No handle, make _LIST_CELL_T_ have the name _LIST_LONG_PREFIX_.
   *  With no handles, _LIST_HANDLE_T_ == _LIST_CELL_T_.
   *  This makes the function definitions sane.
   */
  #ifndef _LIST_CELL_T_
    #define _LIST_CELL_T_ _LIST_LONG_PREFIX_
    #define _LIST_DEFINE_CELL_T_
  #endif
  #define _LIST_HANDLE_T_ _LIST_CELL_T_
  #define _LIST_HANDLE_TO_CELL_(_handle_name_) (_handle_name_)
#endif

#ifndef _LIST_INTRUSIVE_
  #ifdef _LIST_DEFINE_CELL_T_
  /*  The list cell type needs to be defined if it hasn't been by the user,
   *  which would be indicated by them defining _LIST_CELL_T_.
   *  However, it doesn't need to be defined if the list is instrusive.
   */
    struct _LIST_CELL_T_ {
      _LIST_T_ *_LIST_ELEM_NAME_;
      struct _LIST_CELL_T_ *_LIST_NEXT_NAME_;
      #ifdef _LIST_BIDIRECTIONAL_
        struct _LIST_CELL_T_ *_LIST_PREV_NAME_;
      #endif
    };

    typedef struct _LIST_CELL_T_ _LIST_CELL_T_;
  #endif
#endif

#ifdef _LIST_DEFINE_HANDLE_T_
  struct _LIST_HANDLE_T_ {
    _LIST_CELL_T_ * _LIST_HANDLE_CELL_NAME_;
  };

  typedef struct _LIST_HANDLE_T_ _LIST_HANDLE_T_;
#endif

/*  Iterators are more general, high-level mechanisms.
 *  They should act 'the same' under all collections.
 *  As such, there is no interface for customizing their internals.
 */

struct _LIST_ITERATOR_T_ {
  _LIST_CELL_T_ * cell;
};

typedef struct _LIST_ITERATOR_T_ _LIST_ITERATOR_T_;


/* Low level list functions. */

wick_error P_LIST_FUNC(_alloc)(_LIST_CELL_T_ **out_to_store) {
  /* Allocates and nulls the pointers in just one cell. */
  _LIST_CELL_T_ *new_cell = _LIST_ALLOC_CELL_(sizeof(_LIST_CELL_T_));
  if(!new_cell) {
    return WICK_ALLOC_ERROR;
  }
  new_cell->_LIST_NEXT_NAME_ = NULL;
  #ifndef _LIST_INTRUSIVE_
    new_cell->_LIST_ELEM_NAME_ = NULL;
  #endif
  #ifdef _LIST_BIDIRECTIONAL_
    new_cell->_LIST_PREV_NAME_ = NULL;
  #endif
  *out_to_store = new_cell;
  return WICK_SUCCESS;
}

/* Connects two cells together.
   Also handles connecting to null for the beginning and end. */
void P_LIST_FUNC(_connect)(_LIST_CELL_T_ *a, _LIST_CELL_T_ *b) {
  if (a && b) {
    a->_LIST_NEXT_NAME_ = b;
    #ifdef _LIST_BIDIRECTIONAL_
      b->_LIST_PREV_NAME_ = a;
    #endif
  } else if (a) {
    a->_LIST_NEXT_NAME_ = NULL;
  }
  #ifdef _LIST_BIDIRECTIONAL_
    else if (b) {
      b->_LIST_PREV_NAME_ = NULL;
    }
  #endif
}

void P_LIST_FUNC(_free_from_cell)(_LIST_CELL_T_ *to_free);

/* Allocates either both a cell and room for an elem, or neither. */
wick_error P_LIST_FUNC(_parallel_alloc)
                (_LIST_T_ ** elem_to_alloc, _LIST_CELL_T_ ** cell_to_alloc) {
  _LIST_T_ * alloced_elem = _LIST_ALLOC_(sizeof(_LIST_T_));
  _LIST_CELL_T_ * alloced_cell;
  wick_error err = P_LIST_FUNC(_alloc)(&alloced_cell);
  if (err != WICK_SUCCESS || !alloced_elem) {
    /* One allocation failed, but which one? */
    if (err != WICK_SUCCESS && !alloced_elem) {
      /* Both failed, undo neither. */
      return WICK_ALLOC_ERROR;
    }
    if (err != WICK_SUCCESS) {
      /* The cell allocation failed, undo the elem allocation. */
      _LIST_FREE_(alloced_elem);
      return WICK_ALLOC_ERROR;
    }
    if (!alloced_elem) {
      P_LIST_FUNC(_free_from_cell)(alloced_cell);
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
 *  unless the list is non-copying (_LIST_NO_COPY_ELEMS_ is defined).
 */
wick_error P_LIST_FUNC(_new_cell)(_LIST_T_ * element,
                                _LIST_CELL_T_ ** cell) {
  #ifdef _LIST_INTRUSIVE_
    /* No allocation necessary, since the element is the cell.
     * Note that this is not a move, intrusive lists never copy.
     * Instead, the pointer is used.
     */
    *cell = element;
  #else
    #ifdef _LIST_NO_COPY_ELEMS_
      WICK_THROW(P_LIST_FUNC(_alloc)(cell));
      (*cell)->_LIST_ELEM_NAME_ = element;
    #else
      _LIST_T_ * new_element;
      WICK_THROW(P_LIST_FUNC(_parallel_alloc)(&new_element, cell));
      _LIST_ELEM_MOVE_(new_element, element);
      (*cell)->_LIST_ELEM_NAME_ = new_element;
    #endif
  #endif
  return WICK_SUCCESS;
}

/*  Sets an iterator to the last position in the list.
 *  Note: O(n) performance, clearly.
 */
void LIST_FUNC(_end)(_LIST_HANDLE_T_ * list, _LIST_ITERATOR_T_ *iter) {
  _LIST_MEM_CHECK_(iter);
  _LIST_CELL_T_ * cell = _LIST_HANDLE_TO_CELL_(list);
  if (cell) {
    while (cell->_LIST_NEXT_NAME_) {
      cell = cell->_LIST_NEXT_NAME_;
    }
  }
  iter->cell = cell;
}

/* High level sequence functions. */

/* Pushes a new elem onto the *front* of the list. */
wick_error LIST_FUNC(_push)(_LIST_HANDLE_T_ **lst, _LIST_T_ *elem) {
  _LIST_MEM_CHECK_(elem && lst && *lst);
  _LIST_CELL_T_ * old_cell = _LIST_HANDLE_TO_CELL_(*lst);
  WICK_THROW(P_LIST_FUNC(_new_cell)(elem, &(_LIST_HANDLE_TO_CELL_(*lst))));
  P_LIST_FUNC(_connect)(_LIST_HANDLE_TO_CELL_(*lst), old_cell);
  return WICK_SUCCESS;
}

/* Pops an elem off the *front* of the list. */
void LIST_FUNC(_pop)(_LIST_HANDLE_T_ **lst, _LIST_T_ **elem) {
  _LIST_MEM_CHECK_(elem && lst && *lst);
  /* *elem = _LIST_HANDLE_TO_CELL_(*lst)->_LIST_ELEM_NAME_; */
  *elem = _LIST_CELL_TO_ELEM_(_LIST_HANDLE_TO_CELL_(*lst));
  _LIST_CELL_T_ *next = _LIST_HANDLE_TO_CELL_(*lst)->_LIST_NEXT_NAME_;
  _LIST_FREE_CELL_(_LIST_HANDLE_TO_CELL_(*lst));
  #ifdef _LIST_BIDIRECTIONAL_
    next->_LIST_PREV_NAME_ = NULL;
  #endif
  _LIST_HANDLE_TO_CELL_(*lst) = next;
}

/* Pushes a new elem onto the *end* of the list.
 * Note: O(n) performance, clearly.
 */
wick_error LIST_FUNC(_append)(_LIST_HANDLE_T_ **lst, _LIST_T_ *elem) {
  _LIST_MEM_CHECK_(elem && lst && *lst);
  _LIST_CELL_T_ * new_cell = NULL;
  WICK_THROW(P_LIST_FUNC(_new_cell)(elem, &new_cell));
  _LIST_ITERATOR_T_ end;
  LIST_FUNC(_end)(*lst, &end);
  P_LIST_FUNC(_connect)(end.cell, new_cell);
  return WICK_SUCCESS;
}

/* Inserts a new element after a location given by an iterator.
 * Respects non-copying behavior.
 */
wick_error LIST_FUNC(_insert_after)(_LIST_HANDLE_T_ **seq, _LIST_ITERATOR_T_ *iterator, _LIST_T_ *element) {
  _LIST_MEM_CHECK_(iterator && element);
  _LIST_CELL_T_ * new_cell;
  WICK_THROW(P_LIST_FUNC(_new_cell)(element, &new_cell));
  if (iterator->cell) {
    _LIST_CELL_T_ *start_cell = iterator->cell;
    P_LIST_FUNC(_connect)(new_cell, start_cell->_LIST_NEXT_NAME_);
    P_LIST_FUNC(_connect)(start_cell, new_cell);
  } else {
    _LIST_MEM_CHECK_(seq);
    _LIST_HANDLE_TO_CELL_(*seq) = new_cell;
    iterator->cell = new_cell;
  }
  return WICK_SUCCESS;
}

#ifdef _LIST_BIDIRECTIONAL_
/* Inserts a new element before a location given by an iterator.
 * Respects non-copying behavior.
 */
wick_error LIST_FUNC(_insert_before)(_LIST_HANDLE_T_ **seq, _LIST_ITERATOR_T_ *iterator, _LIST_T_ *element) {
  _LIST_MEM_CHECK_(iterator && element);
  _LIST_CELL_T_ * new_cell;
  WICK_THROW(P_LIST_FUNC(_new_cell)(element, &new_cell));
  if (iterator->cell) {
    _LIST_CELL_T_ *start_cell = iterator->cell;
    _LIST_CELL_T_ *previous_cell = iterator->cell->_LIST_PREV_NAME_;
    P_LIST_FUNC(_connect)(new_cell, start_cell);
    P_LIST_FUNC(_connect)(previous_cell, new_cell);
  } else {
    _LIST_MEM_CHECK_(seq);
    _LIST_HANDLE_TO_CELL_(*seq) = new_cell;
    iterator->cell = new_cell;
  }
  return WICK_SUCCESS;
}
#endif

/* Free's a cell, and if, if the list is copying, the element.
 * Note: the argument is an actual cell, not a handle.
 * This function is used by all the remove functions to prevent bugs relating to non-copying.
 */
void P_LIST_FUNC(_delete_cell)(_LIST_CELL_T_ * cell) {
  #ifndef _LIST_NO_COPY_ELEMS_
    #ifndef _LIST_INTRUSIVE_
      _LIST_FREE_(cell->_LIST_ELEM_NAME_);
    #endif
  #endif
  _LIST_FREE_CELL_(cell);
}

/* Removes the element after a location given by an iterator.
 * Respects non-copying behavior.
 */
void LIST_FUNC(_remove_next)(_LIST_HANDLE_T_ **seq, _LIST_ITERATOR_T_ *iterator) {
  _LIST_MEM_CHECK_(seq && iterator && iterator->cell && iterator->cell->_LIST_NEXT_NAME_);
  _LIST_CELL_T_ * next_cell = iterator->cell->_LIST_NEXT_NAME_;
  P_LIST_FUNC(_connect)(iterator->cell, next_cell->_LIST_NEXT_NAME_);
  P_LIST_FUNC(_delete_cell)(next_cell);
}

#ifdef _LIST_BIDIRECTIONAL_
/* Removes the element before a location given by an iterator.
 * Respects non-copying behavior.
 */
void LIST_FUNC(_remove_prev)(_LIST_HANDLE_T_ **seq, _LIST_ITERATOR_T_ *iterator) {
  _LIST_MEM_CHECK_(seq && iterator && iterator->cell && iterator->cell->_LIST_PREV_NAME_);
  _LIST_CELL_T_ * prev_cell = iterator->cell->_LIST_PREV_NAME_;
  P_LIST_FUNC(_connect)(prev_cell->_LIST_PREV_NAME_, iterator->cell);
  P_LIST_FUNC(_delete_cell)(prev_cell);
}

/* Removes the element *at* a location given by an iterator.
 * Respects non-copying behavior.
 */
void LIST_FUNC(_remove)(_LIST_HANDLE_T_ **seq, _LIST_ITERATOR_T_ *iterator) {
  _LIST_MEM_CHECK_(seq && iterator && iterator->cell);
  _LIST_CELL_T_ * next_cell = iterator->cell->_LIST_NEXT_NAME_;
  _LIST_CELL_T_ * prev_cell = iterator->cell->_LIST_PREV_NAME_;
  P_LIST_FUNC(_connect)(prev_cell, next_cell);
  P_LIST_FUNC(_delete_cell)(iterator->cell);
  if (iterator->cell == _LIST_HANDLE_TO_CELL_(*seq)) {
    _LIST_HANDLE_TO_CELL_(*seq) = next_cell;
  }
  iterator->cell = next_cell;
}
#endif

/* Frees an entire list, given a cell as an argument.
 * Respects non-copying behavior.
 */
void P_LIST_FUNC(_free_from_cell)(_LIST_CELL_T_ *to_free) {
  _LIST_CELL_T_ * next;
  while (to_free) {
    next = to_free->_LIST_NEXT_NAME_;
    P_LIST_FUNC(_delete_cell)(to_free);
    to_free = next;
  }
}

/* Frees an entire list given a handle.
 * Respects non-copying behavior.
 */
void LIST_FUNC(_free)(_LIST_HANDLE_T_ *to_free) {
  _LIST_CELL_T_ * current = _LIST_HANDLE_TO_CELL_(to_free);
  P_LIST_FUNC(_free_from_cell)(current);
  _LIST_FREE_HANDLE_(to_free);
}

/* Destructive, in-place map. */
void LIST_FUNC(_map)(_LIST_HANDLE_T_ *list, _LIST_T_ *(*func)(_LIST_T_ *)) {
  _LIST_CELL_T_ *cell = _LIST_HANDLE_TO_CELL_(list);
  if (func) {
    while (cell) {
      _LIST_CELL_TO_ELEM_(cell) = func(_LIST_CELL_TO_ELEM_(cell));
      cell = cell->_LIST_NEXT_NAME_;
    }
  }
}

/* Prints a list in pseudo-json notation.
 * Uses _LIST_ELEM_PRINT_FUNC_ if defined.
 * Otherwise, it prints a message of the form <int @ 0x7f7b62e9a290>.
 */
wick_error LIST_FUNC(_print)(FILE *stream, _LIST_HANDLE_T_ *list) {
  _LIST_MEM_CHECK_(stream);
  _LIST_CELL_T_ *cell = _LIST_HANDLE_TO_CELL_(list);
  fprintf(stream, "[");
  while (cell) {
    #ifdef _LIST_ELEM_PRINT_FUNC_
      _LIST_ELEM_PRINT_FUNC_(stream, _LIST_CELL_TO_ELEM_(cell));
    #else
      fprintf(stream, "<" C_MACRO2STRING(_LIST_T_) " @ %p>", _LIST_CELL_TO_ELEM_(cell));
    #endif
    cell = cell->_LIST_NEXT_NAME_;
    if (cell) {
      fprintf(stream, ", ");
    }
  }
  fprintf(stream, "]");
  return WICK_SUCCESS;
}

/* Sets an iterator to the first position in the list. */
void LIST_FUNC(_start)(_LIST_HANDLE_T_ * list, _LIST_ITERATOR_T_ *iter) {
  _LIST_MEM_CHECK_(iter);
  iter->cell = _LIST_HANDLE_TO_CELL_(list);
}

/* Acts similarly to repeated calls to _next and _prev, with some exceptions.
 * Returns true if the iterator arrived at the indicated position, false otherwise.
 * If the movement fails, the iterator won't be modified.
 */
bool LIST_FUNC(_move)(_LIST_ITERATOR_T_ *iterator, int64_t steps) {
  _LIST_CELL_T_ * target = iterator->cell;
  while (steps > 0 && target) {
    --steps;
    target = target->_LIST_NEXT_NAME_;
  }
  #ifdef _LIST_BIDIRECTIONAL_
    while (steps < 0 && target) {
      ++steps;
      target = target->_LIST_PREV_NAME_;
    }
  #else
    #ifdef _LIST_ERROR_CHECKS_
      if (steps < 0) {
        return WICK_API_ERROR;
      }
    #endif
  #endif
  if (target) {
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
bool LIST_FUNC(_next)(_LIST_ITERATOR_T_ *iterator) {
  /* NOTE: unchecked pointer dereferencing. */
  _LIST_CELL_T_ * target = iterator->cell->_LIST_NEXT_NAME_;
  if (target) {
    iterator->cell = target;
    return true;
  } else {
    return false;
  }
}

#ifdef _LIST_BIDIRECTIONAL_
/* Moves the iterator backward.
 * Returns true if movement succeeded, false otherwise.
 * If the movement fails, the iterator won't be modified.
 */
bool LIST_FUNC(_prev)(_LIST_ITERATOR_T_ *iterator) {
  /* NOTE: unchecked pointer dereferencing. */
  _LIST_CELL_T_ * target = iterator->cell->_LIST_PREV_NAME_;
  if (target) {
    iterator->cell = target;
    return true;
  } else {
    return false;
  }
}
#endif

/* Returns true if the iterator is valid, false otherwise. */
bool LIST_FUNC(_valid_iter)(_LIST_HANDLE_T_ * list, _LIST_ITERATOR_T_ *iterator) {
  if (!iterator || !iterator->cell) {
    return false;
  } else {
    return true;
  }
}

/* Returns the _LIST_T_ * from an iterator.
 * For example: int_list_iterator * --> int *
 * Note that this function assumes the iterator is valid.
 */
_LIST_T_ * LIST_FUNC(_deref)(_LIST_ITERATOR_T_ *iterator) {
  return _LIST_CELL_TO_ELEM_(iterator->cell);
}

/* Returns the _LIST_T_ * from an iterator.
 * For example: int_list_iterator * --> int *
 * If the iterator isn't valid, this function returns NULL.
 */
_LIST_T_ * LIST_FUNC(_safe_deref)(_LIST_HANDLE_T_ * list, _LIST_ITERATOR_T_ *iterator) {
  if (!LIST_FUNC(_valid_iter)(list, iterator)) {
    return NULL;
  }
  return LIST_FUNC(_deref)(iterator);
}

/* Creates a new list.
 * Without handles, this function just sets the list pointer to NULL.
 */
wick_error LIST_FUNC(_new)(_LIST_HANDLE_T_ ** to_store) {
  #ifdef _LIST_USE_HANDLE_
    _LIST_HANDLE_T_ *alloced_handle = _LIST_ALLOC_HANDLE_(sizeof(_LIST_HANDLE_T_));
    if (alloced_handle) {
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
 * Otherwise, new elements will be allocated, and _LIST_ELEM_MOVE_ will be called.
 * Warning: allocating the new list may fail, in which case all memory will be cleared,
 * but some elements will already have had _LIST_ELEM_MOVE_ called on them.
 */
wick_error LIST_FUNC(_copy)(_LIST_HANDLE_T_ * source, _LIST_HANDLE_T_ ** destination) {
  _LIST_ITERATOR_T_ source_iter;
  _LIST_ITERATOR_T_ destination_iter;
  _LIST_HANDLE_T_ * temp_dest;
  wick_error e = WICK_SUCCESS;
  WICK_THROW(LIST_FUNC(_new)(&temp_dest));
  LIST_FUNC(_start)(source, &source_iter);
  LIST_FUNC(_start)(temp_dest, &destination_iter);
  do {
    e = LIST_FUNC(_insert_after)(&temp_dest, &destination_iter, LIST_FUNC(_deref)(&source_iter));
    LIST_FUNC(_next)(&destination_iter);
  } while (e == WICK_SUCCESS && LIST_FUNC(_next)(&source_iter));
  if (e != WICK_SUCCESS) {
    LIST_FUNC(_free)(temp_dest);
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
 *   WICK_THROW(int_list_build(5, &list, 1, 2, 3, 4, 5));
 *
 * Warning: allocating the new list may fail, in which case all memory will be cleared,
 * but some elements will already have had _LIST_ELEM_MOVE_ called on them.
 * Note that non-copying is respected, which may have surprising results occasionally.
 */
wick_error LIST_FUNC(_build)(size_t arg_count, _LIST_HANDLE_T_ **to_store, ...) {
  va_list argptr;
  va_start(argptr, to_store);
  WICK_THROW(LIST_FUNC(_new)(to_store));
  _LIST_CELL_T_ *start = NULL;
  _LIST_CELL_T_ *current = NULL;
  _LIST_CELL_T_ *next = NULL;
  wick_error err;
  for (size_t i = arg_count; i > 0; --i) {
    _LIST_T_ arg = va_arg(argptr, _LIST_T_);
    err = P_LIST_FUNC(_new_cell)(&arg, &next);
    if (err != WICK_SUCCESS) {
      /* If the list is partially allocated, free it. */
      /* Note that free handles NULL pointers fine. */
      /* The end of the list is also safe because list cells are alloced clear. */
      P_LIST_FUNC(_free_from_cell)(start);
      #ifdef _LIST_USE_HANDLE_
        _LIST_FREE_HANDLE_(to_store);
      #endif
      return err;
    }
    if (!start) {
      start = next;
    }
    if (current) {
      P_LIST_FUNC(_connect)(current, next);
    }
    current = next;
  }
  _LIST_HANDLE_TO_CELL_(*to_store) = start;
  va_end(argptr);
  return WICK_SUCCESS;
}

#ifdef _LIST_COMPARE_FUNC_
/* Performs the work in the inner loop of merge. */
wick_error P_LIST_FUNC(_sort_get_earlier_cell)(_LIST_CELL_T_ **a,
                                                    _LIST_CELL_T_ **b,
                                                    _LIST_CELL_T_ **r) {
  if (!*a) {
    *r = *b;
    *b = (*b)->_LIST_NEXT_NAME_;
  } else if (!*b) {
    *r = *a;
    *a = (*a)->_LIST_NEXT_NAME_;
  } else {
    int32_t compare_val = 0;
    WICK_THROW(_LIST_COMPARE_FUNC_(_LIST_CELL_TO_ELEM_(*a), _LIST_CELL_TO_ELEM_(*b), &compare_val));
    if (compare_val <= 0) {
      *r = *a;
      *a = (*a)->_LIST_NEXT_NAME_;
    } else {
      *r = *b;
      *b = (*b)->_LIST_NEXT_NAME_;
    }
  }
  return WICK_SUCCESS;
}

/* Used for sort to perform a merge sort. */
wick_error P_LIST_FUNC(_merge)(_LIST_HANDLE_T_ * a, _LIST_HANDLE_T_ * b, _LIST_HANDLE_T_ ** result) {
  _LIST_CELL_T_ * cell_a = _LIST_HANDLE_TO_CELL_(a);
  _LIST_CELL_T_ * cell_b = _LIST_HANDLE_TO_CELL_(b);
  _LIST_CELL_T_ * cell_prev = NULL;
  _LIST_CELL_T_ * cell_current = NULL;
  _LIST_CELL_T_ * result_start;
  WICK_THROW(P_LIST_FUNC(_sort_get_earlier_cell)(&cell_a, &cell_b, &result_start));
  cell_prev = result_start;
  while (cell_a || cell_b) {
    WICK_THROW(P_LIST_FUNC(_sort_get_earlier_cell)(&cell_a, &cell_b, &cell_current));
    P_LIST_FUNC(_connect)(cell_prev, cell_current);
    cell_prev = cell_prev->_LIST_NEXT_NAME_;
  }
  _LIST_HANDLE_TO_CELL_(*result) = result_start;
  return WICK_SUCCESS;
}

/* Performs a merge sort.
 * TODO: avoid traversing the front parts of the list repeatedly.
 */
wick_error LIST_FUNC(_sort)(_LIST_HANDLE_T_ **list) {
  _LIST_MEM_CHECK_(list);
  if (!*list || !(*list)->_LIST_NEXT_NAME_) {
    return WICK_SUCCESS;
  }
  _LIST_CELL_T_ * counter = _LIST_HANDLE_TO_CELL_(*list)->_LIST_NEXT_NAME_;
  _LIST_CELL_T_ * halfway = _LIST_HANDLE_TO_CELL_(*list);
  _LIST_CELL_T_ * first_half = _LIST_HANDLE_TO_CELL_(*list);
  _LIST_CELL_T_ * second_half = NULL;
  while (counter) {
    counter = counter->_LIST_NEXT_NAME_;
    if (counter) {
      counter = counter->_LIST_NEXT_NAME_;
      halfway = halfway->_LIST_NEXT_NAME_;
    }
  }
  second_half = halfway->_LIST_NEXT_NAME_;
  fflush(stdout);
  P_LIST_FUNC(_connect(NULL, second_half));
  P_LIST_FUNC(_connect(halfway, NULL));
  WICK_THROW(LIST_FUNC(_sort)(&second_half));
  WICK_THROW(LIST_FUNC(_sort)(&first_half));
  WICK_THROW(P_LIST_FUNC(_merge)(first_half, second_half, list));
  return WICK_SUCCESS;
}

/* Compares two lists with the same type using the _LIST_COMPARE_FUNC_
 * to determine if they're equal.
 */
wick_error LIST_FUNC(_equal)(_LIST_HANDLE_T_ *a, _LIST_HANDLE_T_ *b, bool * result) {
  _LIST_ITERATOR_T_ a_iter;
  _LIST_ITERATOR_T_ b_iter;
  LIST_FUNC(_start)(a, &a_iter);
  LIST_FUNC(_start)(b, &b_iter);
  bool a_next = false;
  bool b_next = false;
  int32_t temp_result = 0;
  do {
    WICK_THROW(_LIST_COMPARE_FUNC_(LIST_FUNC(_deref)(&a_iter), LIST_FUNC(_deref)(&b_iter), &temp_result));
    if (temp_result != 0) {
      result = false;
      return WICK_SUCCESS;
    }
    a_next = LIST_FUNC(_next)(&a_iter);
    b_next = LIST_FUNC(_next)(&b_iter);
  } while (a_next && b_next);
  if (a_next != b_next) {
    *result = false;
  } else {
    *result = true;
  }
  return WICK_SUCCESS;
}
#endif

#undef _LIST_T_
#undef _LIST_PREFIX_
#undef _LIST_POSTFIX_
#undef _LIST_LONG_PREFIX_
#undef _LIST_ALLOC_
#undef _LIST_FREE_
#undef _LIST_ERROR_FUNC_
#undef _LIST_BIDIRECTIONAL_
#undef _LIST_ITERATOR_T_
#undef _LIST_DOUBLE_LINKED_
#undef _LIST_ALLOC_CELL_
#undef _LIST_FREE_CELL_
#undef _LIST_CELL_T_
#undef _LIST_ELEM_NAME_
#undef _LIST_NEXT_NAME_
#undef _LIST_PREV_NAME_
#undef _LIST_MEM_CHECK_
#undef _LIST_ELEM_MOVE_
#undef _LIST_NO_COPY_ELEMS_
#undef _LIST_DEFINE_CELL_T_
#undef _LIST_DEFINE_HANDLE_T_
#undef _LIST_HANDLE_T_
#undef _LIST_HANDLE_TO_CELL_
#undef _LIST_HANDLE_CELL_NAME_
#undef _LIST_FREE_HANDLE_
#undef _LIST_ALLOC_HANDLE_
#undef _LIST_COMPARE_FUNC_
#undef _LIST_INTRUSIVE_
#undef _LIST_CELL_TO_ELEM_
#undef _LIST_ELEM_PRINT_FUNC_
#undef _LIST_USE_HANDLE_
#undef _LIST_ERROR_CHECKS_
#undef LIST_FUNC
#undef P_LIST_FUNC
