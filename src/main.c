#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 80
#define SCANF_SAFE_BUFFER "%79s"
#define SETENV_REPLACE 1
#define SETENV_DO_NOT_REPLACE 0

extern char **environ;

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

int main(void) {
  char inputbuf[DEFAULT_BUFFER_SIZE];
  const char *MOLSH_DEFAULT_PROMPT = "(molsh) > ";

  setenv("MOLSH_PROMPT", MOLSH_DEFAULT_PROMPT, SETENV_DO_NOT_REPLACE);
  for (;;) {
    printf("%s", getenv("MOLSH_PROMPT"));
    setenv("MOLSH_PROMPT", MOLSH_DEFAULT_PROMPT, SETENV_REPLACE);

    fgets(inputbuf, DEFAULT_BUFFER_SIZE, stdin);
    inputbuf[strcspn(inputbuf, "\n")] = 0;
    char **argv = parse_argv(inputbuf);
    if (argv[0] == NULL) {
      free(argv);
      continue;
    }

    for (int i = 0; argv[i] != NULL; i++) {
      if (argv[i][0] == '$') {
        argv[i] = getenv(argv[i] + 1);
      }
    }

    struct BuiltinElement element = text_to_builtin(inputbuf);
    struct Command curr_cmd;

    curr_cmd.text = inputbuf;
    curr_cmd.argv = argv;
    curr_cmd.operation = element.operation;
    int exit_code = run_cmd(&curr_cmd, element.func);
    char exit = exit_code + '0';

    setenv("?", &exit, SETENV_REPLACE);
    if (exit_code == ESCAPE_BUILTIN) {
      return EXIT_SUCCESS;
    }
    if (exit_code != EXIT_SUCCESS) {
      setenv("MOLSH_PROMPT", "\033[31m(!)\033[0m (molsh) > ", SETENV_REPLACE);
    }

    free(argv);
  };

  return EXIT_SUCCESS;
};
