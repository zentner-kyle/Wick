#ifndef WICK_STRING_H
#define WICK_STRING_H
#include <stdio.h>
#include <stdbool.h>

struct wick_place;

struct wick_string {
  char * start;
  char * past_end;
  bool owns_string;
};

typedef struct wick_string wick_string;

wick_string * wick_string_init(struct wick_place * env, wick_string * self);

size_t wick_string_size(wick_string buffer);

void wick_string_print(wick_string str);

void wick_string_println(wick_string str);

void wick_string_step(wick_string * buffer1, const wick_string * buffer2);

long int get_file_size(FILE * file);

wick_string wick_string_from_filename(struct wick_place * env, char * filename);

size_t wick_string_len(const wick_string buffer);


#endif /* WICK_STRING_H */
