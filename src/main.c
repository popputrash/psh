#include "exec.h"
#include "parser.h"
#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define TOKDELIM " \t\r\n\a"


void psh_loop(void) {
  char *line;
  char **args;
  int status;
  char cwd[1024];

  do {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("%s> ", cwd);
    } else {
      printf("> ");
    }

    line = read_line();
    args = parse_args(line);
    status = psh_execute(args);

		free(line);
		free(args);

  } while (status);
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  psh_loop();

  exit(EXIT_SUCCESS);
}
