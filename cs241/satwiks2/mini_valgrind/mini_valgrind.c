/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2019
 */
 
#include "mini_valgrind.h"
#include <stdio.h>
#include <string.h>

meta_data *head;
size_t total_memory_requested;
size_t total_memory_freed;
size_t invalid_addresses;

void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
    meta_data* mem = (meta_data*)malloc(sizeof(meta_data) + request_size);
    if (mem == NULL)
        return NULL;
    void* ret = (void*)mem + sizeof(meta_data);
    mem->request_size = request_size;
    mem->filename = filename;
    mem->instruction = instruction;

    mem->next = head;
    head = mem;
    total_memory_requested += request_size;
    

    // your code here
    return ret;
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
    void *ret = mini_malloc(num_elements*element_size, filename, instruction);
    if (ret == NULL)
        return NULL;
    ret = memset(ret, 0, element_size*num_elements);
    return ret;
    // your code here
}



void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
    if (payload == NULL)
        return mini_malloc(request_size, filename, instruction);
    meta_data *cmp = (meta_data*) (payload - sizeof(meta_data));
    if (request_size == 0) {  
        mini_free(payload);
        return NULL;
    }

    //assert(cmp);
    meta_data *iter1 = NULL;
    meta_data *iter2 = head;
    while (iter2 != NULL) {
        if (cmp == iter2) {
            if (iter1) {
                iter1->next = iter2->next;
            }
            else {
                head = iter2->next;
            }

            if (request_size < cmp->request_size)
                total_memory_freed += (cmp->request_size - request_size);
            else
                total_memory_requested += (request_size - cmp->request_size);

            
            cmp = realloc(cmp, request_size + sizeof(meta_data));
            if (cmp == NULL)
                return NULL;

            cmp->next = head;
            head = cmp;
            cmp->filename = filename;
            cmp->request_size = request_size;
            cmp->instruction = instruction;
            return (void*)((void*)cmp + sizeof(meta_data));
        }
        iter1 = iter2;
        iter2 = iter2->next;
    }
    //puts("bad call at realloc");
    invalid_addresses++;
    return NULL;

    // your code here
}

void mini_free(void *payload) {
    if (payload == NULL)
        return;
    //puts("free called");
    meta_data *cmp = (meta_data*) (payload - sizeof(meta_data));
    meta_data* iter1 = NULL;
    meta_data* iter2 = head;
    while(iter2 != NULL) {
        if (cmp == iter2) {
            total_memory_freed += (cmp->request_size);
            if (iter1)
                iter1->next = iter2->next;
            else
                head = iter2->next;

            free(cmp);
            return;
        }
        iter1 = iter2;
        iter2 = iter2->next;
    }
    //puts("bad call at free");
    invalid_addresses++;
    // your code here
}
