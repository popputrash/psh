#ifndef BUILTINS_H
#define BUILTINS_H

int psh_cd(char **args);
int psh_exit(char **args);
int num_builtins(void);

extern char *builtin_names[];
extern int (*builtin_funcs[])(char **);

#endif
