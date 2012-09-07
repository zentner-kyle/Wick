#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct utf8_string {
  uint8_t * start;
  uint8_t * past_end; /*Points to null character.*/
};
typedef struct utf8_string utf8_string; 

size_t utf8_string_size (utf8_string buffer) {
  return buffer.past_end - buffer.start;
}

void utf8_string_print(utf8_string str) {
  uint8_t * character = str.start;
  while(character != str.past_end) {
    printf("%c", *character);
    character++;
  }
}

void utf8_string_println(utf8_string str) {
  utf8_string_print(str);
  printf("\n");
}

void utf8_step(utf8_string * buffer1, const utf8_string * buffer2) {
  buffer1->start = buffer1->past_end;
  buffer1->past_end = buffer2->past_end;
}

utf8_string utf8_string_from_char_array(char * char_array) {
  utf8_string buffer = 
    {(uint8_t *)char_array, (uint8_t *)char_array + strlen(char_array)};
  return buffer;
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

utf8_string utf8_string_from_filename(char * filename) {
  FILE * file;
  utf8_string buffer = {NULL, NULL};
  long int file_size;
  size_t buffer_size;
  size_t bytes_read;
  file = fopen(filename, "r");
  if(file == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    return buffer;
  }

  file_size = get_file_size(file);
  buffer_size = sizeof(uint8_t) * (file_size + 1);
  buffer.start = malloc(buffer_size);

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

size_t utf8_string_len(const utf8_string buffer) {
  size_t count = 0;
  uint8_t * cur = buffer.start;
  uint8_t * ascii_start = cur;

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
