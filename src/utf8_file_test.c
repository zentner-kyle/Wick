#include "common.h"
#include "utf8_string.c"
#include <inttypes.h>

int main(int argv, char * argc[]) {
  if(argv != 2) {
      fprintf(stderr,
              "Incorrect usage:\n"
              "program should be called\n"
              "$ %s file_name\n", argc[0]);
      return 1;
  }
  utf8_string buffer = utf8_string_from_filename(argc[1]);
  size_t bytes_read = utf8_string_size(buffer);
  size_t bytes_written = fwrite(buffer.start, sizeof(char), bytes_read,
      stdout);
  printf("%lu characters\n", utf8_string_len(buffer));
  printf("%lu bytes\n", bytes_written);
  if(bytes_written != bytes_read) {
    fprintf(stderr,
            "Error writing to stdout:\n"
            "Could only write %lu of %lu bytes\n",
            bytes_written, bytes_read);
    return 1;
  }
  return 0;
}
