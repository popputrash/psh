#ifndef PARSER_H
#define PARSER_H

#define TOK_DELIM " \t\r\n\a"

typedef struct{
	char ***commands;
	int num_commands;
} pipeline_t;

char** parse_args(char *line);
pipeline_t parse_pipeline(char **args);
void free_pipeline(pipeline_t *pipeline);



#endif
