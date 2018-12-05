/*This is the file for the header to errors.*/
#ifndef ERRORS
#define ERRORS

#include "parseline.h"

void print_long_command(stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_long_pipe(stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_many_args(char *cmd, stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_null_cmd(stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_bad_out(char *cmd, stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_bad_in(char *cmd, stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_ambgs_in(char *cmd, stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void print_ambgs_out(char *cmd, stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX]);
void free_stages(int num_of_stages, 
        stage *stages[STAGE_MAX]);
#endif

