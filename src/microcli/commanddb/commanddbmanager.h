#ifndef COMMANDDBMANAGER_H
#define COMMANDDBMANAGER_H

#include "../command.h"
#include <stdint.h>
#include <stdbool.h>

typedef void (*CommandFunction)(char* parameters, uint8_t parameters_len);
typedef enum CommandDB_AddErrors {
    COMMAND_DB_ADDERROR_NOERROR,
    COMMAND_DB_ADDERROR_UPPERCASE,
    COMMAND_DB_ADDERROR_NAME_LEN,
    COMMAND_DB_ADDERROR_OUTOFMEM,
} CommandDB_AddErrors;

void CommandDB_init();
void CommandDB_createDatabase();
CommandDB_AddErrors CommandDB_add( char* command_name, 
                                   uint8_t command_name_len, 
                                   CommandFunction func );
// if it returns -1, that means this command doesn't exist
intptr_t CommandDB_getCommandId(char* command_name, uint8_t command_name_len);
CommandFunction CommandDB_getCommandFunc(intptr_t id);

#endif // COMMANDDBMANAGER_H
