/**
 * Critical Concurrency Lab
 * CS 241 - Spring 2019
 */
 
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * This queue is implemented with a linked list of queue_nodes.
 */
typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

struct queue {
    /* queue_node pointers to the head and tail of the queue */
    queue_node *head, *tail;

    /* The number of elements in the queue */
    ssize_t size;

    /**
     * The maximum number of elements the queue can hold.
     * max_size is non-positive if the queue does not have a max size.
     */
    ssize_t max_size;

    /* Mutex and Condition Variable for thread-safety */
    pthread_cond_t cv;
    pthread_mutex_t m;
};

queue *queue_create(ssize_t max_size) {
    queue *new = malloc(sizeof(queue));
    new->head = new->tail = NULL;
    new->size = 0;
    new->max_size = max_size;
    pthread_mutex_init(&new->m, NULL);
    pthread_cond_init(&new->cv, NULL);
    /* Your code here */
    return new;
}

void queue_destroy(queue *this) {
    while (head != tail) {
        queue_node *temp = head;
        head = head->next;
        free(temp);
    }
    pthread_mutex_destroy(&this->m);
    pthread_cond_destroy(&this->cv);
    /* Your code here */
}

void queue_push(queue *this, void *data) {
    if (this->max_size > 0) {
        pthread_mutex_lock(&this->m);
        while(this->size == this->max_size)
            pthread_cond_wait(&this->cv, &this->m);

        queue_node *new = malloc(sizeof(queue_node));
        new->data = data;
        new->next = NULL;
        if (head == NULL) {
            head = new;
            tail = new;
        }
        else{
            this->tail->next = new;
            this->tail = new;
        }
        this->size++;
        pthread_cond_broadcast(&this->cv);
        pthread_mutex_unlock(&this->m);
    }
    else {
        queue_node *new = malloc(sizeof(queue_node));
        new->data = data;
        new->next = NULL;
        if (head == NULL) {
            head = new;
            tail = new;
        }
        else{
            this->tail->next = new;
            this->tail = new;
        }
        this->size++;
    }
    /* Your code here */
}

void *queue_pull(queue *this) {
    pthread_mutex_lock(&this->m);
    while (this->size == 0)
        pthread_cond_wait(&this->cv, &this->m);
    if (head != NULL) {
        queue_node *ret = head;
        void* data = ret->data;
        free(ret);
        if (head == tail) {
            head = NULL;
            tail = NULL;
        }
        else {
            head = head->next;
        }
        this->size--;
        pthread_cond_broadcast(&this->cv);
        pthread_mutex_unlock(&this->m);

    /* Your code here */
    return data;
}
