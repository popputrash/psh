#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int psh_cd(char **args) {
  const char *dir;

  if (args[1] == NULL) {
    dir = getenv("HOME");
    if (dir == NULL) {
      fprintf(stderr, "psh: cd: HOME not set");
      return 1;
    }
  } else {
    dir = args[1];
  }

  if (chdir(dir) != 0) {
    perror("psh: cd");
  }
  return 1;
}

int psh_exit(char **args){
	(void)args;
	return 0;
}

char *builtin_names[] = {
  "cd",
  "exit"
};

int (*builtin_funcs[])(char **) = {&psh_cd, &psh_exit};

int num_builtins(void) { return sizeof(builtin_names) / sizeof(char *); }
