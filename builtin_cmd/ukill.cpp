#include "../include/builtin_cmd.h"
#include <signal.h>

void ukill::run(int argc, char **argv) {
    if (argv[1] == NULL) {
        fprintf(stderr, "ush: expected argument to \"ukill\"\n");
    } else {
        int pid;
        sscanf(argv[1], "%d", &pid);
        int retval = kill(pid,SIGKILL);
        if (retval) {
            perror("ush");
        }
        else {
            printf("Process %d is killed\n", pid);
        }
    }

}

