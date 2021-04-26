/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2019
 */
 
#include "camelCaser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
char *camel_case(const char *input_s) {
    if(strlen(input_s) == 0) {
        char *yolo = (char*)(malloc(sizeof(char)));
        yolo[0] = '\0';
        return yolo;
    }
    // Removing whitespaces
    //for(size_t i=0, j=0; input_s[j]=input_s[i]; j+=!isspace(input_s[i++]));
    size_t index = 0, i = 0;
    int firstchar = 0, nextcharup = 0;
    char* camelcased = (char*)malloc(sizeof(char)*(strlen(input_s) + 1));
    for(; index < strlen(input_s); index++) {
        if (!firstchar && isalnum(input_s[index])) {
            firstchar = 1;
            camelcased[i++] = tolower(input_s[index]);
            nextcharup = 0;
            continue;
        }
        if (nextcharup && isalpha(input_s[index])) {
            nextcharup = 0;
            camelcased[i++] = toupper(input_s[index]);
            continue;
        }
        if (isalnum(input_s[index])) {
            camelcased[i++] = tolower(input_s[index]);
            continue;
        }
        if (!isspace(input_s[index])) {
            camelcased[i++] = input_s[index];
        }
        else {
            nextcharup = 1;
        }
        /*if(isalnum(input_s[index])) {
            if (flag && isalpha(input_s[index])) {
                camelcased[i++] = toupper(input_s[index]);
                flag = 0;
            } 
            else {
                camelcased[i++] = tolower(input_s[index]);
            }
        } 
        else if (isspace(input_s[index]))
            flag = 1;
        else if (!isalnum(input_s[index])){
            flag = 1;
            camelcased[i++] = input_s[index];
        }*/
    }
    //free(input_s);
    camelcased[i] = '\0';
    return camelcased;
}


/**
 * Splits the given string into sentences tokenizing by ispunct()
 * as per the definition in the documentation
 */
char **sentence_split(const char *input_str, size_t num_sentences) {
    char **sentences = (char**)malloc((num_sentences + 1)*sizeof(char*));
    sentences[num_sentences] = NULL;
    size_t len = strlen(input_str);
    char buffer[len];
    size_t str_idx = 0, buffer_count = 0, sentence_count = 0;
    for(; str_idx < len; str_idx++) {

        if(ispunct(input_str[str_idx]) && num_sentences) {

            sentences[sentence_count] = (char*)malloc(sizeof(char)*(buffer_count + 1));
            buffer[buffer_count] = '\0';
            strcpy(sentences[sentence_count], buffer);
            sentence_count++;
            buffer_count = 0;

            //free(buffer);
            //buffer = (char*)malloc(sizeof(char)*sizeof(input_str));
            strcpy(buffer, "");
            num_sentences--;
            if (!num_sentences)
                break;
        }
        else {
            buffer[buffer_count++] = input_str[str_idx];
        }
    }

    //free(buffer);
    return sentences;
}


unsigned long number_sentences(const char *input_str) {
    // NUMBER OF SENTENCES
    size_t idx = 0, num_sentences = 0;
    for (; idx < strlen(input_str); idx++){
        if(ispunct(input_str[idx]))
            num_sentences++;
    }
    //printf("%zu", num_sentences);
    return num_sentences;
}


char **camel_caser(const char *input_str) {
    //puts("1");
    if (input_str == NULL)
        return NULL;
    //puts("2");
    size_t len = strlen(input_str);
    char** sentences = NULL;
    size_t num_sentences = number_sentences(input_str);
    char** camelcased_sentences = (char**)malloc(sizeof(char*)*(num_sentences + 1));
    if (!num_sentences || !len) {
        camelcased_sentences[0] = NULL;
        return camelcased_sentences;
    }

    //puts("3");
    sentences = sentence_split(input_str, num_sentences);
    int i = 0;
    //puts("4");
    while (sentences[i] != NULL) {
        camelcased_sentences[i] = camel_case(sentences[i]);
        i++;
    }
    //puts("5");
    destroy(sentences);
    camelcased_sentences[num_sentences] = NULL;

    //free(sentences);
    return camelcased_sentences;

    
}

void destroy(char **result) {
    if (result == NULL) {
        //puts("y");
        return;
    }
    char **temp =result;
    //int i = 0;
    while (*temp != NULL) {
        /*if (*temp == 0) {
            temp++;
            result = temp;
            continue;
        }*/
        //printf("%d\n", i++);
        //puts(*result);
        if (temp == NULL) {
            puts("yeet");
            break;
        }

        free(*temp);
        temp++;
    }
    free(result);
    return;
}
