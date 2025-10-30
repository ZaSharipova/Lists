#include "DoDump.h"

#include <stdio.h>
#include "Structs.h"
#include "Subsidiary.h"

#include <time.h>
#include <assert.h>
#include <math.h>

static void PrintChangeDescription(FILE *file, ChangeOperationContext *Info);
static void PrintCurrentTime(FILE *file, const char *label);
static void PrintListInfoItem(FILE *file, const char *label, int value);

static void PrintTableStart(FILE *file);
static void PrintTableEnd(FILE *file);
static void PrintTableHeader(FILE *file, int size);
static void PrintTableRowData(FILE *file, const char *label, int size, int *array);
static void PrintTableRowListSpec(FILE *file, const char *label, int size, List *list);

void DoDump(ChangeOperationContext *Info) {
    assert(Info);

    unsigned int bit = 1;
    if (Info->type_of_command_after == kDump && Info->type_of_command_before == kDump) {
        fprintf(Info->file, "<h2> <font color=\"red\"> DUMP Listing Error</h2> </font>  \n");
        fprintf(Info->file, "<h3> errors: ");
        for (unsigned long long i = 0; i < NUMBER_OF_ERRORS; i++) {
            if (Info->error & bit) {
                fprintf(Info->file, "%s ", ListErrorString[i]);
            }
            bit <<= 1;
        }
    } else {
        fprintf(Info->file, "<h2> DUMP\n");
    }

    PrintChangeDescription(Info->file, Info);

    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\">%s {%s} </h4>\n", Info->var_name, Info->filename);

    PrintCurrentTime(Info->file, "made");

    fprintf(Info->file, "<br>");

    //list integers fill
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

static void PrintChangeDescription(FILE *file, ChangeOperationContext *Info) {
    assert(file && Info);

    bool is_before = (Info->type_of_command_before == kDump);
    int command_type = is_before ? Info->type_of_command_after : Info->type_of_command_before;
    
    if (Info->type_of_command_before == kDump && Info->type_of_command_after == kDump) {
        //fprintf(file, "<h3> Error Listing </h3>");
        return;
    }
    const char *time_label = is_before ? "BEFORE" : "AFTER";
    const char *color = is_before ? "#8B4513" : 
                       (command_type == kDelete) ? "#FF0000" : "#7FFF00";

    fprintf(file, " <font color=\"%s\"> %s </font> ", color, time_label);
    
    if (command_type == kInsertAfter || command_type == kInsertBefore) {
        const char *position = (command_type == kInsertAfter) ? "AFTER" : "BEFORE";
        fprintf(file, "<font color=\"%s\"> %s </font> value <font color=\"%s\"> " LIST_SPEC " </font> %s position <font color=\"%s\">%d</font></h3>\n",
                color, "INSERT", color, Info->number, position, color, Info->pos);
    } else if (command_type == kDelete) {
        fprintf(file, "<font color=\"%s\"> %s </font> value from position <font color=\"%s\">%d</font></h3>\n",
                color, "DELETE", color, Info->pos);
    }
}

static void PrintCurrentTime(FILE *file, const char *label) {
    assert(file);
    assert(label);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf_time[MAX_STRING_SIZE] = {};
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
#ifdef _DEBUG
        if (i == 0 || i == list->size - 1) {
            fprintf(file, "<td> " LIST_SPEC " (canary) </td>", abs(list->data[i]));
        } else {
            fprintf(file, "<td> " LIST_SPEC " </td>", list->data[i]);
        }
#else
        fprintf(file, "<td> " LIST_SPEC " </td>", list->data[i]);
#endif
    }
    fprintf(file, "</tr>\n");
}
