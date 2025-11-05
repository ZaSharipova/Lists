#include "Test.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Structs.h"
#include "ListFunctions.h"

#define INIT_INFO(list) \
    Info = {file, list, #list, __FILE__, 0, 0, 0, kSuccess, "\0", kDump, kDump};

ListErrors BeforeChange(ChangeOperationContext *Info, const char *type, int pos, List_t value) {
    assert(Info);
    assert(type);

    ListCommands type_of_change = FuncNameToEnum(type);
    Info->pos = pos;
    Info->number = value;
    Info->type_of_command_after = type_of_change;
    Info->type_of_command_before = kDump;
    DoAllDump(Info);  
    return kSuccess;   
}

ListErrors AfterChange(ChangeOperationContext *Info, const char *type) {
    assert(Info);
    assert(type);

    ListCommands type_of_change = FuncNameToEnum(type);
    Info->type_of_command_before = type_of_change;
    Info->type_of_command_after = kDump;
    DoAllDump(Info);
    Info->type_of_command_before = type_of_change;
    Info->type_of_command_after = type_of_change;
    return kSuccess;
}

#define COUNT_ARGS_IMPL(_1, _2, _3, N, ...) N

#define EXTRACT_ARGS_1(list) NULL, NULL
#define EXTRACT_ARGS_2(list, pos) pos, NULL
#define EXTRACT_ARGS_3(list, pos, value) pos, value

#define EXTRACT_ARGS(...) \
    COUNT_ARGS_IMPL(__VA_ARGS__, EXTRACT_ARGS_3, EXTRACT_ARGS_2, EXTRACT_ARGS_1, EXTRACT_ARGS_0)(__VA_ARGS__)

#define DO_CHANGE(func_name, ...)                               \
  do {                                                          \
    BeforeChange(&Info, #func_name, EXTRACT_ARGS(__VA_ARGS__)); \
    func_name(__VA_ARGS__);                                     \
    AfterChange(&Info, #func_name);                             \
  } while (false)

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

// ListErrors DoBadChange(ChangeOperationContext *Info, ListChanges type_of_arr_to_change, int pos, int value) {
//     assert(Info);

//     Info->pos = pos, Info->number = value;
//     Info->type_of_command_after = kDump, Info->type_of_command_before = kDump;
//     DoAllDump(Info);
//     char change_name[MAX_STRING_SIZE] = "";

//     switch (type_of_arr_to_change) {
//     case (kDataChange):
//         Info->list->data[pos] = value;
//         strcpy(change_name, "DATA");
//         break;
//     case (kNextChange):
//         Info->list->next[pos] = value;
//         strcpy(change_name, "NEXT");
//         break;
//     case (kPrevChange):
//         Info->list->prev[pos] = value;
//         strcpy(change_name, "PREV");
//         break;
//     }

//     snprintf(Info->message, sizeof(Info->message), "Do change %s in position %d with value %d", change_name, pos, value);
//     return kSuccess;
// }
ListErrors Test1(FILE *file, List *list) { //test all
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    //DO_CHANGE(PopBack(list));
    //list->next[0] = list->free;
    //list->next[2] = 700;
    //DoBadChange(&Info, kNextChange, 2, 700);
    //list->next[3] = 0;

    DO_CHANGE(DeleteElement, list, 2);
    DO_CHANGE(InsertElementBeforePosition, list, 4, 10);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 26);
    // CHECK_ERROR_RETURN(DoLinerization(list));
    // DoAllDump(&Info);

    return kSuccess;
}

ListErrors Test2(FILE *file, List *list) { //test delete from tail
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(DeleteElement, list, 3);
    return kSuccess;
}

ListErrors Test3(FILE *file, List *list) { //test delete from head
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(DeleteElement, list, 1);
    DO_CHANGE(InsertElementAfterPosition, list, 0, 25);
    return kSuccess;
}

ListErrors Test4(FILE *file, List *list) { //test insert before head
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 0, 25);

    return kSuccess;
}

ListErrors Test5(FILE *file, List *list) { //test insert before-after
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementBeforePosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 2, 25);
    DO_CHANGE(InsertElementBeforePosition, list, 2, 20);


    return kSuccess;
}

ListErrors Test6(FILE *file, List *list) { //test insert after tail 
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementBeforePosition, list, 1, 20);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 25);
    DO_CHANGE(InsertElementAfterPosition, list, 3, 25);

    return kSuccess;
}

ListErrors Test7(FILE *file, List *list) { //test popback
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 1, 20);
    DO_CHANGE(PopBack, list);

    return kSuccess;
}

ListErrors Test8(FILE *file, List *list) { //test pushback
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(PushBack, list, 1);

    return kSuccess;
}

ListErrors Test9(FILE *file, List *list) { //test pushfront
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(PushFront, list, 1);

    return kSuccess;
}

ListErrors Test10(FILE *file, List *list) { //test popfront
    assert(file);
    assert(list);

    ChangeOperationContext Info = {};
    INIT_INFO(list);

    DO_CHANGE(InsertElementAfterPosition, list, 0, 32);
    DO_CHANGE(InsertElementAfterPosition, list, 0, 20);
    DO_CHANGE(PopFront, list);

    return kSuccess;
}