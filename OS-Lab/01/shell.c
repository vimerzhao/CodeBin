/*
 * simple shell
 */
#include "os.h"

#define MAXARGS 24
#define MAXLINE 128

void eval(char *cmd);
void parseline(char *buf, char **argv);
int buildin_command(char **argv);

int main(void) {
    char cmd[MAXLINE];
    while (1) {
        printf("> ");
        Fgets(cmd, MAXLINE, stdin);
        if (feof(stdin)) {
            exit(0);
        }
        eval(cmd);
    }
    return 0;
}

void eval(char *cmd) {
    char *argv[MAXARGS]; /* argument list of execve */
    char buf[MAXLINE];   /* hold modified command line */
    pid_t pid;

    strcpy(buf, cmd);

    parseline(buf, argv);

    if (argv[0] == NULL) {
        return;     /* Ignore empty lines */
    }

    if (!buildin_command(argv)) {
        if ((pid = Fork()) == 0) {
            if (execve(argv[0], argv, NULL) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
    }

    // reap zombie
    if (wait(NULL) < 0) {
        unix_error("reap error!");
    }
    return;
}

/* real C! */
void parseline(char *buf, char **argv) {
    char *delim;
    int argc;
    buf[strlen(buf)-1] = ' ';    /* replace trailing '\n' */
    while (*buf && (*buf == ' ')) {
        buf++;
    }
    /* build argument list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) {
            buf++;    /* Ignore spaces */
        }
    }
    argv[argc] = NULL;
}

int buildin_command(char **argv) {
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }
    return 0;
}
