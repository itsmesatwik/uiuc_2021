/**
 * Teaching Threads Lab
 * CS 241 - Spring 2019
 * Ismail Dayan dayan2, Satwik Singh satwiks2
 */
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
#include "reduce.h"
#include "reducers.h"
 
/* You might need a struct for each task ... */
typedef struct task_data {
  size_t start_index;
  size_t end_index;
  reducer reducer;
  int *list;
  int *result;
} task_data;
 
/* You should create a start routine for your threads. */
void _reduce(task_data *data) {
  for (size_t i = data->start_index; i < data->end_index; i++) {
    *(data->result) = data->reducer(*data->result, data->list[i]);
  }
}
 
int par_reduce(int *list, size_t list_len, reducer reduce_func, int base_case,
               size_t num_threads) {
   
    if (num_threads > list_len) {
        num_threads = list_len;
    }
 
    size_t nthreads;
    if (num_threads > list_len) {
      nthreads = list_len;
    } else {
      nthreads = num_threads;
    }
 
    size_t slicing_size = list_len / num_threads;
    if (!slicing_size) slicing_size = 1;
 
    task_data data[num_threads];
    pthread_t threads[num_threads];
 
    int result = base_case;
    size_t start_index = 0;
    size_t end_index = 0;
    for (size_t i = 0; i < nthreads; i++) {
      if (i == num_threads - 1) {
        end_index = list_len;
      } else {
        end_index = start_index + slicing_size;
      }
 
      data[i].start_index = start_index;
      data[i].end_index = end_index;
      data[i].reducer = reduce_func;
      data[i].list = list;
      data[i].result = &result;
 
      pthread_create(&threads[i], NULL, (void*) &_reduce, &data[i]);
      // printf("retval: %d\n", retval);
      start_index = end_index;
    }
 
    for (size_t i = 0; i < nthreads; i++) {
      pthread_join(threads[i], NULL);
    }
 
    // printf("%d\n", result);
    return result;
}