#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 80
#define SCANF_SAFE_BUFFER "%79s"

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

  setenv("MOLSH_PROMPT", "(molsh) > ", 0);
  for (;;) {
    printf("%s", getenv("MOLSH_PROMPT"));

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

    // setenv("?", (const char *)exit_code, 1);
    if (exit_code == ESCAPE_BUILTIN) {
      return EXIT_SUCCESS;
    }
    // if (exit_code != EXIT_SUCCESS) {
    //   setenv(const char *name, const char *value, int replace)
    // }

    free(argv);
  };

  return EXIT_SUCCESS;
};
