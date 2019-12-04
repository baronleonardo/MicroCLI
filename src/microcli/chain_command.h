#ifndef CHAIN_COMMAND_H
#define CHAIN_COMMAND_H

#include "command.h"

typedef struct Command Command;

void ChainCmds_init(Command* head_cmd);
void ChainCmds_addCommand(Command* cmd);
// true: successfully executed the chain commands
// false: the execution is not started yet 
//        waiting for the last end chain command
bool ChainCmds_exec(Command* cmd);
// this will free the memory from the whole chain commands
// NOTE: this function is automatically called
void ChainCmds_del();
void ChainCmds_printChainCmdIndicator();

#endif // CHAIN_COMMAND_H
