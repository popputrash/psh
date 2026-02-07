#include "exec.h"
#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

int psh_execute_pipeline(pipeline_t pipeline) {
  if (pipeline.num_commands == 0) {
    return 1;
  }

  if (pipeline.num_commands == 1) {
    return psh_execute(pipeline.commands[0]);
  }

  int num_pipes = pipeline.num_commands - 1;
  int (*pipes)[2] = malloc(num_pipes * sizeof(int[2]));
  pid_t *pids = malloc(pipeline.num_commands * sizeof(pid_t));

  if (!pipes || !pids) {
    fprintf(stderr, "psh: allocation err");
    free(pipes);
    free(pids);
    return 1;
  }

  for (int i = 0; i < num_pipes; i++) {
    if (pipe(pipes[i]) == -1) {
      perror("psh: pipe");
      free(pipes);
      free(pids);
      return 1;
    }
  }

  for (int i = 0; i < pipeline.num_commands; i++) {
    pids[i] = fork();
    if (pids[i] == 0) {
      /* --- Child process --- */
      /* If not the first command, read stdin from previous pipe */
      if (i > 0) {
        dup2(pipes[i - 1][0], STDIN_FILENO);
      }
      /* If not the last command, write stdout to next pipe */
      if (i < num_pipes) {
        dup2(pipes[i][1], STDOUT_FILENO);
      }
      /*
       * Close ALL pipe fds in the child.
       *
       * After dup2, the relevant pipe ends are duplicated onto
       * stdin/stdout. The original fds are no longer needed.
       * Any pipe fds left open would prevent EOF from being
       * seen by reading processes.
       */
      for (int j = 0; j < num_pipes; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
      }
      /* Replace this process with the command */
      if (execvp(pipeline.commands[i][0], pipeline.commands[i]) == -1) {
        perror("psh");
      }
      _exit(EXIT_FAILURE);
    } else if (pids[i] < 0) {
      perror("psh: fork");
    }
  }

  for (int i = 0; i < num_pipes; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  for (int i = 0; i < pipeline.num_commands; i++) {
    if (pids[i] > 0) {
      int status;
      waitpid(pids[i], &status, 0);
    }
  }
	free(pipes);
	free(pids);
	return 1;
}
