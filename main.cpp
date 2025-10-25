#include <stdio.h>

#include "ListFunctions.h"
#include "Structs.h"

int main(void) {
    List list = {};

    ListCtor(&list);
    ListDump(&list);

    InsertList(&list, 1, 32);

    InsertList(&list, 2, 20);
    InsertList(&list, 1, 25);
    ListDump(&list);
    return 0;
}