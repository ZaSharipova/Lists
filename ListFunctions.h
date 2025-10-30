#ifndef LIST_FUNCTIONS_H_
#define LIST_FUNCTIONS_H_

#include <stdio.h>

#include "Structs.h"

#define CHECK_ERROR_RETURN(cond) \
    err = cond;                        \
    if (err != kSuccess) {              \
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

void DoAllDump(ChangeOperationContext *Info);
ListCommands FuncNameToEnum(const char *func_name);

List_t GetListHeadPos(List *list);
List_t GetListTailPos(List *list);

#endif //LIST_FUNCTIONS_H_