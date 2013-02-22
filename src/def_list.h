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

#ifndef DEF_LIST_H
#define DEF_LIST_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef int32_t error_number;

#define WICK_SUCCESS 1

#define WICK_FAILURE 2

#define WICK_MEMORY_ERROR 3

#define WICK_ALLOC_ERROR 4

#define WICK_API_ERROR 5

#define WICK_STOP_ITER_ERROR 6

#define WICK_CONTAINER_FULL_ERROR 7

#define WICK_BAD_DATA 8

#define macro_join_inner(_token_a_, _token_b_) _token_a_ ## _token_b_
#define macro_join(_token_a_, _token_b_) macro_join_inner(_token_a_, _token_b_)

#define token_to_string(token) #token
#define macro_to_string(token) token_to_string(token)

#define ignore_return(_expr_) ((void)(_expr_))

#define WICK_THROW(_error_) \
	do { \
		error_number _ERROR_ = _error_; \
		if (_ERROR_ != WICK_SUCCESS) { \
			return _ERROR_; \
		} \
	} while(0)


#endif /* DEF_LIST_H */
