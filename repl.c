#include <stddef.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// sqlite REPL
struct InputBuffer_t {
  char* buffer;
  size_t buffer_length; // unsigned type
  ssize_t input_length; // signed type
};
typedef struct InputBuffer_t InputBuffer;

enum MetaCommandResult_t {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
};
typedef enum MetaCommandResult_t 

InputBuffer* new_input_buffer() {
  // allocate memory for new input buffer
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() {
  printf("db > ");
}

// **lineptr is a pointer to a pointer to value of type char
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("error reading input\n");
    exit(EXIT_FAILURE);
  }

  // ignore trailing new line
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

int main(int argc, char* argv[]) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    // get buffer pointer from struct that input_buffer points to
    // if input buffer is '.exit', exit success
    /*if (strcmp(input_buffer->buffer, ".exit") == 0) {*/
      /*exit(EXIT_SUCCESS);*/
    /*} else {*/
      /*printf("Unrecognized command '%s'.\n", input_buffer->buffer);*/
    /*}*/

    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer[0])
          continue;
      }
    }
  }
}
