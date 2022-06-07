#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if(argc == 1) {
        fprintf(stderr, "usage %s cmd args\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pid_t child_pid;
    if((child_pid = vfork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        char *cmd = argv[1];
        char *args[argc - 2];
        for (int i = 2; i < argc; i++)
        {
            args[i] = argv[i];
        }

        execlp(cmd, args);
        _exit(0);
    }

    if(wait(NULL) != child_pid) {   // we don't care status of child process
        perror("wait");
        exit(EXIT_FAILURE);
    }
}