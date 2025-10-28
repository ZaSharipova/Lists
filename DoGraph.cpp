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
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=record];\n\n");


    for (int i = 0; i <= list->size; i++) {
        fprintf(file, "    node%d [label=\"idx: %d | data: %.0f | next: %d | prev: %d\"; shape = Mrecord; style = filled; ", i, i, list->data[i], list->next[i], list->prev[i]);
        if (i == 0) {
            fprintf(file, "fillcolor = \"#20B2AA\", color = \"#008B8B\"];\n");

        } else if (i == pos) {
            if (type_of_command == kInsert) {
                fprintf(file, "fillcolor = \"#7FFF00\", color = \"#32CD32\"];\n");
            } else {
            fprintf(file, "fillcolor = \"#FF0000\", color = \"#8B0000\"];\n");
            }
        } else {
            fprintf(file, "fillcolor = \"#FFE4B5\", color = \"#CD853F\"];\n");
        }

    }
    fprintf(file, "\n");

    fprintf(file, "    node0 -> node1");
    for (int i = 2; i <= list->size; i++) {
        fprintf(file, " -> node%d", i);
    }
    fprintf(file, " [style=invis, weight=1000];\n\n");

    fprintf(file, "\n\n");

    for (int i = 1; i <= list->size; i++) {
        if ((list->free != i) && list->next[i] != 0 && (i <= list->number_of_elem)) {
            fprintf(file, "    node%d -> node%d [label=\"next\"];\n", 
            i, list->next[i]);
        }
    }

    fprintf(file, "\n");

    for (int i = 1; i <= list->size; i++) {
        if (list->prev[i] > 0) {
            fprintf(file, "    node%d -> node%d [label=\"prev\", style=dashed, color=blue, constraint=false];\n", 
                    i, list->prev[i]);
        }
    }

    fprintf(file, "\n");

    fprintf(file, "    head [shape=box, label=\"head=%d\"];\n", list->head);
    fprintf(file, "    tail [shape=box, label=\"tail=%d\"];\n", list->tail);
    fprintf(file, "    free [shape=box, label=\"free=%d\"];\n", list->free);

    fprintf(file, "    head -> node%d [color=brown];\n", list->head);
    fprintf(file, "    tail -> node%d [color=brown];\n", list->tail);
    fprintf(file, "    free -> node%d [color=brown];\n", list->free);


fprintf(file, "}\n");
    fclose(file);

    printf("Graphviz dump saved to %s\n", filename);
}

// int main(void) {
//     List list = {};
//     DumpListToGraphviz(&list, "output.txt");
// }