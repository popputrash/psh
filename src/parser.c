#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parse_args(char *line) {
  int bufsize = 64, position = 0;
  char **args = malloc(bufsize * sizeof(char *));
  char *token;

  if (!args) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
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
    token = strtok(NULL, TOK_DELIM);
  }
  args[position] = NULL;
  return args;
}
