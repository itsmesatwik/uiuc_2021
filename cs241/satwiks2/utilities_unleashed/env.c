/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */
 

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "format.h"


extern char **environ;

void set_env(char *env_arg) {
    char *token1, *token2;
    
    token1 = strsep(&env_arg, "=");
    token2 = strsep(&env_arg, "=");

    if (token2[0] == '%') {
        token2++;
        token2 = getenv(token2);
    }
    if (token2 == NULL) {
        print_environment_change_failed();
        exit(123);
    }

    int status = setenv(token1, token2, 1);
    if (status != 0) {
        print_environment_change_failed();
        exit(123);
    }



}

int main(int argc, char *argv[]) {

    if (argc == 1 ) {
        size_t i = 0;
        while (environ[i] != NULL)
            printf("%s\n", environ[i++]);
        return 0;
    }

    if (argc == 2) {
        print_env_usage();
        return 0;
    }

    pid_t child = fork();
    
    // CHILD
    if (child == 0) {
        size_t i = 1;
        while (strcmp(argv[i], "--") != 0) {
            set_env(argv[i]);
            i++;
        }
        i++;

        int status = execvp(argv[i], &argv[i]);
        if (status == -1)
            print_exec_failed();
        exit(123);
    }

    if (child == -1) {
        print_fork_failed();
        return 0;
    }

    int status;
    waitpid(child, &status, 0);

    if (WIFEXITED(status)) {
        exit(WEXITSTATUS(status));
    }

    return 0;
}
