#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage_funcs.h"

/*the actual maximum number of characters 
 * that can be ina command line is 4095 because
 * the line needs to be null terminated so strtok(3)
 * can be used on it*/
#define LINE_MAX 4096
#define CONTROL_D -100

int get_stages(stage *stages[STAGE_MAX], FILE *readfile,
     char ins[STAGE_MAX][IN_LEN],
    char outs[STAGE_MAX][OUT_LEN]);


#endif
