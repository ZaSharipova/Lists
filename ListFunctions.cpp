#include "ListFunctions.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Structs.h"
#include "DoDump.h"
#include "DoGraph.h"
#include "Canary.h"
#include "Subsidiary.h"
#include "FileOperations.h"
#include "SetLogFile.h"

#define INCREASE_VALUE 4

#define DEFAULT_POS -20
#define DEFAULT_STROKE_SIZE 64
#define DEFAULT_LIST_SIZE 4

static void DoSnprintf(ChangeOperationContext *Info);
static ListErrors DoCallocCtor(List *list);
static Realloc_Mode CheckSize(List *list);
static ListErrors ResizeList(List *list, Realloc_Mode realloc_type);
static ListErrors DoRealloc(List *list);

ListErrors ListCtor(List *list) {
    assert(list);

    list->size = DEFAULT_LIST_SIZE;

    DoCallocCtor(list);

    list->data[0] = POISON;
    list->prev[0] = 0;
    list->next[0] = 0;

    FillList(list, 0);
    list->next[1] = 2;
    
#ifdef _DEBUG
    InitCanaries();
    MakeCanary(list);
#endif

    list->free = 1;
    list->number_of_elem = 0;

    return kSuccess;
}

ListErrors ListVerify(List *list) {
    assert(list);

    unsigned int error = 0;

    if (list->size < 0) {
        error |= kNegativeSize;
        return kNegativeSize;
    }

    if (list->data == NULL) {
        error |= kNullData;
        return kNullData;
    }

    if (list->next == NULL) {
        error |= kNullNext;
        return kNullNext;
    }

    if (list->prev == NULL) {
        error |= kNullPrev;
        return kNullPrev;
    }
    
    if (list->next && (GetListHeadPos(list) < 0 || GetListHeadPos(list) >= list->size)) {
        error |= kInvalidHead;
    }

    if (list->prev && (GetListTailPos(list) < 0 || GetListTailPos(list) >= list->size)) {
        error |= kInvalidTail;
    }

    if (list->free < 0 || list->free >= list->size) {
        error |= kInvalidFree;
    }

    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == POISON && list->prev[i] != -1 && i != list->size - 1) {
            printf("sdfgh");
            error |= kInvalidUnusedPos;
            return (ListErrors)error;
        }
    }

    for (int i = 0; i < list->size; i++) {
        int next = list->next[i];
        if ((next >= 0 && next <= list->size - 1 && list->data[i] != POISON && list->data[next] != POISON) && list->prev[next] != i && i != list->size - 1) {
            printf("%d ", i);
            error |= kWrongDirection;
            return kWrongDirection;
        }
    }
#ifdef _DEBUG
    if (list->data[0] != (List_t)canary_left || list->data[list->size - 1] != (List_t)canary_right) {
        error |= kErrorWrongCanary;
    }
#endif

    for (int i = 0; i < list->size; i++) {
        if (abs(list->next[i]) > list->size || (list->data[list->next[i]] == POISON && list->data[i] != POISON)) {
            error |= kInvalidNext;
            fprintf(stderr, "%lu ", error);
            return (ListErrors)error;
        }

        if (abs(list->prev[i]) > list->size) {
            error |= kInvalidPrev;
            return (ListErrors)error;
        }
    }

    int cnt = 0;
    for (int i = list->free; list->next[i] != 0 && list->next[i] != list->size; i = list->next[i], ++cnt) {
        if (cnt > list->size - list->number_of_elem) {
            error |= kHasCycleFree;
        }
    }

    cnt = 0;
    for (int i = GetListHeadPos(list); list->next[i] != 0; i = list->next[i], ++cnt) {
        if (cnt > list->number_of_elem) {
            error |= kHasCycleNext;
        }
    }
    if (cnt < list->number_of_elem - 1) {
        error |= kHasSmallCycleNext;
    }

    cnt = 0;
    for (int i = GetListTailPos(list); list->prev[i] != 0; i = list->prev[i], ++cnt) {
        if (cnt > list->number_of_elem) {
            error |= kHasCyclePrev;
        }
    }
    if (cnt < list->number_of_elem - 1) {
        error |= kHasSmallCyclePrev;
    }

    // if (error != 0) {
    //     ListDump(list, error);
    // }

    return (ListErrors)error;
}

List_t GetListHeadPos(List *list) {
    assert(list);

    return list->next[0];
}

List_t GetListTailPos(List *list) {
    assert(list);

    return list->prev[0];
}

static Realloc_Mode CheckSize(List *list) {
    assert(list);

    if (list->number_of_elem + INCREASE_VALUE >= list->size) {
        return kIncrease;
    }

    else if (list->size == 0) {
        return kIncreaseZero;
    }

    return kNoChange;
}

