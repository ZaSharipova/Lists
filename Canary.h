#ifndef CANARY_H_
#define CANARY_H_

#include <stdint.h>
#include <time.h>

#include "Structs.h"

void InitCanaries(void);
ListErrors MakeCanary(List *list);

extern uint32_t canary_left;
extern uint32_t canary_right;

#endif //CANARY_H_