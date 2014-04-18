#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHELL_LINE_MAX 1024
#define NULL_FILE_ERROR "Attempting to read from a NULL file descriptor."

typedef struct {
    void **array;
    int size;
    int capacity;
} DynamicArray;

typedef enum {
    Pipe,
    OutputRedirection,
    InputRedirection,
    Background,
    Text
} TokenType;

typedef struct {
    TokenType type;
    char *text;
} Token;

typedef struct {
    Token *cmd;
    Token *output;
    Token *input;
    int pipe;
    int background;
    DynamicArray *args;
} Command;

/* Dynamic Array */
DynamicArray* new_darray(int size);
DynamicArray* darray_insert(DynamicArray *array, int index, void *item);
void delete_darray(DynamicArray *array);
void* darray_retrieve(DynamicArray *array, int index); 
DynamicArray* darray_pushback(DynamicArray *array, void *item);  
void darray_error(char *msg);


/* functions */
void internal_error(char *msg);
char* read_line(FILE *inputf);
char* chomp_and_free(char* str);
char* chomp(char* str);

/* Functionality */
DynamicArray *get_path();
void exec_command(DynamicArray *cmds);
DynamicArray *parse_command(DynamicArray *tokens);
DynamicArray *tokenize_command(char *command);
char* expand_tilde(char *path);

/* errors */
void darray_error(char *msg) {
    printf("ERROR: DynamicArray error\n");
    exit(1); return;
}

void internal_error(char *msg) {
    printf("%s", msg);
    exit(1);
}

void exec_error () {
    printf("ERROR: exec error\n");
}

void fork_error() {
    printf("ERROR: fork error\n");
}

Command* new_command(Token *c) {
    Command *new_c = (Command*)malloc(sizeof(Command));
    new_c->cmd = c;
    new_c->output = NULL;
    new_c->input = NULL;
    new_c->pipe = 0;
    new_c->background = 0;
    new_c->args = new_darray(10);
    return new_c;
}
