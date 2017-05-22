/*
 * create two processes from the same parent.
 */
#include "os.h"
int main(int argc, char const* argv[]) {
    pid_t pid1, pid2, reap;
    if ((pid1 = Fork()) == 0) {
        printf("I\'m the first child process: \033[;32m%d\033[0m, created from: \033[;32m%d\033[0m\n", getpid(), getppid());
        exit(0);
    } else {
        printf("Parent process \033[;32m%d\033[0m,create first child process: \033[;32m%d\033[0m\n", getpid(), pid1);

        if ((pid2 = Fork()) == 0) {
            printf("I\'m the second child process: \033[;32m%d\033[0m, created from: \033[;32m%d\033[0m\n", getpid(), getppid());
            exit(0);
        } else {
            printf("Parent process: \033[;32m%d\033[0m,create second child process: \033[;32m%d\033[0m\n", getpid(), pid2);
        }
    }

    // reap zombie
    while ((reap = wait(NULL)) != -1) {
        printf("Parent process: \033[;32m%d\033[0m, reap child process: \033[;32m%d\033[0m\n", getpid(), reap);
    }
    return 0;
}
