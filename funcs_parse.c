/*This is the file that encases all the functions used in parseline.
 *The initial functions deal with error handling while print_stage
 *deals with printing a complete stage and init_stage deals with 
 *filling all the stage specifications.*/

#include "funcs_parse.h"

/*Prints the failed to parse error message.
 *No arguments or return types, solely prints to stderror*/
void print_fail(){
    fprintf(stderr, "failed to parse pipeline\n");
}


/*Prints the invalid null command error message.
 *No arguments or return types, solely prints then exits*/
void print_null_cmd(){
    fprintf(stderr, "invalid null command\n");
    print_fail();
    exit(-4);
}


/*Prints the bad input redirection error message.
 *The argument is
 *No return types*/
void print_bad_in(char *cmd){
    fprintf(stderr, "%s: bad input redirection\n", cmd);
    print_fail();
    exit(-5);
}


/*Prints the bad output redirection error message.
 *The argument is borrowed.
 *No return types*/
void print_bad_out(char *cmd){
    fprintf(stderr, "%s: bad output redirection\n", cmd);
    print_fail();
    exit(-6);
}


/*Function that prints out the details of each stage.
 *Argument is borrowed.
 *No return type.*/
void print_stage(stage *st){
    char *in = "input";
    char *out = "output";
    char *ac = "argc";
    char *av = "argv";
    int i = 0;

    printf("\n");
    printf("--------\n");
    printf("Stage %d: \"%s\"\n", st->stage_num, st->full_stage);
    printf("--------\n");
    printf("%10s: %s\n", in, st->input);
    printf("%10s: %s\n", out, st->output);
    printf("%10s: %d\n", ac, st->argc);
    printf("%10s: ", av);
    for(i = 0; i < (st->argc - 1); i++)
        printf("\"%s\",", st->argv[i]);
    printf("\"%s\"\n", st->argv[i]);
}


/*This function initializes the stage struct based on the
 *specifications of full_stage in st.
 *Arguments s_num and num_stages are borrowed. Argument st
 *is borrowed and modified.
 *No return type.*/
void init_stage(stage *st, int s_num, int num_stages){
    char *arg = NULL;
    char *cmd = NULL;
    char *in = NULL;
    char *out = NULL;
    int input_rd_count = 0;
    int output_rd_count = 0;

    /*Set the current stage's stage_num*/
    st->stage_num = s_num;

    /*Parse the full_stage for the first word*/ 
    if((arg = strtok(st->full_stage, " ")) != NULL){
        /*Save the first word as the command*/
        cmd = arg;
        
        /*If no argument to start stage, null error command*/
        if(strcmp(cmd, "<") == 0 || strcmp(cmd, ">") == 0){
            print_null_cmd();
        }

        /*Set the argc as 0 initially, then save first argv as cmd.*/
        st->argc = 0; 
        st->argv[st->argc] = cmd;
        st->argc++;

        /*For all words following the command in full_stage*/
        while((arg = strtok(NULL, " ")) != NULL){
            /*If it is an input redirection character*/
            if(strcmp(arg, "<") == 0){
                /*If more than one input redirects*/
                if(input_rd_count != 0)
                    print_bad_in(cmd);
                input_rd_count++;
                /*If the stage isn't the first and is part of pipeline*/
                if(s_num != 0){
                    fprintf(stderr, "%s: ambiguous input\n", cmd);
                    print_fail();
                    exit(-7);
                }
                else{
                    if((in = strtok(NULL, " ")) != NULL){
                        st->input = in;
                    }
                    /*No filename after < so error*/
                    else{
                        print_bad_in(cmd);
                    }
                }
            }
            /*If it is an output redirection character*/
            else if(strcmp(arg, ">") == 0){
                /*If more than one output redirect*/
                if(output_rd_count != 0)
                    print_bad_out(cmd);
                output_rd_count++;
                /*If it isn't the last stage and is part of pipeline*/
                if(s_num != num_stages){
                    fprintf(stderr, "%s: ambiguous output\n", cmd);
                    print_fail();
                    exit(-8);
                }
                else{
                    if((out = strtok(NULL, " ")) != NULL){
                        st->output = out;
                    }
                    /*No filename after > so error*/
                    else{
                        print_bad_out(cmd);
                    }
                }
            }
            /*If it is a regular arg*/
            else{
                st->argv[st->argc] = arg;
                st->argc++;
            }
        }
    }
    /*If returns NULL, there is no command and it's a null error*/
    else{
        print_null_cmd();
    }
}
