#include "command.h"
#include "../config.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../drivers/drivers.h"
#include "commanddb/commanddbmanager.h"

/****************** Variables and functions declarations ******************/
// We have 2 types of arguments
// first type : multiple one char arguments - including registers
// second type: one big argument surrounding with single/double quotations
struct Command {
    uintptr_t id;
    uint8_t args_len;
    bool using_multiple_args;
    char args[];
};

struct Command* currentCmd;

typedef enum ArgsType {
    ARGSTYPE_CMDNAME,
    ARGSTYPE_REGISTERS,
    ARSTYPE_MULTIPLEARGS,
    ARGSTYPE_ONEBIGARG,
    ARGSTYPE_ERROR_SYNTAX,
    ARGSTYPE_ERROR_CMDNAME,
} ArgsType;

inline ArgsType __Command_identify(const char* cmd, const char* start, const char* end);
inline bool __Command_process(ArgsType type, const char* start, const char* end);

inline ArgsType __Command_validateRegsiter(const char* start, const char* end);
inline ArgsType __Command_validateArg(const char* start, const char* end);
inline ArgsType __Command_validateOneBigArg(const char* start, const char* end);
inline ArgsType __Command_validateCmdName(const char* start, const char* end);
inline void __Command_processRegister(const char* start, const char* end);
inline void __Command_processArg(const char* start, const char* end);
inline void __Command_processOneBigArg(const char* start, const char* end);
inline void __Command_processCmdName(const char* start, const char* end);
inline void __Command_processSyntaxError(const char* start, const char* end);
inline void __Command_processCmdNameError(const char* start, const char* end);

// array of pointers of functions
void (*const __Command_processors[])(const char* start, const char* end) = {
    __Command_processCmdName,
    __Command_processRegister,
    __Command_processArg,
    __Command_processOneBigArg,
    __Command_processSyntaxError,
    __Command_processCmdNameError,
};
/**************************************************************************/

/****************** public member functions definitions ******************/
Command* Command_parse(const char* cmd, uint8_t cmd_len) {
    const char* start;
    const char* end;
    ArgsType args_type;
    const char* cmd_start = cmd;

    currentCmd = malloc(sizeof(struct Command) + cmd_len);
    if(currentCmd == NULL) return NULL;

    // set/reset currentCmd
    currentCmd->id = 0;
    currentCmd->using_multiple_args = true;
    currentCmd->args_len = 0;

    while(*cmd != '\0') {
        if(*cmd > ' ') {
            start = cmd;

            // Check for one big argument 
            // and get the whole input between quotations
            if(*cmd == '\"') {
                while((*(++cmd) != '\"') && (*cmd != '\0'));
                if(*cmd == '\"') cmd++;
            } else
                while(*(++cmd) > ' ');

            end = cmd - 1;
           
            args_type = __Command_identify(cmd_start, start, end);
            if(__Command_process(args_type, start, end) == false) {
                free(currentCmd);
                return NULL;
            }
        }

        else ++cmd;
    }

    return currentCmd;
}

void Command_exec(Command* command) {
    if(command != NULL) {
        if(command->id != 0)
            CommandDB_getCommandFunc(command->id)( command->args, command->args_len );

        // free allocated command
        free(command);
    }
}
/**************************************************************************/

/****************** private member functions definitions ******************/
ArgsType __Command_identify(const char* cmd, const char* start, const char* end) {
    if(*start == '$') 
        return __Command_validateRegsiter(start, end);
    else if(*start == '-')
        return __Command_validateArg(start, end);
    else if(*start == '\"')
        return __Command_validateOneBigArg(start, end);
    else if(cmd == start) // if true, then check if this is command name
        return __Command_validateCmdName(start, end);
    else 
        return ARGSTYPE_ERROR_SYNTAX;
}

bool __Command_process(ArgsType type, const char* start, const char* end) {
   __Command_processors[type](start, end);

   if( (type == ARGSTYPE_ERROR_CMDNAME) || (type == ARGSTYPE_ERROR_SYNTAX) )
       return false;

   return true;
}

ArgsType __Command_validateRegsiter(const char* start, const char* end) {
    if((end - start + 1) > 2)  return ARGSTYPE_ERROR_SYNTAX;
    if( (*end <= '0') || (*end >= '9') ) return ARGSTYPE_ERROR_SYNTAX;

    if( currentCmd->id == 0 )
        return ARGSTYPE_ERROR_SYNTAX;
    else if (currentCmd->using_multiple_args == false)
        return ARGSTYPE_ERROR_SYNTAX;

    return ARGSTYPE_REGISTERS;
}

ArgsType __Command_validateArg(const char* start, const char* end) {
    if((end - start + 1) > 2) return ARGSTYPE_ERROR_SYNTAX;
    if( (*end < 'a') || (*end > 'z') ) return ARGSTYPE_ERROR_SYNTAX;

    if( currentCmd->id == 0 )
        return ARGSTYPE_ERROR_SYNTAX;
    else if (currentCmd->using_multiple_args == false)
        return ARGSTYPE_ERROR_SYNTAX;

    return ARSTYPE_MULTIPLEARGS;
}

ArgsType __Command_validateOneBigArg(const char* start, const char* end) {
    if(*end != '\"') return ARGSTYPE_ERROR_SYNTAX;

    if( currentCmd->id == 0 )
        return ARGSTYPE_ERROR_SYNTAX;
    else if (currentCmd->args_len != 0)
        return ARGSTYPE_ERROR_SYNTAX;

    return ARGSTYPE_ONEBIGARG;
}

ArgsType __Command_validateCmdName(const char* start, const char* end) {
    if( (end - start + 1) > CMD_NAME_LEN ) 
        return ARGSTYPE_ERROR_CMDNAME;

    if( currentCmd->id != 0 )
        return ARGSTYPE_ERROR_CMDNAME;

    uint8_t len = end - start + 1;
    if(CommandDB_getCommandId(start, len) == 0)
        return ARGSTYPE_ERROR_CMDNAME;

    return ARGSTYPE_CMDNAME;
}

void __Command_processRegister(const char* start, const char* end) {
    currentCmd->args[currentCmd->args_len++] = *end;
}

void __Command_processArg(const char* start, const char* end) {
    currentCmd->args[currentCmd->args_len++] = *end;
}

void __Command_processOneBigArg(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    currentCmd->using_multiple_args = false;
    strncpy(currentCmd->args, start, len);
    currentCmd->args_len = len;
}

void __Command_processCmdName(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    currentCmd->id = CommandDB_getCommandId(start, len);
}

void __Command_processSyntaxError(const char* start, const char* end) {
    Comm_write("Syntax Error at ", sizeof("Syntax Error at ") - 1);
    Comm_write(start, end - start + 1);
    Comm_writeNewLine();
}

void __Command_processCmdNameError(const char* start, const char* end) {
    Comm_write("Unknown Command", sizeof("Unknown Command") - 1);
    Comm_writeNewLine();
}
/**************************************************************************/
