/*This is the file that handles most process controls of the parent.*/
#include "stage_funcs.h"

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
