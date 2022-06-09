#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int icounter = 0, qcounter = 0;

static void sigint_handler(int signo) {
    printf("#%d: you pressed C-c!\n", ++icounter);
    if(icounter == 10){
        exit(EXIT_FAILURE);
    }
}

static void sigquit_handler(int signo) {
    printf("#%d: you Pressed C-\\!\n", ++qcounter);
    if(qcounter == 5) {
        exit(EXIT_FAILURE);
    }
}

int main() {
    int sigcouter = 0;
    if (signal(SIGINT, SIG_IGN) != SIG_IGN) {
        signal(SIGINT, sigint_handler);
    }

    if (signal(SIGQUIT, SIG_IGN) != SIG_IGN) {
        signal(SIGQUIT, sigquit_handler);
    }

    for(;;) pause();
}