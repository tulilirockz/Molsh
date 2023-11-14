#include "builtin.h"
#include "builtin-func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRCMP_FOUND_STR 0

struct CommandDefinition valid_builtins[] = {
    {"extern", OP_EXTERN, (fptr)builtin_extern},
    {"echo", OP_ECHO, (fptr)builtin_echo},
    {"fullenv", OP_FULLENV, (fptr)builtin_fullenv},
    {"getenv", OP_GETENV, (fptr)builtin_getenv},
    {"clear", OP_CLEAR, (fptr)builtin_clear},
    {"cd", OP_CD, (fptr)builtin_cd},
    {"exit", OP_EXIT, (fptr)builtin_exit},
    {"setenv", OP_SETENV, (fptr)builtin_setenv}};

int run_cmd(struct Command *pcmd, fptr op) { return (op(pcmd)); };

struct CommandDefinition *text_to_builtin(int init, char ***argv) {
  struct CommandDefinition *ptr = valid_builtins;
  struct CommandDefinition *endPtr =
      valid_builtins + sizeof(valid_builtins) / sizeof(valid_builtins[0]);

  while (ptr < endPtr) {
    if (strcmp(ptr->commandtext, *argv[init]) == STRCMP_FOUND_STR) {
      return ptr;
    }
    ptr++;
  }
  return &valid_builtins[0]; // To be honest, this is a bit wonky, but we like
                             // to live dangerously
};

char **parse_argv(char *input_buffer) {
  const char delim[] = " ";
  int arg_capacity = 20;
  char *token;
  char **argv = malloc(arg_capacity * sizeof(char *));

  token = strtok(input_buffer, delim);

  int i = 0;
  while (token != NULL) {
    if (i > arg_capacity) {
      arg_capacity *= 2;
      argv = realloc(argv, arg_capacity * sizeof(char *));
    };
    argv[i++] = token;
    token = strtok(NULL, delim);
  }
  argv[i] = NULL;
  return argv;
};
