/**
 * Password Cracker Lab
 * CS 241 - Spring 2019
 */
 
#include "cracker2.h"
#include "format.h"
#include "utils.h"
#include "queue.h"
#include <math.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

static queue *task_queue;
pthread_mutex_t m;
pthread_mutex_t m2;
pthread_barrier_t b;
pthread_barrier_t b2;
pthread_cond_t cv;
pthread_cond_t cv2;
int no_more_strings = 0;
int found = 0;
int num_inp = 0;
int pass = 0;
int fail = 0;
int ended = 0;
size_t threads = 0;
long hash_count = 0;
char *line = NULL;
char *password = NULL;


// void set(char *str, long n) {
    
// }

char **line_split(char *str_) {
	char *str = strdup(str_);
	char *a3 = alloca(strlen(str));
    char **args = malloc(sizeof(char*) * 3);
    args[0] = strtok_r(str," ", &a3);
    args[1] = strtok_r(NULL, " ", &a3);
    args[2] = strtok_r(NULL, " ", &a3);
    return args;
}

void crack(char **args, int id) {
    char *username = args[0];
    char *hash = args[1];
    char *known = strdup(args[2]);
    long i;
    long limit;
    getSubrange(strlen(known)-getPrefixLength(known), threads, id, &i, &limit);


    struct crypt_data cdata;
    cdata.initialized = 0;
    double index = 0;
    setStringPosition(known+getPrefixLength(known), i);
    v2_print_thread_start(id, username, i, known);

    while(index++ < limit) {
        pthread_mutex_lock(&m);
        if (found) {
            pthread_mutex_unlock(&m);
            break;
        }
        pthread_mutex_unlock(&m);
        char *hash_test = crypt_r(known, "xx", &cdata);
        if (strcmp(hash, hash_test) == 0) {
            /*FOUND*/
            pthread_mutex_lock(&m);
            pass++;
            found = 1;
            password = strdup(known);
            //printf("PASSWD = %s\n", goodpass);
            v2_print_thread_result(id, index, 0);
            hash_count += index;
            pthread_mutex_unlock(&m);
            free(known);
            return;
        }
        incrementString(known);
    }
    pthread_mutex_lock(&m);
    int res = 2;
    if (--index != limit)
        res = 1;
    v2_print_thread_result(id, index, res);
    hash_count += index;
    pthread_mutex_unlock(&m);
    free(known);
}


void *crack_start(void *arg) {
	int index = *((int*)arg);
	char **args = NULL;
    // Try to pull from the task_queue
    while (1) {

    	if (index == 1) {
    		pthread_mutex_lock(&m2);
    		pthread_mutex_lock(&m);
    	}
    	else {
    		pthread_mutex_lock(&m2);
    	}

    	while((index == 1 && num_inp == 0 && no_more_strings == 0) || (index != 1 && line == NULL && ended == 0)) {
            if (index == 1) {
                pthread_cond_wait(&cv, &m);
            } else {
                pthread_cond_wait(&cv2, &m2);
            }
        }

        //pthread_mutex_unlock(&m2);
        if (no_more_strings == 1 && num_inp == 0) {
            if (index == 1) {
            	ended = 1;
            	pthread_mutex_unlock(&m2);
                pthread_mutex_unlock(&m);
                pthread_cond_broadcast(&cv2);
                return NULL;
            } 
            else {
                if (line == NULL) {
                	pthread_mutex_unlock(&m2);
                    return NULL;
                }
            }
        }
        char *buf = NULL;
        if (index == 1) {
            if (num_inp != 0) {
                buf = (queue_pull(task_queue));
                line = strdup(buf);
                args = line_split(buf);
                v2_print_start_user(args[0]);
                num_inp--;
                pthread_cond_broadcast(&cv2);
            }
            pthread_mutex_unlock(&m);
        } 
        else {
            buf = strdup(line);
            args = line_split(buf);
        }
        pthread_mutex_unlock(&m2);

        double thread_start = getCPUTime();
		double clock = getTime();

    	crack(args, index);
    	pthread_barrier_wait(&b);
    	if (index == 1) {
    		pthread_mutex_lock(&m2);
    		v2_print_summary(args[0], password, hash_count, getTime() - clock, getCPUTime() - thread_start,1-found);
            free(line);
            line = NULL;
            free(password);
            password = NULL;
            found = 0;
            hash_count = 0;
            pthread_mutex_unlock(&m2);
            pthread_cond_broadcast(&cv2);
        } 
        else {
        	pthread_mutex_lock(&m2);
            while (line != NULL) {
                pthread_cond_wait(&cv2, &m2);
            }
            pthread_mutex_unlock(&m2);
        }
        free(args);
        free(buf);
        pthread_barrier_wait(&b2);

        // Check if there are no more 
    }
    return NULL;
}


int start(size_t thread_count) {
	threads = thread_count;
	pthread_mutex_init(&m, NULL);
	pthread_mutex_init(&m2, NULL);
    pthread_cond_init(&cv, NULL);
    pthread_cond_init(&cv2, NULL);
    pthread_barrier_init(&b, NULL, thread_count);
    pthread_barrier_init(&b2, NULL, thread_count);
    task_queue = queue_create(-1);

    pthread_t *tid= malloc(sizeof(pthread_t) * thread_count);
    size_t t[thread_count];

    for (size_t i = 0; i < thread_count; i++) {
        t[i] = i+1;
        pthread_create(tid+i, NULL, crack_start,t+i);
    }

    // Read the cmd line input
    char *buffer;
    while (1) {
        size_t numchar = 0;
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
    pthread_mutex_lock(&m2);
    pthread_mutex_lock(&m);
    no_more_strings = 1;
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&m);
    pthread_mutex_unlock(&m2);


    for (size_t i = 0; i < thread_count; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&m);
    pthread_mutex_destroy(&m2);
    queue_destroy(task_queue);
    pthread_cond_destroy(&cv);
    pthread_cond_destroy(&cv2);
    pthread_barrier_destroy(&b);
    pthread_barrier_destroy(&b2);
    // TODO your code here, make sure to use thread_count!
    // Remember to ONLY crack passwords in other threads
    return 0; // DO NOT change the return code since AG uses it to check if your
              // program exited normally
}
