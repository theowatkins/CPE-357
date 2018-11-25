#include "parseline.h"
#include "funcs_parse.h"

int main(){
    int c = 0;
    int i = 0;
    int line_count = 0;
    int num_of_stages = 0;
    char line[LINE_MAX] = {0};
    stage *stages[STAGE_MAX] = {0};
    char ins[STAGE_MAX][IN_LEN] = {{0}};
    char outs[STAGE_MAX][OUT_LEN] = {{0}};
    char *full_stages[STAGE_MAX];

    printf("line: ");
    
    while((c = getchar()) != EOF && c != '\n'){
        /*leaves space for line to be null terminated so 
         * that strtok(3) can properly execute on it*/
        if(line_count < LINE_MAX - 1){
            line[line_count] = c;//fill buffer
            line_count++;
        }
        else{
            fprintf(stderr, "command too long\n");
            print_fail();
            exit(1);
        }
    }

    /*checks if last character in line is a pipe 
     * because strtok(3) will ignore it if there are no
     * characters after*/
    if(line[line_count - 1] == '|')
        print_null_cmd();

    //calloc space for first stage
    stages[num_of_stages] = calloc(1, sizeof(stage));
    /*set first stage's full_stage field to be the entire
     * string before the first pipeline character*/
    if((stages[num_of_stages]->full_stage = 
                strtok(line, "|")) != NULL){
        num_of_stages++;
        //calloc space for the next stage
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
                fprintf(stderr, "pipeline too deep\n");
                print_fail();
                exit(1);
            }
        }
    }
    
    //make stage structs
    for(i = 0; i < num_of_stages; i++){
        //set default input and output for stage
        if(i == 0){//first stage
            stages[i]->input = "original stdin";
            if(i == num_of_stages - 1)//last stage
                stages[i]->output = "original stdout";
            else{
                sprintf(outs[i], "pipe to stage %d", i + 1); 
                stages[i]->output = outs[i];
            }
        }
        else{//not first stage
            sprintf(ins[i], "pipe from stage %d", i - 1);
            stages[i]->input = ins[i];
            if(i == num_of_stages - 1)//last stage
                stages[i]->output = "original stdout";
            else{
                sprintf(outs[i], "pipe to stage %d", i + 1); 
                stages[i]->output = outs[i];
            }
        }

        /*store full stages elsewhere so that they will be untouched by 
         * the strtok(3) calls in init_stage*/
        full_stages[i] = calloc(strlen(stages[i]->full_stage), sizeof(char));
        strcpy(full_stages[i], stages[i]->full_stage);

        init_stage(stages[i], i, num_of_stages - 1);

        //reset full_stage field
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
