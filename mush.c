/*This is the main file for the mush shell.*/

#include <sys/types.h>
#include "parseline.h"
#include "stage_funcs.h"
#include "mush.h"


int main(int argc, char **argv){    
    stage *stages[STAGE_MAX] = {NULL};
    int num_of_stages = 0;   
    int pipe_fds[STAGE_MAX - 1][2];
    pid_t children[STAGE_MAX] = {0};
    char ins[STAGE_MAX][IN_LEN] = {{0}};
    char outs[STAGE_MAX][OUT_LEN] = {{0}};



    /*Need to be in a large while loop until control D or exit().*/
    while(num_of_stages != CONTROL_D){

        /*This is the section where we deal with running the parseline and
         *and getting the array of stage pointers and number of stages back.
         *If num_of_stages == CONTROL_D, end the shell
         *If any other negative number, there was an error parsing.*/

        /*If there is no other arguments, run shell through stdio.*/
        if(argc == 1){
            /*NEED TO FREE EVERYTHING HERE, or break?*/
            if((num_of_stages = get_stages(stages, stdin, ins, outs)) == CONTROL_D){
                break;
            }
            else if(num_of_stages < 0){
                fprintf(stderr, "error occured %d\n", num_of_stages);
            }
           
        }
        /*If there is one other argument, run shell using infile.*/
        else if(argc == 2){
            FILE *input = fopen(argv[1], "r");
            if((num_of_stages = get_stages(stages, input, ins, outs)) == CONTROL_D){
                break;
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
        if(num_of_stages > 0){
            if(get_pipes(num_of_stages, pipe_fds) < 0){
                perror("error with creating pipes");
                break;
            }
        }

        /*Fork off every child and save pid in array.*/
        if(fork_children(stages, num_of_stages, children, pipe_fds) < 0){
            perror("fork_children failed");
        }       
 
       
        /*Need to free every stage*/
        for(int i = 0; i < num_of_stages; i++){
            for(int j = 0; stages[i]->argv[j] != NULL; j++){
                free(stages[i]->argv[j]);
            }            
            free(stages[i]);
        }
    }

    return 0;
}
