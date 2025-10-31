#include "DoGraph.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "FileOperations.h"
#include "Canary.h"
#include "Subsidiary.h"

static void PrintGraphHeader(FILE *file);
static FillAndBorderColor GetFillColors(ChangeOperationContext *Info, int pos);

static void PrintNodes(ChangeOperationContext *Info, FILE *file);
static void PrintInvisibleEdges(ChangeOperationContext *Info, FILE *file);

static void FillFree(ChangeOperationContext *Info, FILE *file);

static void PrintOrangeRelatedEdges(ChangeOperationContext *Info, FILE *file, int i, int next, int size);
static void PrintSpecialEdges(ChangeOperationContext *Info, FILE *file, int i, int next, int size);


static void PrintEdges(ChangeOperationContext *Info, FILE *file);

ListErrors DumpListToGraphviz(ChangeOperationContext *Info) {
    assert(Info);

    FILE *file = OpenFile(FILE_FOR_GRAPH_TEXT, WRITE_MODE);
    if (!file) {
        return kErrorOpening;
    }

    if (Info->list->data && Info->list->next && Info->list->prev) {
        PrintGraphHeader(file);
        PrintNodes(Info, file);

        PrintInvisibleEdges(Info, file);

        PrintEdges(Info, file);
        FillFree(Info, file);
    }

    printf("Graphviz dump saved to %s\n", FILE_FOR_GRAPH_TEXT);

    return CloseFile(file);
}

static void PrintOrangeRelatedEdges(ChangeOperationContext *Info, FILE *file, int i, int next, int size) {
    assert(Info);
    assert(file);

    fprintf(file, "    node%d -> node%d [color=\"orange\", penwidth=\"3\"];\n", i, next);
    
    if (Info->list->prev[next] >= 0 && Info->list->prev[next] < size) {
        fprintf(file, "    node%d -> node%d [color=\"orange\", penwidth=\"3\"];\n", next, Info->list->prev[next]);
    } else {
        fprintf(file, "    node%d [shape=octagon, fillcolor=\"firebrick1\", style=filled];\n", Info->list->prev[next]);
    }
}

static void PrintSpecialEdges(ChangeOperationContext *Info, FILE *file, int i, int next, int size) {
    assert(Info);
    assert(file);

    if (Info->list->data[i] == POISON && i != 0) {
        if (next > 0 && next < size) {
            fprintf(file, "    node%d -> node%d [color=\"pink\"];\n", i, next);
        }
    } else {
        fprintf(file, "    node%d -> node%d [color=\"orange\", penwidth=\"3\"];\n", i, next);
        if (next >= 0 && next < size && !(Info->list->prev[next] >= 0 && Info->list->prev[next] < size)) {
            fprintf(file, "    node%d [shape=octagon, fillcolor=\"firebrick1\", style=filled];\n", Info->list->prev[next]);
        }
        fprintf(file, "    node%d [shape=octagon, fillcolor=\"firebrick1\", style=filled];\n", next);
    }
}

static void PrintEdges(ChangeOperationContext *Info, FILE *file) {
    assert(Info);
    assert(file);

    int size = Info->list->size;

    for (int i = 0; i < size; i++) {
        int next = Info->list->next[i];
        
        if (next >= 0 && next < size) {
            if (Info->list->data[i] == POISON && i != 0) {
                PrintSpecialEdges(Info, file, i, next, size);
            } else if (Info->list->prev[next] == i && Info->list->next[next] >= 0 && Info->list->next[next] < size) {
                if (next < i && Info->list->number_of_elem == 1) {
                    continue;
                }
                fprintf(file, "    node%d -> node%d [color=\"purple2\", dir=both];\n", i, next);

            } else if (!(Info->list->next[next] >= 0 && Info->list->next[next] < size) || i != size - 1) {
                PrintOrangeRelatedEdges(Info, file, i, next, size);
            }

        } else {
            PrintSpecialEdges(Info, file, i, next, size);
        }
    }
}

