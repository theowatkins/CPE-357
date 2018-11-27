#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*the actual maximum number of characters 
 * that can be ina command line is 4095 because
 * the line needs to be null terminated so strtok(3)
 * can be used on it*/
#define LINE_MAX 4096
#define STAGE_MAX 4

#endif
