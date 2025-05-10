#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primeHelper(int fd) {
    int base, num;

    if (read(fd, &base, sizeof(int)) == 0) {
        exit(0);  
    }

    printf("prime %d\n", base);

    int np[2];
    pipe(np); 
    if (fork() == 0) { 
        close(np[1]);
        primeHelper(np[0]);
    } else {
        close(np[0]);
        while (read(fd, &num, sizeof(int)) == sizeof(int)) {
            if (num % base != 0) {
                write(np[1], &num, sizeof(int));  
            }
        }
        close(np[1]);
        close(fd);
        wait(0);
        exit(0);
    }
}

int main(int argc, const char *argv[]) {
    int p[2];
    pipe(p);
    if(fork() == 0) {
        close(p[1]);
        primeHelper(p[0]);
    } else {
        close(p[0]);
        int num;
        for(num = 2; num < 36; num++) {
            if(write(p[1], &num, sizeof(int)) != sizeof(int)) {
                fprintf(2, "error: failed to write!\n");
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}