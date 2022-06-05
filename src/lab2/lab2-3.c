#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "usage: %s source destination\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fdsrc = open(argv[1], O_RDONLY);
    if(!fdsrc) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    int fdest = open(argv[2], O_RDWR | O_CREAT, 0644);
    if(!fdest) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char c, last = 'a';
    int sd;
    bool qflag = false, sharpflag = false;
    while ((sd = read(fdsrc, &c, sizeof(c)))) {
        if(sd < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(c == EOF) {
            break;
        }

        if(c == '#') {
            sharpflag = true;
        }

        if(c == '"') {
            qflag = qflag ? false : true; 
        } else if (sharpflag == true && c == '\n') {
            sharpflag = false;
        }
        
        if(c != ' ' || qflag || sharpflag || (last == '\n' && c == '\n')) {
            if(write(fdest, &c, sizeof(c)) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            } else {
                if (lseek(fdest, 1, SEEK_CUR) < 0) {
                    perror("lseek");
                    exit(EXIT_FAILURE);
                }
            }
        }
        last = c;
    }
    if(close(fdsrc) < 0 || close(fdest) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}