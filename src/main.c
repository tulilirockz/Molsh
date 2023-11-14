#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define DEFAULT_BUFFER_SIZE 80
#define SETENV_REPLACE 1
#define SETENV_DO_NOT_REPLACE 0

extern char **environ;

int main(void) {
  char inputbuf[DEFAULT_BUFFER_SIZE];
  const char *MOLSH_DEFAULT_PROMPT = "\033[36m(molsh)\033[0m > ";
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

    struct Command curr_cmd;
    curr_cmd.raw_text = inputbuf;
    curr_cmd.argv = argv;
    curr_cmd.p_cmddef = text_to_builtin(0, &argv);

    for (int i = 0; argv[i] != NULL; i++) {
      if (argv[i][0] == ';') {
        struct Command newcmd;
        newcmd.p_cmddef = text_to_builtin(i, &argv);
        curr_cmd.p_next = &newcmd;
      }
    }

    int exit_code = run_cmd(&curr_cmd, curr_cmd.p_cmddef->func);
    char exit = exit_code + '0';

    setenv("?", &exit, SETENV_REPLACE);
    if (exit_code == ESCAPE_BUILTIN) {
      free(argv);
      return EXIT_SUCCESS;
    }
    if (exit_code != EXIT_SUCCESS) {
      setenv("MOLSH_PROMPT", "\033[31m(molsh)\033[0m > ", SETENV_REPLACE);
    }

    free(argv);
  };

  return EXIT_SUCCESS;
};
