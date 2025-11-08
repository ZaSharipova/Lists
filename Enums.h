#ifndef ENUMS_H_
#define ENUMS_H_

#include <stdio.h>

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

    kHasCycleNext     = 1 << 13,
    kHasCyclePrev     = 1 << 14,
    kHasCycleFree     = 1 << 15,

    kHasSmallCycleNext = 1 << 16,
    kHasSmallCyclePrev = 1 << 17,

    kInvalidNext      = 1 << 18,
    kInvalidPrev      = 1 << 19,

    kInvalidUnusedPos = 1 << 20,
    kWrongDirection   = 1 << 21,
    kFailure          = 1 << 22,
    kSuccess           =      0,
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
    kPopBack,
    kPopFront,
    kPushBack,
    kPushFront,
    kDumpBefore,
    kDump,
    kDumpErrors,
} ListCommands;

enum ListChanges {
    kDataChange,
    kNextChange,
    kPrevChange,
};

#endif //ENUMS_H_