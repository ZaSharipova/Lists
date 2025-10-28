#include "DoDump.h"

#include <stdio.h>
#include "Structs.h"

#include <time.h>
#include <assert.h>

static const char *command_to_str(ListCommands type_of_command);

void DoDump(FILE *file, List *list, const char *var_name, const char *filename, const char *image_file, int pos, List_t number, ListCommands type_of_command_before, ListCommands type_of_command_after) {
    assert(file);
    assert(list);
    assert(var_name);
    assert(filename);
    assert(image_file);

    fprintf(file, "<h2> DUMP");
    if (type_of_command_before == kDump) {
        if (type_of_command_after == kInsert) {
            fprintf(file, " <font color = \"#8B4513\"> BEFORE </font> %s value <font color = #8B4513> %.0f </font> after position <font color = #8B4513>%d </font></h3>\n", command_to_str(type_of_command_after), number, pos);
        } else {
            fprintf(file, " <font color = \"#8B4513\"> BEFORE </font> %s value from position <font color = #8B4513>%d </font></h3>\n", command_to_str(type_of_command_after), pos);
        }
    } else {
        if (type_of_command_before == kInsert) {
            fprintf(file, " <font color = \"#DAA520\"> AFTER </font> <font color = #7FFF00>%s </font> value <font color = #7FFF00> %.0f </font> after position <font color = #7FFF00>%d </font></h3>\n", command_to_str(type_of_command_before), number, pos);
        } else if (type_of_command_before == kDelete) {
            fprintf(file, " <font color = \"#DAA520\"> AFTER </font> <font color = #FF0000>%s </font> value from position <font color = #FF0000>%d </font></h3>\n", command_to_str(type_of_command_before), pos);
        }
    }
    fprintf(file, "<h4 style=\"margin: 3px 0;\">%s {%s} </h4>\n", var_name, filename);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf_time[64] = {};
    strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S\n", tm_now);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> made: %s</h4>\n", buf_time);
    fprintf(file, "<br>");

    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Capacity: %d </h4>\n", list->size);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Size: %d </h4>\n", list->number_of_elem);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Head: %d </h4>\n", list->next[0]);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Tail: %d </h4>\n", list->prev[0]);
    fprintf(file, "<h4 style=\"margin: 3px 0;\"> Free: %d </h4>\n", list->free);

    fprintf(file, "<br>");
    fprintf(file, "<table border=\"2\" style=\"border-radius: 5px; overflow: hidden; border-collapse: separate; border-spacing: 0; width: 30%; font-size: 18px; text-align: center; margin-left: 0px;\">\n");


    //fprintf(file, "<table style=\"border: 2px solid black\">");
    fprintf(file, "<tr>");
    fprintf(file, "<th style=\"padding: 10px 24px; align: left; text-align: center; background-color: #fbf5eef2;\">Index</th>");
    for (int i = 0; i < list->size; i++) {
        fprintf(file, "<th style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2\">%d</th>", i);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</tr>\n");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">Data</td>");
    for (int i = 0; i < list->size; i++) {
        fprintf(file, "<td>%.0f</td>", list->data[i]);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</tr>\n");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">Next</td>");
    for (int i = 0; i < list->size; i++) {
        fprintf(file, "<td>%d</td>", list->next[i]);
    }
    fprintf(file, "</tr>\n");

    fprintf(file, "</tr>\n");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2; \">Prev</td>");
    for (int i = 0; i < list->size; i++) {
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
        return "INSERT";
    } else {
        return "DELETE";
    }
}