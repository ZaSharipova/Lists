#ifndef LIST_FUNCTIONS_H_
#define LIST_FUNCTIONS_H_

#include <stdio.h>

#include "Structs.h"

ListErrors ListCtor(List *lists);
ListErrors ListVerify(List *lists);
ListErrors ListDump(List *lists);

ListErrors InsertList(List *lists, int pos, int number_to_put);
ListErrors RemoveFromList(List *lists, int prev_pos);
#endif //LIST_FUNCTIONS_H_