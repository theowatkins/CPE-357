/*This is the main file for the mush shell.*/

#include "parseline.h"
#include "stage_funcs.h"

int main(int argc, char **argv){    
    stage *stages[STAGE_MAX] = {NULL};
    int num_of_stages = 0;    

    /*Need to be in a large while loop.*/


    /*This is the section where we deal with running the parseline and
     *and getting the array of stage pointers and number of stages back.
     *If num_of_stages == CONTROL_D, end the shell
     *If any other negative number, there was an error parsing.*/

    /*If there is no other arguments, run shell through stdio.*/
    if(argc == 1){
        /*NEED TO FREE EVERYTHING HERE, or break?*/
        if((num_of_stages = get_stages(stages, stdin)) == CONTROL_D){
            exit(CONTROL_D);
        }
        else if(num_of_stages < 0){
            fprintf(stderr, "error occured %d\n", num_of_stages);
        }
       
    }
    /*If there is one other argument, run shell using infile.*/
    else if(argc == 2){
        FILE *input = fopen(argv[1], "r");
        if((num_of_stages = get_stages(stages, input)) == CONTROL_D){
            exit(CONTROL_D);
        }
        else if(num_of_stages < 0){
            fprintf(stderr, "error occured %d\n", num_of_stages);
        }
   }
    /*If there is more than one argument, usage error.*/
    else{
        fprintf(stderr, "usage: %s [ scriptfile ]\n", argv[0]);
        exit(-1);
    }


    /*Need to set signal mask.*/



    /*Need to create all the pipes and save the file descriptors.*/


    /*Fork off every child and save pid in array.*/
    


    for(int i = 0; i < num_of_stages; i++){
        printf("i: %d\n", i);
        print_stage(stages[i]);
    }

    return 0;
}
