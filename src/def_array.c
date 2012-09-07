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

#include "common.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"

#ifndef _ARRAY_PREFIX_
  #define _ARRAY_PREFIX_ _ARRAY_T_
#endif

#ifndef _ARRAY_POSTFIX_
  #define _ARRAY_POSTFIX_ _array
#endif

#define _ARRAY_LONG_PREFIX_ C_TOKEN_JOIN(_ARRAY_PREFIX_, _ARRAY_POSTFIX_)
/* Example: int_array */

#ifndef _ARRAY_HANDLE_T_
  #define _ARRAY_HANDLE_T_ _ARRAY_LONG_PREFIX_
  #define _ARRAY_DEFINE_HANDLE_
#else
  #define _ARRAY_INSTRUSIVE_
#endif

#ifndef _ARRAY_ALLOC_
  #define _ARRAY_ALLOC_(_bytecount_) malloc(_bytecount_)
#endif

#ifndef _ARRAY_FREE_
  #define _ARRAY_FREE_(_elem_) free(_elem_)
#endif

#ifndef _ARRAY_ERROR_FUNC_
  void C_TOKEN_JOIN(_ARRAY_LONG_PREFIX_, _error_func)(char *msg) {
    printf("Error in " C_MACRO2STRING(_ARRAY_LONG_PREFIX_) ": %s\n", msg);
  }
  #define _ARRAY_ERROR_FUNC_ C_TOKEN_JOIN(_ARRAY_LONG_PREFIX_, _error_func)
#endif

#ifndef _ARRAY_ITERATOR_T_
  #define _ARRAY_ITERATOR_T_ C_TOKEN_JOIN(_ARRAY_LONG_PREFIX_, _iterator)
#endif
/* Example: int_array_iterator */

#ifdef _ARRAY_ERROR_CHECKS_
  #define _ARRAY_MEM_CHECK_(_memory_check_expr_) WICK_MEM_CHECK(_memory_check_expr_)
#else
  #define _ARRAY_MEM_CHECK_(_memory_check_expr_) \
    do { \
      (void)(_memory_check_expr_); \
    } while (0)
#endif

#define ARRAY_FUNC(_func_postfix_) C_TOKEN_JOIN(_ARRAY_LONG_PREFIX_, _func_postfix_)
#define P_ARRAY_FUNC(_func_postfix_) C_TOKEN_JOIN(_, C_TOKEN_JOIN(_ARRAY_LONG_PREFIX_, _func_postfix_))

#ifndef _ARRAY_MEM_START_NAME_
  #define _ARRAY_MEM_START_NAME_ mem_start
#endif

#ifndef _ARRAY_START_NAME_
  #define _ARRAY_START_NAME_ start
#endif

#ifndef _ARRAY_PAST_END_NAME_
  #define _ARRAY_PAST_END_NAME_ past_end
#endif

#ifndef _ARRAY_MEM_PAST_END_NAME_
  #define _ARRAY_MEM_PAST_END_NAME_ mem_past_end
#endif

  /* Configurations:
     With handle:
              -------------------------------------     ------------
     user --> | ... start, past_end, mem_past_end | --> | (buffer) |
              -------------------------------------     ------------

     Without handle:
              ----------------------------------------------
     user --> | ... start, past_end, mem_past_end (buffer) |
              ----------------------------------------------

     Constant sized with handle:
              -----------------------     ------------
     user --> | ... start, past_end | --> | (buffer) |
              -----------------------     ------------

     Constant sized without handle:
              -------------------------
     user --> | ... past_end (buffer) |
              -------------------------
  */

/* Insertion mode: */

#define _ARRAY_INSERT_NORMAL_ 1
#define _ARRAY_INSERT_STACK_ 2
#define _ARRAY_INSERT_LOOPAROUND_ 3

#if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_STACK_ || \
    _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_LOOPAROUND_
  #define _ARRAY_USE_START_
  #define _ARRAY_TO_START_(_array_) ((_array_)->(_ARRAY_START_NAME_))
#else
  #define _ARRAY_TO_START_(_array_) (_ARRAY_TO_MEM_START_(_array_))
