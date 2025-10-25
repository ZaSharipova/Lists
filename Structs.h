#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>

struct List {
    int size;
    int *data;
    int *next;
    int *prev;
    int head;
    int tail;
    int free;
    int number_of_elem;
};


typedef enum {
    kSuccess,
    kNoMemory,
    kNullData,
    kNullNext,
    kNullPrev,
} ListErrors;

#endif //STRUCTS_H_