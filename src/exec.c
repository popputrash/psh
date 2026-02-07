#include "exec.h"
#include "builtins.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int psh_execute(char **args) {
  pid_t pid;
  int status;

  if (args[0] == NULL) {
    return 1;
  }

  for (int i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_names[i]) == 0) {
      return (*builtin_funcs[i])(args);
    }
  }

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("psh");
			_exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    perror("psh");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);

    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
	return 1;
}
