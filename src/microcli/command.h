#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Command Command;

Command* Command_new(uint8_t args_size);
bool Command_del(Command* cmd);


void Command_setId(Command* cmd, uintptr_t id);
uintptr_t Command_getId(Command* cmd);

uint8_t Command_getArgsLen(Command* cmd);

bool Command_isUsingMultipleArgs(Command* cmd);

void Command_setNextChainedCommand(Command* cmd, Command* next_cmd);
Command* Command_getNextChainedCommand(Command* cmd);
void Command_setHeadTailChainedCommand(Command* cmd, Command* head_tail_cmd);
Command* Command_getHeadTailChainedCommand(Command* cmd);

void Command_addArg(Command* cmd, char arg);
void Command_addArgs(Command* cmd, const char args[], uint8_t args_len);
uint8_t Command_getArgs(Command* cmd, char* args[]);

#endif // COMMAND_H
