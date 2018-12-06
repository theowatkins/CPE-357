/*This is the file that handles all of the error outputting.*/

#include "parseline.h"
#include "stage_funcs.h"

static void print_fail(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]);

//TOOK OUT PRINT STATEMENT IN PRINT_FAIL WE MIGHT WANT TO RENAME IT!!!
/*Prints the failed to parse error message.
 *No arguments or return types, solely prints to stderror*/
static void print_fail(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    for(int i = 0; i < STAGE_MAX && stages[i] != NULL; i++){
        for(int j = 0; stages[i]->argv[j] != NULL; j++){
            free(stages[i]->argv[j]);
        }
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
}


/*Prints the pipeline too deep error message.
 *No arguments or return types, solely prints then exits*/
void print_long_pipe(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    fprintf(stderr, "pipeline too deep\n");
    print_fail(stages, full_stages);
}


/*Prints the too many arguments error message.
 *No return types, arguments are freed.*/
void print_many_args(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: too many arguments\n", cmd);
    print_fail(stages, full_stages);
}

/*Prints the invalid null command error message.
 *No arguments or return types, solely prints then exits*/
void print_null_cmd(stage *stages[STAGE_MAX], char *full_stages[STAGE_MAX]){
    fprintf(stderr, "invalid null command\n");
    print_fail(stages, full_stages);
}


/*Prints the bad input redirection error message.
 *The argument is
 *No return types*/
void print_bad_in(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: bad input redirection\n", cmd);
    print_fail(stages, full_stages);
}


/*Prints the bad output redirection error message.
 *The argument is borrowed.
 *No return types*/
void print_bad_out(char *cmd, stage *stages[STAGE_MAX], 
        char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: bad output redirection\n", cmd);
    print_fail(stages, full_stages);
}

/*Prints the ambigious iput error message.
 *Aruments are borrowed.
 *No return type.*/
void print_ambgs_in(char *cmd, stage *stages[STAGE_MAX],
         char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: ambiguous input\n", cmd);
    print_fail(stages, full_stages);
}


/*Prints the ambigious output error message.
 *Arguments are borrowed.
 *No return type.*/
void print_ambgs_out(char *cmd, stage *stages[STAGE_MAX],
         char *full_stages[STAGE_MAX]){
    fprintf(stderr, "%s: ambiguous output\n", cmd);
    print_fail(stages, full_stages);
}


/*frees the stages array in the child when it errors*/
void free_stages(int num_of_stages, stage *stages[STAGE_MAX]){ 
    for(int i = 0; i < num_of_stages; i++){
        for(int j = 0; stages[i]->argv[j] != NULL; j++){
            free(stages[i]->argv[j]);
        }
        if(i == 0){
            if(strcmp(stages[i]->input, "original stdin") != 0){
                free(stages[i]->input);
            }
        }
        if(i == num_of_stages - 1){
            if(strcmp(stages[i]->output, "original stdout") != 0){
                free(stages[i]->output);
            }
        }            
        free(stages[i]);
    }
}
