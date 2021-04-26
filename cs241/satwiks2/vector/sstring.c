/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "sstring.h"
#include "vector.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <string.h>

struct sstring {
    vector* vec;
    // Anything you want
};

sstring *cstr_to_sstring(const char *input) {
    sstring *ret = (sstring*)malloc(sizeof(sstring));
    //ret->vec = (vector*)malloc(sizeof(vector));
    ret->vec = char_vector_create();
    size_t index = 0;
    char *t = (char*)malloc(sizeof(char));
    for(; index < strlen(input); index++) {
        *t = input[index];
        vector_push_back(ret->vec, t);
    }
    free(t);
    // your code goes here
    return ret;
}

char *sstring_to_cstr(sstring *input) {
    // your code goes here
    char *ret = (char*)calloc(vector_size(input->vec)+1, sizeof(char));
    size_t index = 0;
    for(; index < vector_size(input->vec); index++) {
        ret[index] = *((char*)vector_get(input->vec, index));
    }
    return ret;
}

int sstring_append(sstring *this, sstring *addition) {
    size_t i = 0;
    for (; i < vector_size(addition->vec); i++) {
        char *t = vector_get(addition->vec, i);
        vector_push_back(this->vec, t);
        //free(t);
    }
    // your code goes here
    return vector_size(this->vec);
}

vector *sstring_split(sstring *this, char delimiter) {
    vector *split = string_vector_create();
    size_t i = 0, buffer_count = 0;
    char* buffer = (char*)calloc(vector_size(this->vec) + 1, sizeof(char));
    for(; i < vector_size(this->vec); i++) {
        if (*((char*)vector_get(this->vec, i)) == delimiter) {
            buffer[buffer_count + 1] = '\0';
            vector_push_back(split, buffer);
            //strcpy(buffer,"");
            free(buffer);
            buffer = (char*)calloc(vector_size(this->vec) + 1, sizeof(char));
            buffer_count =0;
            continue;
        }
        buffer[buffer_count++] = *((char*)vector_get(this->vec, i));
    }
    vector_push_back(split, buffer);
    free(buffer);
    // your code goes here
    return split;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {

    size_t len = strlen(target);
    for(; offset < vector_size(this->vec) - len; offset++) {
        if(*((char*)vector_get(this->vec, offset)) == target[0]) {
            //printf("matched first\n");
            char* slice = sstring_slice(this, offset, (offset + len));
            //printf("%s | %s\n", target, slice);
            if (strcmp(slice, target) == 0) {
                //printf("matched rest\n");
                size_t i = len;
                int j = strlen(substitution) - 1;
                while(i--) {
                    vector_erase(this->vec, offset);
                }
                while(j >= 0) {
                    vector_insert(this->vec, offset, substitution + j);
                    j--;
                }
                free(slice);
                return 0;
            }
            free(slice);
        }
    }
    // your code goes here
    return -1;
}

char *sstring_slice(sstring *this, int start, int end) {
    size_t i = start;// end - start + 1;
    char* slice = (char*)calloc((end-start+1), sizeof(char));
    for (; i < (size_t)end; i++) {
        slice[i - start] = *((char*)vector_get(this->vec, i));
    }
    // your code goes here
    return slice;
}

void sstring_destroy(sstring *this) {
    //puts("here");
    vector_destroy(this->vec);
    //this->vec = NULL;
    free(this);
    //this = NULL;
    // your code goes here
}
