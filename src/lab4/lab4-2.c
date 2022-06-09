#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static int counter = 0;

static void sig_alrm(int signo){
    printf("#%d: Hello.\n", counter + 1);
}

int main(void) {
    for (int i = 0; i < 10; i++)
    {
        counter = i;
        if(signal(SIGALRM, sig_alrm) == SIG_ERR) {
            perror("alarm error");
            exit(EXIT_FAILURE);
        }
        alarm(2);
        pause();
        alarm(0);
    }
    
}