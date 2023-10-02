#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 80
#define SCANF_SAFE_BUFFER "%79s"
#define ESCAPE_BUILTIN 10
#define STRCMP_FOUND_STR 0
#define CURRENT_COMMAND argv[0]
#define EXIT_VALID_BUILTIN 11
#define TRUE 1
#define FALSE 0

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

  setenv("MOLSH_PROMPT", "(molsh) > ", FALSE);
  for (;;) {
    printf("%s", getenv("MOLSH_PROMPT"));

    fgets(inputbuf, sizeof(inputbuf), stdin);
    inputbuf[strcspn(inputbuf, "\n")] = 0;

    char **argv = parse_argv(inputbuf);
    if (argv[0] == NULL) {
      free(argv);
      continue;
    }

    struct Command curr_cmd;

    curr_cmd.text = inputbuf;
    curr_cmd.argv = argv;
    curr_cmd.operation = tokenize_builtin_cli(inputbuf);
    int exit_code = run_cmd(&curr_cmd);

    if (exit_code == ESCAPE_BUILTIN) {
      return EXIT_SUCCESS;
    }
    free(argv);
  };

  return EXIT_SUCCESS;
};
