#include "Test.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "ListFunctions.h"

#define DO_CHANGE(func, list, pos, value)                                                        \
    type_of_change = FuncNameToEnum(#func);                                                      \
    Info = {file, list, #list, __FILE__, pos, value, Info.graph_counter, kDump, type_of_change}; \
    DoAllDump(&Info);                                                                            \
                                                                                                 \
    CHECK_ERROR_RETURN(func(list, pos, value));                                                  \
                                                                                                 \
    Info.type_of_command_before = type_of_change, Info.type_of_command_after = kDump;            \
    DoAllDump(&Info);                                                                            \

ListErrors Test1(FILE *file, List *list) { //test all
    assert(file);
    assert(list);

    ListErrors err = kSuccess;
    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    DO_CHANGE(DeleteElement, list, 2, 0);
    DO_CHANGE(InsertElementBeforePosition, list, 4, 10);

    return kSuccess;
}

ListErrors Test2(FILE *file, List *list) { //test delete from tail
    assert(file);
    assert(list);

    ListErrors err = kSuccess;
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

    ListErrors err = kSuccess;
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

    ListErrors err = kSuccess;
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

    ListErrors err = kSuccess;
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

    ListErrors err = kSuccess;
    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 0, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 25);

    return kSuccess;
}