#include "microcli_engine.h"
#include "../config.h"
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../drivers/drivers.h"
#include "commanddb/commanddbmanager.h"
#include "chain_command.h"
#include <itos.h>

/****************** Variables and functions declarations ******************/

static Command* current_cmd = NULL;
static int8_t cmd_return_status = 0;

typedef enum Exec_Mode {
    EXECMODE_NORMAL,
    EXECMODE_CHAIN,
} Exec_Mode;

static Exec_Mode exec_mode = EXECMODE_NORMAL;

typedef enum ArgsType {
    ARGSTYPE_CMDNAME,
    ARGSTYPE_REGISTERS,
    ARGSTYPE_MULTIPLEARGS,
    ARGSTYPE_ONEBIGARG,
    ARGSTYPE_ERROR_SYNTAX,
    ARGSTYPE_ERROR_CMDNAME,
} ArgsType;

inline static ArgsType Command_argsIdentify(const char* cmd, const char* start, const char* end);
inline static bool Command_process(ArgsType type, const char* start, const char* end);

inline static ArgsType Command_validateRegsiter(const char* start, const char* end);
inline static ArgsType Command_validateArg(const char* start, const char* end);
inline static ArgsType Command_validateOneBigArg(const char* start, const char* end);
inline static ArgsType Command_validateCmdName(const char* start, const char* end);
inline static void Command_processRegister(const char* start, const char* end);
inline static void Command_processArg(const char* start, const char* end);
inline static void Command_processOneBigArg(const char* start, const char* end);
inline static void Command_processCmdName(const char* start, const char* end);
inline static void Command_processSyntaxError(const char* start, const char* end);
inline static void Command_processCmdNameError(const char* start, const char* end);

inline static int8_t __Command_exec(Command* cmd);

// array of pointers of functions
void (*const Command_processors[])(const char* start, const char* end) = {
    Command_processCmdName,
    Command_processRegister,
    Command_processArg,
    Command_processOneBigArg,
    Command_processSyntaxError,
    Command_processCmdNameError,
};
/**************************************************************************/

/****************** public member functions definitions ******************/
Command* Command_parse(const char* cmd, uint8_t cmd_len) {
    const char* start;
    const char* end;
    ArgsType args_type;
    const char* cmd_start = cmd;

    current_cmd = Command_new(cmd_len);

    // TODO: get rid of '\0'
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
           
            args_type = Command_argsIdentify(cmd_start, start, end);
            if(Command_process(args_type, start, end) == false) {
                Command_del(current_cmd);
                return NULL;
            }
        }

        else ++cmd;
    }

    return current_cmd;
}

void Command_exec(Command* cmd) {
    if(cmd != NULL) {
        CommandDB_CmdType cmd_type = 
            CommandDB_getCommandType(Command_getId(cmd));

        switch(cmd_type) {
            case COMMAND_DB_CMDTYPE_NORMAL:
                if(exec_mode == EXECMODE_NORMAL) {
                    __Command_exec( cmd );
                    Command_del(cmd);
                } else {
                    ChainCmds_addCommand(cmd);
                    ChainCmds_printChainCmdIndicator();
                }
                break;
            case COMMAND_DB_CMDTYPE_INVALID:
                if(exec_mode == EXECMODE_NORMAL)
                    Command_del(cmd);
                break;
            case COMMAND_DB_CMDTYPE_CHAINED:
                ChainCmds_init(cmd);
                ChainCmds_printChainCmdIndicator();
                exec_mode = EXECMODE_CHAIN;
                break;
            case COMMAND_DB_CMDTYPE_ENDCHAIN:
                if(ChainCmds_exec(cmd)) {
                    exec_mode = EXECMODE_NORMAL;
                    ChainCmds_del();
                }
                break;
        }
    }
}

int8_t Command_execDirectly(Command* cmd) {
    return __Command_exec(cmd);
}
/**************************************************************************/

