#include "Canary.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "Structs.h"

static uint8_t RandomNonZeroByte() {
    uint8_t b = 0;
    while (b == 0) {
        b = (uint8_t)(rand() & 0xFF);
    }
    return b;
}

static uint32_t InitCanary(void) {
    srand((unsigned int)time(NULL));
    
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result <<= 8;
        result |= RandomNonZeroByte();
    }
    
    return result;
}

uint32_t canary_left = 0;
uint32_t canary_right = 0;

void InitCanaries(void) {
    srand((unsigned int)time(NULL));
    canary_left  = InitCanary();
    canary_right = InitCanary();
}

ListErrors MakeCanary(List *list) {
    assert(list);

#ifdef _DEBUG
    list->data[0] = (List_t)canary_left;
    list->data[list->size - 1] = (List_t)canary_right;
#endif

    return kSuccess;
}