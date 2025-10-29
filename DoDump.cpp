#include "DoDump.h"

#include <stdio.h>
#include "Structs.h"

#include <time.h>
#include <assert.h>

static const char *command_to_str(ListCommands type_of_command);
static void PrintCurrentTime(FILE *file, const char *label);
static void PrintListInfoItem(FILE *file, const char *label, int value);

static void PrintTableStart(FILE *file);
static void PrintTableEnd(FILE *file);
static void PrintTableHeader(FILE *file, int size);
static void PrintTableRowData(FILE *file, const char *label, int size, int *array);
static void PrintTableRowListSpec(FILE *file, const char *label, int size, List *list);

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

    PrintCurrentTime(Info->file, "made");

    fprintf(Info->file, "<br>");

    PrintListInfoItem(Info->file, "Capacity", Info->list->size);
    PrintListInfoItem(Info->file, "Size", Info->list->number_of_elem);
    PrintListInfoItem(Info->file, "Head", Info->list->next[0]);
    PrintListInfoItem(Info->file, "Tail", Info->list->prev[0]);
    PrintListInfoItem(Info->file, "Free", Info->list->free);

    fprintf(Info->file, "<br>");

    PrintTableStart(Info->file);
    PrintTableHeader(Info->file, Info->list->size);
    PrintTableRowListSpec(Info->file, "Data", Info->list->size, Info->list);
    PrintTableRowData(Info->file, "Next", Info->list->size, Info->list->next);
    PrintTableRowData(Info->file, "Prev", Info->list->size, Info->list->prev);
    PrintTableEnd(Info->file);

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

static void PrintCurrentTime(FILE *file, const char *label) {
    assert(file);
    assert(label);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf_time[64] = {};
    strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S\n", tm_now);

    fprintf(file, "<h4 style=\"margin: 3px 0;\">%s: %s</h4>\n", label, buf_time);
}

static void PrintListInfoItem(FILE *file, const char *label, int value) {
    assert(file);
    assert(label);

    fprintf(file, "<h4 style=\"margin: 3px 0;\"> %s: %d </h4>\n", label, value);
}

static void PrintTableStart(FILE *file) {
    assert(file);

    fprintf(file, "<table border=\"2\" style=\"border-radius: 5px; overflow: hidden; border-collapse: separate; border-spacing: 0; width: 30%%; font-size: 18px; text-align: center; margin-left: 0px;\">\n");
}

static void PrintTableEnd(FILE *file) {
    assert(file);

    fprintf(file, "</table>\n");
}

static void PrintTableHeader(FILE *file, int size) {
    assert(file);

    fprintf(file, "<tr>");
    fprintf(file, "<th style=\"padding: 10px 24px; align: left; text-align: center; background-color: #fbf5eef2;\">Index</th>");
    for (int i = 0; i < size; i++) {
        fprintf(file, "<th style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2\">%d</th>", i);
    }
    fprintf(file, "</tr>\n");
}

static void PrintTableRowData(FILE *file, const char *label, int size, int *array) {
    assert(file);
    assert(label);
    assert(array);

    fprintf(file, "<tr>");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">%s</td>", label);
    for (int i = 0; i < size; i++) {
        fprintf(file, "<td>%d</td>", array[i]);
    }
    fprintf(file, "</tr>\n");
}

static void PrintTableRowListSpec(FILE *file, const char *label, int size, List *list) {
    assert(file);
    assert(label);
    assert(list);

    fprintf(file, "<tr>");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">%s</td>", label);
    for (int i = 0; i < size; i++) {
        fprintf(file, "<td> " LIST_SPEC " </td>", list->data[i]);
    }
    fprintf(file, "</tr>\n");
}
