#include <stdio.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "DoGraph.h"

int main(void) {
    List list = {};

    ListCtor(&list);
    ListDump(&list);

    InsertElement(&list, 1, 32);
    ListDump(&list);
    InsertElement(&list, 1, 20);
    ListDump(&list);
    InsertElement(&list, 1, 25);
    //InsertList(&list, 2, 40);
    ListDump(&list);
    DeleteElement(&list, 2);
    ListDump(&list);

    DumpListToGraphviz(&list, "output.txt");
    ListDtor(&list);
    return 0;
}