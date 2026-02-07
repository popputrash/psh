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

pipeline_t parse_pipeline(char **args) {
  pipeline_t pipeline;
  int count = 1;
  if (args[0] == NULL) {
    pipeline.commands = malloc(sizeof(char **));
    if (!pipeline.commands) {
      fprintf(stderr, "psh: allocation error");
      exit(EXIT_FAILURE);
    }
    pipeline.commands[0] = args;
    pipeline.num_commands = 1;
    return pipeline;
  }

  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "|") == 0) {
      count++;
    }
  }

  int cmd_index = 0;
  pipeline.commands[0] = &args[0];
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "|") == 0) {
      if (i == 0 || pipeline.commands[cmd_index] == &args[i]) {
        fprintf(stderr, "psh: syntax err");
        free(pipeline.commands);
        pipeline.commands = NULL;
        pipeline.commands = 0;
        return pipeline;
      }
      args[i] == NULL;
      cmd_index++;
      pipeline.commands[cmd_index] = &args[i + 1];
    }
  }

  if (pipeline.commands[cmd_index][0] == NULL) {
    fprintf(stderr, "psh: syntax err");
    free(pipeline.commands);
    pipeline.commands = NULL;
    pipeline.num_commands = 0;
    return pipeline;
  }
  pipeline.num_commands = count;
  return pipeline;
}

void free_pipeline(pipeline_t *pipeline){
	free(pipeline->commands);
	pipeline->commands = NULL;
	pipeline->num_commands = 0;
}
