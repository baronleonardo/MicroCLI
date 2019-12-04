#include "commanddbmanager.h"
#include "commands/test/test.h"
#include "commands/reg/reg.h"
#include "commands/io/io.h"
#include "commands/inc_dec/inc_dec.h"
#include "commands/end_chain/end.h"
#include "commands/forloop/forloop.h"
#include "commands/ifcondition/ifcondition.h"

void CommandDB_createDatabase() {
    CommandDB_add("test",   sizeof("test") - 1,   cmd_test);
    CommandDB_add("regset", sizeof("regset") - 1, regSet);
    CommandDB_add("regget", sizeof("regget") - 1, regGet);
    CommandDB_add("io",     sizeof("io") - 1,     cmd_io);
    CommandDB_add("inc",    sizeof("inc") - 1,    cmd_inc);
    CommandDB_add("dec",    sizeof("dec") - 1,    cmd_dec);

    CommandDB_addChainCommand("for", sizeof("for") - 1, forloop, COMMAND_DB_MAXITRNUM_INFINITY);
    CommandDB_addChainCommand("if",  sizeof("if") - 1,  cmd_if,  COMMAND_DB_MAXITRNUM_ONE);
   
    /* NOTE: Don't change or delete this part
     * NOTE: Don't add anyother end chain command
     * NOTE: This command is important to indicate the end of chained commands
     *       used for for-loop and if-condition
     */
    CommandDB_addEndChain("end", sizeof("end") - 1, cmd_end);
}
