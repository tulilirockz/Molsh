#pragma once

enum ShellToken {
  OP_ECHO,
  OP_FULLENV,
  OP_GETENV,
  OP_SETENV,
  OP_CLEAR,
  OP_EXIT,
  OP_CD,
  OP_EXTERN
};
enum ExitCode {
  EXIT_SUCCESS,
  EXIT_FAILURE,
  ESCAPE_BUILTIN,
};

typedef struct Command {
  char *text;
  char **argv;
  enum ShellToken operation;
} Command;

typedef int (*fptr)(struct Command *cmd);

struct CommandCallback {
  enum ExitCode exitcode;
  char *stdout;
};

typedef struct BuiltinElement {
  char *commandtext;
  enum ShellToken operation;
  fptr func;
} BuiltinElement;

enum ShellToken tokenize_builtin_cli(char *text);

int run_cmd(struct Command *pcmd, fptr op);

fptr token_to_func(enum ShellToken operation);
