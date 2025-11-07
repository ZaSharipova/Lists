#include "ClassicListFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "Enums.h"
#include "Subsidiary.h"

ListErrors AddressListCtor(AddressList **head) {

    *head = (AddressList *) calloc (1, sizeof(AddressList));
    if (*head == NULL) {
        return kNoMemory;
    }

    (*head)->data = 1;
    (*head)->prev = NULL;
    (*head)->next = NULL;

    return kSuccess;
}

ListErrors AddressListDtor(AddressList *head) {
    assert(head);

    if (head) {
        AddressList *temp = head;
        if (head->next != NULL) AddressListDtor(head->next);
        free(temp);
    }

    return kSuccess;
}

ListErrors AddressListDump(AddressList *list) {
    assert(list);

    // unsigned int bit = 1;
    // printf("DUMP Errors: ");
    // for (unsigned long long i = 0; i < NUMBER_OF_ERRORS; i++) {
    //     if (error & bit) {
    //         fprintf(GetLogFile(), "%s ", ListErrorString[i]);
    //     }
    //     bit <<= 1;
    // }

    AddressList *head = list;
    while (head){
        printf(YELLOW " " LIST_SPEC" , prev - %p, next - %p" RESET, head->data, head->prev, head->next);
        printf("\n\n");
        head = head->next;
    }

    printf("\n\n\n");

    return kSuccess;
}

void InsertAfter(AddressList *pos, List_t *value) {
    if (pos == NULL) return;

    AddressList *new_node = (AddressList *) calloc (1, sizeof(AddressList));
    new_node->data = *value;

    new_node->next = pos->next;
    new_node->prev = pos;

    if (pos->next != NULL) {
        pos->next->prev = new_node;
    }
    pos->next = new_node;
}

void InsertBefore(AddressList **head, AddressList *pos, List_t *value) {
    if (pos == NULL) return;

    AddressList *new_node = (AddressList *) calloc (1, sizeof(AddressList));
    new_node->data = *value;

    new_node->next = pos;
    new_node->prev = pos->prev;

    if (pos->prev != NULL) {
        pos->prev->next = new_node;
    } else {
        *head = new_node;
    }
    pos->prev = new_node;
}

void DeleteNode(AddressList **head, AddressList *pos) {
    if (pos == NULL) return;

    if (pos->prev != NULL) {
        pos->prev->next = pos->next;
    } else {
        *head = pos->next;
    }
    if (pos->next != NULL) {
        pos->next->prev = pos->prev;
    }

    free(pos);
}