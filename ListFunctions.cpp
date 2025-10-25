#include "ListFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "Colors.h"

#define POISON 66666
#define DEFAULT_LISTS_SIZE 4

ListErrors ListCtor(List *lists) {
    assert(lists);

    lists->size = DEFAULT_LISTS_SIZE;

    lists->data = (int *) calloc (lists->size + 1, sizeof(int));
    lists->next = (int *) calloc (lists->size + 1, sizeof(int));
    lists->prev = (int *) calloc (lists->size + 1, sizeof(int));
    if (lists->data == NULL || lists->next == NULL || lists->prev == NULL) {
        printf("No memory in ListCtor to create lists.\n");
        return kNoMemory;
    }

    lists->data[0] = POISON;
    lists->prev[0] = 0;
    lists->next[0] = 0;
    for (size_t i = 1; i <= lists->size; i++) {
        lists->next[i] = i + 1;
        lists->prev[i] = -1;
    }

    lists->head = 0;
    lists->tail = 0;
    lists->free = 1;

    lists->number_of_elem = 0;

    return kSuccess;
}

ListErrors ListVerify(List *lists) {
    assert(lists);

    if (lists->data == NULL) {
        return kNullData;
    }

    if (lists->next == NULL) {
        return kNullNext;
    }

    if (lists->prev == NULL) {
        return kNullPrev;
    }

    return kSuccess;

}

ListErrors InsertList(List *list, int pos, int value) {
    assert(list);

    if (list->free == 0)
        return kNoMemory;

    int new_index = list->free;
    list->free = list->next[new_index];
    list->data[new_index] = value;

    if (list->number_of_elem == 0) {
        list->head = new_index;
        list->tail = new_index;
        //list->next[new_index] = 0;
        list->prev[new_index] = 0;
        list->number_of_elem++;
        return kSuccess;
    }

    if (pos == list->head) {
        list->prev[new_index] = 0;
        list->next[new_index] = list->head;
        list->prev[list->head] = new_index;
        list->head = new_index;
    }

    else {
        int next_elem = list->next[pos];
        list->next[pos] = new_index;
        list->prev[new_index] = pos;
        list->next[new_index] = next_elem;

        if (next_elem) {
            list->prev[next_elem] = new_index;
        } else {
            list->tail = new_index;
        }
    }

    list->number_of_elem++;
    return kSuccess;
}


ListErrors RemoveFromList(List *lists, int prev_pos) {
    assert(lists);

    // if (prev_pos > 0 && prev_pos < lists->size) {
    //     lists->next = 
    // }
    return kSuccess;
}

ListErrors ListDump(List *lists) {
    assert(lists);

    printf("size: %d\n\n", lists->size);

    printf("===============Data===============:\n");
    for (size_t i = 0; i <= lists->size; i++) {
        printf(YELLOW "%d " RESET, lists->data[i]);
    }
    printf("\n\n");

    printf("head: %d\n", lists->head);
    printf("===============Next===============:\n");
    for (size_t i = 0; i <= lists->size; i++) {
        printf(YELLOW "%d " RESET, lists->next[i]);
    }
    printf("\n\n");

    printf("tail: %d\n", lists->tail);
    printf("===============Prev===============:\n");
    for (size_t i = 0; i < lists->size; i++) {
        printf(YELLOW "%d " RESET, lists->prev[i]);
    }
    printf("\n\n");

    printf("%d ", lists->free);

    return kSuccess;
}