/****************** private member functions definitions ******************/
ArgsType Command_argsIdentify(const char* cmd, const char* start, const char* end) {
    if(*start == '$') 
        return Command_validateRegsiter(start, end);
    else if(*start == '-')
        return Command_validateArg(start, end);
    else if(*start == '\"')
        return Command_validateOneBigArg(start, end);
    else if(cmd == start) // if true, then check if this is command name
        return Command_validateCmdName(start, end);
    else 
        return ARGSTYPE_ERROR_SYNTAX;
}

bool Command_process(ArgsType type, const char* start, const char* end) {
   Command_processors[type](start, end);

   if( (type == ARGSTYPE_ERROR_CMDNAME) || (type == ARGSTYPE_ERROR_SYNTAX) )
       return false;

   return true;
}

ArgsType Command_validateRegsiter(const char* start, const char* end) {
    if((end - start + 1) > 2)  return ARGSTYPE_ERROR_SYNTAX;
    if( (*end < '0') || (*end > '9') ) return ARGSTYPE_ERROR_SYNTAX;

    if( Command_getId(current_cmd) == 0 )
        return ARGSTYPE_ERROR_SYNTAX;
    else if( !Command_isUsingMultipleArgs(current_cmd) )
        return ARGSTYPE_ERROR_SYNTAX;

    return ARGSTYPE_REGISTERS;
}

ArgsType Command_validateArg(const char* start, const char* end) {
    if((end - start + 1) > 2) return ARGSTYPE_ERROR_SYNTAX;
    if( (*end < 'a') || (*end > 'z') ) return ARGSTYPE_ERROR_SYNTAX;

    if( Command_getId(current_cmd) == 0 )
        return ARGSTYPE_ERROR_SYNTAX;
    else if( !Command_isUsingMultipleArgs(current_cmd) )
        return ARGSTYPE_ERROR_SYNTAX;

    return ARGSTYPE_MULTIPLEARGS;
}

ArgsType Command_validateOneBigArg(const char* start, const char* end) {
    if(*end != '\"') return ARGSTYPE_ERROR_SYNTAX;

    if( Command_getId(current_cmd) == 0 )
        return ARGSTYPE_ERROR_SYNTAX;
    else if(Command_getArgsLen(current_cmd) != 0)
        return ARGSTYPE_ERROR_SYNTAX;

    return ARGSTYPE_ONEBIGARG;
}

ArgsType Command_validateCmdName(const char* start, const char* end) {
    if( (end - start + 1) > CMD_NAME_LEN ) 
        return ARGSTYPE_ERROR_CMDNAME;

    // if there is already command name assigned before
    if( Command_getId(current_cmd) != 0 )
        return ARGSTYPE_ERROR_CMDNAME;

    uint8_t len = end - start + 1;
    if(CommandDB_getCommandId(start, len) == 0)
        return ARGSTYPE_ERROR_CMDNAME;

    return ARGSTYPE_CMDNAME;
}

void Command_processRegister(const char* start, const char* end) {
    //current_cmd->args[current_cmd->args_len++] = *end;
    Command_addArg(current_cmd, *end);
}

void Command_processArg(const char* start, const char* end) {
    //current_cmd->args[current_cmd->args_len++] = *end;
    Command_addArg(current_cmd, *end);
}

void Command_processOneBigArg(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    Command_addArgs(current_cmd, start, len);
    //current_cmd->using_multiple_args = false;
    //strncpy(current_cmd->args, start, len);
    //current_cmd->args_len = len;
}

void Command_processCmdName(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    // current_cmd->id = CommandDB_getCommandId(start, len);
    Command_setId(current_cmd, CommandDB_getCommandId(start, len));
}

void Command_processSyntaxError(const char* start, const char* end) {
    Comm_write("Syntax Error at ", sizeof("Syntax Error at ") - 1);
    Comm_write(start, end - start + 1);
    Comm_writeNewLine();
}

void Command_processCmdNameError(const char* start, const char* end) {
    Comm_write("Unknown Command", sizeof("Unknown Command") - 1);
    Comm_writeNewLine();
}

int8_t __Command_exec(Command* cmd) {
    char* args;
    uint8_t args_len = Command_getArgs(cmd, &args);
    cmd_return_status = 
        CommandDB_getCommandFunc( Command_getId(cmd) )( args, args_len );

    return cmd_return_status;
}
/**************************************************************************/
