/**
 * Mad Mad Access Patterns
 * CS 241 - Spring 2019
 * khirano2 satwiks2 mihikaa2 mbiju2 ssm5
 */
#include "tree.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void lookup(FILE * file, long eof_pos, char * target_word, uint32_t offset) {
    if (offset < BINTREE_ROOT_NODE_OFFSET ||
            offset > (uint32_t) eof_pos) {
        printNotFound(target_word);
        return;
    }

    //set the tree node
    BinaryTreeNode * cur = malloc(sizeof(BinaryTreeNode));
    int fseek_err = fseek(file, offset, SEEK_SET);
    if (fseek_err) {
        //formatFail(file);
        free(cur);
        return;
    }
    fread(cur, sizeof(BinaryTreeNode), 1, file);

    //set file offset to offset where the word is stored
    fseek_err = fseek(file, sizeof(BinaryTreeNode) + offset, SEEK_SET);
    if (fseek_err) {
        free(cur);
        return;
    }


    //set the current word
    char * cur_word = malloc(77);
    int idx = 0;
    char cur_ch = fgetc(file);
    while (cur_ch != EOF && cur_ch != '\0') {
        cur_word[idx] = cur_ch;
        cur_ch = fgetc(file);
        idx++;
    }
    cur_word[idx] = '\0';

    //compare current word to target
    uint32_t next_off;
    int dif = strcmp(target_word, cur_word);
    if (dif == 0) {
        printFound(target_word, cur->count, cur->price);
    } else if (dif < 0) {
        next_off = cur->left_child;
        lookup(file, eof_pos, target_word, next_off);
    } else {
        next_off = cur->right_child;
        lookup(file, eof_pos, target_word, next_off);
    }

    free(cur_word);
    free(cur);
}
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.
  ./lookup1 <data_file> <word> [<word> ...]
*/

int main(int argc, char **argv) {
    if (argc < 3) {
        printArgumentUsage();
        exit(1);
    }

    //open data_file
    char * filename = argv[1];
    char * mode = "r";
    FILE * datafile = fopen(filename, mode);
    if (datafile == NULL) {
        openFail(filename);
        exit(2);
    }

    //check if data file is valid
    char start[5];
    fread(start, 1, 4, datafile);
    start[4] = '\0';

    if(strcmp(start, "BTRE")) {
        formatFail(filename);
        fclose(datafile);
        exit(2);
    }

    //find position of end of file
    fseek(datafile, 0, SEEK_END);
    long eof_pos = ftell(datafile);

    //look for words
    int idx = 2;
    char * word = argv[idx];
    while(word) {
        lookup(datafile, eof_pos, word, BINTREE_ROOT_NODE_OFFSET);
        idx++;
        word = argv[idx];
    }

    fclose(datafile);
    return 0;
}