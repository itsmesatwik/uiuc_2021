/**
 * Password Cracker Lab
 * CS 241 - Spring 2019
 */
 
#include "cracker1.h"
#include "format.h"
#include "utils.h"
#include "queue.h"
#include <math.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

static queue *task_queue;
pthread_mutex_t m;
pthread_cond_t cv;
int no_more_strings = 0;
int num_inp = 0;
int pass = 0;
int fail = 0;

void set(char *str) {
    for (size_t i = getPrefixLength(str); i < strlen(str); i++) {
        str[i] = 'a';
    }
}

void crack(char **args, int id, double time_s) {
    char *uname = args[0];
    char *hash = args[1];
    char *known = args[2];

    v1_print_thread_start(id, uname);

    struct crypt_data cdata;
    cdata.initialized = 0;
    double i = 0;
    double limit =  pow(26, (strlen(known) - getPrefixLength(known)));
    set(known);
    while(i++ < limit) {
        char *hash_test = crypt_r(known, "xx", &cdata);
        if (strcmp(hash, hash_test) == 0) {
            /*FOUND*/
            pthread_mutex_lock(&m);
            pass++;
            pthread_mutex_unlock(&m);
            v1_print_thread_result(id, uname, known, i+1, getThreadCPUTime() - time_s, 0);
            return;
        }
        incrementString(known);
    }
    pthread_mutex_lock(&m);
    fail++;
    pthread_mutex_unlock(&m);
    v1_print_thread_result(id, uname, known, i+1, getThreadCPUTime() - time_s, 1);
}

char **line_split(char *str) {
    char *a3 = alloca(strlen(str));
    char **args = malloc(sizeof(char*) * 3);
    args[0] = strtok_r(str," ", &a3);
    args[1] = strtok_r(NULL, " ", &a3);
    args[2] = strtok_r(NULL, " ", &a3);
    return args;
}

void *crack_start(void *arg) {
    int index = *((int*)arg);
    double thread_start = getThreadCPUTime();

    // Try to pull from the task_queue
    while (1) { 
        pthread_mutex_lock(&m);
        while(num_inp == 0 && no_more_strings == 0) {
            pthread_cond_wait(&cv, &m);
        }
        if (no_more_strings == 1 && num_inp == 0) {
            pthread_mutex_unlock(&m);
            return NULL;
        }
        char *line = NULL;
        if (num_inp != 0) {
            line = queue_pull(task_queue);
            num_inp--;
        }
        pthread_mutex_unlock(&m);
        if (line == NULL)
            continue;

        char **args = line_split(line);
        crack(args, index, thread_start);
        free(args);
        free(line);

        // Check if there are no more 
    }
    return NULL;
}

int start(size_t thread_count) {
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&cv, NULL);
    task_queue = queue_create(-1);

    pthread_t *tid = malloc(sizeof(pthread_t) * thread_count);
    size_t t[thread_count];

    for (size_t i = 0; i < thread_count; i++) {
        t[i] = i+1;
        pthread_create(&tid[i], NULL, crack_start,t+i);
    }

    // Read the cmd line input
    char* buffer = NULL;
    while (1) {
        size_t numchar;
        buffer = NULL;
        ssize_t status = getline(&buffer, &numchar, stdin);
        if (status == -1)
            break;
        buffer[strlen(buffer) - 1] = '\0';
        queue_push(task_queue, buffer);
        pthread_mutex_lock(&m);
        num_inp++;
        pthread_cond_signal(&cv);
        pthread_mutex_unlock(&m);
    }

    // Set the bool for no more strings to crack
    pthread_mutex_lock(&m);
    no_more_strings = 1;
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&m);

    for (size_t i = 0; i < thread_count; i++) {
        pthread_join(tid[i], NULL);
    }

    v1_print_summary(pass, fail);
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&cv);

    free(buffer);

    free(tid);

    queue_destroy(task_queue);
    // TODO your code here, make sure to use thread_count!
    // Remember to ONLY crack passwords in other threads
    return 0; // DO NOT change the return code since AG uses it to check if your
              // program exited normally
}
