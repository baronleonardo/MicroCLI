#ifndef COMMANDPARSING_H
#define COMMANDPARSING_H

typedef struct Command Command;

const Command* Command_parse(const char* cmd);
void Command_exec(const Command* command);

#endif // COMMANDPARSING_H