#endif

/* On full mode: */

#define _ARRAY_WHEN_FULL_RESIZE_ 1
#define _ARRAY_WHEN_FULL_EJECT_ 2
#define _ARRAY_WHEN_FULL_ERROR_ 3

#if _ARRAY_WHEN_FULL_ == _ARRAY_WHEN_FULL_RESIZE_
  #define _ARRAY_USE_MEM_PAST_END_
  #define _ARRAY_TO_MEM_PAST_END_(_array_) ((_array_)->(_ARRAY_PAST_END_NAME_))
  #define _ARRAY_SIZE_INTERNAL_(_array_) (_ARRAY_TO_MEM_PAST_END_(_array_) - _ARRAY_TO_START_(_array_))
#else
  #define _ARRAY_TO_MEM_PAST_END_(_array_) (_ARRAY_TO_MEM_START_(_array_) + _ARRAY_CONST_SIZE_)
  #define _ARRAY_SIZE_INTERNAL_(_array_) _ARRAY_CONST_SIZE_
  #if _ARRAY_WHEN_FULL_ == _ARRAY_WHEN_FULL_EJECT_
    #ifndef _ARRAY_EJECT_FUNC_
      #define _ARRAY_EJECT_FUNC_(_elem_)
    #endif
#endif

#ifndef _ARRAY_DEFAULT_SIZE_
  #define _ARRAY_DEFAULT_SIZE_ 8
#endif

/* Filling mode: */

#define _ARRAY_FILL_PTR_ 1
#define _ARRAY_FILL_INIT_VAL_ 2
#define _ARRAY_FILL_INIT_FUNC_ 3
#define _ARRAY_FILL_BLOCK_INIT_FUNC_ 4

#if _ARRAY_FILL_MODE_ == _ARRAY_FILL_PTR_
  #define _ARRAY_USE_PAST_END_
  #define _ARRAY_TO_PAST_END_(_array_) ((_array_)->(_ARRAY_PAST_END_NAME_))
#else
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_LOOPAROUND_
  #define _ARRAY_TO_PAST_END_(_array_) (_ARRAY_TO_START_(_array_))
      /* Re-use start pointer as fill pointer. */
  #else
  #define _ARRAY_TO_PAST_END_(_array_) (_ARRAY_TO_MEM_PAST_END_(_array_))
  #endif
#endif

#ifdef _ARRAY_INTERNAL_STORAGE_
  #ifndef _ARRAY_TO_MEM_START_ /* Part of external interface. */
    #define _ARRAY_TO_MEM_START_(array_handle_ptr) \
      ((_ARRAY_T_ *)((void *)(array_handle_ptr) + sizeof(_ARRAY_HANDLE_T_)))
  #endif
#else
  #define _ARRAY_USE_MEM_START_
  #define _ARRAY_TO_MEM_START_(array_handle_ptr) ((array_handle_ptr)->(_ARRAY_MEM_START_NAME_))
#endif

#if _ARRAY_WHEN_FULL_ == _ARRAY_WHEN_FULL_RESIZE_
  /* Growth policy: */
  #ifndef _ARRAY_GROWTH_FUNC_
    #define _ARRAY_GROWTH_FUNC_(_current_elems_) ((_current_elems_) << 1)
  /* TODO */

  /* Shrink policy: */
  /* TODO */
#endif

/* Move mode: */

#define _ARRAY_USE_MOVE_FUNC_ 1
#define _ARRAY_USE_BLOCK_MOVE_FUNC_ 2

#if defined(_ARRAY_MOVE_FUNC_) && defined(_ARRAY_BLOCK_MOVE_FUNC_)
  #error Both _ARRAY_MOVE_FUNC_ and _ARRAY_BLOCK_MOVE_FUNC_ defined.
#endif

#ifdef _ARRAY_MOVE_FUNC_
  #define _ARRAY_MOVE_MODE_ _ARRAY_USE_MOVE_FUNC_
#endif

