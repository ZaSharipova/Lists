#ifndef DO_DUMP_H_
#define DO_DUMP_H_

#include "Structs.h"

void DoDump(FILE *file, List *list, const char *var_info, const char *filename, const char *image_file, int pos, List_t number, ListCommands type_of_command_before, ListCommands type_of_command_after);

#endif //DO_DUMP_H_