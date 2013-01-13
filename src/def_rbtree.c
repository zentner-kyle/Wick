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

#ifndef RBTREE_PREFIX
	#define RBTREE_PREFIX RBTREE_T
#endif

#ifndef RBTREE_POSTFIX
	#define RBTREE_POSTFIX _rbtree
#endif

#define RBTREE_LONG_PREFIX C_TOKEN_JOIN(RBTREE_PREFIX, RBTREE_POSTFIX)

#ifndef RBTREE_ALLOC
	#define RBTREE_ALLOC malloc
#endif

#ifndef RBTREE_FREE
	#define RBTREE_FREE free
#endif

#ifndef RBTREE_ELEM_MOVE
	#define RBTREE_ELEM_MOVE( target, source ) *(target) = *(source)
#endif

#ifndef RBTREE_ELEM_COPY
	#define RBTREE_ELEM_MOVE
#endif

#define RBTREE_FUNC(func_postfix) C_TOKEN_JOIN( C_TOKEN_JOIN( RBTREE_LONG_PREFIX, _ ), func_postfix )

#define RBTREE_FUNC_P(func_postfix) C_TOKEN_JOIN( _, RBTREE_FUNC( func_postfix ) )

#ifdef RBTREE_USE_CONTEXT
	#ifndef RBTREE_CONTEXT_ARG
		#error "Need context arg to for RBTREE: e.g. , wick_context context"
	#endif
	#ifndef RBTREE_CONTEXT_T
		#error ""
	#endif
	#ifndef RBTREE_CONTEXT_NAME
		#error ""
	#endif
#endif

#ifndef RBTREE_ITERATOR_T
	#define RBTREE_ITERATOR_T C_TOKEN_JOIN(RBTREE_LONG_PREFIX, _iterator)
#endif

#ifdef RBTREE_ERROR_CHECKS
	#ifdef RBTREE_RESPOND_TO_ERRORS
		#ifndef RBTREE_CONTEXT_MEM_ERROR
			#ifndef RBTREE_CONTEXT_ERROR
				#error "RBTREE asked to respond to errors but no RBTREE_CONTEXT_ERROR provided."
			#endif
			#define RBTREE_CONTEXT_MEM_ERROR RBTREE_CONTEXT_ERROR
		#endif
		#ifdef RBTREE_CONTEXT_MEM_ERROR
			#define RBTREE_MEM_CHECK( memory_check_expr ) \
				if ( ! (memory_check_expr) ) { \
					RBTREE_CONTEXT_MEM_ERROR( RBTREE_CONEXT_NAME ) \
				}
	#else
		#define RBTREE_MEM_CHECK( memory_check_expr ) assert( memory_check_expr )
	#endif
#else
	#define RBTREE_MEM_CHECK( memory_check_expr )
#endif

/* Field accessors have the type:
 * (RBTREE_CELL_T * -> ^a)
 * In other words, they dereference their argument, 
 * but don't dereference the field.
 */

#ifdef RBTREE_INTRUSIVE_T
	#ifndef RBTREE_INTRUSIVE_FIELD
		#error "Could not find where to place data in intrusive RBTREE."
	#endif
	#define RBTREE_CELL_T RBTREE_T
	#define RBTREE_ELEM( cell ) (cell)
	#define RBTREE_LEFT( cell ) ((*cell).(RBTREE_INTRUSIVE_FIELD).left)
	#define RBTREE_RIGHT( cell ) ((*cell).(RBTREE_INTRUSIVE_FIELD).right)
	#define RBTREE_COLOR( cell ) ((*cell).(RBTREE_INTRUSIVE_FIELD).color)
	#ifdef RBTREE_CACHE_KEYS
		#define RBTREE_KEY( cell ) ((*cell).(RBTREE_INTRUSIVE_FIELD).key)
	#endif
#else
	#define RBTREE_CELL_TO_ELEM( cell ) ((*cell).elem)
	#define RBTREE_LEFT( cell ) ((*cell).left)
	#define RBTREE_RIGHT( cell ) ((*cell).right)
	#define RBTREE_COLOR( cell ) ((*cell).color)
	#ifdef RBTREE_CACHE_KEYS
		#define RBTREE_KEYS( cell ) (RBTREE_KEY_FUNC( (cell).elem ))
	#endif
