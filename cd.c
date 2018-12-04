
void exec_cd(stage *st){
    //make sure there is only one arg to cd
    if(st->argc == 1){
        fprintf(stderr, "cd: missing argument\n");
        exit(1);
    }
    else if(st->argc > 2){
        fprintf(stderr, "cd: too many arguments\n");
        exit(1);
    }
    
    if(chdir(st->argv[1]) != 0){
        fprintf(stderr, "%s: %s\n", st->argv[1], strerror(errno));
        exit(1);
    }
}
