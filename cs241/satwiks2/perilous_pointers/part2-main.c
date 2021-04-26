/**
 * Perilous Pointers Lab
 * CS 241 - Spring 2019
 */
 
#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
    // your code here
    first_step(81); // FIRST
    int *second = (int*)malloc(sizeof(int));;
    *second = 132;
    second_step(second); // SECOND
    //free(second);
    int** third = (int**)malloc(sizeof(int*));
    third[0] = second;
    *second = 8942;
    double_step(third); // THIRD
    free (third);
    free (second);

    char* strange = (char*)malloc(sizeof(char)*5 + sizeof(int));
    *(int*)(strange + 5) = 15; 
    strange_step(strange);// FOURTH
    free(strange);

    void* empty = (void*)malloc(sizeof(char)*4);
    ((char*)empty)[3] = 0;
    empty_step(empty); // FIFTH
    free(empty);

    void *two_st;
    char* tw = (char*)malloc(4*sizeof(char));
    tw[3] = 'u';
    two_st = tw;
    two_step(two_st, tw); // SIXTH
    free (two_st); //free(tw);

    char *three1 = "abcdef", *three2, *three3;
    three2 = three1+2;
    three3 = three2 + 2;
    three_step(three1, three2, three3); // SEVENTH

    char first[] = {0,0,0,0};
    char second_[] = {8,8,8,8};
    char third_[] = {16,16,16,16};
    step_step_step(first, second_, third_); // EIGHTH

    char *odd_ = "A";
    int odd2_ = 65;
    it_may_be_odd(odd_, odd2_); // NINTH

    char tok[10];
    strcpy(tok, "A,CS241,");
    tok_step (tok); // TENTH

    void* orange = (void*)malloc(sizeof(int));
   *((int*)orange) = 0x00000201;
    void* blue = orange;
    ((char*) blue)[0] = 1;
    the_end(orange, blue);
    free(orange);
    return 0;
}