#endif

#ifdef RBTREE_INTRUSIVE
	#define RBTREE_CELL_T RBTREE_T
	#define RBTREE_ELEM( cell ) (cell)
#else
	#define RBTREE_CELL_TO_ELEM( cell ) ((cell)->RBTREE_ELEM_NAME)
#endif

#ifndef RBTREE_CELL_T
	#define RBTREE_CELL_T RBTREE_LONG_PREFIX
  	/*  The rbtree cell type needs to be defined if it hasn't been by the user,
  	 *  which would be indicated by them defining RBTREE_CELL_T or
		 *  by setting RBTREE_INTRUSIVE.
  	 */
		struct RBTREE_CELL_T {
			RBTREE_T ELEM;
			#ifdef RBTREE_CACHE_KEYS
				RBTREE_KEY_T key;
			#endif
			uint8_t color;
			struct RB_CELL_T *left;
			struct RB_CELL_T *right;
		};

		typedef struct RBTREE_CELL_T RBTREE_CELL_T;
#endif

#ifndef RBTREE_KEY_FROM_VAL
	#define RBTREE_KEY_FROM_VAL( val ) (val)
	#define RBTREE_KEY_T RBTREE_T
#endif

/*  Iterators are more general, high-level mechanisms.
 *  They should act 'the same' under all collections.
 *  As such, there is no interface for customizing their internals.
 */

struct RBTREE_ITERATOR_T {
	RBTREE_T last_elem;
};

typedef struct RBTREE_ITERATOR RBTREE_ITERATOR;

void RBTREE_FUNC_P( alloc )( RBTREE_CELL_T **out_to_store RBTREE_CONTEXT_ARG ) {
	/* Allocates and nulls the pointers in just one cell. */
	RBTREE_CELL_T * new_cell = RBTREE_ALLOC_CELL( sizeof(RBTREE_CELL_T) );
	if ( ! new_cell ) {
		RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_NAME );
		return;
	}
	RBTREE_COLOR(new_cell) = 0; /* TODO: decide how colors are encoded. */
	RBTREE_LEFT(new_cell) = 0;
	RBTREE_RIGHT(new_cell) = 0;
}

void RBTREE_FUNC( new )( RBTREE_CELL_T ** to_store RBTREE_CONTEXT_ARG ) {
	*to_store = 0;
}

void RBTREE_FUNC( free_cell )( RBTREE_CELL_T * to_free ) {
	if ( RBTREE_LEFT( to_free ) ) {
		RBTREE_FUNC( free_cell )( RBTREE_LEFT( to_free ) );
	}
	if ( RBTREE_RIGHT( to_free ) ) {
		RBTREE_FUNC( free_cell )( RBTREE_RIGHT( to_free ) );
	}
	#ifdef RBTREE_ELEM_FINALIZE
		RBTREE_ELEM_FINALIZE( RBTREE_ELEM( free_cell ) );
	#endif
	RBTREE_FREE( to_free );
}

RBTREE_CELL_T * RBTREE_FUNC_P( copy )( RBTREE_CELL_T * source RBTREE_CONTEXT_ARG ) {
	RBTREE_CELL_T * new_cell = RBTREE_ALLOC( sizeof(RBTREE_CELL_T) );
	if ( new_cell ) {
		RBTREE_COLOR( new_cell ) = RBTREE_COLOR( source );
		RBTREE_LEFT( new_cell ) = RBTREE_FUNC_P( copy_cell )( RBTREE_LEFT( source ) RBTREE_CTXT);
		if ( ! RBTREE_LEFT( new_cell ) ) {
			RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_ARG_NAME );
			return NULL;
		}
		RBTREE_RIGHT( new_cell ) = RBTREE_FUNC_P( copy_cell )( RBTREE_RIGHT( source ) RBTREE_CTXT);
		if ( ! RBTREE_RIGHT( new_cell ) ) {
			RBTREE_FUNC( free_cell )( RBTREE_LEFT( new_cell ) );
			RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_ARG_NAME );
			return NULL;
		}
		RBTREE_ELEM_COPY( RBTREE_ELEM( new_cell ), RBTREE_ELEM( source ) );
	}
	return new_cell;
}
