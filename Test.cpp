#include "Test.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Structs.h"
#include "ListFunctions.h"

#define DO_CHANGE(func, list, pos, value)                                                                  \
    type_of_change = FuncNameToEnum(#func);                                                                \
    Info = {file, list, #list, __FILE__, pos, value, Info.graph_counter, kSuccess, "\0", kDump, type_of_change}; \
    DoAllDump(&Info);                                                                                      \
                                                                                                           \
    Info.error = func(list, pos, value);                                                                   \
    if (Info.error & ~kSuccess) {                                                                          \
        Info.type_of_command_after = kDump;                                                                \
        DoAllDump(&Info);                                                                                  \
        return Info.error;                                                                                 \
    }                                                                                                      \
                                                                                                           \
    Info.type_of_command_before = type_of_change, Info.type_of_command_after = kDump;                      \
    DoAllDump(&Info);                                                                                      \
    Info.type_of_command_before = type_of_change, Info.type_of_command_after = type_of_change;             \


// #define DO_BAD_CHANGE(type_of_arr_to_change, pos, value)       \

//     Info.pos = pos, Info.value = value; \
//     DoAllDump(&Info);                   \
//     switch (type_of_arr_to_change) { \
//     case (kDataChange): \
//         list.data[pos] = value; \
//         snprintf(Info.message, sizeof(Info.message), "Do change Data in position %d with value %d", pos, value); \
//     case (kNextChange): \
//         list.next[pos] = value; \
//     case (kPrevChange): \
//         list->prev[pos] = value; \
//     snprintf(Info.message, sizeof(Info.message), "Do change ")
//     }

ListErrors DoBadChange(ChangeOperationContext *Info, ListChanges type_of_arr_to_change, int pos, int value) {
    assert(Info);

    Info->pos = pos, Info->number = value;
    Info->type_of_command_after = kDump, Info->type_of_command_before = kDump;
    DoAllDump(Info);
    char change_name[MAX_STRING_SIZE] = "";

    switch (type_of_arr_to_change) {
    case (kDataChange):
        Info->list->data[pos] = value;
        strcpy(change_name, "DATA");
        break;
    case (kNextChange):
        Info->list->next[pos] = value;
        strcpy(change_name, "NEXT");
        break;
    case (kPrevChange):
        Info->list->prev[pos] = value;
        strcpy(change_name, "PREV");
        break;
    }

    snprintf(Info->message, sizeof(Info->message), "Do change %s in position %d with value %d", change_name, pos, value);
    return kSuccess;
}
ListErrors Test1(FILE *file, List *list) { //test all
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};
     

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    //list->next[0] = list->free;
    //list->next[2] = 700;
    DoBadChange(&Info, kNextChange, 2, 700);
    //list->next[3] = 0;

    DO_CHANGE(DeleteElement, list, 2, 0);
    DO_CHANGE(InsertElementBeforePosition, list, 4, 10);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    // CHECK_ERROR_RETURN(DoLinerization(list));
    // DoAllDump(&Info);

    return kSuccess;
}

ListErrors Test2(FILE *file, List *list) { //test delete from tail
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(DeleteElement, list, 3, 0);
    return kSuccess;
}

ListErrors Test3(FILE *file, List *list) { //test delete from head
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(DeleteElement, list, 1, 0);
    DO_CHANGE(InsertElementAfterPosition, list, 0, 25);
    return kSuccess;
}

ListErrors Test4(FILE *file, List *list) { //test insert before head
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 0, 25);

    return kSuccess;
}

ListErrors Test5(FILE *file, List *list) { //test insert before-after
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementBeforePosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(InsertElementBeforePosition, list, 2, 20);


    return kSuccess;
}

ListErrors Test6(FILE *file, List *list) { //test insert after tail 
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementBeforePosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 25);

    return kSuccess;
}