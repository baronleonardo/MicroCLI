#ifndef COMMANDPARSING_H
#define COMMANDPARSING_H

#include <stdint.h>

typedef struct Command Command;

Command* Command_parse(const char* cmd, uint8_t cmd_len);
void Command_exec(Command* command);

#endif // COMMANDPARSING_H