#ifdef _ARRAY_BLOCK_MOVE_FUNC_
  #define _ARRAY_MOVE_MODE_ _ARRAY_USE_BLOCK_MOVE_FUNC_
#endif

#ifndef _ARRAY_MOVE_MODE_
  #define _ARRAY_MOVE_MODE_ _ARRAY_USE_BLOCK_MOVE_FUNC_
  #define _ARRAY_BLOCK_MOVE_FUNC_ memmove
#endif

#if _ARRAY_MODE_ == _ARRAY_USE_MOVE_FUNC_ && !defined(_ARRAY_MOVE_FUNC_)
  #define _ARRAY_MOVE_FUNC_(_target_, _source_) *(_target_) = *(_source_)
#endif

#if defined(_ARRAY_BLOCK_MOVE_FUNC_) && !defined(_ARRAY_MOVE_FUNC_)
  #define _ARRAY_MOVE_FUNC_(_target_, _source_) _ARRAY_BLOCK_MOVE_FUNC_(_target_, _source_, sizeof(_ARRAY_T_));
#endif

/* End move mode. */

#ifdef _ARRAY_DEFINE_HANDLE_
  struct _ARRAY_HANDLE_T_ {
    #ifdef _ARRAY_USE_MEM_START_
      _ARRAY_T_ * _ARRAY_MEM_START_NAME_;
    #endif
    #ifdef _ARRAY_USE_MEM_PAST_END_
      _ARRAY_T_ * _ARRAY_MEM_PAST_END_NAME_;
    #endif
    #ifdef _ARRAY_USE_START_
      _ARRAY_T_ * _ARRAY_START_NAME_;
    #endif
    #ifdef _ARRAY_USE_PAST_END_
      _ARRAY_T_ * _ARRAY_PAST_END_NAME_;
    #endif
  };

  typedef struct _ARRAY_HANDLE_T_ _ARRAY_HANDLE_T_;
#endif

struct _ARRAY_ITERATOR_T_ {
  _ARRAY_T_ * position;
  _ARRAY_HANDLE_T_ * array;
  /* Could use a flag for looparound to record looped / non looped parts. */
};


typedef struct _ARRAY_ITERATOR_T_ _ARRAY_ITERATOR_T_;


#define _ARRAY_NEXT_ELEM_(start_ptr) ((start_ptr) + 1)
#define _ARRAY_PREV_ELEM_(start_ptr) ((start_ptr) - 1)
#define _ARRAY_TO_END_(_array_) (_ARRAY_PREV_ELEM_(_ARRAY_TO_PAST_END_(_array_)))
#define _ARRAY_TO_MEM_END_(_array_) (_ARRAY_PREV_ELEM_(_ARRAY_TO_MEM_PAST_END_(_array_)))

