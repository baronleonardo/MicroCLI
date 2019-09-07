#include "command.h"
#include "../config.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../communication/communication.h"

struct Command {
    char name[CMD_NAME_LEN];
    uint8_t name_len;
    bool use_data;
    uint8_t args_len;
    union {
        char options[CMD_ARGS_LEN];
        char data[CMD_REG_LEN];
    };
} currentCmd;

typedef enum ArgsType {
    ARGSTYPE_CMDNAME,
    ARGSTYPE_REGISTERS,
    ARGSTYPE_OPTIONS,
    ARGSTYPE_DATA,
    ARGSTYPE_ERROR,
} ArgsType;

inline ArgsType __Command_identify(const char* start, const char* end);
inline bool __Command_process(ArgsType type, const char* start, const char* end);

inline ArgsType __Command_validateRegisters(const char* start, const char* end);
inline ArgsType __Command_validateOptions(const char* start, const char* end);
inline ArgsType __Command_validateData(const char* start, const char* end);
inline ArgsType __Command_validateCmdName(const char* start, const char* end);
inline void __Command_processRegisters(const char* start, const char* end);
inline void __Command_processOptions(const char* start, const char* end);
inline void __Command_processData(const char* start, const char* end);
inline void __Command_processCmdName(const char* start, const char* end);
inline void __Command_processError(const char* start, const char* end);

void (*const __Command_processors[])(const char* start, const char* end) = {
    __Command_processCmdName,
    __Command_processRegisters,
    __Command_processOptions,
    __Command_processData,
    __Command_processError,
};

const char error_msg[] = "Syntax Error at ";
const uint8_t error_msg_len = sizeof(error_msg) - 1;

const Command* Command_parse(const char* cmd) {
    const char* start;
    const char* end;
    ArgsType args_type;

    // set/reset currentCmd
    currentCmd.name_len = 0;
    currentCmd.use_data = false;
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
}

ArgsType __Command_identify(const char* start, const char* end) {
    if(*start == '$') 
        return __Command_validateRegisters(start, end);
    else if(*start == '-')
        return __Command_validateOptions(start, end);
    else if(*start == '\"' || *start == '\'')
        return __Command_validateData(start, end);
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
    else if (currentCmd.use_data == true)
        return ARGSTYPE_ERROR;
    else if(currentCmd.args_len >= CMD_ARGS_LEN) 
        return ARGSTYPE_ERROR;

    return ARGSTYPE_REGISTERS;
}

ArgsType __Command_validateOptions(const char* start, const char* end) {
    if((end - start + 1) > 2) return ARGSTYPE_ERROR;
    // check if *end is in [a-zA-Z]
    if( (*end < 'A') || (*end > 'z') ) return ARGSTYPE_ERROR;
    else if( (*end < 'a') && (*end > 'Z') ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len == 0 )
        return ARGSTYPE_ERROR;
    else if (currentCmd.use_data == true)
        return ARGSTYPE_ERROR;
    else if(currentCmd.args_len >= CMD_ARGS_LEN) 
        return ARGSTYPE_ERROR;

    return ARGSTYPE_OPTIONS;
}

ArgsType __Command_validateData(const char* start, const char* end) {
    if( (end - start + 1) > CMD_REG_LEN ) return ARGSTYPE_ERROR;
    if( (*end != '\'') && (*end != '\"') ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len == 0 )
        return ARGSTYPE_ERROR;
    else if (currentCmd.args_len != 0)
        return ARGSTYPE_ERROR;

    return ARGSTYPE_DATA;
}

ArgsType __Command_validateCmdName(const char* start, const char* end) {
    if( (end - start + 1) > CMD_NAME_LEN ) return ARGSTYPE_ERROR;

    if( currentCmd.name_len != 0 )
        return ARGSTYPE_ERROR;

    return ARGSTYPE_CMDNAME;
}

void __Command_processRegisters(const char* start, const char* end) {
    currentCmd.options[currentCmd.args_len++] = *end;
}

void __Command_processOptions(const char* start, const char* end) {
    currentCmd.options[currentCmd.args_len++] = *end;
}

void __Command_processData(const char* start, const char* end) {
    uint8_t len = end - start + 1;

    currentCmd.use_data = true;
    strncpy(currentCmd.data, start, len);
    currentCmd.data[len] = '\0';
    currentCmd.args_len = len;
}

void __Command_processCmdName(const char* start, const char* end) {
    uint8_t len = end - start + 1;
    strncpy(currentCmd.name, start, len);
    currentCmd.name[len] = '\0';
    currentCmd.name_len = len;
}

void __Command_processError(const char* start, const char* end) {
    Comm_write(error_msg, error_msg_len);
    Comm_write(start, end - start + 1);
    Comm_writeChar(CMD_DELIMITER);
}
