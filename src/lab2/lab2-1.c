#include <stdio.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUFSIZE 10240

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s usage: src des", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *srcfile = argv[1];
    char *destfile = argv[2];

    // open source file
    int fdsrc = open(srcfile, O_RDONLY);
    if(fdsrc < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int fdest = open(destfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(!fdest) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if(!lseek(fdsrc, -10240, SEEK_END)) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    // read last 10kb to a array;
    char array[BUFSIZE + 1];
    array[BUFSIZE] = '\0';
    if(read(fdsrc, array, BUFSIZE) < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if(write(fdest, array, BUFSIZE) < 0) {
        perror("wirte");
        exit(EXIT_FAILURE);
    }

    if(close(fdsrc) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if(close(fdest) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}