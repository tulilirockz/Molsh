#pragma once

enum ShellOperation {
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

// raw_text: "ls"
// args: {ls -lah .}
// p_cmddef: OP_EXTERN
// p_callback: 0, ".git\tsrc\t.editorconfig\n"
typedef struct Command {
  char *raw_text;
  char **argv;
  struct CommandDefinition *p_cmddef;
  struct CommandCallback *p_callback;
  struct Command *p_next;
} Command;

typedef int (*fptr)(struct Command *cmd);

typedef struct CommandCallback {
  enum ExitCode exitcode;
  char *stdout;
} CommandCallback;

typedef struct CommandDefinition {
  char *commandtext;
  int argc;
  enum ShellOperation operation;
  fptr func;
} CommandDefinition;

struct CommandDefinition *text_to_builtin(int init, char ***argv);

int run_cmd(struct Command *pcmd, fptr op);

char **parse_argv(char *text);
