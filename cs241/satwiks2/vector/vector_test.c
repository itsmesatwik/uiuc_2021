/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "vector.h"
#include<stdio.h>
#include<stdlib.h>

void print_(vector* vec);
int main(int argc, char *argv[]) {
    //int arr[9] = {0,1,2,3,4,5,6,7,8};
    char arr2[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    vector *vec = char_vector_create();
    //printf("%lu\n", vector_size(vec));
    int i = 0;
    while (i != 9) {
        vector_push_back(vec, arr2 + i);
        i++;
    }
    //printf("%lu\n", vector_size(vec));
    print_(vec);
    vector_pop_back(vec);
    puts("---------------------------------\n");
    print_(vec);
    puts("---------------------------------\n");
    vector_pop_back(vec);
    print_(vec);
    puts("---------------------------------\n");
    vector_set(vec, 1, "6");
    print_(vec);
    puts("---------------------------------\n");
    vector_erase(vec, 1);
    print_(vec);
    puts("---------------------------------\n");
    vector_insert(vec, 1, "b");
    print_(vec);
    puts("---------------------------------\n");
    puts(*((char**)vector_at(vec, 1)));



    vector_clear(vec);
    print_(vec);
    puts("---------------------------------\n");
    //printf("%lu\n", vector_size(vec));
    //puts("what");
    //puts("wat");
    vector_destroy(vec);



    // Write your test cases here
    return 0;
}

void print_(vector* vec) {
    size_t i = 0;
    //puts("1");
    for (; i < vector_size(vec); i++) {
        printf("i: %zu   ", i);
        char* ye = (char*)vector_get(vec, i);
        //puts("2");
        printf("arr[i]: %c\n",*ye );
        //puts("3");
    }
}