static ListErrors ResizeList(List *list, Realloc_Mode realloc_type) {
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    int size_to_remember = list->size;

    if (realloc_type == kIncrease) {
        list->size += INCREASE_VALUE;

    } else if (realloc_type == kIncreaseZero) {
        list->size = 1;
    }

    CHECK_ERROR_RETURN(DoRealloc(list));

    FillList(list, size_to_remember - 2);
#ifdef _DEBUG
    MakeCanary(list);
#endif

    CHECK_ERROR_RETURN(ListVerify(list));
    return err;
}

ListErrors InsertElementAfterPosition(List *list, int pos, List_t value) {
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    Realloc_Mode realloc_type = CheckSize(list);
     if (realloc_type != kNoChange) {
        CHECK_ERROR_RETURN(ResizeList(list, realloc_type));
    }

    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "Wrong pos entered.\n");
        return kInvalidParam;
    }

    int new_index = list->free;
    list->free = list->next[new_index];

    list->data[new_index] = value;

    list->prev[new_index] = pos;
    list->prev[list->next[pos]] = new_index;
    list->next[new_index] = list->next[pos];
    list->next[pos] = new_index;

    list->number_of_elem++;
    CHECK_ERROR_RETURN(ListVerify(list));
    return kSuccess;
}

ListErrors InsertElementBeforePosition(List *list, int pos, List_t value) {
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    Realloc_Mode realloc_type = CheckSize(list);
    if (realloc_type != kNoChange) {
        CHECK_ERROR_RETURN(ResizeList(list, realloc_type));
    }

    if (pos < 1 || pos >= list->size || list->data[pos] == POISON) {
        fprintf(stderr, "Pos = 1 is the first one, so pos < 1 is invalid.\n");
        return kInvalidPos;
    }

    int new_index = list->free;
    list->free = list->next[new_index];

    list->data[new_index] = value;

    list->next[new_index] = pos;
    list->prev[new_index] = list->prev[pos];
    list->next[list->prev[pos]] = new_index;
    list->prev[pos] = new_index;

    list->number_of_elem++;
    list->next[2] = 1000;
    CHECK_ERROR_RETURN(ListVerify(list));
    return kSuccess;
}

ListErrors DeleteElement(List *list, int pos, List_t value) { //value unused
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    Realloc_Mode realloc_type = CheckSize(list);
    if (realloc_type != kNoChange) {
        CHECK_ERROR_RETURN(ResizeList(list, realloc_type));
    }

    if (pos <= 0 || list->size == 0 || pos >= list->size) {
        fprintf(stderr, "Pos <= 0 or size == 0, so DELETE is impossible.\n");
        return kInvalidPos;
    }

    list->next[list->prev[pos]] = list->next[pos];
    list->prev[list->next[pos]] = list->prev[pos];

    list->data[pos] = POISON;
    list->next[pos] = list->free;
    list->prev[pos] = -1;
    list->free = pos;

    list->number_of_elem--;

    CHECK_ERROR_RETURN(ListVerify(list));
    return kSuccess;
}

ListErrors ListDtor(List *list) {
    assert(list);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->size = 0;
    list->free = 0;
    list->number_of_elem = 0;

    return kSuccess;
}

void FillList(List *list, int pos) {
    assert(list);

    for (int i = pos + 1; i < list->size; i++) {
        list->data[i] = POISON;
        list->next[i] = i + 1;
        list->prev[i] = -1;
    }
    list->next[list->size - 1] = 0;
}

ListErrors ListDump(List *list, unsigned int error) {
    assert(list);

    unsigned int bit = 1;
    fprintf(GetLogFile(), "<h2> <font color=\"red\"> DUMP Listing Error</h2> </font>  \n");
    fprintf(GetLogFile(), "<h3> errors: ");
    for (unsigned long long i = 0; i < NUMBER_OF_ERRORS; i++) {
        if (error & bit) {
            fprintf(GetLogFile(), "%s ", ListErrorString[i]);
        }
        bit <<= 1;
    }

    fprintf(GetLogFile(), "</h3>\n");

    // ChangeOperationContext  Info = {GetLogFile(), list, "list", __FILE__, -1, -1, Info.graph_counter, kDump, kDump};
    // DumpListToGraphviz(&Info);
    // DoSnprintf(&Info);
    // DoDump(&Info);

    printf("size: %d\n\n", list->size);

    for (size_t i = 0; i < (size_t)list->size; i++) {
        printf("%lu ", i);
    }
    printf("\n");

    if (list->data){
        printf("===============Data===============:\n");
        for (size_t i = 0; i < (size_t)list->size; i++) {
            printf(YELLOW " " LIST_SPEC" " RESET, list->data[i]);
        }
        printf("\n\n");
    }

    if (list->next) {
        printf("head: %d\n", list->next[0]);
        printf("===============Next===============:\n");
        for (size_t i = 0; i < (size_t)list->size; i++) {
            printf(YELLOW "%d " RESET, list->next[i]);
        }
        printf("\n\n");
    }

    if (list->prev) {
        printf("tail: %d\n", list->prev[0]);
        printf("===============Prev===============:\n");
        for (size_t i = 0; i < (size_t)list->size; i++) {
            printf(YELLOW "%d " RESET, list->prev[i]);
        }
        printf("\n\n");
    }

    printf("free: %d\n", list->free);
    printf("------------------------------------\n");

    printf("\n\n\n\n");
    return (ListErrors)error;
}

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

