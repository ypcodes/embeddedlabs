#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s file", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int fd = open(filename, O_RDWR | O_CREAT);
    if(!fd) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t p;
    if((p = lock_test(fd, F_RDLCK, 0, SEEK_SET, 0)) != 0) {
        printf("pid %d lock this file with read lock\n", p);
        exit(EXIT_FAILURE);
    }

    if((p = lock_test(fd, F_WRLCK, 0, SEEK_CUR, 0)) != 0) {
        printf("pid %d lock this file with writing lock\n", p);
        exit(EXIT_FAILURE);
    }

    if(lockreg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0) {
        perror("lock");
        exit(EXIT_FAILURE);
    }

    getchar();

    if(lockreg(fd, F_SETLKW, F_UNLCK, 0, SEEK_SET, 0) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    getchar();
}