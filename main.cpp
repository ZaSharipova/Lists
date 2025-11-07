#include <stdio.h>
#include <stdlib.h>

#include "ListFunctions.h"
#include "Structs.h"
#include "FileOperations.h"
#include "Test.h"
#include "SetLogFile.h"

#include "ClassicListFunctions.h"

int main(void) {
    SetLogFile("alldump.html");
    FILE *file = GetLogFile();
    ListErrors err = kSuccess;
    List list = {};
    CHECK_ERROR_RETURN(ListCtor(&list));

    err = Test7(file, &list);
    if (err & ~kSuccess) {
        fprintf(stderr, "Returning error code %d\n", err);
        ListDtor(&list);
        CloseFile(file);
        return err;
    }
    ListDtor(&list);
    CloseFile(file);

    // AddressList *head = NULL;
    // AddressListCtor(&head);

    // List_t elem = 2;
    // InsertAfter(head, &elem);
    // elem = 3;
    // InsertAfter(head->next, &elem);
    // elem = 0;
    // InsertBefore(&head, head, &elem);

    // AddressListDump(head);

    // DeleteNode(&head, head->next);
    // AddressListDump(head);

    // AddressListDtor(head);
    
    return 0;
}