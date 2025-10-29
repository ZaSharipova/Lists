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

// ListErrors DoChangeFunc(FILE *file, ListErrors (*func)(List*, int, List_t), List *list, int pos, List_t value, 
//     ListCommands type_of_change, const char *file_name, const char *list_name) {
//     assert(func);
//     assert(list);
//     assert(file_name);
//     assert(list_name);

//     static int graph_counter = 0;
    
//     ChangeOperationContext Info = {file, list, list_name, file_name, pos, value, kDump, type_of_change};
//     DoAllDump(&Info, &graph_counter);

//     ListErrors err = kSuccess;
//     CHECK_ERROR_RETURN(func(list, pos, value));

//     Info.type_of_command_before = type_of_change, Info.type_of_command_after = kDump;   
//     DoAllDump(&Info, &graph_counter);

//     return kSuccess;
// }

ListErrors Test1(FILE *file, List *list) {
    assert(file);
    assert(list);

    ListErrors err = kSuccess;
    ListCommands type_of_change = kDump;

    ChangeOperationContext Info = {};

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    //printf("%d ", Info.graph_counter);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    //printf("%d ", Info.graph_counter);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    //printf("%d ", Info.graph_counter);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    //    printf("%d ", Info.graph_counter);
    DO_CHANGE(DeleteElement, list, 2, 25);
    //    printf("%d ", Info.graph_counter);
    DO_CHANGE(InsertElementBeforePosition, list, 4, 10);
     //   printf("%d ", Info.graph_counter);

    return kSuccess;
}

// ListErrors Test2(FILE *file, List *list) {
//     assert(file);
//     assert(list);

//     ListErrors err = kSuccess;

//     DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 1, 20, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 3, 26, kInsertAfter);
//     return kSuccess;
// }

// ListErrors Test3(FILE *file, List *list) {
//     assert(file);
//     assert(list);

//     ListErrors err = kSuccess;

//     DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 1, 20, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);
//     DOCHANGE(file, DeleteElement, *list, 2, 0, kDelete);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 3, 26, kInsertAfter);

//     return kSuccess;
// }

// ListErrors Test4(FILE *file, List *list) {
//     assert(file);
//     assert(list);

//     ListErrors err = kSuccess;

//     DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
//     DOCHANGE(file, InsertElementBeforePosition, *list, 1, 20, kInsertBefore);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);

//     return kSuccess;
// }

// ListErrors Test5(FILE *file, List *list) {
//     assert(file);
//     assert(list);

//     ListErrors err = kSuccess;

//     DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 0, 20, kInsertAfter);
//     DOCHANGE(file, InsertElementAfterPosition, *list, 1, 25, kInsertAfter);
//     DOCHANGE(file, DeleteElement, *list, 3, 0, kDelete);

//     return kSuccess;
// }