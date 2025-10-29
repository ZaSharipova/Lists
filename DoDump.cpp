#include "DoDump.h"

#include <stdio.h>
#include "Structs.h"

#include <time.h>
#include <assert.h>

static const char *command_to_str(ListCommands type_of_command);

void DoDump(ChangeOperationContext *Info) {
    assert(Info);

    fprintf(Info->file, "<h2> DUMP");
    if (Info->type_of_command_before == kDump) {
        fprintf(Info->file, " <font color = \"#8B4513\"> BEFORE </font> %s value", command_to_str(Info->type_of_command_after));
        if (Info->type_of_command_after == kInsertAfter || Info->type_of_command_after == kInsertBefore) {
            fprintf(Info->file, "<font color = #8B4513> " LIST_SPEC" </font> %s position <font color = #8B4513>%d </font></h3>\n", Info->number, 
                (Info->type_of_command_after == kInsertAfter) ? "AFTER" : "BEFORE", Info->pos);

        } else if (Info->type_of_command_after == kDelete) {
            fprintf(Info->file, " from position <font color = #8B4513>%d </font></h3>\n", Info->pos);
        }

    } else {
        fprintf(Info->file, " <font color = \"#DAA520\">  AFTER </font> ");
        if (Info->type_of_command_before == kInsertAfter || Info->type_of_command_before == kInsertBefore) {
            fprintf(Info->file, "<font color = #7FFF00> %s </font> value <font color = #7FFF00> " LIST_SPEC" </font> %s position <font color = #7FFF00>%d </font></h3>\n", 
            command_to_str(Info->type_of_command_before), Info->number, (Info->type_of_command_before == kInsertAfter) ? "AFTER" : "BEFORE", Info->pos);

        } else if (Info->type_of_command_before == kDelete) {
            fprintf(Info->file, "<font color = #FF0000> %s </font> value from position <font color = #FF0000>%d </font></h3>\n", command_to_str(Info->type_of_command_before), Info->pos);
        }
    }
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\">%s {%s} </h4>\n", Info->var_name, Info->filename);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf_time[64] = {};
    strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S\n", tm_now);
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> made: %s</h4>\n", buf_time);
    fprintf(Info->file, "<br>");

    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> Capacity: %d </h4>\n", Info->list->size);
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> Size: %d </h4>\n", Info->list->number_of_elem);
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> Head: %d </h4>\n", Info->list->next[0]);
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> Tail: %d </h4>\n", Info->list->prev[0]);
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> Free: %d </h4>\n", Info->list->free);

    fprintf(Info->file, "<br>");
    fprintf(Info->file, "<table border=\"2\" style=\"border-radius: 5px; overflow: hidden; border-collapse: separate; border-spacing: 0; width: 30%%; font-size: 18px; text-align: center; margin-left: 0px;\">\n");

    //fprintf(file, "<table style=\"border: 2px solid black\">");
    fprintf(Info->file, "<tr>");
    fprintf(Info->file, "<th style=\"padding: 10px 24px; align: left; text-align: center; background-color: #fbf5eef2;\">Index</th>");
    for (int i = 0; i < Info->list->size; i++) {
        fprintf(Info->file, "<th style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2\">%d</th>", i);
    }
    fprintf(Info->file, "</tr>\n");

    fprintf(Info->file, "</tr>\n");
    fprintf(Info->file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">Data</td>");
    for (int i = 0; i < Info->list->size; i++) {
        fprintf(Info->file, "<td> " LIST_SPEC" </td>", Info->list->data[i]);
    }
    fprintf(Info->file, "</tr>\n");

    fprintf(Info->file, "</tr>\n");
    fprintf(Info->file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">Next</td>");
    for (int i = 0; i < Info->list->size; i++) {
        fprintf(Info->file, "<td>%d</td>", Info->list->next[i]);
    }
    fprintf(Info->file, "</tr>\n");

    fprintf(Info->file, "</tr>\n");
    fprintf(Info->file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2; \">Prev</td>");
    for (int i = 0; i < Info->list->size; i++) {
        fprintf(Info->file, "<td>%d</td>", Info->list->prev[i]);
    }
    fprintf(Info->file, "</tr>\n");

    fprintf(Info->file, "</table>\n");

    fprintf(Info->file, "<br><br>\n");

    fprintf(Info->file, "<img src = %s>", Info->image_file);
    fprintf(Info->file, "<hr style=\"height:1px; background-color:black; border:none; width:100%%;\">\n");
    fprintf(Info->file, "<br>");
}

static const char *command_to_str(ListCommands type_of_command) {

    if (type_of_command == kInsertAfter || type_of_command == kInsertBefore) {
        return "INSERT";

    } else {
        return "DELETE";
    }
}