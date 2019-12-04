#include "command.h"
#include <stdlib.h>
#include <string.h>

// We have 2 types of arguments
// first type : multiple one char arguments - including registers
// second type: one big argument surrounding with single/double quotations
struct Command {
    uintptr_t id;
    uint8_t args_len;
    bool using_multiple_args;
    struct {
        struct Command* next;
        // if this command is head then this pointer will point to tail
        // else if this command is tail then this pointer will point to head
        struct Command* head_tail_node;
    } chained_cmds;
    char args[];
};

Command* Command_new(uint8_t args_size) {
    Command* cmd = malloc(sizeof(Command) + args_size);
    if(cmd == NULL) return NULL;

    cmd->id = 0;
    cmd->using_multiple_args = true;
    cmd->args_len = 0;
    cmd->chained_cmds.next = NULL;
    cmd->chained_cmds.head_tail_node = NULL;

    return cmd;
}

bool Command_del(Command* cmd) {
    if(cmd == NULL) 
        return false;
    else {
        free(cmd);
        return true;
    }
}


inline void Command_setId(Command* cmd, uintptr_t id) {
    cmd->id = id;
}

uintptr_t Command_getId(Command* cmd) {
    return cmd->id;
}

inline uint8_t Command_getArgsLen(Command* cmd) {
    return cmd->args_len;
}

inline bool Command_isUsingMultipleArgs(Command* cmd) {
    return cmd->using_multiple_args;
}

inline void Command_setNextChainedCommand(Command* cmd, Command* next_cmd) {
    cmd->chained_cmds.next = next_cmd;
}

inline Command* Command_getNextChainedCommand(Command* cmd) {
    return cmd->chained_cmds.next;
}

inline void Command_setHeadTailChainedCommand(Command* cmd, Command* head_tail_cmd) {
    cmd->chained_cmds.head_tail_node = head_tail_cmd;
}

inline Command* Command_getHeadTailChainedCommand(Command* cmd) {
    return cmd->chained_cmds.head_tail_node;
}

inline void Command_addArg(Command* cmd, char arg) {
    cmd->args[cmd->args_len++] = arg;
}

inline void Command_addArgs(Command* cmd, const char args[], uint8_t args_len) {
    cmd->using_multiple_args = false;
    strncpy(cmd->args, args, args_len);
    cmd->args_len = args_len;
}
inline uint8_t Command_getArgs(Command* cmd, char* args[]) {
    *args = cmd->args;
    return cmd->args_len;
}


