#include "DoGraph.h"

#include <stdio.h>

#include "Structs.h"

void DumpListToGraphviz(List *list, const char *filename, int pos, ListCommands type_of_command) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file %s\n", filename);
        return;
    }

    fprintf(file, "digraph List {\n");
    fprintf(file, "    splines = ortho;\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=record];\n\n");


    for (int i = 0; i < list->size; i++) {
        fprintf(file, "    node%d [label=\"idx: %d | data: %.0f | next: %d | prev: %d\"; shape = Mrecord; style = filled; ", i, i, list->data[i], list->next[i], list->prev[i]);
        if (i == 0) {
            fprintf(file, "    fillcolor = \"#20B2AA\", color = \"#008B8B\"];\n");

        }
        else if (type_of_command == kInsert && i == list->prev[0]) {
            fprintf(file, "fillcolor = \"#7FFF00\", color = \"#32CD32\"];\n");
        } else if (type_of_command == kDelete && pos == i) {
            fprintf(file, "fillcolor = \"#FF0000\", color = \"#8B0000\"];\n");

        } else {
            if (list->data[i] == POISON) {
                fprintf(file, "fillcolor = \"#fbf5eef2\", color = \"#CD853F\"];\n");
            } else {
                fprintf(file, "fillcolor = \"#FFE4B5\", color = \"#CD853F\"];\n");
            }
        }

    }
    fprintf(file, "\n");

    fprintf(file, "    node0 -> node1");
    for (int i = 2; i < list->size; i++) {
        fprintf(file, " -> node%d", i);
    }
    fprintf(file, " [style=invis, weight=1000];\n\n");

    fprintf(file, "\n\n");

    for (int i = 0; i < list->size; i++) {
        if ((list->data[i] != POISON || i == 0)) {
            fprintf(file, "    node%d -> node%d [color=purple2];\n", 
            i, list->next[i]);
        }
    }

    fprintf(file, "\n");

    for (int i = 0; i < list->size; i++) {
        if (list->data[i] != POISON || i == 0) {
            fprintf(file, "    node%d -> node%d [color=mediumaquamarine, constraint=false];\n", 
                    i, list->prev[i]);
        }
    }

    fprintf(file, "\n");

    fprintf(file, "    free [shape=ellipse fillcolor=\"#DCDCDC\" style=filled label=\"free = %d\"];\n", list->free);
    fprintf(file, " {rank=same; free; node%d; }", list->free);
    fprintf(file, "    free -> node%d [color=\"#8B4513\"];\n", list->free);


fprintf(file, "}\n");
    fclose(file);

    printf("Graphviz dump saved to %s\n", filename);
}

// int main(void) {
//     List list = {};
//     DumpListToGraphviz(&list, "output.txt");
// }