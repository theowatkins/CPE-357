/*This is the driver C file for parseline*/

#include "parseline.h"
#include "stage_funcs.h"
#include "errors.h"

int main(){
    int c = 0;
    int i = 0;
    int line_count = 0;
    int num_of_stages = 0;
    char line[LINE_MAX] = {0};
    stage *stages[STAGE_MAX] = {NULL}; /* Does this need to be {NULL}?*/
    char ins[STAGE_MAX][IN_LEN] = {{0}};
    char outs[STAGE_MAX][OUT_LEN] = {{0}};
    char *full_stages[STAGE_MAX] = {NULL};

    printf("line: ");
    
    /*Get each character from stdin if it's not EOF or newline*/
    while((c = getchar()) != EOF && c != '\n'){
        /* Leaves space for line to be null terminated so 
         * that strtok(3) can properly execute on it*/
        if(line_count < LINE_MAX - 1){
            /*Fill the buffer*/
            line[line_count] = c;
            line_count++;
        }
        /*If the next char results in buffer of length LINE_MAX*/
        else{
            print_long_command(stages, full_stages); 
        }
    }


    /* If last char is a pipe and handles as invlaid null command error 
     * because strtok(3) will ignore the pipe if there are no
     * characters after*/
    if(line[line_count - 1] == '|'){
        print_null_cmd(stages, full_stages);
    }


    /*This section handles parsing the input into separate pipeline commands
     *and allocates space for each stage (struct) and sets the full_stage 
     *accordingly*/

    /* calloc space for first stage -> there always is a stage 0*/
    stages[num_of_stages] = calloc(1, sizeof(stage));

    /*set first stage's full_stage field to be the entire char * 
     *before the first pipeline character*/
    if((stages[num_of_stages]->full_stage = strtok(line, "|")) != NULL){
        num_of_stages++;
        stages[num_of_stages] = calloc(1, sizeof(stage));
        
        /*while there are more stages and STAGE_MAX hasn't
         * been reached, continue setting the full_stage field
         * to the next string returned by strtok and callocing 
         * space for the next stage*/
        while((stages[num_of_stages]->full_stage = 
                    strtok(NULL, "|")) != NULL){
            num_of_stages++;
            if(num_of_stages <= STAGE_MAX)
                stages[num_of_stages] = calloc(1, sizeof(stage));
            else{
                print_long_pipe(stages, full_stages);
            }
        }
    }
   

    /*This sections creates the rest of the stage struct*/ 

    /*Begin by iterating through the number of stages*/
    for(i = 0; i < num_of_stages; i++){
        /*Set the default input and output for stage*/
        /*Stage 0's default is stdin, stdout, or pipeline*/
        if(i == 0){
            stages[i]->input = "original stdin";
            /*If it is the last stage, deault out of stdout*/
            if(i == num_of_stages - 1){
                stages[i]->output = "original stdout";
            }
            /*If not the last stage, default out of pipe*/
            else{
                sprintf(outs[i], "pipe to stage %d", i + 1); 
                stages[i]->output = outs[i];
            }
        }
        /*Every other stage's deafult is pipe, stdout*/
        else{
            sprintf(ins[i], "pipe from stage %d", i - 1);
            stages[i]->input = ins[i];
            /*If it's the last stage, stdout for output*/
            if(i == num_of_stages - 1){
                stages[i]->output = "original stdout";
            }
            /*IF not last stage, defult out of pipe.*/
            else{
                sprintf(outs[i], "pipe to stage %d", i + 1); 
                stages[i]->output = outs[i];
            }
        }

        /*Store full_stage elsewhere so that the char * will be untouched by 
         *the strtok(3) calls in init_stage*/
        full_stages[i] = calloc(strlen(stages[i]->full_stage) + 1,
             sizeof(char));
        strncpy(full_stages[i], stages[i]->full_stage, 
            strlen(stages[i]->full_stage));

        init_stage(stages[i], i, num_of_stages - 1, stages, full_stages);

        /*Reset full_stage field with the previously saved value*/
        stages[i]->full_stage = full_stages[i];
    }

    //print structs and free them
    for(i = 0; i < num_of_stages; i++){
        print_stage(stages[i]);
        free(full_stages[i]);
        free(stages[i]);
    }
    //free space calloced for a new stage but not filled
    free(stages[i]);

    return 0;
}
