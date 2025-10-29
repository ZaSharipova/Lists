#include "DoGraph.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "FileOperations.h"
#include "Canary.h"
#include "Subsidiary.h"

static void PrintGraphHeader(FILE *file);
static FillAndBorderColor GetFillColors(ChangeOperationContext *Info, int pos);
static void FillFree(ChangeOperationContext *Info, FILE *file);

ListErrors DumpListToGraphviz(ChangeOperationContext *Info) {
    assert(Info);

    FILE *file = OpenFile(FILE_FOR_GRAPH_TEXT, WRITE_MODE);
    if (!file) {
        return kErrorOpening;
    }

    PrintGraphHeader(file);
    //printf("%d ", Info->type_of_command_before);

    for (int i = 0; i < Info->list->size; i++) {
        FillAndBorderColor colors = GetFillColors(Info, i);

#ifdef _DEBUG
        if (i == 0 || i == Info->list->size - 1) {
            fprintf(file, "    node%d [label=\"idx: %d | data (canary): " LIST_SPEC" | next: %d | prev: %d\"; shape = Mrecord; style = filled; ", i, i, Info->list->data[i], Info->list->next[i], Info->list->prev[i]);
        } else {
            fprintf(file, "    node%d [label=\"idx: %d | data: " LIST_SPEC" | next: %d | prev: %d\"; shape = Mrecord; style = filled; ", i, i, Info->list->data[i], Info->list->next[i], Info->list->prev[i]);
        }
#else
        fprintf(file, "    node%d [label=\"idx: %d | data: " LIST_SPEC" | next: %d | prev: %d\"; shape = Mrecord; style = filled; ", i, i, Info->list->data[i], Info->list->next[i], Info->list->prev[i]);
#endif
        fprintf(file, "fillcolor = \"%s\"; color = \"%s\"];\n", colors.fillColor, colors.borderColor);

    }
    fprintf(file, "\n");

    fprintf(file, "    node0 -> node1");
    for (int i = 2; i < Info->list->size; i++) {
        fprintf(file, " -> node%d", i);
    }
    fprintf(file, " [style=invis, weight=1000];\n\n");

    for (int i = 0; i < Info->list->size; i++) {
        if ((i != Info->list->size - 1)) {
            fprintf(file, "    node%d -> node%d [color=purple2];\n", i, Info->list->next[i]);
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

    FillFree(Info, file);
    printf("Graphviz dump saved to %s\n", FILE_FOR_GRAPH_TEXT);
    return CloseFile(file);
}

static void PrintGraphHeader(FILE *file) {
    assert(file);

    fprintf(file, "digraph List {\n");
    fprintf(file, "    splines = ortho;\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=record];\n\n");
    fprintf(file, "    nodesep=1.0;\n");
    fprintf(file, "    ranksep=1.0;\n");
}

static FillAndBorderColor GetFillColors(ChangeOperationContext *Info, int pos) {
    if (pos == 0) {
        return (FillAndBorderColor){"#20B2AA", "#008B8B"};

    } else if (Info->type_of_command_before == kDelete && pos == Info->list->free) {
        return (FillAndBorderColor){"#FF0000", "#8B0000"};

    } else if (Info->list->data[pos] == POISON
#ifdef _DEBUG
           || Info->list->data[pos] == canary_right
#endif
    ) {
        return (FillAndBorderColor){"#fbf5eef2", "#CD853F"};

    } else if ((Info->type_of_command_before == kInsertAfter  && pos == Info->list->next[Info->pos]) 
    || (Info->type_of_command_before == kInsertBefore && pos == Info->list->prev[Info->pos])) {
        return (FillAndBorderColor){"#7FFF00", "#32CD32"};

    } else {
        return (FillAndBorderColor){"#FFE4B5", "#CD853F"};
    }
}

static void FillFree(ChangeOperationContext *Info, FILE *file) {
    fprintf(file, "    free [shape=ellipse fillcolor=\"#DCDCDC\" style=filled label=\"free = %d\"];\n", Info->list->free);
    fprintf(file, "    {rank=same; free; node%d; }", Info->list->free);
    fprintf(file, "    free -> node%d [color=\"#8B4513\"];\n", Info->list->free);

    fprintf(file, "}\n");
}