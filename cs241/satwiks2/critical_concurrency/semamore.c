/**
 * Critical Concurrency Lab
 * CS 241 - Spring 2019
 */
 

#include "semamore.h"

/**
 * Initializes the Semamore. Important: the struct is assumed to have been
 * allocated by the user.
 * Example:
 * 	Semamore *s = malloc(sizeof(Semamore));
 * 	semm_init(s, 5, 10);
 *
 */
void semm_init(Semamore *new, int value, int max_val) {
    /* Your code here */
    new->value = value;
    new->max_val = max_val;
    new->m = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_init(&(new->cv), NULL);
}

/**
 *  Should block when the value in the Semamore struct (See semamore.h) is at 0.
 *  Otherwise, should decrement the value.
 */
void semm_wait(Semamore *s) {
    pthread_mutex_lock(&(s->m));
    while(s->value == 0)
        pthread_cond_wait(&(s->cv), &(s->m));
    s->value--;
    pthread_mutex_unlock(&(s->m));
    pthread_cond_broadcast(&s->cv);
    /* Your code here */
}

/**
 *  Should block when the value in the Semamore struct (See semamore.h) is at
 * max_value.
 *  Otherwise, should increment the value.
 */
void semm_post(Semamore *s) {
    pthread_mutex_lock(&(s->m));
    while (s->value == s->max_val)
        pthread_cond_wait(&(s->cv), &(s->m));
    s->value++;
    pthread_mutex_unlock(&(s->m));
    pthread_cond_broadcast(&s->cv);
    /* Your code here */
}

/**
 * Takes a pointer to a Semamore struct to help cleanup some members of the
 * struct.
 * The actual Semamore struct must be freed by the user.
 */
void semm_destroy(Semamore *s) {
    pthread_mutex_destroy(s->m);
    pthread_cond_destroy(s->cv);

    /* Your code here */
}
