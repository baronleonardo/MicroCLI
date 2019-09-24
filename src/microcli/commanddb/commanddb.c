#include "commanddbmanager.h"
#include "commands/test/test.h"

void CommandDB_createDatabase() {
    CommandDB_add("test", sizeof("test") - 1, cmd_test);
}
