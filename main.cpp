#include <stdio.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "DoGraph.h"
#include "DoDump.h"

int main(void) {
    List list = {};

    ListCtor(&list);
    ListDump(&list);

    InsertElement(&list, 1, 32);
    ListDump(&list);
    InsertElement(&list, 2, 20);
    ListDump(&list);
    InsertElement(&list, 3, 25);
    //InsertList(&list, 2, 40);
    ListDump(&list);
    // DeleteElement(&list, 3);
    // ListDump(&list);

    DumpListToGraphviz(&list, "output.txt");
    DoDump(&list);
    ListDtor(&list);
    return 0;
}