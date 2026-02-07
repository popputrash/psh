#ifndef EXEC_H
#define EXEC_H

#include "parser.h"

int psh_execute(char **args);
int psh_execute_pipeline(pipeline_t pipeline);

#endif
