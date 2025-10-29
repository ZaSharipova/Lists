#include <stdio.h>
#include <stdlib.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "FileOperations.h"
#include "Test.h"

int main(void) {
    FILE *file = OpenFile("alldump.html", "w");
    if (!file) {
        return kErrorOpening;
    }

    ListErrors err = kSuccess;
    List list = {};
    CHECK_ERROR_RETURN(ListCtor(&list));

    CHECK_ERROR_RETURN(Test1(file, &list));

    ListDtor(&list);
    return CloseFile(file);
}