#include "commanddbmanager.h"
#include <stdlib.h>
#include <string.h>
#include "../../config.h"

/**************************** Declarations ****************************/
#define ALPHA_LENS          26
#define MAX_ITER_INDICATOR  0b1000000

typedef struct CommandDB_Node {
    uintptr_t id;
    const char* cmd_name;
    int8_t cmd_name_len;    // if (-ve): chained cmd, else if (+ve): normal cmd
    CommandFunction func;
    struct CommandDB_Node* next;
} CommandDB_Node;

static CommandDB_Node* cmds_db[ALPHA_LENS] = { 0 };
static uint32_t endChainCmdId = 0;
/**********************************************************************/

/**************************** Definitions ****************************/
void CommandDB_init() {
    CommandDB_createDatabase();
}

CommandDB_AddErrors CommandDB_add( const char* cmd_name, 
                                   int8_t cmd_name_len, 
                                   CommandFunction func ) {
    if(cmd_name[0] > 'z' || cmd_name[0] < 'a') 
        return COMMAND_DB_ADDERROR_UPPERCASE;
    if((cmd_name_len & ~(MAX_ITER_INDICATOR)) > CMD_NAME_LEN)
        return COMMAND_DB_ADDERROR_NAME_LEN;

    // create new node
    CommandDB_Node* new_node = malloc(sizeof(CommandDB_Node));
    if(new_node == NULL) return COMMAND_DB_ADDERROR_OUTOFMEM;
    new_node->id            = (uintptr_t)new_node;
    new_node->cmd_name      = cmd_name;
    new_node->cmd_name_len  = cmd_name_len;
    new_node->func          = func;
    new_node->next          = NULL;
    
    CommandDB_Node** db_node_ptr = &cmds_db[cmd_name[0] - 'a'];

    if( *db_node_ptr == NULL )
        *db_node_ptr = new_node;
    else {
        // find last node in linkedlist of cmd DB if exists
        for(; (*db_node_ptr)->next != NULL; *db_node_ptr = (*db_node_ptr)->next);
        (*db_node_ptr)->next = new_node;
    }

    return COMMAND_DB_ADDERROR_NOERROR;
}

CommandDB_AddErrors CommandDB_addChainCommand( const char* cmd_name, 
                                               int8_t cmd_name_len, 
                                               CommandFunction func,
                                               CommandDB_ChainedCmd_MaxIterNum max_iter_num) {
    CommandDB_AddErrors error_type;
    if(max_iter_num == COMMAND_DB_MAXITRNUM_ONE)
        cmd_name_len += MAX_ITER_INDICATOR;
    error_type = CommandDB_add(cmd_name, -cmd_name_len, func);

    return error_type;
}

CommandDB_AddErrors CommandDB_addEndChain( const char* cmd_name,
                                           int8_t cmd_name_len,
                                           CommandFunction func) {
    CommandDB_AddErrors error_type;
    error_type = CommandDB_add(cmd_name, cmd_name_len, func);
    if(error_type == COMMAND_DB_ADDERROR_NOERROR)
        endChainCmdId = CommandDB_getCommandId(cmd_name, cmd_name_len);

    return error_type;
}

uintptr_t CommandDB_getCommandId(const char* cmd_name, int8_t cmd_name_len) {
    // find if cmd_name exists
    // else return 0
    CommandDB_Node* node_ptr = cmds_db[cmd_name[0] - 'a'];

    for(; node_ptr != NULL; node_ptr = node_ptr->next) {
        if (strncmp(cmd_name, node_ptr->cmd_name, cmd_name_len) == 0)
            return node_ptr->id;
    }

    return 0;
}

CommandFunction CommandDB_getCommandFunc(uintptr_t id) {
    if(CommandDB_getCommandType(id) == COMMAND_DB_CMDTYPE_INVALID)
        return NULL;
    else
        return ((CommandDB_Node *)id)->func;
}

CommandDB_CmdType CommandDB_getCommandType(uintptr_t id) {
    if( id == 0 )
        return COMMAND_DB_CMDTYPE_INVALID;
    else if( *((uintptr_t*)id) != id ) // if id equals id in Command_node then this is valid
        return COMMAND_DB_CMDTYPE_INVALID;
    else if( ((CommandDB_Node *)id)->cmd_name_len < 0 )
        return COMMAND_DB_CMDTYPE_CHAINED;
    else if( id == endChainCmdId )
        return COMMAND_DB_CMDTYPE_ENDCHAIN;
    else
        return COMMAND_DB_CMDTYPE_NORMAL;
}

CommandDB_ChainedCmd_MaxIterNum CommandDB_getMaxIterOfChainCmd(uintptr_t id) {
    int8_t cmd_name_len = ((CommandDB_Node *)id)->cmd_name_len;
    if(cmd_name_len >= 0) 
        return -1;
    else if( (-cmd_name_len & MAX_ITER_INDICATOR) != 0)
        return COMMAND_DB_MAXITRNUM_ONE;
    else
        return COMMAND_DB_MAXITRNUM_INFINITY;
}
/**********************************************************************/
