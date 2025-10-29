#include "DoGraph.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "FileOperations.h"
#include "Canary.h"
#include "Subsidiary.h"

ListErrors DumpListToGraphviz(ChangeOperationContext *Info) {
    assert(Info);

    FILE *file = OpenFile(FILE_FOR_GRAPH_TEXT, WRITE_MODE);
    if (!file) {
        return kErrorOpening;
    }

    fprintf(file, "digraph List {\n");
    fprintf(file, "    splines = ortho;\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=record];\n\n");
    fprintf(file, "    nodesep=1.0;\n");
    fprintf(file, "    ranksep=1.0;\n");

    fprintf(file, "    minlen = 2;\n");
    fprintf(file, "    overlap=false;\n");
    fprintf(file, "    concentrate=false;\n");
    fprintf(file, "    newrank=true;\n"); //

    for (int i = 0; i < Info->list->size; i++) {
        fprintf(file, "    node%d [label=\"idx: %d | data: " LIST_SPEC" | next: %d | prev: %d\"; shape = Mrecord; style = filled; ", i, i, Info->list->data[i], Info->list->next[i], Info->list->prev[i]);
        if (i == 0) {
            fprintf(file, "    fillcolor = \"#20B2AA\", color = \"#008B8B\"];\n");

        }
        else if ((Info->type_of_command_before == kInsertAfter || Info->type_of_command_before == kInsertBefore) && i == Info->pos) {
            fprintf(file, "fillcolor = \"#7FFF00\", color = \"#32CD32\"];\n");
        } else if (Info->type_of_command_before == kDelete && i == Info->list->free) {
            fprintf(file, "fillcolor = \"#FF0000\", color = \"#8B0000\"];\n");

        } else {
            if (Info->list->data[i] == POISON //
#ifdef _DEBUG
    || Info->list->data[i] == canary_right
    
#endif
            ){
                fprintf(file, "fillcolor = \"#fbf5eef2\", color = \"#CD853F\"];\n");
            } else {
                fprintf(file, "fillcolor = \"#FFE4B5\", color = \"#CD853F\"];\n");
            }
        }

    }
    fprintf(file, "\n");

    fprintf(file, "    node0 -> node1");
    for (int i = 2; i < Info->list->size; i++) {
        fprintf(file, " -> node%d", i);
    }
    fprintf(file, " [style=invis, weight=1000];\n\n");

    fprintf(file, "\n\n");


    for (int i = 0; i < Info->list->size; i++) {
        if ((Info->list->data[i] != POISON || i == 0)) {
#ifdef _DEBUG
        if (Info->list->data[i] != canary_right && Info->list->data[i] != canary_left || i == 0) {
            fprintf(file, "    node%d -> node%d [color=purple2];\n", 
                    i, Info->list->next[i]);
            }
#else 
    fprintf(file, "    node%d -> node%d [color=purple2];\n", 
                    i, Info->list->next[i]);
#endif
        }
    }

    fprintf(file, "\n");

    for (int i = 0; i < Info->list->size; i++) {
        if ((Info->list->data[i] != POISON || i == 0) && Info->list->prev[i] != -1) {

#ifdef _DEBUG
            if (Info->list->data[i] != canary_right && Info->list->data[i] != canary_left || i == 0) {
            fprintf(file, "    node%d -> node%d [color=mediumaquamarine, constraint=false];\n", 
                    i, Info->list->prev[i]);
            }
#else 
    fprintf(file, "    node%d -> node%d [color=mediumaquamarine, constraint=false];\n", 
                    i, Info->list->prev[i]);
#endif
        }
    }

    fprintf(file, "\n");

    fprintf(file, "    free [shape=ellipse fillcolor=\"#DCDCDC\" style=filled label=\"free = %d\"];\n", Info->list->free);
    fprintf(file, "    {rank=same; free; node%d; }", Info->list->free);
    fprintf(file, "    free -> node%d [color=\"#8B4513\"];\n", Info->list->free);

    fprintf(file, "}\n");

    printf("Graphviz dump saved to %s\n", FILE_FOR_GRAPH_TEXT);

    return CloseFile(file);
}