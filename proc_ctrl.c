/*This is the file that handles most process controls of the parent.*/

#include <sys/wait.h>
#include "stage_funcs.h"
#include "errors.h"

/*This function takes in the num_of_stages and the empty 
 *data structure of an array for two int arrays. This gets modified*/
int get_pipes(int num_of_stages, int pipe_fds[STAGE_MAX - 1][2]){
    for(int i = 0; i < num_of_stages - 1; i++){
        if(pipe(pipe_fds[i]) < 0){
            return -1;
        }
    }
    return 0;
}



/*This is the function that handles forking all the children. 
 *All arguments are borrowed and not modified.*/
int fork_children(stage *stages[STAGE_MAX], int num_of_stages, 
        pid_t children[STAGE_MAX], int pipe_fds[STAGE_MAX - 1][2]){
    int infd = 0;
    int outfd = 1;

    for(int i = 0; i < num_of_stages; i++){
        /*If the fork failes, perror and close parent fds*/
        if((children[i] = fork()) < 0){
            perror("Error with forking child");
            for(int h = 0; h < num_of_stages - 1; h++){
                close(pipe_fds[h][0]);
                close(pipe_fds[h][1]);
            }
            return -1;
        }
        /*If pid is 0, it's the child.*/
        else if(children[i] == 0){
            /*First need to deal with the input file*/
            /*If it is the first child, deal with input of og or file*/
            if(i == 0){
                /*If not standard in, open file*/
                if(strcmp(stages[0]->input, "original stdin") != 0){
                    infd = open(stages[0]->input, O_RDONLY); 
                    if(dup2(infd, 0) < 0){
                        perror("Error with dup2 in input");
                        free_child_stages(num_of_stages, stages);
                        exit(-1);
                    }
                    close(infd);
                }
                /*Don't need to dup if input is stdin*/
            }
            /*If not first child, input is pipe*/
            else{
                if(dup2(pipe_fds[i - 1][0],0) < 0){
                    perror("Error with dup2 in input");
                    free_child_stages(num_of_stages, stages);
                    exit(-1);
                }
            }

            /*Next deal with the output file*/
            /*If it is the last child, deal with output of og or file*/
            if(i == num_of_stages - 1){
                /*If not standard out, open file*/
                if(strcmp(stages[num_of_stages - 1]->output, "original stdout")
                         != 0){
                    outfd = creat(stages[num_of_stages - 1]->output, 
                            S_IRWXU|S_IRGRP|S_IWOTH);
                    if(dup2(outfd, 1) < 0){
                        perror("Error with dup2 in output");
                        free_child_stages(num_of_stages, stages);
                        exit(-1);
                    }
                    close(outfd);
                }
                /*Don't need to dup if output is stdout*/
            }
            /*If not last child, output is pipe.*/
            else{ 
                if(dup2(pipe_fds[i][1], 1) < 0){
                    perror("Error with dup2 in output");
                    free_child_stages(num_of_stages, stages);
                    exit(-1);
                }
            }

            /*Close all pipe fds since we've now dupped properly*/
            for(int j = 0; j < num_of_stages - 1; j++){
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            /*All the set-up was accomplished, so exec*/
            if(execvp(stages[i]->argv[0], stages[i]->argv) < 0){
                perror(stages[i]->argv[0]);
                free_child_stages(num_of_stages, stages);
                exit(-1);
            }
        }
        /*Now you have to deal with the parent*/
        else{
            /**/
            if(i > 0 && i < num_of_stages){
                close(pipe_fds[i - 1][0]);
                close(pipe_fds[i - 1][1]);
            }
        }

        /*Need to wait for every child*/
        int status = 0;
        if(waitpid(children[i], &status, 0) != children[i]){
            perror("error with waitpid");
            return -1;
        }
        /*Need to check if CTRL+C was signaled*/
        if(WIFSIGNALED(status)){
            for(int k = i + 1; k < num_of_stages - 1; k++){
                close(pipe_fds[k][0]);
                close(pipe_fds[k][1]);
            }
            return -1;
        }
        /*Need to check if it exited properly*/
        else if(WIFEXITED(status)){
            /*Check to see if exec failed*/
            if(WEXITSTATUS(status) == -1){
                if(i == 0){
                    for(int j = 0; j < num_of_stages - 1; j++){
                        close(pipe_fds[j][0]);
                        close(pipe_fds[j][1]);
                    }
                }
                else{
                    for(int k = i; k < num_of_stages - 1; k++ ){
                        close(pipe_fds[k][0]);
                        close(pipe_fds[k][1]);
                    }
                }
            }
        }
    }
    return 0;
}


