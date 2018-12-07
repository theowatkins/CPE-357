/*This is the file that handles all of the error outputting.*/

#include "parseline.h"
#include "stage_funcs.h"
#include "errors.h"


static void free_fail(stage *stages[STAGE_MAX], 
    char *full_stages[STAGE_MAX], int num_of_stages);

/*Used to print the failed to parse error message, now just frees 
 *stages and full_stages.
 *all arguments are freed, except num_of_stages*/
static void free_fail(stage *stages[STAGE_MAX],
        char *full_stages[STAGE_MAX], int num_of_stages){
    /*Need to free the stages*/
    free_stages(num_of_stages, stages);

    /*Need to free the full_stages*/
    for(int i = 0; i < STAGE_MAX && full_stages[i] != NULL; i++){
        free(full_stages[i]);
    }   
}


/*Prints the command line too long errror message.
 *No arguments or return type, solely prints then exits.*/
void print_long_command(stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "command too long\n");
    free_fail(stages, full_stages, num_of_stages);
}


/*Prints the pipeline too deep error message.
 *No arguments or return types, solely prints then exits*/
void print_long_pipe(stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "pipeline too deep\n");
    free_fail(stages, full_stages, num_of_stages);
}


/*Prints the too many arguments error message.
 *No return types, arguments are freed.*/
void print_many_args(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "%s: too many arguments\n", cmd);
    free_fail(stages, full_stages, num_of_stages);
}

/*Prints the invalid null command error message.
 *No arguments or return types, solely prints then exits*/
void print_null_cmd(stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "invalid null command\n");
    free_fail(stages, full_stages, num_of_stages);
}


/*Prints the bad input redirection error message.
 *The argument is
 *No return types*/
void print_bad_in(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "%s: bad input redirection\n", cmd);
    free_fail(stages, full_stages, num_of_stages);
}


/*Prints the bad output redirection error message.
 *The argument is borrowed.
 *No return types*/
void print_bad_out(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "%s: bad output redirection\n", cmd);
    free_fail(stages, full_stages, num_of_stages);
}

/*Prints the ambigious iput error message.
 *Aruments are borrowed.
 *No return type.*/
void print_ambgs_in(char *cmd, stage *stages[STAGE_MAX],
         char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "%s: ambiguous input\n", cmd);
    free_fail(stages, full_stages, num_of_stages);
}


/*Prints the ambigious output error message.
 *Arguments are borrowed.
 *No return type.*/
void print_ambgs_out(char *cmd, stage *stages[STAGE_MAX],
         char *full_stages[STAGE_MAX], int num_of_stages){
    fprintf(stderr, "%s: ambiguous output\n", cmd);
    free_fail(stages, full_stages, num_of_stages);
}


/*frees the stages array in the child when it errors
 *stages is modified as it is freed, num isn't
 *No return type.*/
void free_stages(int num_of_stages, stage *stages[STAGE_MAX]){ 
    for(int i = 0; i < num_of_stages && stages[i] != NULL; i++){
        for(int j = 0; j < stages[i]->argc; j++){
            free(stages[i]->argv[j]);
        }
        if(i == 0){
            if(stages[i]->input != NULL &&
                    strcmp(stages[i]->input, "original stdin") != 0){
                free(stages[i]->input);
            }
        }
        if(i == num_of_stages - 1){
            if(stages[i]->output != NULL && 
                    strcmp(stages[i]->output, "original stdout") != 0){
                free(stages[i]->output);
            }
        }            
        free(stages[i]);
    }
}
