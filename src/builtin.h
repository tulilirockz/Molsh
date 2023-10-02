#pragma once

typedef int (*fptr)();
enum ShellToken {
  OP_ECHO,
  OP_FULLENV,
  OP_GETENV,
  OP_SETENV,
  OP_CLEAR,
  OP_EXIT,
  OP_EXTERN
};
struct Command {
  char *text;                // /bin/ls
  char **argv;               // /bin/ls -lah
  enum ShellToken operation; // OP_EXTERN
};

enum ShellToken tokenize_builtin_cli(char *text);
int run_cmd(struct Command *cmd);
