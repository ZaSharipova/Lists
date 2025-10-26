#include "DoDump.h"

#include <stdio.h>
#include "Structs.h"

#include <time.h>
#include <assert.h>

void DoDump(List *list) {
    assert(list);

    FILE *file = fopen("alldump.html", "a");
    if (!file) {
        printf("Failed to open file %s\n", "alldump.hml");
        return;
    }

    fprintf(file, "<h3> DUMP </h3>\n");

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf[64] = {};
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_now);

    fprintf(file, "%s", buf);

    fprintf(file, "<h4> Size: %d </h4>", list->size);
    fprintf(file, "<h4> number of elements: %d </h4>", list->number_of_elem);
    fprintf(file, "<h4> Head: %d </h4>", list->head);
    fprintf(file, "<h4> Tail: %d </h4>", list->tail);
    fprintf(file, "<h4> Free: %d </h4>", list->free);

    fprintf(file, "<h4> Index ||");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, " %d |", i);
    }
    fprintf(file, "</h4>\n");

    fprintf(file, "<h4> Data ||");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, " %.0f |", list->data[i]);
    }
    fprintf(file, "</h4>\n");

    fprintf(file, "<h4> Next |");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, " %d |", list->next[i]);
    }
    fprintf(file, "</h4>\n");


    fprintf(file, "<h4> Prev ||");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, " %d |", list->prev[i]);
    }
    fprintf(file, "</h4>\n");

    fprintf(file, "<img src = graph.png>");
}