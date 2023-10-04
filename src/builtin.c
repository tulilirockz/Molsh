#include "builtin.h"
#include "builtin-func.h"
#include <string.h>

#define STRCMP_FOUND_STR 0

struct BuiltinElement valid_builtins[] = {
    {"echo", OP_ECHO, (fptr)builtin_echo},
    {"fullenv", OP_FULLENV, (fptr)builtin_fullenv},
    {"getenv", OP_GETENV, (fptr)builtin_getenv},
    {"clear", OP_CLEAR, (fptr)builtin_clear},
    {"cd", OP_CD, (fptr)builtin_cd},
    {"exit", OP_EXIT, (fptr)builtin_exit},
    {"setenv", OP_SETENV, (fptr)builtin_setenv},
    {"extern", OP_EXTERN, (fptr)builtin_extern}};

int run_cmd(struct Command *pcmd, fptr op) { return (op(pcmd)); };

enum ShellToken tokenize_builtin_cli(char *text) {
  for (int i = 0; i <= sizeof(valid_builtins) / sizeof(valid_builtins[0]);
       i++) {
    if (strcmp(valid_builtins[i].commandtext, text) == STRCMP_FOUND_STR) {
      return valid_builtins[i].operation;
    }
  }
  return OP_EXTERN;
};

fptr token_to_func(enum ShellToken operation) {
  for (int i = 0; i <= sizeof(valid_builtins) / sizeof(valid_builtins[0]);
       i++) {
    if (valid_builtins[i].operation == operation) {
      return valid_builtins[i].func;
    }
  }
  return (fptr)builtin_extern;
}
