#include "commanddbmanager.h"
#include <stdlib.h>
#include <string.h>
#include "../../config.h"

/**************************** Declarations ****************************/
#define ALPHA_LENS      26

typedef struct CommandDB_Node {
    intptr_t id;
    char* cmd_name;
    uint8_t cmd_name_len;
    CommandFunction func;
    struct CommandDB_Node* next;
} CommandDB_Node;

CommandDB_Node* commands_db[ALPHA_LENS] = { 0 };
/**********************************************************************/

/**************************** Definitions ****************************/
void CommandDB_init() {
    CommandDB_createDatabase();
}

CommandDB_AddErrors CommandDB_add( char* command_name, 
                                   uint8_t command_name_len, 
                                   CommandFunction func ) {
    if(command_name[0] > 'z' || command_name[0] < 'a') 
        return COMMAND_DB_ADDERROR_UPPERCASE;
    if(command_name_len > CMD_NAME_LEN) 
        return COMMAND_DB_ADDERROR_NAME_LEN;

    // create new node
    CommandDB_Node* new_node = malloc(sizeof(CommandDB_Node));
    if(new_node == NULL) return COMMAND_DB_ADDERROR_OUTOFMEM;
    new_node->id            = (intptr_t)new_node;
    new_node->cmd_name      = command_name;
    new_node->cmd_name_len  = command_name_len;
    new_node->func          = func;
    new_node->next          = NULL;
    
    CommandDB_Node** db_node_ptr = &commands_db[command_name[0] - 'a'];

    if( *db_node_ptr == NULL )
        *db_node_ptr = new_node;
    else {
        // find last node in linkedlist of command DB if exists
        for(; (*db_node_ptr)->next != NULL; *db_node_ptr = (*db_node_ptr)->next);
        (*db_node_ptr)->next = new_node;
    }

    return COMMAND_DB_ADDERROR_NOERROR;
}

intptr_t CommandDB_getCommandId(char* command_name, uint8_t command_name_len) {
    // find if command_name exists
    // else return -1
    CommandDB_Node* node_ptr = commands_db[command_name[0] - 'a'];

    for(; node_ptr != NULL; node_ptr = node_ptr->next) {
        if (strncmp(command_name, node_ptr->cmd_name, node_ptr->cmd_name_len) == 0)
            return node_ptr->id;
    }

    return -1;
}

CommandFunction CommandDB_getCommandFunc(intptr_t id) {
    return ((CommandDB_Node *)id)->func;
}
/**********************************************************************/
