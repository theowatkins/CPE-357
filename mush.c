/*This is the main file for the mush shell.*/

#include "parseline.h"
#include "stage_funcs.h"

int main(int argc, char **argv){    
    stage *stages[STAGE_MAX] = {NULL};
    int num_of_stages = 0;    

    if(argc == 1){
        if((num_of_stages = get_stages(stages, stdin)) < 0){
            fprintf(stderr, "error occured %d\n", num_of_stages);
        }
    }
    else if(argc == 2){
        FILE *input = fopen(argv[1], "r");
        if((num_of_stages = get_stages(stages, input)) < 0){
            fprintf(stderr, "error occured: %d\n", num_of_stages);
        }
        
    }
    else{
        fprintf(stderr, "usage: %s [ scriptfile ]\n", argv[0]);
        exit(-1);
    }

    for(int i = 0; i < num_of_stages; i++){
        printf("i: %d\n", i);
        print_stage(stages[i]);
    }

    return 0;
}
