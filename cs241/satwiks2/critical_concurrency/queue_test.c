/**
 * Critical Concurrency Lab
 * CS 241 - Spring 2019
 */
 
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s test_number\n", argv[0]);
        exit(1);
    }
    printf("Please write tests cases\n");
    return 0;
}
