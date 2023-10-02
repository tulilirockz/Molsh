#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ESCAPE_BUILTIN 10
#define STRCMP_FOUND_STR 0
#define EXIT_VALID_BUILTIN 11

extern char **environ;

int builtin_clear(struct Command *cmd) {
  fprintf(stdout, "\033[2J");   // clean screen
  fprintf(stdout, "\033[1;1H"); // move cursor to the first line
  return EXIT_SUCCESS;
}

int builtin_echo(struct Command *cmd) {
  for (int i = 1; cmd->argv[i] != NULL; i++) {
    printf("%s ", cmd->argv[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}

int builtin_fullenv(struct Command *cmd) {
  char **s = environ;

  for (; *s; s++) {
    printf("%s\n", *s);
  }
  return EXIT_SUCCESS;
}

int builtin_getenv(struct Command *cmd) {
  for (int i = 1; cmd->argv[i] != NULL; i++) {
    char *output = getenv(cmd->argv[i]);
    if (output == NULL) {
      printf("\n");
      return EXIT_SUCCESS;
    }

    printf("%s\n", output);
  }

  return EXIT_SUCCESS;
}

int builtin_setenv(struct Command *cmd) {
  setenv(cmd->argv[1], cmd->argv[2], 1);
  return EXIT_SUCCESS;
};

int builtin_exit(struct Command *cmd) { return ESCAPE_BUILTIN; }

int builtin_extern(struct Command *cmd) {
  pid_t native_child = fork();
  if (native_child == 0) {
    execv(cmd->text, cmd->argv);
    printf("\n");
    exit(0);
  }
  wait(&native_child);
  return EXIT_SUCCESS;
};

int run_cmd(struct Command *cmd) {
  switch (cmd->operation) {
  case OP_SETENV:
    return builtin_setenv(cmd);
  case OP_EXIT:
    return builtin_exit(cmd);
  case OP_FULLENV:
    return builtin_fullenv(cmd);
  case OP_GETENV:
    return builtin_getenv(cmd);
  case OP_ECHO:
    return builtin_echo(cmd);
  case OP_CLEAR:
    return builtin_clear(cmd);
  case OP_EXTERN:
    return builtin_extern(cmd);
  }
};

enum ShellToken tokenize_builtin_cli(char *text) {
  if (strcmp(text, "echo") == STRCMP_FOUND_STR) {
    return OP_ECHO;
  }

  if (strcmp(text, "fullenv") == STRCMP_FOUND_STR) {
    return OP_FULLENV;
  }

  if (strcmp(text, "getenv") == STRCMP_FOUND_STR) {
    return OP_GETENV;
  }

  if (strcmp(text, "setenv") == STRCMP_FOUND_STR) {
    return OP_SETENV;
  }

  if (strcmp(text, "clear") == STRCMP_FOUND_STR) {
    return OP_CLEAR;
  }

  if (strcmp(text, "exit") == STRCMP_FOUND_STR) {
    return OP_EXIT;
  }

  return OP_EXTERN;
};
