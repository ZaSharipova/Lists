#include "ListFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "Colors.h"

#define DEFAULT_LIST_SIZE 4

#define INCREASE_VALUE 2
#define DECREASE_VALUE 4

static Realloc_Mode CheckSize(List *list);

ListErrors ListCtor(List *list) {
    assert(list);

    list->size = DEFAULT_LIST_SIZE;

    list->data = (List_t *) calloc (list->size, sizeof(List_t));
    list->next = (int *) calloc (list->size, sizeof(int));
    list->prev = (int *) calloc (list->size, sizeof(int));
    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("No memory in ListCtor to create list.\n");
        return kNoMemory;
    }

    list->data[0] = POISON;
    list->prev[0] = 0;
    list->next[0] = 0;

    FillList(list, 0);
    // for (size_t i = 1; i <= list->size; i++) {
    //     list->data[i] = POISON;
    //     list->next[i] = i + 1;
    //     list->prev[i] = -1;
    // }

    //list->next[list->size - 1] = 0;
    list->head = 0;
    list->tail = 0;
    list->free = 1;

    list->number_of_elem = 0;

    return kSuccess;
}

ListErrors ListVerify(List *list) {
    assert(list);

    if (list->data == NULL) {
        return kNullData;
    }

    if (list->next == NULL) {
        return kNullNext;
    }

    if (list->prev == NULL) {
        return kNullPrev;
    }

    if (list->tail < 0) {
        return kNegativeTail;
    }

    if (list->head < 0) {
        return kNegativeHead;
    }

    if (list->free < 0) {
        return kNegativeFree;
    }

    return kSuccess;

}

static Realloc_Mode CheckSize(List *list) {
    assert(list);

    if (list->number_of_elem * INCREASE_VALUE >= list->size) {
        return kIncrease;

    } else if (list->number_of_elem * DECREASE_VALUE <= list->size 
        && list->number_of_elem != 0 && list->size >= 4) {
        return kDecrease;
    }

    if (list->size == 0) {
        return kIncreaseZero;
    }

    return kNoChange;
}

// ListErrors FillPoison(List *list) {
//     assert(list);


// }

ListErrors ResizeList(List *list, Realloc_Mode realloc_type) {
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    int size_to_remember = list->size;
    if (realloc_type == kIncrease) {
        list->size *= INCREASE_VALUE;

    } else if (realloc_type == kDecrease) {
        list->size /= DECREASE_VALUE;

    } else {
        list->size = 1;
    }

    List_t *ptr_data = list->data;
    List_t *realloc_data = (List_t *) realloc (ptr_data, (size_t)(list->size) * sizeof(*realloc_data));

    int *ptr_int = list->next;
    int *realloc_next = (int *) realloc (ptr_int, (size_t)(list->size) * sizeof(*realloc_next));

    ptr_int = list->prev;
    int *realloc_prev = (int *) realloc (ptr_int, (size_t)(list->size) *sizeof(*realloc_prev));


    if (realloc_data == NULL || realloc_next == NULL || realloc_prev == NULL) {
        ListDump(list); //
        return kNoMemory;
    }

    list->data = realloc_data;
    list->next = realloc_next;
    list->prev = realloc_prev;
    FillList(list, size_to_remember - 1);

    CHECK_ERROR_RETURN(ListVerify(list));
    return err;
}

ListErrors InsertElement(List *list, int pos, List_t value) {
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    if (list->number_of_elem == list->size - 2) {
        Realloc_Mode realloc_type = CheckSize(list);
        if (realloc_type != kNoChange) {
            CHECK_ERROR_RETURN(ResizeList(list, realloc_type));
        }
    }

    int new_index = list->free;
    list->free = list->prev[new_index];

    list->data[new_index] = value;

    list->prev[new_index] = pos;

    list->prev[list->next[pos]] = new_index;
    list->next[new_index] = list->next[pos];
    list->next[pos] = new_index;

    list->number_of_elem++;
    CHECK_ERROR_RETURN(ListVerify(list));
    return kSuccess;
}

ListErrors DeleteElement(List *list, int pos) {
    assert(list);

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(ListVerify(list));

    if (list->number_of_elem == list->size - 2) {
        Realloc_Mode realloc_type = CheckSize(list);
        if (realloc_type != kNoChange) {
            CHECK_ERROR_RETURN(ResizeList(list, realloc_type));
        }
    }

    list->next[list->prev[pos]] = list->next[pos];
    list->prev[list->next[pos]] = list->prev[pos];

    list->data[pos] = POISON;
    list->next[pos] = 0;
    list->prev[pos] = list->free;
    list->free = pos;

    list->number_of_elem--;

    CHECK_ERROR_RETURN(ListVerify(list));
    return kSuccess;
}


ListErrors ListDump(List *list) {
    assert(list);

    printf("size: %d\n\n", list->size);

    for (size_t i = 0; i < list->size; i++) {
        printf("%lu ", i);
    }
    printf("\n");

    printf("===============Data===============:\n");
    for (size_t i = 0; i < list->size; i++) {
        printf(YELLOW "%.0f " RESET, list->data[i]);
    }
    printf("\n\n");

    printf("head: %d\n", list->head);
    printf("===============Next===============:\n");
    for (size_t i = 0; i < list->size; i++) {
        printf(YELLOW "%d " RESET, list->next[i]);
    }
    printf("\n\n");

    printf("tail: %d\n", list->tail);
    printf("===============Prev===============:\n");
    for (size_t i = 0; i < list->size; i++) {
        printf(YELLOW "%d " RESET, list->prev[i]);
    }
    printf("\n\n");

    printf("free: %d\n", list->free);
    printf("------------------------------------\n");

    printf("\n\n\n\n");
    return kSuccess;
}

ListErrors ListDtor(List *list) {
    assert(list);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->size = 0;
    list->free = 0;
    list->head = 0;
    list->tail = 0;
    list->number_of_elem = 0;

    return kSuccess;
}

void FillList(List *list, int pos) {
    for (int i = pos + 1; i < list->size; i++) {
        list->data[i] = POISON;
        list->next[i] = -1;
        list->prev[i] = i + 1;
    }
    // list->next[list->size - 1] = 0;
    list->prev[1] = 2;
}