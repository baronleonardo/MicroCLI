#include "commanddbmanager.h"
#include "commands/test/test.h"
#include "commands/reg/reg.h"

void CommandDB_createDatabase() {
    CommandDB_add("test", sizeof("test") - 1, cmd_test);
    CommandDB_add("regset", sizeof("regset") - 1, regSet);
    CommandDB_add("regget", sizeof("regget") - 1, regGet);
}
