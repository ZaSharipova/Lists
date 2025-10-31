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

    err = Test1(file, &list);
    if (err & ~kSuccess) {
        fprintf(stderr, "Returning error code %d\n", err);
        ListDtor(&list);
        CloseFile(file);
        return err;
    }

    ListDtor(&list);
    CloseFile(file);
    return 0;
}