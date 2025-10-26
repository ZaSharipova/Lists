#include "DoGraph.h"

#include <stdio.h>

#include "Structs.h"

void DumpListToGraphviz(List *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file %s\n", filename);
        return;
    }

    fprintf(file, "digraph List {\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=record];\n\n");

    for (int i = 1; i < list->size; i++) {
        // fprintf(file, "    node%d [label=\"idx: %d\\n data: %.0f\\nnext: %d\\nprev: %d\"];\n",
        // i, i, list->data[i], list->next[i], list->prev[i]);
        fprintf(file, "    node%d [label=\"idx: %d | data: %.0f | next: %d | prev: %d\"; shape = Mrecord; style = filled; fillcolor = \"#00FF00\", color = \"#32CD32\"];\n",
        i, i, list->data[i], list->next[i], list->prev[i]);

    }
    fprintf(file, "\n");

    fprintf(file, "    node1 -> node2 -> node3 -> node4 [style=invis, weight=10];");

    fprintf(file, "\n\n");

    for (int i = 1; i < list->size; i++) {
        if (list->next[i] != 0) {
            fprintf(file, "    node%d -> node%d [label=\"next\"];\n", 
                    i, list->next[i]);
        }
    }

    fprintf(file, "\n");

    for (int i = 1; i < list->size; i++) {
        if (list->prev[i] != 0) {
            fprintf(file, "    node%d -> node%d [label=\"prev\", style=dashed, color=blue, constraint=false];\n", 
                    i, list->prev[i]);
        }
    }

    fprintf(file, "\n");

    fprintf(file, "    head [shape=box, label=\"head=%d\"];\n", list->head);
    fprintf(file, "    tail [shape=box, label=\"tail=%d\"];\n", list->tail);
    fprintf(file, "    free [shape=box, label=\"free=%d\"];\n", list->free);

    fprintf(file, "    head -> node%d [color=green];\n", list->head);
    fprintf(file, "    tail -> node%d [color=red];\n", list->tail);
    if (list->free != 0)
        fprintf(file, "    free -> node%d [color=orange];\n", list->free);

    fprintf(file, "}\n");
    fclose(file);

    printf("Graphviz dump saved to %s\n", filename);
}

// int main(void) {
//     List list = {};
//     DumpListToGraphviz(&list, "output.txt");
// }