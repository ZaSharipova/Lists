#include "Test.h"

#include <stdio.h>
#include <assert.h>

#include "Structs.h"
#include "ListFunctions.h"

ListErrors Test1(FILE *file, List *list) {
    assert(file);
    assert(list);

    ListErrors err = kSuccess;

    DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 1, 20, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 3, 26, kInsertAfter);
    DOCHANGE(file, DeleteElement, *list, 2, 25, kDelete);
    DOCHANGE(file, InsertElementBeforePosition, *list, 4, 10, kInsertBefore);

    return kSuccess;
}

ListErrors Test2(FILE *file, List *list) {
    assert(file);
    assert(list);

    ListErrors err = kSuccess;

    DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 1, 20, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 3, 26, kInsertAfter);
    return kSuccess;
}

ListErrors Test3(FILE *file, List *list) {
    assert(file);
    assert(list);

    ListErrors err = kSuccess;

    DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 1, 20, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);
    DOCHANGE(file, DeleteElement, *list, 2, 0, kDelete);
    DOCHANGE(file, InsertElementAfterPosition, *list, 3, 26, kInsertAfter);

    return kSuccess;
}

ListErrors Test4(FILE *file, List *list) {
    assert(file);
    assert(list);

    ListErrors err = kSuccess;

    DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
    DOCHANGE(file, InsertElementBeforePosition, *list, 1, 20, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);

    return kSuccess;
}

ListErrors Test5(FILE *file, List *list) {
    assert(file);
    assert(list);

    ListErrors err = kSuccess;

    DOCHANGE(file, InsertElementAfterPosition, *list, 0, 32, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 1, 20, kInsertAfter);
    DOCHANGE(file, InsertElementAfterPosition, *list, 2, 25, kInsertAfter);
    DOCHANGE(file, DeleteElement, *list, 3, 0, kDelete);

    return kSuccess;
}