/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include"format.h"
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        print_time_usage();
        return 0;
    }

    pid_t child = fork();

    if(child < 0)
        print_fork_failed();

    // PARENT
    if (child == 0) {
        argv++;
        int status = execvp(argv[0], argv);
        if(status == -1)
            print_exec_failed();

    }

    // CHILD
    else {
        struct timespec begin, end;
        int status;

        clock_gettime(CLOCK_MONOTONIC, &begin);
        waitpid(child, &status, WUNTRACED);
        clock_gettime(CLOCK_MONOTONIC, &end);

        float time_taken = end.tv_sec - begin.tv_sec + (end.tv_nsec - begin.tv_nsec)/(float)1000000000;
        if (status == 0)
            display_results(argv, time_taken);
    }

    return 0;
}
