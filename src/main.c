#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 80
#define SCANF_SAFE_BUFFER "%79s"
#define ESCAPE_BUILTIN 10
#define STRCMP_FOUND_STR 0

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

int builtin_comparisons(char **argv, int argc) {
  if (strcmp(argv[0], "echo") == STRCMP_FOUND_STR) {
    for (int i = 1; argv[i] != NULL; i++) {
      printf("%s ", argv[i]);
    }
    printf("\n");
    return EXIT_SUCCESS;
  }
  if (strcmp(argv[0], "exit") == STRCMP_FOUND_STR) {
    return ESCAPE_BUILTIN;
  }
  return EXIT_SUCCESS;
};

int main(void) {
  char inputbuf[DEFAULT_BUFFER_SIZE];

  for (;;) {
    printf("\r( molsh ) > ");

    fgets(inputbuf, sizeof(inputbuf), stdin);
    inputbuf[strcspn(inputbuf, "\n")] = 0;

    char **argv = parse_argv(inputbuf);
    int argc = sizeof(argv) / sizeof(argv[0]);
    int exit_code = builtin_comparisons(argv, argc);
    free(argv);
    if (exit_code == ESCAPE_BUILTIN) {
      return EXIT_SUCCESS;
    };
  };

  return EXIT_SUCCESS;
};
