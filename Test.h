#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>

#include "Structs.h"

#define DOCHANGE(file, func, list, pos, value, type_of_command)                          \
    err = DoChangeFunc(file, func, &list, pos, value, type_of_command, __FILE__, #list); \
    if (err != 0) {                                                                      \
        return err;                                                                      \
    }

ListErrors Test1(FILE *file, List *list); 
ListErrors Test2(FILE *file, List *list); 
ListErrors Test3(FILE *file, List *list); 
ListErrors Test4(FILE *file, List *list); 
ListErrors Test5(FILE *file, List *list); 

#endif //TEST_H_