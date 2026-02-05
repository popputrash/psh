#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void psh_loop();

char *read_line() {
  char *line = NULL;
  ssize_t bufsize = 0;

  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);
    } else {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }
  return line;
}

#define TOKDELIM " \t\r\n\a"
char **parse_args(char *line) {
  int bufsize = 64, position = 0;
  char **args = malloc(bufsize * sizeof(char *));
  char *token;

  if (!args) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(line, TOKDELIM);
  while (token != NULL) {
    args[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize = bufsize * 2;
      args = realloc(args, bufsize * sizeof(char *));

      if (!args) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, TOKDELIM);
  }
  args[position] = NULL;
  return args;
}

int psh_execute(char **args) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("psh");
    }
  } else if (pid < 0) {
    perror("psh");
  }else{
		do{
			waitpid(pid, &status, WUNTRACED);

		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

int main(int argc, char **argv) {

  psh_loop();

  exit(EXIT_SUCCESS);
}

void psh_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    line = read_line();
    args = parse_args(line);
    status = psh_execute(args);
  } while (status);

}
