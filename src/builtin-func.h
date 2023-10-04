#pragma once
int builtin_cd(struct Command *pcmd);
int builtin_extern(struct Command *pcmd);
int builtin_clear(struct Command *pcmd);
int builtin_echo(struct Command *pcmd);
int builtin_fullenv(struct Command *pcmd);
int builtin_getenv(struct Command *pcmd);
int builtin_setenv(struct Command *pcmd);
int builtin_exit(struct Command *pcmd);
