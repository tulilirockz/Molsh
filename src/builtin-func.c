#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int builtin_cd(struct Command *p_cmd) {
  chdir(p_cmd->argv[1]);
  return EXIT_SUCCESS;
}

int builtin_clear(struct Command *p_cmd) {
  fprintf(stdout, "\033[2J");   // clean screen
  fprintf(stdout, "\033[1;1H"); // move cursor to the first line
  return EXIT_SUCCESS;
}

int builtin_echo(struct Command *p_cmd) {
  for (int i = 1; p_cmd->argv[i] != NULL; i++) {
    printf("%s ", p_cmd->argv[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}

int builtin_fullenv(struct Command *p_cmd) {
  char **s = environ;

  for (; *s; s++) {
    printf("%s\n", *s);
  }
  return EXIT_SUCCESS;
}

int builtin_getenv(struct Command *p_cmd) {
  for (int i = 1; p_cmd->argv[i] != NULL; i++) {
    char *output = getenv(p_cmd->argv[i]);
    if (output == NULL) {
      printf("\n");
      return EXIT_SUCCESS;
    }
    printf("%s\n", output);
  }
  return EXIT_SUCCESS;
}

int builtin_setenv(struct Command *p_cmd) {
  return setenv(p_cmd->argv[1], p_cmd->argv[2], 1);
};

int builtin_exit(struct Command *p_cmd) { return ESCAPE_BUILTIN; }

int builtin_extern(struct Command *p_cmd) {
  pid_t native_child = fork();
  int exit_code = 0;

  if (native_child == -1) {
    perror("Failed to fork process");
    _Exit(1);
  }

  if (native_child == 0) {
    execvp(p_cmd->raw_text, p_cmd->argv);
    printf("\n");
    _Exit(0);
  }

  waitpid(native_child, &exit_code, 0);
  return exit_code;
};
