#ifndef SUBSIDIARY_H_
#define SUBSIDIARY_H_

#include <stdio.h>

#define FILE_FOR_GRAPH_TEXT "output.txt"
#define WRITE_MODE "w"

#define POISON 666

typedef struct {
    const char *fillColor;
    const char *borderColor;
} FillAndBorderColor;

#define RED "\033[31m"
#define RESET "\033[0m"

static const char *ListErrorString[] = {
    "Negative Size",
    "Null Data Pointer",
    "Null Next Pointer",
    "Null Prev Pointer",
    "Negative Head Index",
    "Negative Tail Index",
    "Negative Free Index",
    "Wrong Canary Value",
    "No Memory Available",
    "Invalid Parameter",
    "Error Opening File",
    "Error Closing File",
    "Invalid Position",
    "Failure",
};

#endif //SUBSIDIARY_H_