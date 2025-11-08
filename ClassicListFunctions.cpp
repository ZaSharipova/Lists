#include "ClassicListFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "Enums.h"
#include "Subsidiary.h"

ListErrors AddressListCtor(AddressList **head) {

    *head = (AddressList *) calloc (1, sizeof(AddressList));
    if (*head == NULL) {
        return kNoMemory;
    }

    (*head)->data = 0;
    (*head)->prev = NULL;
    (*head)->next = NULL;

    return kSuccess;
}

ListErrors AddressListDtor(AddressList *head) {
    assert(head);

    if (head) {
        AddressList *temp = head;
        if (head->next != NULL) AddressListDtor(head->next);
        free(temp);
    }

    return kSuccess;
}

ListErrors AddressListDump(AddressList *list) {
    assert(list);

    // unsigned int bit = 1;
    // printf("DUMP Errors: ");
    // for (unsigned long long i = 0; i < NUMBER_OF_ERRORS; i++) {
    //     if (error & bit) {
    //         fprintf(GetLogFile(), "%s ", ListErrorString[i]);
    //     }
    //     bit <<= 1;
    // }

    AddressList *head = list;
    while (head){
        printf(YELLOW "" LIST_SPEC" , prev - %p, next - %p" RESET, head->data, head->prev, head->next);
        printf("\n\n");
        head = head->next;
    }

    printf("\n\n\n");

    return kSuccess;
}

void InsertAfter(AddressList *pos, List_t *value) {
    if (pos == NULL) return;

    AddressList *new_node = (AddressList *) calloc (1, sizeof(AddressList));
    new_node->data = *value;

    new_node->next = pos->next;
    new_node->prev = pos;

    if (pos->next != NULL) {
        pos->next->prev = new_node;
    }
    pos->next = new_node;
}

void InsertBefore(AddressList **head, AddressList *pos, List_t *value) {
    if (pos == NULL) return;

    AddressList *new_node = (AddressList *) calloc (1, sizeof(AddressList));
    new_node->data = *value;

    new_node->next = pos;
    new_node->prev = pos->prev;

    if (pos->prev != NULL) {
        pos->prev->next = new_node;
    } else {
        *head = new_node;
    }
    pos->prev = new_node;
}

void DeleteNode(AddressList **head, AddressList *pos) {
    if (pos == NULL) return;

    if (pos->prev != NULL) {
        pos->prev->next = pos->next;
    } else {
        *head = pos->next;
    }
    if (pos->next != NULL) {
        pos->next->prev = pos->prev;
    }

    free(pos);
}

#include "Structs.h"
#include "Subsidiary.h"

#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>

static void PrintChangeDescription(FILE *file, ChangeOperationContextAddr *Info);
static void PrintCurrentTime(FILE *file, const char *label);
static void PrintListInfoItem(FILE *file, const char *label, AddressList *value);

static void PrintTableStart(FILE *file);
static void PrintTableEnd(FILE *file);
static void PrintTableHeader(FILE *file, int size);
static void PrintTableRowData(FILE *file, const char *label, int size, AddressList *array);
static void PrintTableRowListSpec(FILE *file, const char *label, int size, AddressList *list);

void DoDumpAddr(ChangeOperationContextAddr *Info) {
    assert(Info);

    unsigned int bit = 1;
    if (Info->type_of_command_before == kDumpErrors) {
        fprintf(Info->file, "<h2> <font color=\"red\"> DUMP Listing Error</h2> </font>  \n");
        fprintf(Info->file, "<h2> <font color = \"red\"> %s</font> </h2> ", Info->message);
        fprintf(Info->file, "<h3> errors: </h3>");
        for (unsigned long long i = 0; i < NUMBER_OF_ERRORS; i++) {
            if (Info->error & bit) {
                fprintf(Info->file, "<h4> <font color=\"red\"> %s </font> </h4> \n", ListErrorString[i]);
            }
            bit <<= 1;
        }
        fprintf(Info->file, "<br>");
    } else {
        fprintf(Info->file, "<h2> DUMP\n");
    }
    // if (Info->message[0] != '\0') {
    //     fprintf(Info->file, "%s", Info->message);
    // }

    PrintChangeDescription(Info->file, Info);

    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\">%s {%s} </h4>\n", Info->var_name, Info->filename);

    PrintCurrentTime(Info->file, "made");

    fprintf(Info->file, "<br>");

    //list integers fill
    if (Info->list->data && Info->list->next && Info->list->prev) {
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> %s: %d </h4>\n", "Capacity", Info->list->size);
    fprintf(Info->file, "<h4 style=\"margin: 3px 0;\"> %s: %d </h4>\n", "Size", Info->list->number_of_elem);
    PrintListInfoItem(Info->file, "Head", Info->list->next);
    PrintListInfoItem(Info->file, "Tail", Info->list->prev);
    // PrintListInfoItem(Info->file, "Free", Info->list->free);
    }

    fprintf(Info->file, "<br>");

    // if (Info->list->data && Info->list->next && Info->list->prev) {
    // PrintTableStart(Info->file);
    // PrintTableHeader(Info->file, Info->list->size);
    // PrintTableRowListSpec(Info->file, "Data", Info->list->size, Info->list);
    // PrintTableRowData(Info->file, "Next", Info->list->size, Info->list->next);
    // PrintTableRowData(Info->file, "Prev", Info->list->size, Info->list->prev);
    // PrintTableEnd(Info->file);
    // }

    fprintf(Info->file, "<br><br>\n");

    fprintf(Info->file, "<img src = %s>", Info->image_file);
    fprintf(Info->file, "<hr style=\"height:1px; background-color:black; border:none; width:100%%;\">\n");
    fprintf(Info->file, "<br>");
}

