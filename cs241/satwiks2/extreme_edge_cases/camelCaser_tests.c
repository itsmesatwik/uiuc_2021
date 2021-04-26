/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"
#include "camelCaser_tests.h"

void print_(char** arr){
    int i = 0;
    while (arr[i] != NULL) {
        puts(arr[i]);
        i++;
    }
    //puts("yolo\n");
}

int test_camelCaser(char **(*camelCaser)(const char *),
                    void (*destroy)(char **)) {


    // NULL TEST
    char** res = (*camelCaser)(NULL);
    if (res != 0)
        return 0;
    destroy(res);
    puts("Passed NULL");


    //print_(res) standard test;
    res = (*camelCaser)("abc.def.");
    if (strcmp(res[0], "abc") != 0 || strcmp(res[1], "def") != 0)
        return 0;
    //puts("q");
    destroy(res);
    puts("Passed abc def");


    //SPACES
    res = (*camelCaser)("   abc   def  . ");
    //print_(res);
    if (strcmp(res[0], "abcDef") != 0)
        return 0;
    destroy(res);
    puts("Passed abcDef spaces");
    
    res = (*camelCaser)("AbC DeF.");
    if (strcmp(res[0], "abcDef") != 0)
        return 0;
    destroy(res);
    puts("Passed Caps");


    res = (*camelCaser)("  abc  ");
    if (res[0] != NULL)
        return 0;
    destroy(res);
    puts("Passed no punct"); 

    char ** temp= (*camelCaser)("   ");
    if (temp[0] != NULL) {
        return 0;
    }
    destroy(temp);
    puts("Passed all spaces");

    temp = (*camelCaser)("");
    if (temp[0] != NULL) {
        return 0;
    }
    destroy(temp);
    puts("Passed empty");
   
    res = (*camelCaser)("\n\n\t77a77bc;de f;;WHAT");
    //print_(res);
    if ( strcmp(res[2], "") != 0 || strcmp(res[0], "77a77bc") != 0
            || strcmp(res[1], "deF") != 0 )
        return 0;
    //puts("die");
    destroy(res);
    puts("Passed empty punct");


    res = (*camelCaser)("Abc a.def.");
    if(res[0][3] != 'A' || res[0][0] != 'a')
        return 0;
    destroy(res);
    puts("Passed tolower and toupper");

    
    res = (*camelCaser)("Abc \bV.");
    //print_(res);
    if( res[0][3] != '\b')
        return 0;
    destroy(res);
    puts("Passed special char existence");

    res = (*camelCaser)("1234ABCDEF.1234A b c d f.");
    if (strcmp(res[0], "1234abcdef") != 0 || strcmp(res[1], "1234aBCDF") !=0)
        return 0;
    destroy(res);
    puts("Passed numeric and capitalization");

    //res = (*camelCaser)("abc.def.");

    
        res = (*camelCaser)(" ~ ! @       satwik\\$ is/blue%(>/?)       s\a s^&*_-+=\\   \t\b| DCU\bwat / .");
    if ((*res[0] + *res[1] + *res[2] + *res[4] + *res[7]
             + *res[8] + *res[9] + *res[10] + *res[11] != 0) ||
        (strcmp(res[3],"satwik") != 0)||
        (strcmp(res[5],"is") != 0) || 
        (strcmp(res[6],"blue") != 0))
        return 0;
    //print_(res);
    destroy(res);
    puts("Passed super robot monkey team hyperforce");


    res  = (*camelCaser)("satwik satwik 123 satwik SIngh 123 541 f; satwik satwik satwik t\\satwik sATwik sAtwIK%satwik satw12i12k 12satwIK, Satwik satwik; satwik; satwik satwik;1;123;4;5;6;7;8;;23;5;satwik satwik 4653;satwiksatwik \\ ! ;");
    //print_(res);
    if ( strcmp(res[0], "satwikSatwik123SatwikSingh123541F") != 0 ||
            strcmp(res[1], "satwikSatwikSatwikT") != 0 ||
            strcmp(res[2], "satwikSatwikSatwik")!=0 ||
            strcmp(res[3], "satwikSatw12i12k12Satwik")!=0 ||
            strcmp(res[4], "satwikSatwik")!=0 ||
            strcmp(res[5], "satwik")!=0 ||
            strcmp(res[7], "1") !=0 ||
            strcmp(res[8], "123") != 0 ||
            strcmp(res[17], "satwikSatwik4653")!= 0)
        return 0;
    destroy(res);

    puts("Passed stress");


   // res = (*camelCaser)(";;;;;;;;;;;;;;;;;;;;;;;;;");
    //print_(res);

    // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
    return 1;
}


