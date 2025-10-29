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
ListErrors ListDump(List *lists, unsigned int error);
ListErrors ListDtor(List *list);

ListErrors InsertElementAfterPosition(List *lists, int pos, List_t number_to_put);
ListErrors InsertElementBeforePosition(List *list, int pos, List_t value);
ListErrors DeleteElement(List *lists, int pos, List_t value);

ListErrors DoChangeFunc(FILE *file, ListErrors (*func)(List*, int, List_t), List *list, int pos, List_t value, 
    ListCommands type_of_change, const char *file_name, const char *list_name);

void FillList(List *list, int pos);

#endif //LIST_FUNCTIONS_H_