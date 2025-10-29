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

    List list = {};
    ListCtor(&list); //

    ListErrors err = kSuccess;
    CHECK_ERROR_RETURN(Test5(file, &list));

    ListDtor(&list); //
    return CloseFile(file);
}