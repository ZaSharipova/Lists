#include "Test.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "ListFunctions.h"

#define DO_CHANGE(func, list, pos, value)                                                                  \
    type_of_change = FuncNameToEnum(#func);                                                                \
    Info = {file, list, #list, __FILE__, pos, value, Info.graph_counter, kSuccess, kDump, type_of_change}; \
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

ListErrors Test1(FILE *file, List *list) { //test all
    assert(file);
    assert(list);

    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    list->next[0] = list->free;
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