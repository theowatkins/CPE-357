/*this is the header that contains necessary info for mush.c*/

#ifndef MUSH
#define MUSH

#include <signal.h>
#include <sys/types.h>
#include "stage_funcs.h"

int get_pipes(int num_of_stages, int pipe_fds[STAGE_MAX - 1][2]);
int fork_children(stage *stages[STAGE_MAX], int num_of_stages, gid_t child_gid,
    pid_t children[STAGE_MAX], int pipe_fds[STAGE_MAX - 1][2]);

#endif
