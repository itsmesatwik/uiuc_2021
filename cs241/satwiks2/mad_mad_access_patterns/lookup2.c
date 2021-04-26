/**
 * Mad Mad Access Patterns
 * CS 241 - Spring 2019
 * khirano2 satwiks2 mihikaa2 mbiju2 ssm5
 */
#include "tree.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

static off_t filesize;
static int fd;

void lookup(char * file, const char * target_word, uint32_t offset) {
    //out of bounds: word not found
    if (offset < BINTREE_ROOT_NODE_OFFSET || 
            offset > (uint32_t) filesize) {
        printNotFound(target_word);
        return;
    }

    BinaryTreeNode * cur = (BinaryTreeNode *) (file + offset);
    char * cur_word = (char *)cur + sizeof(BinaryTreeNode);

    //compare current word to target
    uint32_t next_off;
    int dif = strcmp(target_word, cur_word);
    if (dif == 0) {
        printFound(target_word, cur->count, cur->price);
    } else if (dif < 0) {
        next_off = cur->left_child;
        lookup(file, target_word, next_off);
    } else {
        next_off = cur->right_child;
        lookup(file, target_word, next_off);
    }

}

char * mmapDataFile(const char * filename) {
    //from the mmap section in the coursebook

    //open file to get file size
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        openFail(filename);
        exit(2);
    }

    struct stat filestats;
    fstat(fd, &filestats);
    filesize = filestats.st_size;

    char * datafile = mmap(NULL, (size_t) filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    return datafile;
}

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses mmap to access the data.
  ./lookup2 <data_file> <word> [<word> ...]
*/

int main(int argc, char **argv) {
    if (argc < 3) {
        printArgumentUsage();
        exit(1);
    }

    //open data file
    char * filename = argv[1];
    char * datafile = mmapDataFile(filename);

    //check for file mapping error
    if (datafile == MAP_FAILED) {
        mmapFail(filename);
        exit(2);
    }

    //check for file format error
    if(strncmp("BTRE", datafile, 4)) {
        formatFail(filename);
        munmap(datafile, filesize);
        close(fd);
        exit(2);
    }

    //search for each word
    int idx = 2;
    char * word = argv[idx];
    while (word) {
        lookup(datafile, word, BINTREE_ROOT_NODE_OFFSET);
        idx++;
        word = argv[idx];
    }

    //cleanup
    munmap(datafile, filesize);
    close(fd);
    return 0;
}