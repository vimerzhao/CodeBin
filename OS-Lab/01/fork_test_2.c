/*
 * create a child process from parent and another process from the child
 */
#include "os.h"
int main(int argc, char const* argv[]) {
    int pid1, pid2, reap;

    if ((pid1 = Fork()) == 0) {
        printf("I\'m the first child process: \033[;32m%d\033[0m, created from: \033[;32m%d\033[0m\n", getpid(), getppid());

        if ((pid2 = Fork()) == 0) {
            printf("I\'m the second child process: \033[;32m%d\033[0m, created from: \033[;32m%d\033[0m\n", getpid(), getppid());
            exit(0);
        } else {
            printf("Parent process: \033[;32m%d\033[0m, create child process: \033[;32m%d\033[0m\n", getpid(), pid2);

            while ((reap = wait(NULL)) != -1) {
                printf("Parent process: \033[;32m%d\033[0m, reap child process: \033[;32m%d\033[0m\n", getpid(), reap);
            }
            exit(0);
        }
    } else {
        printf("Parent process: \033[;32m%d\033[0m, create child process: \033[;32m%d\033[0m\n", getpid(), pid1);
    }

    while ((reap = wait(NULL)) != -1) {
        printf("Parent process: \033[;32m%d\033[0m, reap child process: \033[;32m%d\033[0m\n", getpid(), reap);
    }
    return 0;
}
