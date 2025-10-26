#ifndef LIST_FUNCTIONS_H_
#define LIST_FUNCTIONS_H_

#include <stdio.h>

#include "Structs.h"

#define CHECK_ERROR_RETURN(cond) \
    if (cond < 0) {              \
        return err;              \
    }


ListErrors ListCtor(List *lists);
ListErrors ListVerify(List *lists);
ListErrors ListDump(List *lists);
ListErrors ListDtor(List *list);

ListErrors ResizeList(List *list, Realloc_Mode realloc_type);
ListErrors InsertElement(List *lists, int pos, List_t number_to_put);
ListErrors DeleteElement(List *lists, int pos);
#endif //LIST_FUNCTIONS_H_