void ARRAY_FUNC(_start)(_ARRAY_HANDLE_T_ * array, _ARRAY_ITERATOR_T_ *iter);
void ARRAY_FUNC(_end)(_ARRAY_HANDLE_T_ * array, _ARRAY_ITERATOR_T_ *iter);
wick_error ARRAY_FUNC(_push)(_ARRAY_HANDLE_T_ **array, _ARRAY_T_ *elem);
void ARRAY_FUNC(_pop)(_ARRAY_HANDLE_T_ **array, _ARRAY_T_ **elem);
wick_error ARRAY_FUNC(_insert_after)(_ARRAY_HANDLE_T_ **seq, _ARRAY_ITERATOR_T_ *iterator, _ARRAY_T_ *element);
wick_error ARRAY_FUNC(_insert_before)(_ARRAY_HANDLE_T_ **seq, _ARRAY_ITERATOR_T_ *iterator, _ARRAY_T_ *element);
void ARRAY_FUNC(_remove_next)(_ARRAY_HANDLE_T_ **seq, _ARRAY_ITERATOR_T_ *iterator);
void ARRAY_FUNC(_remove_prev)(_ARRAY_HANDLE_T_ **seq, _ARRAY_ITERATOR_T_ *iterator);
void ARRAY_FUNC(_remove)(_ARRAY_HANDLE_T_ **seq, _ARRAY_ITERATOR_T_ *iterator);
void ARRAY_FUNC(_free)(_ARRAY_CELL_T_ *to_free);
void ARRAY_FUNC(_map)(_ARRAY_HANDLE_T_ *array, _ARRAY_T_ *(*func)(_ARRAY_T_ *));
wick_error ARRAY_FUNC(_print)(FILE *stream, _ARRAY_HANDLE_T_ *array);
bool ARRAY_FUNC(_move)(_ARRAY_ITERATOR_T_ *iterator, int64_t steps);
bool ARRAY_FUNC(_next)(_ARRAY_ITERATOR_T_ *iterator);
bool ARRAY_FUNC(_prev)(_ARRAY_ITERATOR_T_ *iterator);
bool ARRAY_FUNC(_valid_iter)(_ARRAY_ITERATOR_T_ *iterator);
_ARRAY_T_ * ARRAY_FUNC(_safe_deref)(_ARRAY_ITERATOR_T_ *iterator);
_ARRAY_T_ * ARRAY_FUNC(_deref)(_ARRAY_ITERATOR_T_ *iterator);
wick_error ARRAY_FUNC(_copy)(_ARRAY_HANDLE_T_ * source, _ARRAY_HANDLE_T_ ** destination);
wick_error ARRAY_FUNC(_build)(size_t arg_count, _ARRAY_HANDLE_T_ **to_store, ...);
wick_error ARRAY_FUNC(_sort)(_ARRAY_HANDLE_T_ **array);
wick_error ARRAY_FUNC(_equal)(_ARRAY_HANDLE_T_ *a, _ARRAY_HANDLE_T_ *b, bool * result);
wick_error ARRAY_FUNC(_new)(_ARRAY_HANDLE_T_ ** to_store);

/* Begin definitions. */

bool P_ARRAY_FUNC(_filled)(_ARRAY_HANDLE_T_ * array, _ARRAY_T_ * ptr) {
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_NORMAL_ || \ 
      _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_STACK_
    return _ARRAY_TO_START_(array) <= ptr && ptr < _ARRAY_TO_PAST_END_(array);
  #endif
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_LOOPAROUND_  
      if(_ARRAY_TO_START_(array) <= _ARRAY_TO_PAST_END_(array)) {
        return _ARRAY_TO_START_(array) <= ptr && ptr < _ARRAY_TO_PAST_END_(array);
      } else {
        return (_ARRAY_TO_MEM_START_(array) <= ptr && ptr < _ARRAY_TO_PAST_END_(array))
                || (_ARRAY_TO_START_(array) <= ptr && ptr < _ARRAY_TO_MEM_PAST_END_(array));
          /* There may be more optimal forms of this expression when a fill ptr isn't used. */
      }
  #endif
}

bool P_ARRAY_FUNC(_unfilled)(_ARRAY_HANDLE_T_ * array, _ARRAY_T_ * ptr) {
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_NORMAL_
    return _ARRAY_TO_PAST_END_(array) <= ptr && ptr < _ARRAY_TO_MEM_PAST_END_(array);
  #endif
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_STACK_
    return _ARRAY_TO_MEM_START_(array) <= ptr && ptr < _ARRAY_TO_START_(array);
  #endif
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_LOOPAROUND_  
      if(_ARRAY_TO_START_(array) <= _ARRAY_TO_PAST_END_(array)) {
        return (_ARRAY_TO_MEM_START_(array) <= ptr && ptr < _ARRAY_TO_START_(array))
                || (_ARRAY_TO_PAST_END_(array) <= ptr && ptr < _ARRAY_TO_MEM_PAST_END_(array));
      } else {
        return _ARRAY_TO_PAST_END_(array) <= ptr && ptr < _ARRAY_TO_START_(array);
          /* There may be more optimal forms of this expression when a fill ptr isn't used. */
      }
  #endif
}

void ARRAY_FUNC(_start)(_ARRAY_HANDLE_T_ * array, _ARRAY_ITERATOR_T_ *iter) {
  iter->array = array;
  iter->position = _ARRAY_TO_START_(array);
}

