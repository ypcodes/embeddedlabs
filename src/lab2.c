// 嵌入式 实验2
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
    char *filename = "tempfile";
    char *mess = "here is some text\n";
    // open
    int fd = open(filename, O_CREAT | O_RDWR);
    if (fd < 0) {
        perror("open: ");
        exit(EXIT_FAILURE);
    }

    // write
    if(!write(fd, mess, strlen(mess))) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if(close(fd)) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    int fd2 = open(filename, O_RDWR);
    if(fd2 < 0) {
        perror("open: ");
        exit(EXIT_FAILURE);
    }

    if (!lseek(fd2, 0, SEEK_END)) {
        perror("lseek: ");
        exit(EXIT_FAILURE);
    }

    char *mess2 = "here is another text\n";
    if(!write(fd2, mess2, strlen(mess2))) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if(close(fd2)) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    return 0;
}