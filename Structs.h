#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>

#include "Enums.h"
#define MAX_STRING_SIZE 64

typedef int List_t;
#define LIST_SPEC "%d"

struct List {
    int size;
    List_t *data;
    int *next;
    int *prev;
    int free;
    int number_of_elem;
};

typedef struct ChangeOperationContext {
    FILE *file;
    List *list;
    const char *var_name;
    const char *filename;      

    int pos;
    List_t number;
    int graph_counter;
    ListErrors error;

    char message[MAX_STRING_SIZE];
    ListCommands type_of_command_before;
    ListCommands type_of_command_after;

    char image_file[MAX_STRING_SIZE];
} ChangeOperationContext;

struct AddressList {
    int size;
    List_t data;
    struct AddressList *next;
    struct AddressList *prev;
    int number_of_elem;
};

#endif //STRUCTS_H_