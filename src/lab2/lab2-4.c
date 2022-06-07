// 通讯录读取
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lockreg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock = {.l_type = type, .l_start = offset, .l_whence = whence, .l_len = len};
    return (fcntl(fd, cmd, &lock));
}

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock = {
    .l_type = type,   /* F_RDLCK or F_WRLCK */
    .l_start = offset,  /* byte offset, relative to l_whence */
    .l_whence = whence, /* SEEK_SET, SEEK_CUR, SEEK_END */
    .l_len = len       /* #bytes (0 means to EOF) */
    };

    if (fcntl(fd, F_GETLK, &lock) < 0){
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (lock.l_type == F_UNLCK)
        return (0); /* false, region isn’t locked by another proc */
    return (lock.l_pid);
}

void read_file(int fd) {
    struct flock lock = {.l_type = F_RDLCK, .l_start = 0, .l_whence = SEEK_SET, .l_len = 0};
    if(fcntl(fd, F_SETLKW, &lock) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    int i;
    char c;
    while((i = read(fd, &c, sizeof(c)) != EOF)) {
        if(i < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(write(STDOUT_FILENO, &c, sizeof(c)) < 0) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if(c == EOF) break;
    }
    if(fcntl(fd, F_UNLCK, &lock) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

void write_file(int fd, char *msg) {
    if(lock_test(fd, F_WRLCK, 0, SEEK_END, 0) != 0) {
        printf("the file is locked");
        exit(EXIT_FAILURE);
    }
    struct flock lock = {.l_type = F_WRLCK, .l_start = 0, .l_whence = SEEK_SET, .l_len = 0};
    if(fcntl(fd, F_SETLKW, &lock) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    if(lseek(fd, 0, SEEK_END) < 0) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    if(write(fd, msg, strlen(msg)) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    
    if(fcntl(fd, F_UNLCK, &lock) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

#define _GNU_SOURCE

int main(int argc, char *argv[]) {
    if(argc == 1) {
        fprintf(stderr, "usage: %s [option] file", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[2], O_RDWR | O_CREAT, 0644);
    if(fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "-r") < 0) {
        read_file(fd);
    } else if (strcmp(argv[1], "-w") < 0) {
        char line[1024];
        scanf("%1023[^\n]", line);
        write_file(fd, line);
    }
    close(fd);
}