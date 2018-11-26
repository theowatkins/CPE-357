#ifndef FUNCS_PARSE_H
#define FUNCS_PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseline.h"

#define ARG_MAX 64
//max length of default input message plus null
#define IN_LEN 19
//max length of default output message plus null
#define OUT_LEN 17

typedef struct stage{
    int stage_num;
    int argc;
    char *full_stage;
    char *argv[ARG_MAX];
    char *input;
    char *output;
}stage;

void print_stage(stage *st);
void init_stage(stage *st, int s_num, int num_stages,
    stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]);

#endif