// static void PrintEdges(ChangeOperationContext *Info, FILE *file) {
//     assert(Info);
//     assert(file);

//     int size = Info->list->size;

//     for (int i = 0; i < size; i++) {
//         int next = Info->list->next[i];
//         int prev = Info->list->prev[i];

//         if (Info->list->data[i] == POISON && i != 0) {
//             if (next > 0 && next < size) {
//                 fprintf(file, "    node%d -> node%d [color=\"pink\"];\n", i, next);
//             }
//             continue;
//         }

//         else if (next != -1) {
//             fprintf(file,
//                 "    node%d [shape=octagon, fillcolor=\"firebrick1\", style=filled];\n"
//                 "    node%d -> node%d [color=\"firebrick\", penwidth=\"3\"];\n",
//                 (next >= size) ? next : i, i, (next >= size) ? next : i);
//         }

//         if (prev >= 0 && prev < size) {
//             if (Info->list->next[prev] != i) {
//                 fprintf(file, "    node%d -> node%d [color=\"orange\", penwidth=\"3\"];\n", prev, i);
//             }
//         } else if (prev != -1) {
//             fprintf(file,
//                 "    node%d [shape=octagon, fillcolor=\"firebrick1\", style=filled];\n"
//                 "    node%d -> node%d [color=\"firebrick\", penwidth=\"3\"];\n",
//                 (prev >= size) ? prev : i, (prev >= size) ? prev : i, i);
//         }

//         if ((next == -1 || next >= size) && (prev == -1 || prev >= size)) {
//             fprintf(file, "    node%d [shape=octagon, fillcolor=\"red\", style=filled];\n", i);
//         }
//     }
// }

static void PrintGraphHeader(FILE *file) {
    assert(file);

    fprintf(file, "digraph List {\n");
    fprintf(file, "    splines = ortho;\n");
    fprintf(file, "    rankdir=LR;\n");
    fprintf(file, "    node [shape=record];\n\n");
    fprintf(file, "    nodesep=0.3;\n");
    // fprintf(file, "    ranksep=1.0;\n");
}

