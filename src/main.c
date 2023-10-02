#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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

void clrscr(void) {
  fprintf(stdout, "\033[2J");   // clean screen
  fprintf(stdout, "\033[1;1H"); // move cursor to the first line
}

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
  if (strcmp(CURRENT_COMMAND, "echo") == STRCMP_FOUND_STR) {
    for (int i = 1; argv[i] != NULL; i++) {
      printf("%s ", argv[i]);
    }
    printf("\n");
    return EXIT_VALID_BUILTIN;
  }

  if (strcmp(CURRENT_COMMAND, "fullenv") == STRCMP_FOUND_STR) {
    char **s = environ;

    for (; *s; s++) {
      printf("%s\n", *s);
    }
    return EXIT_VALID_BUILTIN;
  }

  if (strcmp(CURRENT_COMMAND, "getenv") == STRCMP_FOUND_STR) {
    // getenv [VARIABLE...]
    for (int i = 1; argv[i] != NULL; i++) {
      char *output = getenv(argv[i]);
      if (output == NULL) {
        printf("\n");
        return EXIT_VALID_BUILTIN;
      }

      printf("%s\n", output);
    }

    return EXIT_VALID_BUILTIN;
  }

  if (strcmp(CURRENT_COMMAND, "setenv") == STRCMP_FOUND_STR) {
    // setenv [VARIABLE] [VALUE]
    setenv(argv[1], argv[2], TRUE);
    return EXIT_VALID_BUILTIN;
  }

  if (strcmp(CURRENT_COMMAND, "clear") == STRCMP_FOUND_STR) {
    clrscr();
    return EXIT_VALID_BUILTIN;
  }

  if (strcmp(CURRENT_COMMAND, "exit") == STRCMP_FOUND_STR) {
    return ESCAPE_BUILTIN;
  }
  return EXIT_SUCCESS;
};

int main(void) {
  char inputbuf[DEFAULT_BUFFER_SIZE];
  char commandbuf[DEFAULT_BUFFER_SIZE];
  memset(commandbuf, 0, sizeof(commandbuf));

  setenv("MOLSH_PROMPT", "(molsh)> ", FALSE);
  for (;;) {
    printf("%s", getenv("MOLSH_PROMPT"));

    fgets(inputbuf, sizeof(inputbuf), stdin);
    inputbuf[strcspn(inputbuf, "\n")] = 0;

    char **argv = parse_argv(inputbuf);
    if (argv[0] == NULL) {
      free(argv);
      continue;
    }
    int argc = sizeof(argv) / sizeof(argv[0]); // NOLINT
    int exit_code = builtin_comparisons(argv, argc);

    if (exit_code == ESCAPE_BUILTIN) {
      return EXIT_SUCCESS;
    }
    if (exit_code == EXIT_VALID_BUILTIN) {
      continue;
    }

    pid_t native_child = fork();
    if (native_child == 0) {
      execv(inputbuf, argv);
      printf("\n");
      exit(0);
    }
    wait(&native_child);
    free(argv);

    memset(commandbuf, 0, sizeof(commandbuf));
  };

  return EXIT_SUCCESS;
};
