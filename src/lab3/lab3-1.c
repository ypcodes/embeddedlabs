#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>

int main() {
    pid_t child_pid = fork();
    if(child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        exit(0);
    }

    // give you time to observe that child is a zombie process by ps(1)
    sleep(10);

    system("ps aux | grep 'Z' ");   
}