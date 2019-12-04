#ifndef MICROCLI_ENGINE_H
#define MICROCLI_ENGINE_H

#include <stdint.h>
#include "command.h"

typedef struct Command Command;

Command* Command_parse(const char* cmd, uint8_t cmd_len);
void Command_exec(Command* cmd);
int8_t Command_execDirectly(Command* cmd);

#endif // MICROCLI_ENGINE_H
