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

ListErrors DoLinerization(List *list);

void FillList(List *list, int pos);

void DoAllDump(ChangeOperationContext *Info);
ListCommands FuncNameToEnum(const char *func_name);

List_t GetListHeadPos(List *list);
List_t GetListTailPos(List *list);

ListErrors PushBack(List *list, List_t value);
ListErrors PushFront(List *list, List_t value);
ListErrors PopBack(List *list);
ListErrors PopFront(List *list);

#endif //LIST_FUNCTIONS_H_