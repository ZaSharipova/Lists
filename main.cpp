#include <stdio.h>
#include <stdlib.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "DoGraph.h"
#include "DoDump.h"

#define DEFAULT_POS -20
#define DoChange(func, list, pos, number, type_of_change)                          \
    DumpListToGraphviz(&list, "output.txt", DEFAULT_POS, kDump);           \
    snprintf(img_name, sizeof(img_name), "Images/graph_%d.svg", graph_counter++); \
    snprintf(cmd, sizeof(cmd), "dot output.txt -T svg -o %s", img_name);   \
    system(cmd);                                                           \
    DoDump(file, &list, #list, __FILE__, img_name, pos, number, kDump, type_of_change);                        \
    func;                                                                  \
    ListDump(&list);                                                       \
    DumpListToGraphviz(&list, "output.txt", pos, type_of_change);          \
    snprintf(img_name, sizeof(img_name), "Images/graph_%d.svg", graph_counter++); \
    snprintf(cmd, sizeof(cmd), "dot output.txt -T svg -o %s", img_name);   \
    system(cmd);                                                           \
    DoDump(file, &(list), #list, __FILE__, img_name, pos, number, type_of_change, kDump);                      \


int main(void) {
    List list = {};
    FILE *file = fopen("alldump.html", "w");
    if (!file) {
        printf("Failed to open file %s\n", "alldump.hml");
        return -1;
    }

    ListCtor(&list);
    ListDump(&list);

    int graph_counter = 0;
    char cmd[100] = {}, img_name[64] = {};
    DoChange(InsertElementAfterPosition(&list, 0, 32), list, 0, 32.0, kInsert);

    DoChange(InsertElementAfterPosition(&list, 1, 20), list, 1, 20.0, kInsert);

    DoChange(InsertElementAfterPosition(&list, 2, 25), list, 1, 25, kInsert);

    DoChange(InsertElementAfterPosition(&list, 3, 26), list, 3, 26, kInsert);
    //DoChange(DeleteElement(&list, 1), list, 1, 0, kDelete);
    DoChange(DeleteElement(&list, 3), list, 3, 0, kDelete);
    DoChange(InsertElementBeforePosition(&list, 2, 10), list, 2, 10, kInsert);

    ListDtor(&list);
    fclose(file);
    return 0;
}