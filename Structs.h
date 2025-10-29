#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>

#include "Enums.h"
#define MAX_STRING_SIZE 64

typedef int List_t;
#define LIST_SPEC "%d"

#define POISON 66666

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
    int default_pos;
    int free;
    List_t number;

    ListCommands type_of_command_before;
    ListCommands type_of_command_after;

    char image_file[MAX_STRING_SIZE];
} ChangeOperationContext;

#endif //STRUCTS_H_