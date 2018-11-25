#include "funcs_parse.h"

void print_fail(){
    fprintf(stderr, "failed to parse pipeline\n");
}

void print_null_cmd(){
    fprintf(stderr, "invalid null command\n");
    print_fail();
    exit(1);
}

void print_bad_in(char *cmd){
    fprintf(stderr, "%s: bad input redirection\n", cmd);
    print_fail();
    exit(1);
}

void print_bad_out(char *cmd){
    fprintf(stderr, "%s: bad output redirection\n", cmd);
    print_fail();
    exit(1);
}

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

void init_stage(stage *st, int s_num, int num_stages){
    char *arg = NULL;
    char *cmd = NULL;
    char *in = NULL;
    char *out = NULL;
    int input_rd_count = 0;
    int output_rd_count = 0;

    st->stage_num = s_num;//set stage number

    if((arg = strtok(st->full_stage, " ")) != NULL){
        cmd = arg;//store first arg as cmd
        //if no argument to start stage
        if(strcmp(cmd, "<") == 0 || strcmp(cmd, ">") == 0){
            print_null_cmd();
        }

        st->argv[st->argc] = cmd;//argc is initially 0(bc of calloc)
        st->argc++;//increase argc

        while((arg = strtok(NULL, " ")) != NULL){
            if(strcmp(arg, "<") == 0){
                if(input_rd_count != 0)//more than one input redirect
                    print_bad_in(cmd);
                input_rd_count++;
                if(s_num != 0){//has pipe in
                    fprintf(stderr, "%s: ambiguous input\n", cmd);
                    print_fail();
                    exit(1);
                }
                else{
                    if((in = strtok(NULL, " ")) != NULL)
                        st->input = in;
                    else//no arg after redirect
                        print_bad_in(cmd);
                }
            }
            else if(strcmp(arg, ">") == 0){
                if(output_rd_count != 0)//more than one output redirect
                    print_bad_out(cmd);
                output_rd_count++;
                if(s_num != num_stages){//has pipe out
                    fprintf(stderr, "%s: ambiguous output\n", cmd);
                    print_fail();
                    exit(1);
                }
                else{
                    if((out = strtok(NULL, " ")) != NULL)
                        st->output = out;
                    else
                        print_bad_out(cmd);
                }
            }
            else{//regular arg
                st->argv[st->argc] = arg;
                st->argc++;
            }
        }
    }
    else{//no args in stage
        print_null_cmd();
    }

}