static ListErrors DoCallocCtor(List *list) {
    assert(list);

    list->data = (List_t *) calloc ((size_t)list->size, sizeof(List_t));
    if (list->data == NULL) {
        ListDump(list, kNoMemory);
        return kNoMemory;
    }

    list->next = (int *) calloc ((size_t)list->size, sizeof(int));
    if (list->next == NULL) {
        free(list->data);
        ListDump(list, kNoMemory);
        return kNoMemory;
    }

    list->prev = (int *) calloc ((size_t)list->size, sizeof(int));
    if (list->next == NULL) {
        free(list->data);
        free(list->next);
        ListDump(list, kNoMemory);
        return kNoMemory;
    }

    return kSuccess;
}

static ListErrors DoRealloc(List *list) {
    assert(list);

    List_t *ptr_data = list->data;
    List_t *realloc_data = (List_t *) realloc (ptr_data, (size_t)(list->size) * sizeof(*realloc_data));
    if (realloc_data == NULL) {
        ListDump(list, kNoMemory);
        return kNoMemory;
    }

    int *ptr_int = list->next;
    int *realloc_next = (int *) realloc (ptr_int, (size_t)(list->size) * sizeof(*realloc_next));
    if (realloc_next == NULL) {
        free(realloc_data);
        ListDump(list, kNoMemory);
        return kNoMemory;
    }

    ptr_int = list->prev;
    int *realloc_prev = (int *) realloc (ptr_int, (size_t)(list->size) *sizeof(*realloc_prev));
    if (realloc_prev == NULL) {
        free(realloc_data);
        free(realloc_prev);
        ListDump(list, kNoMemory);
        return kNoMemory;
    }

    list->data = realloc_data;
    list->next = realloc_next;
    list->prev = realloc_prev;

    return kSuccess;
}

ListCommands FuncNameToEnum(const char *func_name) {
    assert(func_name);

    if (strncmp(func_name, "InsertElementAfterPosition", 27) == 0) {
        return kInsertAfter;

    } else if (strncmp(func_name, "InsertElementBeforePosition", 28) == 0) {
        return kInsertBefore;

    } else if (strncmp(func_name, "DeleteElement", 14) == 0) {
        return kDelete;

    }
    return kDump;

}
void DoAllDump(ChangeOperationContext *Info) {
    assert(Info);

    DumpListToGraphviz(Info);
    DoSnprintf(Info);

    DoDump(Info);
}

static void DoSnprintf(ChangeOperationContext *Info) {
    assert(Info);

    snprintf(Info->image_file, sizeof(Info->image_file), "Images/graph_%d.svg", Info->graph_counter);
    (Info->graph_counter)++;
    char cmd[DEFAULT_STROKE_SIZE] = {};
    snprintf(cmd, sizeof(cmd), "dot output.txt -T svg -o %s", Info->image_file);
    
    system(cmd);
}

// ListErrors ListDumpBefore(ChangeOperationContext *Info, int *graph_counter, int pos, ListCommands type_of_change) {
//     DumpListToGraphviz(Info);
//     DoSnprintf(Info, graph_counter);

//     Info->pos = pos, Info->type_of_command_before = kDump, Info->type_of_command_after = type_of_change;
//     DoDump(Info);
// }

// ListErrors ListDumpAfter(ChangeOperationContext *Info, int *graph_counter, int pos, ListCommands type_of_change) {
//     assert(Info);
//     assert(graph_counter);

//     ListErrors err = kSuccess;

//     Info->type_of_command_before = type_of_change, Info->type_of_command_after = kDump;   
//     CHECK_ERROR_RETURN(DumpListToGraphviz(Info));
//     DoSnprintf(Info, graph_counter);

//     DoDump(Info);

//     return kSuccess;
// }