static void PrintChangeDescription(FILE *file, ChangeOperationContextAddr *Info) {
    assert(file);
    assert(Info);

    bool is_before = (Info->type_of_command_before == kDumpBefore);
    int command_type = is_before ? Info->type_of_command_after : Info->type_of_command_before;
    
    if (Info->type_of_command_before == kDumpErrors) {
        //fprintf(file, "<h3> Error Listing </h3>");
        return;
    }

    if (Info->type_of_command_before == kDump) {
        return;
    }

    const char *time_label = is_before ? "BEFORE" : "AFTER";
    const char *color = is_before ? "#8B4513" : 
                       (command_type == kDelete || command_type == kPopBack || command_type == kPopFront) ? "#DC143C" : "#7FFF00";

    fprintf(file, " <font color=\"%s\"> %s </font> ", color, time_label);
    
    if (command_type == kInsertAfter || command_type == kInsertBefore) {
        const char *position = (command_type == kInsertAfter) ? "AFTER" : "BEFORE";
        fprintf(file, "<font color=\"%s\"> %s </font> value <font color=\"%s\"> " LIST_SPEC " </font> %s position <font color=\"%s\">%d</font></h3>\n",
                color, "INSERT", color, Info->number, position, color, Info->pos);
    } else if (command_type == kDelete || command_type == kPopBack || command_type == kPopFront) {
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

static void PrintListInfoItem(FILE *file, const char *label, AddressList *value) {
    assert(file);
    assert(label);

    fprintf(file, "<h4 style=\"margin: 3px 0;\"> %s: %p </h4>\n", label, value);
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

// static void PrintTableRowData(FILE *file, const char *label, int size, AddressList *array) {
//     assert(file);
//     assert(label);
//     assert(array);

//     fprintf(file, "<tr>");
//     fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">%s</td>", label);
//     for (int i = 0; i < size; i++) {
//         fprintf(file, "<td>%d</td>", array[i]);
//     }
//     fprintf(file, "</tr>\n");
// }

static void PrintTableRowListSpec(FILE *file, const char *label, int size, AddressList *list) {
    assert(file);
    assert(label);
    assert(list);

    fprintf(file, "<tr>");
    fprintf(file, "<td style=\"padding: 10px 24px; text-align: center; background-color: #fbf5eef2;\">%s</td>", label);

    for (int i = 0; i < size; i++) {
#ifdef _DEBUG
        if (i == 0 || i == list->size - 1) {
            fprintf(file, "<td> " LIST_SPEC " (canary) </td>", abs(list->data));
        } else {
            fprintf(file, "<td> " LIST_SPEC " </td>", list->data);
        }
#else
        fprintf(file, "<td> " LIST_SPEC " </td>", list->data);
#endif
    }
    fprintf(file, "</tr>\n");
}

static void DoSnprintf(ChangeOperationContextAddr *Info) {
    assert(Info);

    snprintf(Info->image_file, sizeof(Info->image_file), "Images/graph_%d.svg", Info->graph_counter);
    (Info->graph_counter)++;
    char cmd[40] = {};
    snprintf(cmd, sizeof(cmd), "dot output.txt -T svg -o %s", Info->image_file);
    
    system(cmd);
}

#include <stdio.h>
#include <assert.h>

#include "Enums.h"

void PrintDotNode(FILE *file, const AddressList *node) {
    assert(file);
    assert(node);

    fprintf(file, "    \"%p\" [ label=\"{Addr: %p | Data: %d | {Prev:  %p | Next: %p}}\"; shape=Mrecord; color=black];\n", 
            (void *)node, (void *)node, node->data, 
            node->prev, 
            node->next);
    

    if (node->prev) {
        fprintf(file, "    \"%p\" -> \"%p\" \n [color=blue]", (void *)node, (void *)node->prev);
    }

    if (node->next) {
        fprintf(file, "    \"%p\" -> \"%p\" \n[color=green]", (void *)node, (void *)node->next);
        PrintDotNode(file, node->next);
    }
}

void DoGraphAddr(const ChangeOperationContextAddr *Info, const char *filename) {
    assert(Info);
    assert(filename);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error, file not opened.");
        return;
    }

    fprintf(file, "digraph {\n");
    //fprintf(file, "    rankdir=TB;\n");
    fprintf(file, "    node [shape=record, style=filled, fillcolor=lightblue];\n");
    fprintf(file, "    edge [fontsize=10];\n\n");
    fprintf(file, "    graph [fontname=\"Arial\"];\n");
    fprintf(file, "    node [fontname=\"Arial\"];\n");
    fprintf(file, "    edge [fontname=\"Arial\"];\n");      

    if (Info->list) {
        PrintDotNode(file, Info->list);
    }
    fprintf(file, "}\n");

    fclose(file);

    snprintf((char *)Info->image_file, sizeof(Info->image_file), "Images/graph_%d.svg", 100);
    system("dot output.txt -T svg -o Images/graph_100.svg");
    printf("\nImage is already done.");
}

// static void DoSnprintf(ChangeOperationContext *Info) {
//     assert(Info);

//     snprintf(Info->image_file, sizeof(Info->image_file), "Images/graph_%d.svg", Info->graph_counter);
//     (Info->graph_counter)++;
//     char cmd[40] = {};
//     snprintf(cmd, sizeof(cmd), "dot output.txt -T svg -o %s", Info->image_file);
    
//     system(cmd);
// }