void ARRAY_FUNC(_end)(_ARRAY_HANDLE_T_ * array, _ARRAY_ITERATOR_T_ *iter) {
  iter->array = array;
  iter->position = _ARRAY_PREV_ELEM_(_ARRAY_TO_PAST_END_(array));
}

void P_ARRAY_FUNC(_block_move)(_ARRAY_T_ * start, _ARRAY_T_ * past_end, _ARRAY_T_ * destination) {
  #ifdef _ARRAY_BLOCK_MOVE_FUNC_
    _ARRAY_BLOCK_MOVE_FUNC_(start, destination, ((void *)start) - ((void *)past_end));
  #else
    while (start != past_end) {
      _ARRAY_MOVE_FUNC_(destination, start);
      destination = _ARRAY_NEXT_ELEM_(destination);
      start = _ARRAY_NEXT_ELEM_(start);
    }
  #endif
}

void P_ARRAY_FUNC(_shift)(_ARRAY_T_ * start, _ARRAY_T_ * past_end, int64_t positions){
  #ifndef _ARRAY_BLOCK_MOVE_FUNC_
  if (positions > 0) {
    _ARRAY_T_ * current_source = _ARRAY_PREV_ELEM_(past_end);
    do {
      _ARRAY_ELEM_MOVE_(_ARRAY_NEXT_ELEM_(current_source), current_source);
      current_source = _ARRAY_NEXT_ELEM_(current_source);
    } while (current_source != start);
  } else if (positions < 0) {
    _ARRAY_T_ * current_source = start;
    while (current_source != past_end) {
      _ARRAY_ELEM_MOVE_(_ARRAY_PREV_ELEM_(current_source), current_source);
      current_source = _ARRAY_PREV_ELEM_(current_source);
    }
  }
  #else
    _ARRAY_BLOCK_MOVE_FUNC_(start, start + positions, ((void *)start) - ((void *)past_end));
  #endif
}

#define _ARRAY_ELEM_COUNT_TO_BYTES_(_elem_count) (sizeof(_ARRAY_T_) * (_elem_count))
#define _ARRAY_HANDLE_TO_INTERNAL_STORAGE_(_handle_) \
  ((_ARRAY_T_ *)(((void *) (_handle_)) + sizeof(_ARRAY_HANDLE_T_)))

wick_error P_ARRAY_FUNC(_grow_alloc)(_ARRAY_HANDLE_T_ ** new_handle, _ARRAY_T_ ** new_mem_region,
    size_t target_elem_count) {

  #ifdef _ARRAY_INTERNAL_STORAGE_
    size_t bytes_to_alloc = sizeof(_ARRAY_HANDLE_T_) + _ARRAY_ELEM_COUNT_TO_BYTES(target_elem_count);
    void * mem_block = _ARRAY_ALLOC_(bytes_to_alloc);
    if (mem_block == NULL) {
      return WICK_ALLOC_ERROR;
    }
    *new_handle = (_ARRAY_HANDLE_T_ *) mem_block;
    *new_mem_region = _ARRAY_HANDLE_TO_INTERNAL_STORAGE_(*new_handle);
  #else
    size_t bytes_to_alloc = _ARRAY_ELEM_COUNT_TO_BYTES(target_elem_count);
    void * mem_block = _ARRAY_ALLOC_(bytes_to_alloc);
    if (mem_block == NULL) {
      return WICK_ALLOC_ERROR;
    }
    *new_mem_region = (_ARRAY_T_ *) mem_block;
  #endif
    return WICK_SUCCESS;
}

void P_ARRAY_FUNC(_shift_open_front)(_ARRAY_HANDLE_T_ * array) {
  P_ARRAY_FUNC(_shift)(_ARRAY_TO_START_(*array), _ARRAY_TO_PAST_END_(*array), 1);
  #ifdef _ARRAY_USE_PAST_END_
    _ARRAY_TO_PAST_END_(*array) = _ARRAY_NEXT_ELEM_(_ARRAY_TO_PAST_END_(*array));
  #endif
}

