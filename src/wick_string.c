#include "wick_string.h"
#include "wick_place.h"
#include "wick_alloc.h"
#include <stdbool.h>

wick_string * wick_string_init(wick_place * env, wick_string * self) {
  self->start = self->past_end = 0;
  self->owns_string = false;
  return self;
}

size_t wick_string_size(wick_string buffer) {
  return buffer.past_end - buffer.start;
}

void wick_string_print(wick_string str) {
  char * character = str.start;
  while(character != str.past_end) {
    printf("%c", *character);
    character++;
  }
}

void wick_string_println(wick_string str) {
  wick_string_print(str);
  printf("\n");
}

void wick_string_step(wick_string * buffer1, const wick_string * buffer2) {
  buffer1->start = buffer1->past_end;
  buffer1->past_end = buffer2->past_end;
}

long int get_file_size(FILE * file) {
  fpos_t start_position;
  long int file_size;
  fgetpos(file, &start_position);
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  fsetpos(file, &start_position);
  return file_size;
}

wick_string wick_string_from_filename(wick_place * env, char * filename) {
  FILE * file;
  wick_string buffer;
  long int file_size;
  size_t buffer_size;
  size_t bytes_read;
  wick_string_init(env, &buffer);
  file = fopen(filename, "r");
  if(file == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    return buffer;
  }

  file_size = get_file_size(file);
  buffer_size = sizeof(char) * (file_size + 1);
  buffer.start = wick_host_alloc(buffer_size);

  if(!buffer.start) {
    fprintf(stderr, "Error: not enough memory.\n");
    fclose(file);
    return buffer;
  }

  buffer.start[buffer_size - 1] = '\0';
  buffer.past_end = &buffer.start[buffer_size - 1];

  bytes_read = fread(buffer.start, sizeof(char), file_size, file);
  if(bytes_read != file_size) {
    fprintf(stderr,
            "Error reading file: %s\n"
            "Could only read %lu out of %ld chars.\n",
            filename, bytes_read, file_size);
    fclose(file);
    return buffer;
  }
  return buffer;
}

size_t wick_string_len(const wick_string buffer) {
  size_t count = 0;
  char * cur = buffer.start;
  char * ascii_start = cur;

  while (cur < buffer.past_end && *cur <= 127)
  ascii: ++cur;

  count += cur - ascii_start;
  while (cur < buffer.past_end) {
    if (*cur <= 127) {
      ascii_start = cur;
      goto ascii;
    } else {
      switch (0xF0 & *cur) {
        case 0xE0: cur += 3; break;
        case 0xF0: cur += 4; break;
        default:   cur += 2; break;
      }
    }
    ++count;
  }
  return count;
}
