#include "DoDump.h"

#include <stdio.h>
#include "Structs.h"

#include <time.h>
#include <assert.h>

static const char *command_to_str(ListCommands type_of_command);

void DoDump(FILE *file, List *list, const char *var_name, const char *filename, const char *image_file, ListCommands type_of_command_before, ListCommands type_of_command_after) {
    assert(file);
    assert(list);
    assert(var_name);
    assert(filename);
    assert(image_file);

    fprintf(file, "<h2> DUMP <font color = red> ");
    if (type_of_command_before == kDump) {
        fprintf(file, "BEFORE </font> %s</h3>\n", command_to_str(type_of_command_after));
    } else {
        fprintf(file, "AFTER </font> %s</h3>\n", command_to_str(type_of_command_before));
    }
    fprintf(file, "<h4> %s {%s} </h4>\n", var_name, filename);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf_time[64] = {};
    strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S\n", tm_now);
    fprintf(file, "<h4>%s</h4>\n", buf_time);

    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Size: %d </h4>\n", list->size);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Number of elements: %d </h4>\n", list->number_of_elem);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Head: %d </h4>\n", list->head);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Tail: %d </h4>\n", list->tail);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Free: %d </h4>\n", list->free);

    fprintf(file, "<table border=\"1\" style=\"border-collapse: collapse; width: 30%%; font-size: 18px; text-align: center; margin-left: 10px;\">\n");


    fprintf(file, "<tr>");
    fprintf(file, "<th style=\"padding: 10px 24px; text-align: center;\">Index</th>");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, "<th style=\"padding: 10px 24px; text-align: center;\">%d</th>", i);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</tr>\n");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center;\">Data</td>");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, "<td>%.0f</td>", list->data[i]);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</tr>\n");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center;\">Next</td>");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, "<td>%d</td>", list->next[i]);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</tr>\n");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center;\">Prev</td>");
    for (int i = 1; i <= list->size; i++) {
        fprintf(file, "<td>%d</td>", list->prev[i]);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</table>\n");

    fprintf(file, "<br><br>\n");

    fprintf(file, "<img src = %s>", image_file);
    fprintf(file, "<hr style=\"height:1px; background-color:black; border:none; width:100%%;\">\n");
    fprintf(file, "<br>");

}

static const char *command_to_str(ListCommands type_of_command) {
    if (type_of_command == kInsert) {
        return "Insert";
    } else {
        return "Delete";
    }
}