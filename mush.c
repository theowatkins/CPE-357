/*This is the main file for the mush shell.*/

#define _XOPEN_SOURCE
#define _BSD_SOURCE

#include "parseline.h"
#include "stage_funcs.h"
#include "mush.h"
#include "errors.h"


pid_t pid = 0;

void handler(int sig){
    killpg(pid, SIGINT);
}

int main(int argc, char **argv){    
    pid = getpid();
    stage *stages[STAGE_MAX] = {NULL};
    int num_of_stages = 0;   
    int pipe_fds[STAGE_MAX - 1][2];
    pid_t children[STAGE_MAX] = {0};
    char ins[STAGE_MAX][IN_LEN] = {{0}};
    char outs[STAGE_MAX][OUT_LEN] = {{0}};
    struct sigaction sig_ac;


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
                printf("\n");
                break;
            }
            /*This deals with parseline errors -- NEED TO CHANGE DUE TO FREEING*/
            else if(num_of_stages < 0){
                fprintf(stderr, "error occured %d\n", num_of_stages);
                exit(-1);
            }
           
        }
        /*If there is one other argument, run shell using infile.*/
        else if(argc == 2){
            FILE *input = fopen(argv[1], "r");
            if((num_of_stages = get_stages(stages, input, ins, outs)) == CONTROL_D){
                break;
            }
            /*This deals with the parseline errors. -- NEED TO CHANGE DUE TO FREEING*/
            else if(num_of_stages < 0){
                fprintf(stderr, "error occured %d\n", num_of_stages);
                exit(-1);
            }
            if(fclose(input) != 0){
                perror("closing readfile");
                exit(-1);
            }

        }
        /*If there is more than one argument, usage error.*/
        else{
            fprintf(stderr, "usage: %s [ scriptfile ]\n", argv[0]);
            exit(-1);
        }

        /*set up sigaction struct*/
        memset(&sig_ac, 0, sizeof(sig_ac));//clears sigaction memory
        sig_ac.sa_handler = &handler;//give sigaction a handler func
        sigemptyset(&sig_ac.sa_mask);
        if(sigaction(SIGALRM, &sig_ac, NULL) < 0){
            perror("sigaction error");
            exit(-1);
        }


        /*Need to create all the pipes and save the file descriptors.*/
        if(num_of_stages > 0){
            if(get_pipes(num_of_stages, pipe_fds) < 0){
                perror("error with creating pipes");
                break;
            }
        }
        else
            continue;
        
        /*if stages[0] is a cd command, attempt to execute it*/
        if(strcmp(stages[0]->argv[0], "cd") == 0){
            if(num_of_stages > 1){
                fprintf(stderr, "cd must be only argument in pipeline\n");
                free_stages(num_of_stages, stages);
                continue;
            }
            exec_cd(stages[0]);
            free_stages(num_of_stages, stages);
            continue;
        }

        /*Fork off every child and save pid in array.*/
        if(fork_children(stages, num_of_stages, (gid_t)pid, children, pipe_fds) < 0){
            perror("fork_children failed");
        }       
 
       
        /*Need to free every stage*/
        free_stages(num_of_stages, stages);
    }

    return 0;
}
