#ifndef CLASSIC_LIST_FUNCTIONS_H_
#define CLASSIC_LIST_FUNCTIONS_H_

#include "Structs.h"

ListErrors AddressListCtor(AddressList **head);
ListErrors AddressListDtor(AddressList *head);

void InsertAfter(AddressList *pos, List_t *value);
void InsertBefore(AddressList **head, AddressList *pos, List_t *value);
void DeleteNode(AddressList **head, AddressList *pos);

ListErrors AddressListDump(AddressList *list);


#endif // CLASSIC_LIST_FUNCTIONS_H_