wick_error ARRAY_FUNC(_push)(_ARRAY_HANDLE_T_ **array, _ARRAY_T_ *elem) {
  #if _ARRAY_INSERT_MODE_ == _ARRAY_INSERT_NORMAL_
    if(ARRAY_FUNC(_unfilled)(*array, _ARRAY_TO_PAST_END_(*array))) {
      P_ARRAY_FUNC(_shift_open_front)(*array);
      _ARRAY_MOVE_FUNC_(_ARRAY_TO_START_(*array), elem);
    } else {
    #if _ARRAY_WHEN_FULL_ == _ARRAY_WHEN_FULL_RESIZE_
      size_t new_size = _ARRAY_GROWTH_FUNC_(_ARRAY_TO_PAST_END_(*array) - _ARRAY_TO_START_(*array));
    #elif _ARRAY_WHEN_FULL_ == _ARRAY_WHEN_FULL_EJECT_
      _ARRAY_EJECT_FUNC_(_ARRAY_TO_END_(array));
      P_ARRAY_FUNC(_shift_open_front)(*array);
      _ARRAY_MOVE_FUNC_(_ARRAY_TO_START_(*array), elem);
    #elif _ARRAY_WHEN_FULL_ == _ARRAY_WHEN_FULL_ERROR_
      return WICK_CONTAINER_FULL_ERROR;               
    #endif                                            
    }
  #endif
    /* if(ARRAY_FUNC(_unfilled)(*array, _ARRAY_PREV_ELEM_(_ARRAY_TO_START_(*array)))) { */
    /*   #if _ARRAY_MODE_ == _ARRAY_MOVE_FUNC_ */
    /*                           _ARRAY_MOVE_FUNC_(_ARRAY_PREV_ELEM_) */
    /* } */
}

wick_error P_ARRAY_FUNC(_parallel_alloc)(_ARRAY_HANDLE_T_ ** handle) {
  #ifdef _ARRAY_INTERNAL_STORAGE_
    size_t bytes_to_alloc = (sizeof(_ARRAY_T_) * _ARRAY_DEFAULT_SIZE_) + sizeof(_ARRAY_HANDLE_T_);
    void * mem_block = _ARRAY_ALLOC_(bytes_to_alloc);
    if (mem_block == NULL) {
      return WICK_ALLOC_ERROR;
    }
    _ARRAY_HANDLE_T_ * allocated_handle = (_ARRAY_HANDLE_T_ *) mem_block;
    _ARRAY_T_ * allocated_array = (_ARRAY_T_ *) _ARRAY_TO_MEM_START_(_ARRAY_HANDLE_T_);
    _ARRAY_TO_MEM_START_(allocated_handle) = allocated_array;
    *handle = allocated_handle;
    return WICK_SUCCESS;
  #else
    size_t bytes_to_alloc = (sizeof(_ARRAY_T_) * _ARRAY_DEFAULT_SIZE_);
    void * mem_block = _ARRAY_ALLOC_(bytes_to_alloc);
    if (mem_block == NULL) {
      return WICK_ALLOC_ERROR;
    }
    _ARRAY_T_ * alloced_array = (_ARRAY_T_ *) mem_block;
    
  #endif
}

wick_error ARRAY_FUNC(_new)(_ARRAY_HANDLE_T_ ** to_store) {
  #ifdef _ARRAY_INTERNAL_STORAGE_
    size_t bytes_to_alloc = (sizeof(_ARRAY_T_) * _ARRAY_DEFAULT_SIZE_) + sizeof(_ARRAY_HANDLE_T_);
  #else
    size_t bytes_to_alloc = (sizeof(_ARRAY_T_) * _ARRAY_DEFAULT_SIZE_);
  #endif
  _ARRAY_T_ * alloced_block = (_ARRAY_T_ *)_ARRAY_ALLOC_(bytes_to_alloc);
  if (alloced_block) {
    _ARRAY_FREE_(*to_store);
    *to_store = alloced_block;
    return WICK_SUCCESS;
  } else {
    return WICK_ALLOC_ERROR;
  }
}

