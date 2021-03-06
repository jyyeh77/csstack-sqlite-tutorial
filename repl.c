#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct InputBuffer_t {
    // buffer is a pointer to data type char, or an array of characters
    char* buffer;
    // size_t is unsigned type, can't repr negative values, used for lengths
    size_t buffer_length;
    // ssize_t is signed type, can represent -1
    ssize_t input_length;
};
// alias type InputBuffer_t as InputBuffer
typedef struct InputBuffer_t InputBuffer;

enum MetaCommandResult_t {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
};
typedef enum MetaCommandResult_t MetaCommandResult;

enum PrepareResult_t {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
};
typedef enum PrepareResult_t PrepareResult;

enum StatementType_t {
    STATEMENT_INSERT,
    STATEMENT_SELECT
};
typedef enum StatementType_t StatementType;

const uint32_t COLUMN_USERNAME_SIZE = 32;
const uint32_t COLUMN_EMAIL_SIZE = 255;
struct Row_t {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
};
typedef struct Row_t Row;

struct Statement_t {
    StatementType type;
    Row row_to_insert; // used by insert statement
};
typedef struct Statement_t Statement;

// define representation of row
// macro to get size of Row attribute -
// TODO: figure out if casting 0 to Struct* is good practice?, why macro?
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);

InputBuffer* new_input_buffer() {
    // malloc returns pointer to allocate memory for input buffer
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL; // from stdlib
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(
        InputBuffer* input_buffer,
        Statement* statement) {
    // if first 6 chars of input buffer are 'insert'
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        // parse arguments in insert
        int args_assigned = sscanf(
                input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
                statement->row_to_insert.username, statement->row_to_insert.email);
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    // input exactly equal to "select"
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            printf("This is where we would do an insert.\n");
            break;
        case (STATEMENT_SELECT):
            printf("This is where we would do a select.\n");
            break;
    }
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

        // meta commands prefixed with '.'
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer-> buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
}