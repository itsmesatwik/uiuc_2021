/**
 * mapreduce Lab
 * CS 241 - Spring 2019
 * ghli2, satwiks2
 */
 
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv) {
    char *ifn = argv[1];
    char *ofn = argv[2];
    char *man = argv[3];
    char *ren = argv[4];
    int mc = atoi(argv[5]);
    char smc[10];
    pid_t *pids = alloca((2*mc+1)*sizeof(pid_t));
    unsigned (*ecodes)[2] = alloca((2*mc+1)*2*sizeof(pid_t));
    char aname[100];

    int stat;

    // Create an input pipe for each mapper.
    int (*min)[2] = alloca(mc*2*sizeof(int));
    for (int i = 0; i < mc; ++i) {
        int ret = pipe(min[i]);
        if (ret < 0) {
            //fprintf(stderr, "pipe kek'd: %s\n", strerror(errno));
            exit(1);
        }
    }

    // Create one input pipe for the reducer.
    int rin[2];
    if (pipe(rin) < 0) {
        //fprintf(stderr, "pipe kek'd: %s\n", strerror(errno));
        exit(1);
    }

    // Open the output file.
    FILE *of = fopen(ofn, "w");
    if (!of) {
        //fprintf(stderr, "Opening %s kek'd: %s\n", ofn, strerror(errno));
        exit(1);
    }

    // Start a splitter process for each mapper.
    snprintf(smc, 10, "%d", mc);
    for (int i = 0; i <  mc; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            if  (dup2(min[i][1], 1) < 0) {
                //fprintf(stderr, "dup to stdout kek'd on splitter %d: %s\n", i, strerror(errno));
                exit(1);
            }
            char sn[10];
            snprintf(sn, 10, "%d", i);
            if (execlp("./splitter", "./splitter", ifn, smc, sn, NULL) < 0) {
                //fprintf(stderr, "exec kek'd on splitter %d: %s\n", i, strerror(errno));
                exit(1);
            }
        } else if (pid < 0) {
            //fprintf(stderr, "FAILED TO FORK\n");
        } else {
            if (close(min[i][1]) < 0) {
                //fprintf(stderr, "close of input pipe of mapper %d kek'd: %s\n", i, strerror(errno));
                exit(1);
            }
            pids[i] = pid;
        }
    }

    // Start all the mapper processes.
    sprintf(aname, "./%s", man);
    for (int i = 0; i < mc; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            if (dup2(min[i][0], 0) < 0) {
                //fprintf(stderr, "dup to stdin kek'd on mapper %d: %s\n", i, strerror(errno));
                exit(1);
            }
            if (dup2(rin[1], 1) < 0) {
                //fprintf(stderr, "dup to stdout kek'd on mapper %d: %s\n", i, strerror(errno));
                exit(1);
            }
            if (execlp(aname, aname, NULL) < 0) {
                //fprintf(stderr, "mapper %d exec kek'd: %s\n", i, strerror(errno));
                exit(1);
            }
        } else if (pids[i] < 0) {
            //fprintf(stderr, "FAILED TO FORK\n");
        } else {
            if (close(min[i][0]) < 0) {
                //fprintf(stderr, "close of output pipe of mapper %d kek'd: %s\n", i, strerror(errno));
                exit(1);
            }
            pids[mc+i] = pid;
        }
    }

    if (close(rin[1]) < 0) {
        //fprintf(stderr, "close of input pipe of reducer kek'd: %s\n", strerror(errno));
        exit(1);
    }

    // Start the reducer process.
    sprintf(aname, "./%s", ren);
    pid_t pid = fork();
    if (pid == 0) {
        if (dup2(rin[0], 0) < 0) {
            //fprintf(stderr, "dup to stdin kek'd on reducer: %s\n", strerror(errno));
            exit(1);
        }
        if (dup2(fileno(of), 1) < 0) {
            //fprintf(stderr, "dup to stdout kek'd on reducer: %s\n", strerror(errno));
            exit(1);
        }
        if (execlp(aname, aname, NULL) < 0) {
            //fprintf(stderr, "reducer exec kek'd: %s\n", strerror(errno));
            exit(1);
        }
    } else if (pid < 0) {
        //printf("FAILED TO FORK\n");
    } else {
        pids[2*mc] = pid;
    }
    close(rin[0]);

    // Wait for the reducer to finish.
    for (int i = 0; i < 2*mc+1; ++i) {
        waitpid(pids[i], &stat, 0);
        ecodes[i][0] = pids[i];
        if (WIFEXITED(stat))
                ecodes[i][1] = WEXITSTATUS(stat);
    }

    // Print nonzero subprocess exit codes.
    for (int i = 0; i < 2*mc+1; ++i) {
        if (ecodes[i][1]) {
            //fprintf(stderr, "idx: %d, pid: %d, code: %d\n", i, ecodes[i][0], ecodes[i][1]);
            char *name;
            if (i < mc)
                name = "./splitter";
            else if (i == 2*mc)
                name = ren;
            else
                name = man;
            print_nonzero_exit_status(name, ecodes[i][1]);
        }
    }

    fclose(of);
    // Count the number of lines in the output file.
    print_num_lines(ofn);
    return 0;
}
