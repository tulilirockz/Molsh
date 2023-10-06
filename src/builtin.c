#include "builtin.h"
#include "builtin-func.h"
#include <stdio.h>
#include <string.h>

#define STRCMP_FOUND_STR 0

struct BuiltinElement valid_builtins[] = {
    {"extern", OP_EXTERN, (fptr)builtin_extern},
    {"echo", OP_ECHO, (fptr)builtin_echo},
    {"fullenv", OP_FULLENV, (fptr)builtin_fullenv},
    {"getenv", OP_GETENV, (fptr)builtin_getenv},
    {"clear", OP_CLEAR, (fptr)builtin_clear},
    {"cd", OP_CD, (fptr)builtin_cd},
    {"exit", OP_EXIT, (fptr)builtin_exit},
    {"setenv", OP_SETENV, (fptr)builtin_setenv}};

int run_cmd(struct Command *pcmd, fptr op) { return (op(pcmd)); };

struct BuiltinElement text_to_builtin(char *text) {
  struct BuiltinElement *ptr = valid_builtins;
  struct BuiltinElement *endPtr =
      valid_builtins + sizeof(valid_builtins) / sizeof(valid_builtins[0]);

  while (ptr < endPtr) {
    if (strcmp(ptr->commandtext, text) == STRCMP_FOUND_STR) {
      return *ptr;
    }
    ptr++;
  }
  return valid_builtins[0]; // To be honest, this is a bit wonky, but we like to
                            // live dangerously
};
