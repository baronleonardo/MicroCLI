#include "chain_command.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "commanddb/commanddbmanager.h"
#include "microcli_engine.h"
#include "../config.h"
#include "../drivers/drivers.h"

static Command* main_head_cmd = NULL;
static Command* current_cmd = NULL;

struct NestedChainedCommands {
    Command* stack[CMD_CHAINED_STACK_LEN];
    uint8_t len;
} nestedChainedCmds = { {0}, 0 };

static void __ChainCmds_exec(Command* head_cmd);
static inline void ChainCmds_printMaxNestedChainCmdsReached();

void ChainCmds_init(Command* head_cmd) {
    if(nestedChainedCmds.len < CMD_CHAINED_STACK_LEN) {
        nestedChainedCmds.stack[nestedChainedCmds.len++] = head_cmd;
        if(main_head_cmd == NULL) {
            main_head_cmd = head_cmd;
            current_cmd = head_cmd;
        } else
            ChainCmds_addCommand(head_cmd);
    } else {
        ChainCmds_del();
        ChainCmds_printMaxNestedChainCmdsReached();
    }
}

void ChainCmds_addCommand(Command* cmd) {
    Command_setNextChainedCommand(current_cmd, cmd);
    current_cmd = cmd;
}

bool ChainCmds_exec(Command* cmd) {
    if(main_head_cmd != NULL) {
        Command* head_cmd = 
            nestedChainedCmds.stack[--nestedChainedCmds.len];
        Command_setHeadTailChainedCommand(head_cmd, cmd);
        Command_setHeadTailChainedCommand(cmd, head_cmd);
        ChainCmds_addCommand(cmd);

        // TODO: we need to run end_chain cmd and get its return status
        // to be used to shutdown the chain without execution

        if(nestedChainedCmds.len == 0) {
            __ChainCmds_exec(main_head_cmd);
            return true;
        }

        return false;
    }

    return true;
}

void __ChainCmds_exec(Command* head_cmd) {
    bool isAllCyclesFinished = false;
    bool isCycleFinished = false;
    int8_t cmd_return_status = 0;

    while(!isAllCyclesFinished) {
        current_cmd = head_cmd;
        cmd_return_status = Command_execDirectly( current_cmd );

        isCycleFinished = false;

        if(cmd_return_status <= 0) {
            isAllCyclesFinished = true;
        } else {
            while(!isCycleFinished) {
                current_cmd = Command_getNextChainedCommand(current_cmd);

                CommandDB_CmdType cmdType =
                    CommandDB_getCommandType( Command_getId(current_cmd) );

                switch(cmdType) {
                    case COMMAND_DB_CMDTYPE_CHAINED:
                        // this part is for nested chained commands
                        ChainCmds_exec(current_cmd);
                        current_cmd = Command_getHeadTailChainedCommand(current_cmd);
                        break;
                    case COMMAND_DB_CMDTYPE_ENDCHAIN:
                        isCycleFinished = true;
                        break;
                    case COMMAND_DB_CMDTYPE_INVALID:
                        isCycleFinished = true;
                        isAllCyclesFinished = true;
                        break;
                    case COMMAND_DB_CMDTYPE_NORMAL:
                        Command_execDirectly( current_cmd );
                        break;
                }
            }
        }

        if( CommandDB_getMaxIterOfChainCmd(Command_getId( head_cmd )) == COMMAND_DB_MAXITRNUM_ONE )
            isAllCyclesFinished = true;
    }
}

void ChainCmds_del() {
    Command* cmd = main_head_cmd;
    do {
        Command_del(cmd);
    } while( (cmd = Command_getNextChainedCommand(cmd)) != NULL );
    main_head_cmd = NULL;
}

void ChainCmds_printChainCmdIndicator() {
    for(uint8_t iii = 0; iii < nestedChainedCmds.len; ++iii)
        Comm_write("..", sizeof("..") - 1);
    Comm_writeChar(' ');
}

void ChainCmds_printMaxNestedChainCmdsReached() {
    Comm_write("You reached the maximum number of nested chain commands",
            sizeof("You reached the maximum number of nested chain commands") - 1);
    Comm_writeNewLine();
}
