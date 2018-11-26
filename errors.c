/*This is the file that handles all of the error outputting.*/

#include "parseline.h"
#include "stage_funcs.h"

static void print_fail(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]);

/*Prints the failed to parse error message.
 *No arguments or return types, solely prints to stderror*/
static void print_fail(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    fprintf(stderr, "failed to parse pipeline\n");
    for(int i = 0; i <= STAGE_MAX && stages[i] != NULL; i++){
        free(stages[i]);
    }
    for(int i = 0; i < STAGE_MAX && full_stages[i] != NULL; i++){
        free(full_stages[i]);
    }   
}


/*Prints the command line too long errror message.
 *No arguments or return type, solely prints then exits.*/
void print_long_command(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    fprintf(stderr, "command too long\n");
    print_fail(stages, full_stages);
    exit(-1);
}


/*Prints the pipeline too deep error message.
 *No arguments or return types, solely prints then exits*/
void print_long_pipe(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    fprintf(stderr, "pipeline too deep\n");
    print_fail(stages, full_stages);
    exit(-2);
}


/*Prints the too many arguments error message.
 *No return types, arguments are freed.*/
void print_many_args(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: too many arguments\n", cmd);
    print_fail(stages, full_stages);
    exit(-3);
}

/*Prints the invalid null command error message.
 *No arguments or return types, solely prints then exits*/
void print_null_cmd(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    fprintf(stderr, "invalid null command\n");
    print_fail(stages, full_stages);
    exit(-4);
}


/*Prints the bad input redirection error message.
 *The argument is
 *No return types*/
void print_bad_in(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: bad input redirection\n", cmd);
    print_fail(stages, full_stages);
    exit(-5);
}


/*Prints the bad output redirection error message.
 *The argument is borrowed.
 *No return types*/
void print_bad_out(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: bad output redirection\n", cmd);
    print_fail(stages, full_stages);
    exit(-6);
}

/*Prints the ambigious iput error message.
 *Aruments are borrowed.
 *No return type.*/
void print_ambgs_in(char *cmd, stage *stages[STAGE_MAX],
         char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: ambiguous input\n", cmd);
    print_fail(stages, full_stages);
    exit(-7);
}


/*Prints the ambigious output error message.
 *Arguments are borrowed.
 *No return type.*/
void print_ambgs_out(char *cmd, stage *stages[STAGE_MAX],
         char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: ambiguous output\n", cmd);
    print_fail(stages, full_stages);
    exit(-8);
}
