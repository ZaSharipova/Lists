#ifndef ENUMS_H_
#define ENUMS_H_

#include <stdio.h>

#define NUMBER_OF_ERRORS 15

typedef enum {
    kNegativeSize     = 1 << 0,
    kNullData         = 1 << 1,
    kNullNext         = 1 << 2,
    kNullPrev         = 1 << 3,
    kInvalidHead      = 1 << 4,
    kInvalidTail      = 1 << 5,
    kInvalidFree      = 1 << 6,
    kErrorWrongCanary = 1 << 7,

    kNoMemory         = 1 << 8,
    kInvalidParam     = 1 << 9,
    kErrorOpening     = 1 << 10,
    kErrorClosing     = 1 << 11,
    kInvalidPos       = 1 << 12,
    kFailure          = 1 << 13,
    kSuccess          =      0,
} ListErrors;

typedef enum {
    kIncrease,
    kDecrease,
    kIncreaseZero,
    kNoChange,
} Realloc_Mode;

typedef enum {
    kInsertBefore,
    kInsertAfter,
    kDelete,
    kDump,
} ListCommands;

#endif //ENUMS_H_