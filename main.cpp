#include <stdio.h>
#include <stdlib.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "DoGraph.h"
#include "DoDump.h"

#define DEFAULT_POS -20
#define DoChange(func, list, pos, type_of_change)                          \
    DumpListToGraphviz(&list, "output.txt", DEFAULT_POS, kDump);           \
    snprintf(img_name, sizeof(img_name), "graph_%d.png", graph_counter++); \
    snprintf(cmd, sizeof(cmd), "dot output.txt -T png -o %s", img_name);   \
    system(cmd);                                                           \
    DoDump(file, &list, #list, __FILE__, img_name, kDump, type_of_change);                        \
    func;                                                                  \
    ListDump(&list);                                                       \
    DumpListToGraphviz(&list, "output.txt", pos, type_of_change);          \
    snprintf(img_name, sizeof(img_name), "graph_%d.png", graph_counter++); \
    snprintf(cmd, sizeof(cmd), "dot output.txt -T png -o %s", img_name);   \
    system(cmd);                                                           \
    DoDump(file, &(list), #list, __FILE__, img_name, type_of_change, kDump);                      \


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
    DoChange(InsertElement(&list, 0, 32), list, 1, kInsert);
    // InsertElement(&list, 1, 32);
    // ListDump(&list);
    DoChange(InsertElement(&list, 1, 20), list, 1, kInsert);
    // InsertElement(&list, 2, 20);
    // ListDump(&list);
    DoChange(InsertElement(&list, 2, 25), list, 2, kInsert);
    // InsertElement(&list, 3, 25);
    DoChange(InsertElement(&list, 3, 26), list, 3, kInsert);
    //DoChange(InsertElement(&list, 5, 25), list, 5, kInsert);
    //InsertList(&list, 2, 40);
    // ListDump(&list);
    DoChange(DeleteElement(&list, 3), list, 3, kDelete);
    // DeleteElement(&list, 3);
    // ListDump(&list);

    // DumpListToGraphviz(&list, "output.txt");
    //DoDump(&list);
    ListDtor(&list);
    fclose(file);
    return 0;
}