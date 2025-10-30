#include <stdio.h>
#include <stdlib.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "FileOperations.h"
#include "Test.h"
#include "SetLogFile.h"

int main(void) {
    SetLogFile("alldump.html");
    FILE *file = GetLogFile();
    ListErrors err = kSuccess;
    List list = {};
    CHECK_ERROR_RETURN(ListCtor(&list));

    CHECK_ERROR_RETURN(Test1(file, &list));

    ListDtor(&list);
    return CloseFile(file);
}