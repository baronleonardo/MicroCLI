#include "command.h"
#include "../config.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../drivers/drivers.h"
#include "commanddb/commanddbmanager.h"

/****************** Variables and functions declarations ******************/
struct Command {
    char name[CMD_NAME_LEN];
    uint8_t name_len;
    bool using_reg_data;
    uint8_t args_len;
    union {
        char args[CMD_ARGS_COUNT];
        char reg_data[CMD_REG_LEN];
    };
} currentCmd;

typedef enum ArgsType {
    ARGSTYPE_CMDNAME,
    ARGSTYPE_REGISTERS,
    ARGSTYPE_args,
    ARGSTYPE_REGDATA,
    ARGSTYPE_ERROR,
} ArgsType;

inline ArgsType __Command_identify(const char* start, const char* end);
inline bool __Command_process(ArgsType type, const char* start, const char* end);

inline ArgsType __Command_validateRegisters(const char* start, const char* end);
inline ArgsType __Command_validateArgs(const char* start, const char* end);
inline ArgsType __Command_validateRegData(const char* start, const char* end);
inline ArgsType __Command_validateCmdName(const char* start, const char* end);
inline void __Command_processRegisters(const char* start, const char* end);
inline void __Command_processArgs(const char* start, const char* end);
inline void __Command_processRegData(const char* start, const char* end);
inline void __Command_processCmdName(const char* start, const char* end);
inline void __Command_processError(const char* start, const char* end);

// array of pointers of functions
void (*const __Command_processors[])(const char* start, const char* end) = {
    __Command_processCmdName,
    __Command_processRegisters,
    __Command_processArgs,
    __Command_processRegData,
    __Command_processError,
};
/**************************************************************************/

/****************** public member functions definitions ******************/
const Command* Command_parse(const char* cmd) {
    const char* start;
    const char* end;
    ArgsType args_type;

    // set/reset currentCmd
    currentCmd.name_len = 0;
    currentCmd.using_reg_data = false;
    currentCmd.args_len = 0;

    while(*cmd != '\0') {
        if(*cmd > ' ') {
            start = cmd;
            while(*(++cmd) > ' ');
            end = cmd - 1;
            
            args_type = __Command_identify(start, end);
            if(__Command_process(args_type, start, end) == false)
                return NULL;
        }

        else ++cmd;
    }

    return &currentCmd;
}

void Command_exec(const Command* command) {
   intptr_t cmd_id = CommandDB_getCommandId(currentCmd.name, currentCmd.name_len);
   if(cmd_id != -1) {
        CommandDB_getCommandFunc(cmd_id)(
            currentCmd.using_reg_data == true ? currentCmd.reg_data : currentCmd.args,
            currentCmd.args_len );
   } else {
        Comm_write("Unknown Command", sizeof("Unknown Command") - 1);
        Comm_writeNewLine();
   }
}
/**************************************************************************/

/****************** private member functions definitions ******************/
ArgsType __Command_identify(const char* start, const char* end) {
    if(*start == '$') 
        return __Command_validateRegisters(start, end);
    else if(*start == '-')
        return __Command_validateArgs(start, end);
    else if(*start == '\"' || *start == '\'' || *start == '#')
        return __Command_validateRegData(start, end);
    else
        return __Command_validateCmdName(start, end);
}

bool __Command_process(ArgsType type, const char* start, const char* end) {
   __Command_processors[type](start, end);

   if(type == ARGSTYPE_ERROR) return false;

   return true;
}

ArgsType __Command_validateRegisters(const char* start, const char* end) {
    if((end - start + 1) > 2)  return ARGSTYPE_ERROR;
    if( (*end <= '0') || (*end >= '9') ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len == 0 )
        return ARGSTYPE_ERROR;
    else if (currentCmd.using_reg_data == true)
        return ARGSTYPE_ERROR;
    else if(currentCmd.args_len >= CMD_ARGS_COUNT) 
        return ARGSTYPE_ERROR;

    return ARGSTYPE_REGISTERS;
}

ArgsType __Command_validateArgs(const char* start, const char* end) {
    if((end - start + 1) > 2) return ARGSTYPE_ERROR;
    // check if *end is in [a-zA-Z]
    if( (*end < 'A') || (*end > 'z') ) return ARGSTYPE_ERROR;
    else if( (*end < 'a') && (*end > 'Z') ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len == 0 )
        return ARGSTYPE_ERROR;
    else if (currentCmd.using_reg_data == true)
        return ARGSTYPE_ERROR;
    else if(currentCmd.args_len >= CMD_ARGS_COUNT) 
        return ARGSTYPE_ERROR;

    return ARGSTYPE_args;
}

ArgsType __Command_validateRegData(const char* start, const char* end) {
    if( (end - start + 1) > CMD_REG_LEN ) return ARGSTYPE_ERROR;
    if(*start != '#')
        if( (*end != '\'') && (*end != '\"') ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len == 0 )
        return ARGSTYPE_ERROR;
    else if (currentCmd.args_len != 0)
        return ARGSTYPE_ERROR;

    return ARGSTYPE_REGDATA;
}

ArgsType __Command_validateCmdName(const char* start, const char* end) {
    if( (end - start + 1) > CMD_NAME_LEN ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len != 0 )
        return ARGSTYPE_ERROR;

    return ARGSTYPE_CMDNAME;
}

void __Command_processRegisters(const char* start, const char* end) {
    currentCmd.args[currentCmd.args_len++] = *end;
}

void __Command_processArgs(const char* start, const char* end) {
    currentCmd.args[currentCmd.args_len++] = *end;
}

void __Command_processRegData(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    currentCmd.using_reg_data = true;
    strncpy(currentCmd.reg_data, start, len);
    // TODO: remove this part
    currentCmd.reg_data[len] = '\0';
    currentCmd.args_len = len;
}

void __Command_processCmdName(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    strncpy(currentCmd.name, start, len);
    // TODO: remove this part
    currentCmd.name[len] = '\0';
    currentCmd.name_len = len;
}

void __Command_processError(const char* start, const char* end) {
    Comm_write("Syntax Error at ", sizeof("Syntax Error at ") - 1);
    Comm_write(start, end - start + 1);
    Comm_writeNewLine();
}
/**************************************************************************/
