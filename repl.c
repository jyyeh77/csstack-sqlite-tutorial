#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct InputBuffer_t {
    // buffer is a pointer to data type char
    char* buffer;
    // size_t is unsigned type, can't repr negative values, used for lengths
    size_t buffer_length;
    // ssize_t is signed type, can represent -1
    ssize_t input_length;
};
// alias type InputBuffer_t as InputBuffer
typedef struct InputBuffer_t InputBuffer;

InputBuffer* new_input_buffer() {
    // malloc returns pointer to allocate memory for input buffer
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL; // from stdlib
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() {
    printf("db > ");
}

// lineptr -> pointer to variable that points to buffer w/read line
// equivalent to char *lineptr[]
// n -> pointer to variable that saves size of allocated buffer
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }
    }
}