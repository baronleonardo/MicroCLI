#ifndef COMMANDDBMANAGER_H
#define COMMANDDBMANAGER_H

#include "../command.h"
#include <stdint.h>
#include <stdbool.h>

typedef int8_t (*CommandFunction)(char* parameters, uint8_t parameters_len);
typedef enum CommandDB_AddErrors {
    COMMAND_DB_ADDERROR_NOERROR,
    COMMAND_DB_ADDERROR_UPPERCASE,
    COMMAND_DB_ADDERROR_NAME_LEN,
    COMMAND_DB_ADDERROR_OUTOFMEM,
} CommandDB_AddErrors;

typedef enum CommandDB_CmdType {
    COMMAND_DB_CMDTYPE_INVALID,
    COMMAND_DB_CMDTYPE_NORMAL,
    COMMAND_DB_CMDTYPE_CHAINED,
    COMMAND_DB_CMDTYPE_ENDCHAIN,
} CommandDB_CmdType;

typedef enum CommandDB_ChainedCmd_MaxIterNum {
    COMMAND_DB_MAXITRNUM_INFINITY,
    COMMAND_DB_MAXITRNUM_ONE,
} CommandDB_ChainedCmd_MaxIterNum;

void CommandDB_init();
void CommandDB_createDatabase();
CommandDB_AddErrors CommandDB_add( const char* cmd_name, 
                                   int8_t cmd_name_len, 
                                   CommandFunction func );
CommandDB_AddErrors CommandDB_addChainCommand( const char* cmd_name, 
                                               int8_t cmd_name_len, 
                                               CommandFunction func,
                                               CommandDB_ChainedCmd_MaxIterNum max_iter_num);
CommandDB_AddErrors CommandDB_addEndChain( const char* cmd_name, 
                                           int8_t cmd_name_len, 
                                           CommandFunction func);
// if it returns 0, that means this command doesn't exist
uintptr_t CommandDB_getCommandId(const char* cmd_name, int8_t cmd_name_len);
CommandFunction CommandDB_getCommandFunc(uintptr_t id);
CommandDB_CmdType CommandDB_getCommandType(uintptr_t id);
CommandDB_ChainedCmd_MaxIterNum CommandDB_getMaxIterOfChainCmd(uintptr_t id);

#endif // COMMANDDBMANAGER_H