static FillAndBorderColor GetFillColors(ChangeOperationContext *Info, int pos) {
    assert(Info);

    if (pos == 0) {
        return (FillAndBorderColor){"#20B2AA", "#008B8B"};

    } else if (Info->type_of_command_before == kDelete && pos == Info->list->free) {
        return (FillAndBorderColor){"#FF0000", "#8B0000"};

    } else if (Info->list->data[pos] == POISON
#ifdef _DEBUG
           || Info->list->data[pos] == (List_t)canary_right
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

static void PrintNodes(ChangeOperationContext *Info, FILE *file) {
    assert(Info);
    assert(file);

    if (Info->list->data && Info->list->next && Info->list->prev) {
    for (int i = 0; i < Info->list->size; i++) {
        FillAndBorderColor colors = GetFillColors(Info, i);

#ifdef _DEBUG
        const char *data_label = (i == 0 || i == Info->list->size - 1) ? "data (canary)" : "data";
#else
        const char *data_label = "data";
#endif
        fprintf(file, "    node%d [label=\"idx: %d | %s: " LIST_SPEC " | next: %d | prev: %d\"; shape=Mrecord; style=filled; ",
                i, i, data_label, Info->list->data[i], Info->list->next[i], Info->list->prev[i]);

        fprintf(file, "fillcolor = \"%s\"; color = \"%s\"];\n", colors.fillColor, colors.borderColor);
    }
    }
}

static void PrintInvisibleEdges(ChangeOperationContext *Info, FILE *file) {
    assert(Info);
    assert(file);

    fprintf(file, "    node0 -> node1");
    for (int i = 2; i < Info->list->size; i++) {
        fprintf(file, " -> node%d", i);
    }
    fprintf(file, " [style=invis, weight=1000];\n\n");
}

// static void PrintNextEdges(ChangeOperationContext *Info, FILE *file) {
//     assert(Info);
//     assert(file);

//     for (int i = 0; i < Info->list->size; i++) {
//         const char *color = NULL;
//         if (Info->list->data[i] == POISON) {
//             color = "hotpink";
//         } else if ((i == 0 && Info->list->number_of_elem == 1)) {
//             fprintf(file, "    node0 -> node%d [color=\"indianred3\", dir=both];\n", Info->list->next[0]);
//         } else if ((i != Info->list->size - 1 && i != 0 && Info->list->number_of_elem != 1) || (Info->list->number_of_elem == 0 && i == 0) || i == 0) {
//             color = "purple2";
//         }
//         if (color != NULL) {
//             if ((abs(Info->list->next[i])) > Info->list->size || abs(Info->list->prev[Info->list->next[i]]) > Info->list->size) {
//                 fprintf(file, " node%d [shape=octagon, fillcolor=\"firebrick1\" style=filled]", Info->list->next[i]);
//                 fprintf(file, "    node%d -> node%d [color=\"firebrick\", penwidth=\"3\"];\n", i, Info->list->next[i], color);
//             } else {
//                 fprintf(file, "    node%d -> node%d [color=%s];\n", i, Info->list->next[i], color);
//             }
//         }
//     }
// }

// static void PrintPrevEdges(ChangeOperationContext *Info, FILE *file) {
//     assert(Info);
//     assert(file);

//     for (int i = 0; i < Info->list->size; i++) {
//         if ((abs(Info->list->prev[i])) > Info->list->size) {
//             // if (abs(Info->list->prev[Info->list->next[i]]) > Info->list->size) {
//             //     fprintf(file, "    node%d -> node%d [color=yellov, prnwidth=\"2\"];\n", i, Info->list->prev[i]);
//             //     return;
//             // } else {
//                 fprintf(file, "    node%d -> node%d [color=firebrick, constraint=false];\n", i, Info->list->prev[i]);
//                 return;
//             // }
//         }
//         if (((Info->list->data[i] != POISON || i == 0) && Info->list->prev[i] != -1 && (Info->list->prev[i] != 0 
//             || Info->list->number_of_elem != 1)) || (i == 0 && Info->list->number_of_elem == 0)) {
// #ifdef _DEBUG
//             if ((Info->list->data[i] != (List_t)canary_right && Info->list->data[i] != (List_t)canary_left) 
//             || (i == 0 && Info->list->number_of_elem == 0) || (Info->list->prev[i] != 0 && Info->list->number_of_elem != 1)) {
//                 fprintf(file, "    node%d -> node%d [color=mediumaquamarine, constraint=false];\n", i, Info->list->prev[i]);
//             }
// #else
//                 fprintf(file, "    node%d -> node%d [color=mediumaquamarine, constraint=false];\n", i, Info->list->prev[i]);
// #endif
//         }
//     }
// }

static void FillFree(ChangeOperationContext *Info, FILE *file) {
    assert(Info);
    assert(file);

    fprintf(file, "    head [shape=ellipse fillcolor=\"#DCDCDC\" style=filled label=\"head=%d\"];\n", Info->list->next[0]);
    fprintf(file, "    tail [shape=ellipse fillcolor=\"#DCDCDC\" style=filled label=\"tail=%d\"];\n", Info->list->prev[0]);

    fprintf(file, "    head -> node%d [color=brown, maxlen=1];\n", Info->list->next[0]);
    fprintf(file, "    tail -> node%d [color=brown, maxlen=1];\n", Info->list->prev[0]);

    //fprintf(file, "  { rank = same; head; tail; free; }");
    fprintf(file, "    free [shape=ellipse fillcolor=\"#DCDCDC\" style=filled label=\"free = %d\"];\n", Info->list->free);
    fprintf(file, "    {rank=same; free; node%d; }", Info->list->free);
    fprintf(file, "    free -> node%d [color=\"#8B4513\"];\n", Info->list->free);

    fprintf(file, "}\n");
}