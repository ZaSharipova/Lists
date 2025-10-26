#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>

typedef  double List_t;

struct List {
    int size;
    List_t *data;
    int *next;
    int *prev;
    int head;
    int tail;
    int free;
    int number_of_elem;
};

typedef enum {
    kSuccess = 0,
    kNoMemory = -1,
    kNullData = -2,
    kNullNext = -3,
    kNullPrev = -4,
    kInvalidParam = -5,
    kNegativeHead = -6,
    kNegativeTail = -7,
    kNegativeFree = -8,
    kZeroSize = -9,
    kFailure = -10,
} ListErrors;

typedef enum {
    kIncrease,
    kDecrease,
    kIncreaseZero,
    kNoChange,
} Realloc_Mode;

#endif //STRUCTS_H_